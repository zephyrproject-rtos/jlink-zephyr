/*
 * Zephyr RTOS plugin for JlinkGDBServer.
 *
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "RTOSPlugin.h"

#include "acutest.h"


/*************************
 * Dummy data
 *************************/

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
};
#define LEN_DEBUG_OFFSETS (OFFSET_THREAD_ARCH+1)


struct my_kernel {
	uint32_t version;
	uint32_t current;
	uint32_t threads;
};

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

struct my_thread {
	char name[32];
	struct saved_registers saved;
	uint32_t priority;
	uint32_t arch;
	uint32_t state;
	uint32_t next;
};

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

struct dummy_data {
	uint32_t padding[1]; // Cannot start from offset zero.
	struct my_kernel kernel;
	uint32_t debug_offsets[LEN_DEBUG_OFFSETS];
	struct my_thread thread1;
	struct my_thread thread2;
	struct stack stack;
	uint8_t extra_mem[2048];
} dummy_data = {
	.kernel = {
		.version = 1,
		.current = offsetof(struct dummy_data, thread1),
		.threads = offsetof(struct dummy_data, thread1)
	},
	.thread1 = {
		.name = "First Thread",
		.saved = {.psp = offsetof(struct dummy_data, stack) },
		.next = offsetof(struct dummy_data, thread2)
	},
	.thread2 = {
		.name = "Second thread",
		.saved = {.psp = offsetof(struct dummy_data, stack) }
	},
	.stack = {
		.r0 = 0xdeadbeef,
		.pc = 0xcafebabe,
	},
	.debug_offsets = {
	offsetof(struct my_kernel, version),
	offsetof(struct my_kernel, current),
	offsetof(struct my_kernel, threads),
	offsetof(struct my_thread, next),
	offsetof(struct my_thread, next),
	offsetof(struct my_thread, state),
	offsetof(struct my_thread, state),
	offsetof(struct my_thread, priority),
	offsetof(struct my_thread, saved.psp),
	offsetof(struct my_thread, name),
	offsetof(struct my_thread, arch),
	}
};

uint8_t *mem = (uint8_t *) &dummy_data;



/*************************
 * GDB API
 *************************/

void pfFree(void* p)
{
	free(p);
}

void* pfAlloc(unsigned Size)
{
	return malloc(Size);
}

void* pfRealloc(void* p, unsigned Size)
{
	return realloc(p, Size);
}

void pfLogOutf(const char* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);
	vprintf(sFormat, args);
	va_end(args);
}

int pfReadMem(U32 Addr, char* pData, unsigned int NumBytes)
{
	memcpy(pData, &mem[Addr], NumBytes);
	return NumBytes;
}

char pfReadU8(U32 Addr, U8* pData)
{
	memcpy(pData, &mem[Addr], 1);
	return 0;
}

char pfReadU16(U32 Addr, U16* pData)
{
	memcpy(pData, &mem[Addr], 2);
	return 0;
}

char pfReadU32(U32 Addr, U32* pData)
{
	memcpy(pData, &mem[Addr], 4);
	return 0;
}

int  pfWriteMem(U32 Addr, const char* pData, unsigned NumBytes)
{
	memcpy(&mem[Addr], pData, NumBytes);
	return 0;
}

void pfWriteU8(U32 Addr, U8 Data)
{
	memcpy(&mem[Addr], &Data, 1);
}

void pfWriteU16(U32 Addr, U16 Data)
{
	memcpy(&mem[Addr], &Data, 2);
}

void pfWriteU32(U32 Addr, U32 Data)
{
	memcpy(&mem[Addr], &Data, 4);
}

U32 pfLoad16TE(const U8* p)
{
	uint16_t val;
	memcpy(&val, p, 2);
	return val;
}

U32 pfLoad24TE(const U8* p)
{
	uint32_t val;
	memcpy(&val, p, 3);
	return val;
}
U32 pfLoad32TE(const U8* p)
{
	uint32_t val;
	memcpy(&val, p, 4);
	return val;
}

U32 pfReadReg(U32 RegIndex)
{
	return 0xdeadbeef;
}

static GDB_API api =
{
	.pfFree = pfFree,
	.pfAlloc = pfAlloc,
	.pfRealloc = pfRealloc,
	.pfLogOutf = pfLogOutf,
	.pfDebugOutf = pfLogOutf,
	.pfWarnOutf = pfLogOutf,
	.pfErrorOutf = pfLogOutf,
	.pfReadMem = pfReadMem,
	.pfReadU8 = pfReadU8,
	.pfReadU16 = pfReadU16,
	.pfReadU32 = pfReadU32,
	.pfWriteMem = pfWriteMem,
	.pfWriteU8 = pfWriteU8,
	.pfWriteU16 = pfWriteU16,
	.pfWriteU32 = pfWriteU32,
	.pfLoad16TE = pfLoad16TE,
	.pfLoad24TE = pfLoad16TE,
	.pfLoad32TE = pfLoad16TE,
	.pfReadReg = pfReadReg,
};

/************************* API ***************************/
extern int RTOS_Init(const GDB_API *pAPI, uint32_t core);
extern uint32_t RTOS_GetVersion();
extern RTOS_SYMBOLS* RTOS_GetSymbols();
extern uint32_t RTOS_GetNumThreads();
extern uint32_t RTOS_GetCurrentThreadId();
extern uint32_t RTOS_GetThreadId(uint32_t n);
extern int RTOS_GetThreadDisplay(char *pDisplay, uint32_t threadid);
extern int RTOS_GetThreadReg(char *pHexRegVal, uint32_t RegIndex, uint32_t threadid);
extern int RTOS_GetThreadRegList(char *pHexRegList, uint32_t threadid);
extern int RTOS_SetThreadReg(char* pHexRegVal, uint32_t RegIndex, uint32_t threadid);
extern int RTOS_SetThreadRegList(char *pHexRegList, uint32_t threadid);
extern int RTOS_UpdateThreads();

/********************** TESTS ****************************/

void test_init(void)
{
	/* Hardcoded Cortex-M3 core identifier */
	int ret = RTOS_Init(&api, 0x030000FF);
	TEST_ASSERT(ret == 1);

	RTOS_SYMBOLS *symbols = RTOS_GetSymbols();
	TEST_ASSERT(symbols != NULL);
	symbols[0].address = offsetof(struct dummy_data, kernel);
	symbols[1].address = offsetof(struct dummy_data, debug_offsets);
}

void test_parsing(void)
{
	test_init();
	int ret = RTOS_UpdateThreads();
	TEST_ASSERT(ret == 0);

	uint32_t n = RTOS_GetNumThreads();
	TEST_CHECK(n == 2);

	uint32_t id = RTOS_GetCurrentThreadId();
	TEST_CHECK(id != 0);

	uint32_t id2 = RTOS_GetThreadId(0);
	uint32_t id1 = RTOS_GetThreadId(1);
	TEST_CHECK(id == id1);

	char val[10];
	ret = RTOS_GetThreadReg(val, RTOS_PLUGIN_CPU_REG_CORTEX_M_PC, id2);
	TEST_CHECK(ret == 0);
	TEST_CHECK(0 == strcmp("bebafeca", val));
	ret = RTOS_GetThreadReg(val, RTOS_PLUGIN_CPU_REG_CORTEX_M_R0, id2);
	TEST_CHECK(ret == 0);
	TEST_CHECK(0 == strcmp("efbeadde", val));

	// Force clear
	RTOS_Init(&api, 0);
}

TEST_LIST = {
   { "test_init", test_init },
   { "test_parsing", test_parsing},
   { NULL, NULL }     /* zeroed record marking the end of the list */
};
