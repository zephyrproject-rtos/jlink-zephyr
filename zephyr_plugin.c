/*
 * Zephyr RTOS plugin for JlinkGDBServer.
 *
 * Copyright (c) 2024 Nordic Semiconductor ASA
 * Copyright (c) 2024 Commonwealth Scientific and Industrial Research Organisation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "RTOSPlugin.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef WIN32
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT __attribute__((visibility("default")))
#endif

#define VERBOSE_LOGGING            0

#define PLUGIN_VERSION             101

/**
 * J-Link core identification.
 */
#define JLINK_CORE_NONE             0x00000000
#define JLINK_CORE_ANY              0xFFFFFFFF
#define JLINK_CORE_CORTEX_M3        0x030000FF
#define JLINK_CORE_CORTEX_M3_R1P0   0x03000010
#define JLINK_CORE_CORTEX_M3_R1P1   0x03000011
#define JLINK_CORE_CORTEX_M3_R2P0   0x03000020
#define JLINK_CORE_CORTEX_M0        0x060000FF
#define JLINK_CORE_CORTEX_M_V8BASEL 0x060100FF
#define JLINK_CORE_CORTEX_M4        0x0E0000FF
#define JLINK_CORE_CORTEX_M7        0x0E0100FF
#define JLINK_CORE_CORTEX_M_V8MAINL 0x0E0200FF

/**
 * List of debug information that Zephyr exposes.
 *
 * When Zephyr kernel is build with CONFIG_THREAD_INFO it exposes
 * runtime List with debug information.
 * See zephyr/subsys/debug/thread_info.c
 *
 * This list must be kept in same order as in Zephyr kernel.
 *
 * OFFSET_X is offset of X in bytes inside _kernel structure.
 * OFFSET_THREAD_X is offset of X in bytes inside struct k_thread.
 */
enum {
  OFFSET_VERSION,
  OFFSET_CURRENT,
  OFFSET_THREADS,
  OFFSET_THREAD_ENTRY,
  OFFSET_THREAD_NEXT,
  OFFSET_THREAD_STATE,
  OFFSET_THREAD_OPTIONS,
  OFFSET_THREAD_PRIORITY,
  OFFSET_THREAD_STACKPTR,
  OFFSET_THREAD_NAME,
  OFFSET_THREAD_ARCH,
  OFFSET_THREAD_PREEMPT_FLOAT,
  OFFSET_THREAD_COOP_FLOAT,
  OFFSET_THREAD_ARM_EXC_RETURN,
  OFFSET_END,
};
#define LEN_DEBUG_OFFSETS (OFFSET_END)
uint32_t debug_offsets[LEN_DEBUG_OFFSETS];

#define OFFSET_UNIMPLEMENTED	0xffffffff

#define EXC_RETURN_FTYPE_MASK (1 << 4)
#define EXC_RETURN_FTYPE_STANDARD EXC_RETURN_FTYPE_MASK
#define EXC_RETURN_FTYPE_EXTENDED 0

/** @defgroup zep_k_states Zephyr Kernel states.
 * See kernel_structs.h and ksched.h on Zephyr for more information.
 * @{
 */
#define STATE_PENDING       (1 << 1)
#define STATE_NOT_STARTED   (1 << 2)
#define STATE_DEAD          (1 << 3)
#define STATE_SUSPENDED     (1 << 4)
#define STATE_ABORTING      (1 << 5)
#define STATE_QUEUED        (1 << 7)
/** @} */

/** List of registers that Zephyr kernel structure keep.
 * This is platform specific, so only 32bit ARM has similar.
 * See: include/arch/arm/aarch32/thread.h from Zephyr
 */
struct saved_registers {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t psp;
};

/** @brief Arm Cortex stack content.
 * See: https://developer.arm.com/documentation/100235/0003/the-cortex-m33-processor/exception-model/exception-entry-and-return?lang=en
 * Note the offset values, we should have them reversed in the struct, compared to the image in the manual.
 * This way the offsets match. Address of R0 is where PSP points to. PSP+4 is R1, etc..
 */
struct stack {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;
};

/** @brief Arm Cortex extended stack content.
 * FPU stack frame determined from https://developer.arm.com/documentation/dai0298/a/
 */
struct extended_stack {
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t s12;
    uint32_t s13;
    uint32_t s14;
    uint32_t s15;
    uint32_t fpscr;
    /* Not described in application note beyond an empty box.
     * Suspect it exists to keep 8 byte alignment.
     */
    uint32_t reserved;
};

/** Internal structure to keep thread data.
 */
struct thread_t
{
    char name[32];
    uint32_t base;
    uint8_t state;
    int8_t prio;
    bool registers_loaded;
    bool extended_stack_frame;
    struct stack stack;
    struct extended_stack extended_stack;
    struct saved_registers saved;
    struct thread_t *next;
};

/** Has RTOS_UpdateThreads run? */
bool threads_updated = false;
/** Head of thread list */
struct thread_t *threads_head = NULL;
/** Currently running thread */
uint32_t current_base = 0;
/* size_t size */
uint8_t size_t_size = 0;
/** Num thread offset */
uint64_t num_thread_offsets = 0;

/** @defgroup symindex Symbol table indexes.
 * @{
 */
#define SYMBOLS_INDEX_KERNEL          0
#define SYMBOLS_INDEX_THREAD_INFO     1
#define SYMBOLS_INDEX_NUM_THREAD_INFO 2
#define SYMBOLS_INDEX_SIZE_T_SIZE     3
#define SYMBOLS_INDEX_POST_KERNEL     4
/** @} */

/** Symbol table for JLink to resolve */
static RTOS_SYMBOLS _Symbols[] = {
#ifdef _MSC_VER
  {"_kernel", 0, 0},
  {"_kernel_thread_info_offsets", 0, 0},
  {"_kernel_thread_info_num_offsets", 0, 0},
  {"_kernel_thread_info_size_t_size", 0, 0},
  {"z_sys_post_kernel", 1, 0},
  { NULL, 0, 0 },
#else
  {.name = "_kernel"},
  {.name = "_kernel_thread_info_offsets"},
  {.name = "_kernel_thread_info_num_offsets"},
  {.name = "_kernel_thread_info_size_t_size"},
  {.name = "z_sys_post_kernel", .optional = 1},
  { NULL, 0, 0 },
#endif
};

/** JLink GDBServer API */
static const GDB_API *api = NULL;


struct thread_t *new_thread(void)
{
    struct thread_t *new = api->pfAlloc(sizeof(struct thread_t));
    if (!new) {
        api->pfLogOutf("ERROR: failed to allocate memory");
        return NULL;
    }
    memset(new, 0, sizeof(struct thread_t));
    new->next = threads_head;
    threads_head = new;
    return new;
}

void del_thread(struct thread_t *t)
{
    api->pfFree(t);
}

/**
 * @brief clear all thread states.
 *
 */
static void clear(void)
{
    struct thread_t *p;
    for (p = threads_head; p;) {
        struct thread_t *q = p->next;
        del_thread(p);
        p = q;
    }
    threads_head = NULL;
    current_base = 0;
}

struct thread_t *base_to_thread(uint32_t base) {
    struct thread_t *p;
    for (p = threads_head; p; p = p->next) {
        if (p->base == base) {
            return p;
        }
    }
    return NULL;
}

int n_threads(void)
{
    struct thread_t *p;
    int n = 0;
    for (p = threads_head; p; p = p->next) {
        ++n;
    }
    return n;
}

struct thread_t *thread(int n)
{
    struct thread_t *p;
    for (p = threads_head; p; p = p->next) {
        if (n == 0) {
            return p;
        }
        --n;
    }
    return NULL;
}

/**
 * @brief check if we need to have dummy handler thread for interrupt context.
 *
 */
static void update_handler_thread(void)
{
 #define SPSEL (1<<1)

    // Check CONTROL.SPSEL if we are in interrupt/excepion or reset
    uint32_t control = api->pfReadReg(RTOS_PLUGIN_CPU_REG_CORTEX_M_CONTROL);
    if ((control & SPSEL) == 0) {
        struct thread_t *t;
        // We must show the handler thread as well
#ifndef _NO_DEBUG_LOG
        api->pfLogOutf("%s(): Adding handler thread\n", __func__);
#endif
        t = new_thread();
        sprintf(t->name, "EXCEPTION/INTERRUPT");
        current_base = 0;
    }
}

/**
 * @brief Get register value based in given index from JLink defined enum.
 *
 * Values should be loaded first by callind read_stack().
 *
 * @param index
 * @return register value.
 */
static uint32_t get_regval(struct thread_t *t, int index)
{
    switch(index){
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R0:   return t->stack.r0;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R1:   return t->stack.r1;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R2:   return t->stack.r2;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R3:   return t->stack.r3;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R4:   return t->saved.r4;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R5:   return t->saved.r5;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R6:   return t->saved.r6;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R7:   return t->saved.r7;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R8:   return t->saved.r8;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R9:   return t->saved.r9;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R10:  return t->saved.r10;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R11:  return t->saved.r11;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_R12:  return t->stack.r12;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_SP:   return t->saved.psp;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_LR:   return t->stack.lr;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_PC:   return t->stack.pc;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_XPSR: return t->stack.xpsr;
    case RTOS_PLUGIN_CPU_REG_CORTEX_M_PSP:  return t->saved.psp;
    default: return 0;
    }
}

/**
 * @brief Restore stack pointer to state before PendSV exception
 *
 * @param t thread object to align
 */
static void unwind_pendsv_stack_pointer(struct thread_t *t)
{
    /* Unwind the automatic register saving on the stack on exceptions */
    t->saved.psp += sizeof(t->stack);
    /* Unwind extended stack frame if enabled */
    if (t->extended_stack_frame) {
        t->saved.psp += sizeof(t->extended_stack);
    }

    /* AAPCS requires that stack pointers are 8 byte aligned.
     * In exceptions, bit 9 of the xPSR register indicates whether
     * the CPU was forced to manually re-align the stack. For ARMv6
     * bit 9 is part of the EPSR, while on ARMv8 bit 9 is part of
     * RETPSR. The meaning is the same on both architectures.
     */
    if (t->stack.xpsr & (1 << 9)) {
        /* Undo the forced 8 byte alignment.
         * Because the PSP must be 4 byte aligned at a minimum,
         * the CPU must have pushed 1 word to the stack.
         */
        t->saved.psp += sizeof(uint32_t);
        api->pfDebugOutf("XPSR indicated forced stack alignment: 0x%08X\n", t->stack.xpsr);
    }
}

/**
 * @brief Read stack and registers for given thread.
 *
 * @param t thread object.
 * @return zero on success or negative value on failure.
 */
static int read_stack(struct thread_t *t) {
    int ret;
    if (t->registers_loaded) {
        /* Skip reload, we already have values */
        return 0;
    }
#if !defined(_NO_DEBUG_LOG) && VERBOSE_LOGGING
    api->pfLogOutf("%s (0x%x)\n", __func__, t->base);
#endif

    /* Determine if space for FPU registers has been reserved on the stack */
    t->extended_stack_frame = false;
    if ((num_thread_offsets > OFFSET_THREAD_ARM_EXC_RETURN) &&
        (debug_offsets[OFFSET_THREAD_ARM_EXC_RETURN] != OFFSET_UNIMPLEMENTED)) {
        uint32_t addr = t->base + debug_offsets[OFFSET_THREAD_ARCH] + debug_offsets[OFFSET_THREAD_ARM_EXC_RETURN];
        uint8_t exc_return;
        /* Query the 'mode_exc_return' struct field */
        ret = api->pfReadMem(addr, (char *)&exc_return, sizeof(exc_return));
        if (ret < (int) sizeof(exc_return)) {
            api->pfLogOutf("ERROR: pfReadMem() ret %d\n", ret);
            return ret;
        }
        /* Check for extended stack frames */
        if ((exc_return & EXC_RETURN_FTYPE_MASK) == EXC_RETURN_FTYPE_EXTENDED) {
            api->pfLogOutf("0x%x (%s) is using extended stack frames (FPU)\n", t->base, t->name);
            t->extended_stack_frame = true;
        }
    }

    /* Fetch saved_registers structure from Zephyr's thread structure
     * this strucure contains PSP as well */
    ret = api->pfReadMem(t->base + debug_offsets[OFFSET_THREAD_STACKPTR] - sizeof(t->saved) + sizeof(t->saved.psp), (char *)&t->saved, sizeof(t->saved));
    if (ret < (int) sizeof(t->saved)) {
        api->pfLogOutf("ERROR: pfReadMem() ret %d\n", ret);
        return ret;
    }

    /* Fetch content of the stack using PSP */
    ret = api->pfReadMem(t->saved.psp, (char*)&t->stack, sizeof(t->stack));
    if (ret < (int) sizeof(t->stack)) {
        api->pfLogOutf("ERROR: pfReadMem() ret %d\n", ret);
        return ret;
    }

    /* Unwind the stack pointer saved in PendSV */
    unwind_pendsv_stack_pointer(t);

    t->registers_loaded = true;
    return 0;
}

/**
 * @brief Parse a given thread object and store to our internal list.
 *
 * @param thread_base address of thread object.
 * @param next_thread write next thread object address here.
 * @return zero on success or negative error code on failure.
 */
static int parse_thread(uint32_t thread_base, uint32_t *next_thread)
{
    int r;
    struct thread_t *t = new_thread();
    if (!t) {
        return -1;
    }
    t->base = thread_base;

    /* Fetch name */
    r = api->pfReadMem(t->base + debug_offsets[OFFSET_THREAD_NAME], (char *)&t->name, 32);
    if (r < 1) {
         api->pfLogOutf("pfReadMem() ret %d\n", r);
         return -1;
    }

    /* Fetch next thread */
    r = api->pfReadU32(t->base + debug_offsets[OFFSET_THREAD_NEXT], next_thread);
    if (r) {
         api->pfLogOutf("pfReadU32() ret %d\n", r);
         return -1;
    }

    /* Fetch priority */
    r = api->pfReadU8(t->base + debug_offsets[OFFSET_THREAD_PRIORITY], (void *) &t->prio);
    if (r) {
         api->pfLogOutf("pfReadU8() ret %d\n", r);
         return -1;
    }

    /* Fetch state */
    r = api->pfReadU8(t->base + debug_offsets[OFFSET_THREAD_STATE], &t->state);
    if (r) {
         api->pfLogOutf("pfReadU8() ret %d\n", r);
         return -1;
    }

    api->pfLogOutf("Parsed thread 0x%x (%s)\n", t->base, t->name);
    return 0;
}

static bool is_set(uint8_t val, uint8_t mask) {
    return (val & mask) != 0;
}

/**
 * @brief Convert Zephyr thread state fields to descriptive string.
 *
 * @param state State value.
 * @return string describing thread state.
 */
static const char *state_to_str(struct thread_t *t) {
    if (is_set(t->state, STATE_NOT_STARTED)) {
        return "NOT STARTED";
    } else if (is_set(t->state, STATE_SUSPENDED)) {
        return "SUSPENDED";
    } else if (is_set(t->state, STATE_PENDING)) {
        return "PENDING";
    } else if (is_set(t->state, STATE_QUEUED)) {
        return (current_base==t->base)?"RUNNING":"QUEUED";
    } else if (is_set(t->state, STATE_ABORTING)) {
        return "ABORTING";
    } else if (is_set(t->state, STATE_DEAD)) {
        return "DEAD";
    }
    return "UNKNOWN";
}

static uint32_t swap_endianess(uint32_t x)
{
    return
        ((x >> 24) & 0x000000ff) |
        ((x >> 8 ) & 0x0000ff00) |
        ((x << 8 ) & 0x00ff0000) |
        ((x << 24) & 0xff000000);
}

/*********************************************************************
*
*      JLink GDBServer Plugin API
*
**********************************************************************/

EXPORT int RTOS_Init(const GDB_API *pAPI, uint32_t core) {
    api = pAPI;
    clear();
#ifndef _NO_DEBUG_LOG
    api->pfLogOutf("%s() core = %d\n", __func__, core);
#endif

    /* Check that we support the given core */
    switch (core) {
    case JLINK_CORE_CORTEX_M0:
    case JLINK_CORE_CORTEX_M3:
    case JLINK_CORE_CORTEX_M3_R1P0:
    case JLINK_CORE_CORTEX_M3_R1P1:
    case JLINK_CORE_CORTEX_M3_R2P0:
    case JLINK_CORE_CORTEX_M4:
    case JLINK_CORE_CORTEX_M7:
    case JLINK_CORE_CORTEX_M_V8MAINL:
    case JLINK_CORE_CORTEX_M_V8BASEL:
        return 1;
    default:
        return 0;
    }
}

EXPORT uint32_t RTOS_GetVersion(void) {
    return PLUGIN_VERSION;
}

EXPORT RTOS_SYMBOLS* RTOS_GetSymbols(void) {
#ifndef _NO_DEBUG_LOG
    api->pfLogOutf("%s()\n", __func__);
#endif
    return _Symbols;
}

EXPORT uint32_t RTOS_GetCurrentThreadId(void) {
#if !defined(_NO_DEBUG_LOG) && VERBOSE_LOGGING
    api->pfLogOutf("%s()\n", __func__);
#endif
    return current_base;
}

EXPORT uint32_t RTOS_GetThreadId(uint32_t n) {
    struct thread_t *t = thread(n);
#if !defined(_NO_DEBUG_LOG) && VERBOSE_LOGGING
    api->pfLogOutf("%s(%d)\n", __func__, n);
#endif
    if (t)
        return t->base;
    else
        return 0;
}

EXPORT int RTOS_GetThreadDisplay(char *pDisplay, uint32_t threadid) {
    struct thread_t *t = base_to_thread(threadid);
#if !defined(_NO_DEBUG_LOG) && VERBOSE_LOGGING
    api->pfLogOutf("%s(*, %d)\n", __func__, threadid);
#endif
    if (t) {
        return sprintf(pDisplay, "%.32s %s PRIO %hhu", t->name, state_to_str(t), t->prio);
    }
    return sprintf(pDisplay, "Unknown thread");
}


EXPORT int RTOS_GetThreadReg(char *pHexRegVal, uint32_t RegIndex, uint32_t threadid) {
    struct thread_t *t;
    int ret;
    uint32_t rval;

#if !defined(_NO_DEBUG_LOG) && VERBOSE_LOGGING
    api->pfLogOutf("%s(, %d, %d)\n", __func__, RegIndex, threadid);
#endif
    if (threadid == 0 || threadid == current_base) {
        return -1;
    }
    t = base_to_thread(threadid);
    if (!t) {
        api->pfLogOutf("unknown thread\n");
        return -2;
    }

    ret = read_stack(t);
    if (ret) {
        api->pfLogOutf("read_stack() failed\n");
        return -2;
    }

    rval = get_regval(t, RegIndex);
    sprintf(pHexRegVal, "%08x", swap_endianess(rval));
    return 0;
}

EXPORT int RTOS_GetThreadRegList(char *pHexRegList, uint32_t threadid) {
    struct thread_t *t;
    int ret;
    int i;

#if !defined(_NO_DEBUG_LOG) && VERBOSE_LOGGING
    api->pfLogOutf("%s(, %d)\n", __func__, threadid);
#endif
    if (threadid == 0 || threadid == current_base) {
        return -1;
    }
    t = base_to_thread(threadid);
    if (!t) {
        api->pfLogOutf("unknown thread\n");
        return -2;
    }
    ret = read_stack(t);
    if (ret) {
        api->pfLogOutf("read_stack() failed\n");
        return -1;
    }

    /* This format is undocumented, but apparently I need to send X number of
     * register values using hex string and on a low-endian format. I'm using swap_endianess() to
     * turn the value "upside down" so the output is what JLink expects.
     * Seggers embOS example sends 17 registers, so I do the same, but I don't know how many
     * I should really send.
     */
    for (i = 0; i < 17; ++i) {
        uint32_t rval = get_regval(t, i);
        pHexRegList += sprintf(pHexRegList, "%08x", swap_endianess(rval));
    }
    return 0;
}

EXPORT int RTOS_SetThreadReg(char* pHexRegVal, uint32_t RegIndex, uint32_t threadid) {
    (void) pHexRegVal;
    (void) RegIndex;
    (void) threadid;
    // not supported
    return -1;
}

EXPORT int RTOS_SetThreadRegList(char *pHexRegList, uint32_t threadid) {
    (void) pHexRegList;
    (void) threadid;
    // not supported
    return -1;
}

EXPORT int RTOS_UpdateThreads(void) {
    int r;
    uint32_t thread;
    uint32_t next;
    uint32_t post_kern;
    uint32_t thread_info_size_t_size_addr = _Symbols[SYMBOLS_INDEX_SIZE_T_SIZE].address;
    uint32_t thread_info_num_offsets_addr = _Symbols[SYMBOLS_INDEX_NUM_THREAD_INFO].address;
    uint32_t thread_info_offsets_addr = _Symbols[SYMBOLS_INDEX_THREAD_INFO].address;
    uint32_t kern_addr = _Symbols[SYMBOLS_INDEX_KERNEL].address;
    static bool offset_summary_printed;

#if !defined(_NO_DEBUG_LOG) && VERBOSE_LOGGING
    api->pfLogOutf("%s()\n", __func__);
#endif
    threads_updated = true;

    if (thread_info_offsets_addr == 0 || kern_addr == 0) {
        api->pfLogOutf("RTOS_UpdateThreads() thread_info symbols not found\n");
        return -1;
    }

    clear();

    post_kern = _Symbols[SYMBOLS_INDEX_POST_KERNEL].address;
    if (post_kern) {
        uint8_t flag;
        r = api->pfReadU8(post_kern, &flag);
        if (r) {
            api->pfLogOutf("pfReadU8() returned %d\n", r);
            return -1;
        }
        if (flag == 0) {
            api->pfLogOutf("kernel not running!\n");
            return -1;
        }
    }

    /* Get size of size_t */
    r = api->pfReadMem(thread_info_size_t_size_addr, (char *)&size_t_size, 1);
    if (r != sizeof(uint8_t)) {
        api->pfLogOutf("pfReadMem() returned %d expected %d\n", r, sizeof(uint8_t));
        return -2;
    }
    if(size_t_size > sizeof(num_thread_offsets)) {
        api->pfLogOutf("size_t is too large to fit in space allocated for variables (%d > %d)\n",
                       size_t_size, sizeof(num_thread_offsets));
        return -2;
    }

    /* Query number of thread_info offsets.
     * Reading 32 bits into a 64bit variable is fine as it is zeroed and little endian.
     */
    r = api->pfReadMem(thread_info_num_offsets_addr, (char *)&num_thread_offsets, size_t_size);
    if (r != size_t_size) {
        api->pfLogOutf("pfReadMem() returned %d expected %d\n", r, size_t_size);
        return -2;
    }

    if (!offset_summary_printed) {
        /* __func__ is not always available so manually specify function name */
        api->pfLogOutf("RTOS_UpdateThreads(): size_t is %d bytes\n", size_t_size);
        api->pfLogOutf("RTOS_UpdateThreads(): _kernel_thread_info_offsets contains %d offsets\n", num_thread_offsets);
        offset_summary_printed = true;
    }

    /* Fetch the thread_info structure, that contains all the required offset values */
    r = api->pfReadMem(thread_info_offsets_addr, (char *)debug_offsets, sizeof(debug_offsets));
    if (r != sizeof(uint32_t) * LEN_DEBUG_OFFSETS) {
        api->pfLogOutf("pfReadMem() returned %d expected %d\n", r, sizeof(debug_offsets));
        return -2;
    }

    /* Get pointer to current thread */
    r = api->pfReadU32(kern_addr + debug_offsets[OFFSET_CURRENT], &current_base);
    if (r != 0) {
        api->pfLogOutf("pfReadU32() returned %d\n", r);
        return -1;
    }

    /* Find a pointer to first thread from the list */
    r = api->pfReadU32(kern_addr + debug_offsets[OFFSET_THREADS], &thread);
    if (r != 0) {
        api->pfLogOutf("pfReadU32() ret %d\n", r);
        return -3;
    }
    for (next = 0; thread; thread = next) {
        r = parse_thread(thread, &next);
        if (r) {
            api->pfLogOutf("parse_thread() failed\n");
            return -4;
        }
    }

    /* In case we are not in Thread mode, append the exception handler to list */
    update_handler_thread();

#ifndef _NO_DEBUG_LOG
    api->pfLogOutf("%s(): Updated %d threads\n", __func__, n_threads());
#endif

    return 0;
}

EXPORT uint32_t RTOS_GetNumThreads(void) {
#if !defined(_NO_DEBUG_LOG) && VERBOSE_LOGGING
    api->pfLogOutf("%s() --> %d\n", __func__, n_threads());
#endif
    /* RTOS_GetNumThreads can be called before RTOS_UpdateThreads when first
     * attaching a debugger. As RTOS_UpdateThreads populates the thread
     * structures, this means no thread information would be displayed until
     * the next time the debugger pauses. Manually running RTOS_UpdateThreads
     * in this situation ensures that thread information is available
     * immediately.
     */
    if (!threads_updated) {
        if (RTOS_UpdateThreads() < 0) {
            return 0;
        }
    }
    return n_threads();
}
