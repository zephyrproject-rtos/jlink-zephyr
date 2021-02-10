/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (C) 2004 - 2005    SEGGER Microcontroller Systeme GmbH      *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : JLinkARMDLL.h
Purpose : "C"- level interface of <JLinkARM.dll>
---------------------------END-OF-HEADER------------------------------
*/

#ifndef JLINKARM_CONST_H            //  Avoid multiple inclusion
#define JLINKARM_CONST_H

#ifdef _WIN32
  #ifndef WIN32
    #define WIN32
# endif
#endif

#include "TYPES.h"

#ifdef WIN32
  #define STDCALL __stdcall
#else
  #define STDCALL
#endif

/*********************************************************************
*
*       Defines required for API
*
**********************************************************************
*/
//
// J-Link virtually maps the different memory types of the 8051 to different 16 MB areas
// Do NOT change after C2 support has been released, since DLL + Firmware rely on this layout
// Layout:
//   Area0: CODE
//   Area1: IDATA
//   Area2: DDATA
//   Area3: XDATA
//   Area4: DSR
//
#define JLINK_EFM8_VIRTUAL_AREA_SIZE         (0x1000000)

#define JLINK_EFM8_START_ADDR_CODE           (0x0000000)                                                   // 64KB. First 32KB are fixed, the other 32KB are the current bank
#define JLINK_EFM8_START_ADDR_IDATA          (JLINK_EFM8_START_ADDR_CODE + JLINK_EFM8_VIRTUAL_AREA_SIZE)   // 256 bytes
#define JLINK_EFM8_START_ADDR_DDATA          (JLINK_EFM8_START_ADDR_IDATA + JLINK_EFM8_VIRTUAL_AREA_SIZE)  // 256 bytes  00-7F is RAM, with register banks as usual. 80-FF is the sfr area of the currently selected bank
#define JLINK_EFM8_START_ADDR_XDATA          (JLINK_EFM8_START_ADDR_DDATA + JLINK_EFM8_VIRTUAL_AREA_SIZE)
#define JLINK_EFM8_START_ADDR_DSR            (JLINK_EFM8_START_ADDR_XDATA + JLINK_EFM8_VIRTUAL_AREA_SIZE)  // Virtually 64 KB. Maps non-memory mapped DSR registers to virtual memory, to allow easy access to them
#define JLINK_EFM8_START_ADDR_C2             (JLINK_EFM8_START_ADDR_DSR + JLINK_EFM8_VIRTUAL_AREA_SIZE)    // 256 bytes. Allows memory accesses directly via C2
//
// Explicit access to different DDATA pages
//
#define JLINK_EFM8_START_ADDR_DDATA_PAGE0    (JLINK_EFM8_START_ADDR_DDATA + 0x00100)                       // 256 bytes. Same as DDATA, only sfrs are page-0 sfrs (no matter what SFRPAGE register currently selects)
#define JLINK_EFM8_START_ADDR_DDATA_PAGE1    (JLINK_EFM8_START_ADDR_DDATA + 0x00200)                       // 256 bytes. Same as DDATA, only sfrs are page-1 sfrs (no matter what SFRPAGE register currently selects)
#define JLINK_EFM8_START_ADDR_DDATA_PAGE2    (JLINK_EFM8_START_ADDR_DDATA + 0x00300)                       // 256 bytes. Same as DDATA, only sfrs are page-2 sfrs (no matter what SFRPAGE register currently selects)
// [...] DDATA_PAGE2-xxx
//
// Explicit access to different flash areas
// Allows debugger to perform a continuous flash download via a single memory-write
//
#define JLINK_EFM8_START_ADDR_CODE_BANK0     (JLINK_EFM8_START_ADDR_CODE + 0x10000)                        // 32KB, Always visible at 0x30000. Also be visible at 0x38000 if PSBANK == 0x00
#define JLINK_EFM8_START_ADDR_CODE_BANK1     (JLINK_EFM8_START_ADDR_CODE + 0x18000)                        // 32KB, Also be visible at 0x38000 if PSBANK == 0x11
#define JLINK_EFM8_START_ADDR_CODE_BANK2     (JLINK_EFM8_START_ADDR_CODE + 0x20000)                        // 32KB, Also visible at 0x38000 if PSBANK register == 0x22
//[...] CODE_BANK3-xxx

//
// Virtual offsets of non-memory mapped DSR registers, into DSR zone
//
#define JLINK_EFM8_OFF_REG_DSR_VERSION       (0x00)
#define JLINK_EFM8_OFF_REG_DSR_DERIVATIVE    (0x01)

#define JLINK_EFM8_FLACL_FORMAT_LOCKBYTE_COMPLEMENT                     (0)
#define JLINK_EFM8_FLACL_FORMAT_LOCKBYTE                                (1)

#define JLINK_EFM8_MEM_ACC_TYPE_D     (0)
#define JLINK_EFM8_MEM_ACC_TYPE_I     (1)
#define JLINK_EFM8_MEM_ACC_TYPE_C2    (2)


#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_DSRCMD                 ( 0)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BPENA                  ( 1)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP0L                   ( 2)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP0H                   ( 3)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP1L                   ( 4)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP1H                   ( 5)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP2L                   ( 6)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP2H                   ( 7)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP3L                   ( 8)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP3H                   ( 9)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP4L                   (10)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP4H                   (11)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP5L                   (12)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP5H                   (13)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP6L                   (14)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP6H                   (15)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP7L                   (16)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_BP7H                   (17)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_R0B0             (18)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_R1B0             (19)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_R2B0             (20)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_DPTR_L           (21)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_DPTR_H           (22)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_PSW              (23)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_A                (24)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_SECTOR_SIZE_SHIFT           (25)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_PSBANK                 (26)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_SFRPAGE                (27)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_XRAM_ACC_MODE               (28)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_XADRH                  (29)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_XADRL                  (30)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_XDATA                  (31)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_FLSCL                  (32)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_RSTSRC                 (33)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_VDM0CN                 (34)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_NUM_SECTORS                 (35)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_FLACL                  (36)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_FLACL_SFRPAGE               (37)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_FLACL_FORMAT                (38)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDRL_DFLASH_START          (39)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDRH_DFLASH_START          (40)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DFLASH_SECTOR_SIZE_SHIFT    (41)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_SFRPAGE          (42)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_PSCTL                  (43)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_PSCTL            (44)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_CLKSEL                 (45)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_CLKSEL           (46)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_ADDR_FLKEY                  (47)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_DEBUG_ADDR_FLKEY            (48)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR0_ADDR             (49)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR0_DEBUG_ADDR       (50)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR0_ACC_TYPE         (51)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR1_ADDR             (52)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR1_DEBUG_ADDR       (53)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR1_ACC_TYPE         (54)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR2_ADDR             (55)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR2_DEBUG_ADDR       (56)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR2_ACC_TYPE         (57)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR3_ADDR             (58)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR3_DEBUG_ADDR       (59)
#define JLINK_EFM8_CUST_CPU_MODULE_DATA_OFF_REMAP_SFR3_ACC_TYPE         (60)

/*********************************************************************
*
*       Emulator product IDs
*
*/
#define JLINK_EMU_PRODUCT_ID_UNKNOWN            (0x00)
#define JLINK_EMU_PRODUCT_ID_JLINK              (0x01)
#define JLINK_EMU_PRODUCT_ID_JLINK_CF           (0x02)
#define JLINK_EMU_PRODUCT_ID_JLINK_CE           (0x03)
#define JLINK_EMU_PRODUCT_ID_JLINK_KS           (0x04)
#define JLINK_EMU_PRODUCT_ID_DIGI_LINK          (0x05)
#define JLINK_EMU_PRODUCT_ID_MIDAS              (0x06)
#define JLINK_EMU_PRODUCT_ID_SAMICE             (0x07)
#define JLINK_EMU_PRODUCT_ID_JTRACE             (0x08)
#define JLINK_EMU_PRODUCT_ID_JTRACE_CS          (0x09)
#define JLINK_EMU_PRODUCT_ID_FLASHER_ARM        (0x0A)
#define JLINK_EMU_PRODUCT_ID_JLINK_PRO          (0x0B)
#define JLINK_EMU_PRODUCT_ID_JLINK_EDU          (0x0C)
#define JLINK_EMU_PRODUCT_ID_JLINK_ULTRA        (0x0D)
#define JLINK_EMU_PRODUCT_ID_FLASHER_PPC        (0x0E)
#define JLINK_EMU_PRODUCT_ID_FLASHER_RX         (0x0F)
#define JLINK_EMU_PRODUCT_ID_JLINK_OB_RX200     (0x10)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE         (0x11)
#define JLINK_EMU_PRODUCT_ID_JLINK_OB_SAM3U128  (0x12)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_CORTEXM (0x13)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_LPC     (0x14)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_STM32   (0x15)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_FSL     (0x16)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_ADI     (0x17)
#define JLINK_EMU_PRODUCT_ID_ENERGY_MICRO_EFM32 (0x18)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_XMC4000 (0x19)
#define JLINK_EMU_PRODUCT_ID_JLINK_LITE_XMC4200 (0x20)
#define JLINK_EMU_PRODUCT_ID_JLINK_LPC_LINK2    (0x21)
#define JLINK_EMU_PRODUCT_ID_FLASHER_PRO        (0x22)

/*********************************************************************
*
*       Global error codes
*
*  The error codes which are listed here can be returned by
*  any DLL API-function which has a return value.
*/
#define JLINK_ERR_EMU_NO_CONNECTION            -256 // (0xFFFFFF00) No connection to emulator / Connection to emulator lost
#define JLINK_ERR_EMU_COMM_ERROR               -257 // (0xFFFFFEFF) Emulator communication error (host-interface module reproted error)
#define JLINK_ERR_DLL_NOT_OPEN                 -258 // (0xFFFFFEFE) DLL has not been opened but needs to be (JLINKARM_Open() needs to be called first)
#define JLINK_ERR_VCC_FAILURE                  -259 // (0xFFFFFEFD) Target system has no power (Measured VTref < 1V)
#define JLINK_ERR_INVALID_HANDLE               -260 // (0xFFFFFEFC) File handle / memory area handle needed for operation, but given handle is not valid
#define JLINK_ERR_NO_CPU_FOUND                 -261 // (0xFFFFFEFB) Could not find supported CPU
#define JLINK_ERR_EMU_FEATURE_NOT_SUPPORTED    -262 // (0xFFFFFEFA) Emulator does not support the selected feature (Usually returned by functions which need specific emulator capabilities)
#define JLINK_ERR_EMU_NO_MEMORY                -263 // (0xFFFFFEF9) Emulator does not have enough memory to perform the requested operation
#define JLINK_ERR_TIF_STATUS_ERROR             -264 // (0xFFFFFEF8) Things such as "TCK is low but should be high"
#define JLINK_ERR_FLASH_PROG_COMPARE_FAILED    -265
#define JLINK_ERR_FLASH_PROG_PROGRAM_FAILED    -266
#define JLINK_ERR_FLASH_PROG_VERIFY_FAILED     -267
#define JLINK_ERR_OPEN_FILE_FAILED             -268
#define JLINK_ERR_UNKNOWN_FILE_FORMAT          -269
#define JLINK_ERR_WRITE_TARGET_MEMORY_FAILED   -270
#define JLINK_ERR_DEVICE_FEATURE_NOT_SUPPORTED -271 // Connected device does not support specific feature
#define JLINK_ERR_WRONG_USER_CONFIG            -272 // User configured DLL parameters incorrectly
#define JLINK_ERR_NO_TARGET_DEVICE_SELECTED    -273 // User did not specify the core / device J-Link shall connect to
#define JLINK_ERR_CPU_IN_LOW_POWER_MODE        -274

/*********************************************************************
*
*       Memory access errors
*/
#define JLINKARM_ERR_CPU_NO_CLOCK  0x80000001
#define JLINKARM_ERR_CPU_NO_POWER  0x80000002

/*********************************************************************
*
*       JLINKARM_FLASH_ API error codes
*/
#define JLINK_FLASH_ERR_BLOCK_VERIFICATION_ERROR 1
#define JLINK_FLASH_ERR_ITEM_VERIFICATION_ERROR  2
#define JLINK_FLASH_ERR_TIMEOUT                  3
#define JLINK_FLASH_ERR_PROGRAM_ERROR            4
#define JLINK_FLASH_ERR_PROGRAM_1_OVER_0         5
#define JLINK_FLASH_ERR_SECTOR_IS_LOCKED         6
#define JLINK_FLASH_ERR_ERASE_ERROR              7
#define JLINK_FLASH_ERR_NO_FLASH_MEMORY          8
#define JLINK_FLASH_ERR_GENERIC_ERROR            9
#define JLINK_FLASH_ERR_ALGO_SPECIFIC_ERROR     -2
#define JLINK_FLASH_ERR_NO_FLASH_BANK           -3
#define JLINK_FLASH_ERR_PROGRAM_DOES_NOT_FIT    -4

/*********************************************************************
*
*       Pin status
*/
#define JLINKARM_PIN_STATUS_LOW       0
#define JLINKARM_PIN_STATUS_HIGH      1
#define JLINKARM_PIN_STATUS_UNKNOWN   255

/*********************************************************************
*
*       PIN override functionalities
*/
#define JLINK_PIN_OVERRIDE_MODE_RELEASE         0  // Release pin from override mode
#define JLINK_PIN_OVERRIDE_MODE_PIO_IN          1
#define JLINK_PIN_OVERRIDE_MODE_PIO_OUT_LOW     2
#define JLINK_PIN_OVERRIDE_MODE_PIO_OUT_HIGH    3
#define JLINK_PIN_OVERRIDE_MODE_UART_TX         4
#define JLINK_PIN_OVERRIDE_MODE_UART_RX         5
#define JLINK_PIN_OVERRIDE_MODE_UART_RXTX       6  // Half-duplex UART where Rx and Tx are on the same pin

/*********************************************************************
*
*       Interfaces
*/
#define JLINKARM_TIF_JTAG              0
#define JLINKARM_TIF_SWD               1
#define JLINKARM_TIF_BDM3              2  // Do NOT use. Not supported anymore. Only there for backward compatbility inside the DLL
#define JLINKARM_TIF_FINE              3
#define JLINKARM_TIF_ICSP              4
#define JLINKARM_TIF_SPI               5
#define JLINKARM_TIF_C2                6

/*********************************************************************
*
*       HSS flags that can be passed on Start()
*       Max. 32 Flags
*/
#define JLINK_HSS_FLAG_TIMESTAMP_US           (1uL << 0)

/*********************************************************************
*
*       PIN defines
*/
#define JLINK_PIN_MAX_NUM_PINS                    (8)

/*********************************************************************
*
*       SPI flags
*/
#define JLINK_SPI_FLAG_MASK_CS_START_STATE        (3 << 0)
#define JLINK_SPI_FLAG_MASK_CS_END_STATE          (3 << 2)
#define JLINK_SPI_FLAG_MASK_HANDLE_CS_ON_REPEATS  (1 << 4)
#define JLINK_SPI_FLAG_CS_START_STATE_U           (0 << 0)
#define JLINK_SPI_FLAG_CS_START_STATE_0           (2 << 0)
#define JLINK_SPI_FLAG_CS_START_STATE_1           (3 << 0)
#define JLINK_SPI_FLAG_CS_END_STATE_U             (0 << 2)
#define JLINK_SPI_FLAG_CS_END_STATE_0             (2 << 2)
#define JLINK_SPI_FLAG_CS_END_STATE_1             (3 << 2)
#define JLINK_SPI_FLAG_HANDLE_CS_ON_REPEATS       (1 << 4)

#define JLINK_SPI_CMD_TRANSFER                    (1)
#define JLINK_SPI_CMD_TRANSFER_UNTIL              (2)

/*********************************************************************
*
*       STRACE commands
*/
#define JLINK_STRACE_CMD_SET_TRACE_EVENT          (0)
#define JLINK_STRACE_CMD_CLR_TRACE_EVENT          (1)
#define JLINK_STRACE_CMD_CLR_ALL_TRACE_EVENTS     (2)
#define JLINK_STRACE_CMD_SET_BUFF_SIZE            (3)

/*********************************************************************
*
*       STRACE event and operation specifiers
*/
#define JLINK_STRACE_EVENT_TYPE_CODE_FETCH  (0)
#define JLINK_STRACE_EVENT_TYPE_DATA_ACC    (1)
#define JLINK_STRACE_EVENT_TYPE_DATA_LOAD   (2)
#define JLINK_STRACE_EVENT_TYPE_DATA_STORE  (3)

#define JLINK_STRACE_OP_TRACE_START         (0)
#define JLINK_STRACE_OP_TRACE_STOP          (1)
#define JLINK_STRACE_OP_TRACE_INCLUDE_RANGE (2)
#define JLINK_STRACE_OP_TRACE_EXCLUDE_RANGE (3)

/*********************************************************************
*
*       Device families
*/
#define JLINKARM_DEV_FAMILY_AUTO          0
#define JLINKARM_DEV_FAMILY_CM1           1     // Cortex-M1
#define JLINKARM_DEV_FAMILY_CF            2     // ColdFire
#define JLINKARM_DEV_FAMILY_CM3           3     // Cortex-M3
#define JLINKARM_DEV_FAMILY_SIM           4     // Simulator
#define JLINKARM_DEV_FAMILY_XSCALE        5     // XScale
#define JLINKARM_DEV_FAMILY_CM0           6     // Cortex-M0
#define JLINKARM_DEV_FAMILY_ARM7          7     // ARM7
#define JLINKARM_DEV_FAMILY_CORTEX_A8     8     // Cortex-A8 / Cortex-A9
#define JLINKARM_DEV_FAMILY_ARM9          9     // ARM9
#define JLINKARM_DEV_FAMILY_ARM10        10     // ARM10
#define JLINKARM_DEV_FAMILY_ARM11        11     // ARM11
#define JLINKARM_DEV_FAMILY_CORTEX_R4    12     // Cortex-R4
#define JLINKARM_DEV_FAMILY_RX           13     // Renesas RX core
#define JLINKARM_DEV_FAMILY_CM4          14     // Cortex-M4
#define JLINKARM_DEV_FAMILY_CORTEX_A5    15     // Cortex-A5
#define JLINKARM_DEV_FAMILY_POWER_PC     16     // PowerPC
#define JLINK_DEV_FAMILY_MIPS            17     // MIPS architecture
#define JLINK_DEV_FAMILY_EFM8            18     // SiLabs EFM8 (8-bit 8051 derivates)
#define JLINKARM_DEV_FAMILY_ANY         255

/*********************************************************************
*
*       Cores
*/
#define JLINK_CORE_NONE             0x00000000
#define JLINK_CORE_ANY              0xFFFFFFFF
#define JLINK_CORE_CORTEX_M1        0x010000FF
#define JLINK_CORE_COLDFIRE         0x02FFFFFF
#define JLINK_CORE_CORTEX_M3        0x030000FF
#define JLINK_CORE_CORTEX_M3_R1P0   0x03000010
#define JLINK_CORE_CORTEX_M3_R1P1   0x03000011
#define JLINK_CORE_CORTEX_M3_R2P0   0x03000020
#define JLINK_CORE_SIM              0x04FFFFFF
#define JLINK_CORE_XSCALE           0x05FFFFFF
#define JLINK_CORE_CORTEX_M0        0x060000FF
#define JLINK_CORE_CORTEX_M_V8BASEL 0x060100FF
#define JLINK_CORE_ARM7             0x07FFFFFF
#define JLINK_CORE_ARM7TDMI         0x070000FF
#define JLINK_CORE_ARM7TDMI_R3      0x0700003F
#define JLINK_CORE_ARM7TDMI_R4      0x0700004F
#define JLINK_CORE_ARM7TDMI_S       0x070001FF
#define JLINK_CORE_ARM7TDMI_S_R3    0x0700013F
#define JLINK_CORE_ARM7TDMI_S_R4    0x0700014F
//
// Order of Cortex-A cores looks unfortunate but we cannot change it anymore
// since we need to stay binary compatible
//
#define JLINK_CORE_CORTEX_A8        0x080000FF
#define JLINK_CORE_CORTEX_A7        0x080800FF  // Same family as Cortex-A9. Only low-level differences
#define JLINK_CORE_CORTEX_A9        0x080900FF  // Cortex-A9. Cortex-A8 compatible only small differences for future multi-core debugging support.
#define JLINK_CORE_CORTEX_A12       0x080A00FF  // Same family as Cortex-A9. Only low-level differences
#define JLINK_CORE_CORTEX_A15       0x080B00FF  // Same family as Cortex-A9. Only low-level differences
#define JLINK_CORE_CORTEX_A17       0x080C00FF  // Same family as Cortex-A9. Only low-level differences
#define JLINK_CORE_ARM9             0x09FFFFFF
#define JLINK_CORE_ARM9TDMI_S       0x090001FF
#define JLINK_CORE_ARM920T          0x092000FF
#define JLINK_CORE_ARM922T          0x092200FF
#define JLINK_CORE_ARM926EJ_S       0x092601FF
#define JLINK_CORE_ARM946E_S        0x094601FF
#define JLINK_CORE_ARM966E_S        0x096601FF
#define JLINK_CORE_ARM968E_S        0x096801FF
#define JLINK_CORE_ARM11            0x0BFFFFFF
#define JLINK_CORE_ARM1136          0x0B36FFFF
#define JLINK_CORE_ARM1136J         0x0B3602FF
#define JLINK_CORE_ARM1136J_S       0x0B3603FF
#define JLINK_CORE_ARM1136JF        0x0B3606FF
#define JLINK_CORE_ARM1136JF_S      0x0B3607FF
#define JLINK_CORE_ARM1156          0x0B56FFFF
#define JLINK_CORE_ARM1176          0x0B76FFFF
#define JLINK_CORE_ARM1176J         0x0B7602FF
#define JLINK_CORE_ARM1176J_S       0x0B7603FF
#define JLINK_CORE_ARM1176JF        0x0B7606FF
#define JLINK_CORE_ARM1176JF_S      0x0B7607FF
#define JLINK_CORE_CORTEX_R4        0x0C0000FF  // Device family: 0x0D => Cortex-R, sub-family 0x00: Cortex-R4
#define JLINK_CORE_CORTEX_R5        0x0C0100FF  // Device family: 0x0D => Cortex-R, sub-family 0x01: Cortex-R5
//
// RX is somewhat difficult
// We need to distinguish between them on a low-level-core basis because handling is different for some families
// This is why there is not a single "JLINK_CORE_RX" define
//
#define JLINK_CORE_RX               0x0DFFFFFF  // Device family: 0x0D, sub family not specified, revision not specified
// RX61x, RX62x, RX63x sub family
#define JLINK_CORE_RX610            0x0D00FFFF  // Device family: 0x0D, sub family 0x00, revision not specified
#define JLINK_CORE_RX62N            0x0D01FFFF  // Device family: 0x0D, sub family 0x01, revision not specified
#define JLINK_CORE_RX62T            0x0D02FFFF
#define JLINK_CORE_RX63N            0x0D03FFFF
#define JLINK_CORE_RX630            0x0D04FFFF
#define JLINK_CORE_RX63T            0x0D05FFFF
#define JLINK_CORE_RX621            0x0D06FFFF
#define JLINK_CORE_RX62G            0x0D07FFFF
#define JLINK_CORE_RX631            0x0D08FFFF
#define JLINK_CORE_RX65N            0x0D09FFFF
// RX2xx sub family
#define JLINK_CORE_RX210            0x0D10FFFF
#define JLINK_CORE_RX21A            0x0D11FFFF
#define JLINK_CORE_RX220            0x0D12FFFF
#define JLINK_CORE_RX230            0x0D13FFFF
#define JLINK_CORE_RX231            0x0D14FFFF
#define JLINK_CORE_RX23T            0x0D15FFFF
#define JLINK_CORE_RX24T            0x0D16FFFF
// RX1xx sub family
#define JLINK_CORE_RX111            0x0D20FFFF  // Device family: 0x0D, sub family 0x20, revision not specified
#define JLINK_CORE_RX110            0x0D21FFFF
#define JLINK_CORE_RX113            0x0D22FFFF
#define JLINK_CORE_RX130            0x0D23FFFF
// RX64x sub family
#define JLINK_CORE_RX64M            0x0D30FFFF
#define JLINK_CORE_RX71M            0x0D31FFFF

#define JLINK_CORE_CORTEX_M4        0x0E0000FF
#define JLINK_CORE_CORTEX_M7        0x0E0100FF  // Device family: 0x0E (M4), sub family: 0x01
#define JLINK_CORE_CORTEX_M_V8MAINL 0x0E0200FF
#define JLINK_CORE_CORTEX_A5        0x0F0000FF
#define JLINK_CORE_POWER_PC         0x10FFFFFF
#define JLINK_CORE_POWER_PC_N1      0x10FF00FF  // Core with Nexus-1  support
#define JLINK_CORE_POWER_PC_N2      0x10FF01FF  // Core with Nexus-2+ support
#define JLINK_CORE_MIPS             0x11FFFFFF  // Dev family: MIPS, sub family not specified
#define JLINK_CORE_MIPS_M4K         0x1100FFFF  // Dev family: MIPS, sub family: 0x00 (M4K core)
#define JLINK_CORE_MIPS_MICROAPTIV  0x1101FFFF  // Dev family: MIPS, sub family: 0x01 (microAptiv core)
#define JLINK_CORE_EFM8_UNSPEC      0x12FFFFFF  // Dev family: SiLabs EFM8, sub family 0xFF (exact core not specified)
#define JLINK_CORE_CIP51            0x1200FFFF  // Dev family: SiLabs EFM8, sub family 0x00 (CIP51 core)

/*********************************************************************
*
*       CPU capabilities
*/
#define JLINKARM_EMU_CPU_CAP_RESERVED               (1 <<  0)         // Always 1
#define JLINKARM_EMU_CPU_CAP_READ_MEM               (1 <<  1)         // Supports command "CPU_CMD_READ_MEM"
#define JLINKARM_EMU_CPU_CAP_WRITE_MEM              (1 <<  2)         // Supports command "CPU_CMD_WRITE_MEM"
#define JLINKARM_EMU_CPU_CAP_READ_REGS              (1 <<  3)         // Supports command "CPU_CMD_READ_REGS"
#define JLINKARM_EMU_CPU_CAP_WRITE_REGS             (1 <<  4)         // Supports command "CPU_CMD_WRITE_REGS"
#define JLINKARM_EMU_CPU_CAP_GO                     (1 <<  5)         // Supports command "CPU_CMD_GO"
#define JLINKARM_EMU_CPU_CAP_STEP                   (1 <<  6)         // Supports command "CPU_CMD_STEP"
#define JLINKARM_EMU_CPU_CAP_HALT                   (1 <<  7)         // Supports command "CPU_CMD_HALT"
#define JLINKARM_EMU_CPU_CAP_IS_HALTED              (1 <<  8)         // Supports command "CPU_CMD_IS_HALTED"
#define JLINKARM_EMU_CPU_CAP_RESET                  (1 <<  9)         // Supports command "CPU_CMD_RESET"
#define JLINKARM_EMU_CPU_CAP_RUN_STOP               (1 << 10)         // Supports command "CPU_CMD_RUN_STOP"
#define JLINKARM_EMU_CPU_CAP_TERMINAL               (1 << 11)         // Supports command "CPU2_CMD_READ_TERMINAL". Do not use. Use "EMU_CPU_CAP_DCC" instead for checking if JLINKARM_ReadTerminal() can be used.
                                                 // (1 << 12)         // Used internally only
                                                 // (1 << 13)         // Used internally only
#define JLINKARM_EMU_CPU_CAP_DCC                    (1 << 14)         // Supports command "CPU2_CMD_DCC"
#define JLINKARM_EMU_CPU_CAP_HSS                    (1 << 15)         // Supports command "CPU2_CMD_HSS"

/*********************************************************************
*
*       Emulator Capabilities (Query with JLINKARM_GetEmuCaps())
*/
#define JLINKARM_EMU_CAP_RESERVED                   (1 <<  0)       // Always 1
#define JLINKARM_EMU_CAP_GET_HW_VERSION             (1 <<  1)       // Supports command "EMU_CMD_GET_HARDWARE_VERSION"
#define JLINKARM_EMU_CAP_WRITE_DCC                  (1 <<  2)       // Supports command "EMU_CMD_WRITE_DCC"
#define JLINKARM_EMU_CAP_ADAPTIVE_CLOCKING          (1 <<  3)       // Supports adaptive clocking
#define JLINKARM_EMU_CAP_READ_CONFIG                (1 <<  4)       // Supports command "EMU_CMD_READ_CONFIG"
#define JLINKARM_EMU_CAP_WRITE_CONFIG               (1 <<  5)       // Supports command "EMU_CMD_WRITE_CONFIG"
#define JLINKARM_EMU_CAP_TRACE_ARM79                (1 <<  6)       // Supports trace commands
#define JLINKARM_EMU_CAP_WRITE_MEM                  (1 <<  7)       // Supports command "EMU_CMD_WRITE_MEM"
#define JLINKARM_EMU_CAP_READ_MEM                   (1 <<  8)       // Supports command "EMU_CMD_READ_MEM"
#define JLINKARM_EMU_CAP_SPEED_INFO                 (1 <<  9)       // Supports command "EMU_CMD_GET_SPEED"
#define JLINKARM_EMU_CAP_EXEC_CODE                  (1 << 10)       // Supports command "EMU_CMD_CODE_..."
#define JLINKARM_EMU_CAP_GET_MAX_BLOCK_SIZE         (1 << 11)       // Supports command "EMU_CMD_CODE_..."
#define JLINKARM_EMU_CAP_GET_HW_INFO                (1 << 12)       // Supports command "EMU_CMD_GET_HW_INFO"
#define JLINKARM_EMU_CAP_SET_KS_POWER               (1 << 13)       // Supports command "EMU_CMD_SET_KS_POWER"
#define JLINKARM_EMU_CAP_RESET_STOP_TIMED           (1 << 14)       // Supports command "EMU_CMD_HW_RELEASE_RESET_STOP_TIMED"
#define JLINKARM_EMU_CAP_GET_LICENSE_INFO           (1 << 15)       // Supports command "EMU_CMD_GET_LICENSE_INFO"
#define JLINKARM_EMU_CAP_MEASURE_RTCK_REACT         (1 << 16)       // Supports command "EMU_CMD_MEASURE_RTCK_REACT"
#define JLINKARM_EMU_CAP_SELECT_IF                  (1 << 17)       // Supports command "EMU_CMD_HW_SELECT_IF"
#define JLINKARM_EMU_CAP_RW_MEM_ARM79               (1 << 18)       // Supports command "EMU_CMD_WRITE_MEM_ARM79", "CMD_READ_MEM_ARM79"
#define JLINKARM_EMU_CAP_GET_COUNTERS               (1 << 19)       // Supports command "EMU_CMD_GET_COUNTERS"
#define JLINKARM_EMU_CAP_READ_DCC                   (1 << 20)       // Supports command "EMU_CMD_READ_DCC"
#define JLINKARM_EMU_CAP_GET_CPU_CAPS               (1 << 21)       // Supports command "EMU_CMD_GET_CPU_CAPS"
#define JLINKARM_EMU_CAP_EXEC_CPU_CMD               (1 << 22)       // Supports command "EMU_CMD_EXEC_CPU_CMD"
#define JLINKARM_EMU_CAP_SWO                        (1 << 23)       // Supports command "EMU_CMD_SWO"
#define JLINKARM_EMU_CAP_WRITE_DCC_EX               (1 << 24)       // Supports command "EMU_CMD_WRITE_DCC_EX"
#define JLINKARM_EMU_CAP_UPDATE_FIRMWARE_EX         (1 << 25)       // Supports command "EMU_CMD_UPDATE_FIRMWARE_EX"
#define JLINKARM_EMU_CAP_FILE_IO                    (1 << 26)       // Supports command "EMU_CMD_FILE_IO"
#define JLINKARM_EMU_CAP_REGISTER                   (1 << 27)       // Supports command "EMU_CMD_REGISTER"
#define JLINKARM_EMU_CAP_INDICATORS                 (1 << 28)       // Supports command "EMU_CMD_INDICATORS"
#define JLINKARM_EMU_CAP_TEST_NET_SPEED             (1 << 29)       // Supports command "EMU_CMD_TEST_NET_SPEED"
#define JLINKARM_EMU_CAP_RAWTRACE                   (1 << 30)       // Supports command "EMU_CMD_RAWTRACE"
#define JLINKARM_EMU_CAP_GET_CAPS_EX                (1 << 31)       // Supports command "EMU_CMD_GET_CAPS_EX"

/*********************************************************************
*
*       Extended Emulator Capabilities (Query with JLINKARM_GetEmuCapsEx())
*/
#define JLINKARM_EMU_CAP_EX_RESERVED                (0)             // Always 1
#define JLINKARM_EMU_CAP_EX_GET_HW_VERSION          (1)             // Supports command "EMU_CMD_GET_HARDWARE_VERSION"
#define JLINKARM_EMU_CAP_EX_WRITE_DCC               (2)             // Supports command "EMU_CMD_WRITE_DCC"
#define JLINKARM_EMU_CAP_EX_ADAPTIVE_CLOCKING       (3)             // Supports adaptive clocking
#define JLINKARM_EMU_CAP_EX_READ_CONFIG             (4)             // Supports command "EMU_CMD_READ_CONFIG"
#define JLINKARM_EMU_CAP_EX_WRITE_CONFIG            (5)             // Supports command "EMU_CMD_WRITE_CONFIG"
#define JLINKARM_EMU_CAP_EX_TRACE_ARM79             (6)             // Supports trace commands
#define JLINKARM_EMU_CAP_EX_WRITE_MEM               (7)             // Supports command "EMU_CMD_WRITE_MEM"
#define JLINKARM_EMU_CAP_EX_READ_MEM                (8)             // Supports command "EMU_CMD_READ_MEM"
#define JLINKARM_EMU_CAP_EX_SPEED_INFO              (9)             // Supports command "EMU_CMD_GET_SPEED"
#define JLINKARM_EMU_CAP_EX_EXEC_CODE               (10)            // Supports command "EMU_CMD_CODE_..."
#define JLINKARM_EMU_CAP_EX_GET_MAX_BLOCK_SIZE      (11)            // Supports command "EMU_CMD_CODE_..."
#define JLINKARM_EMU_CAP_EX_GET_HW_INFO             (12)            // Supports command "EMU_CMD_GET_HW_INFO"
#define JLINKARM_EMU_CAP_EX_SET_KS_POWER            (13)            // Supports command "EMU_CMD_SET_KS_POWER"
#define JLINKARM_EMU_CAP_EX_RESET_STOP_TIMED        (14)            // Supports command "EMU_CMD_HW_RELEASE_RESET_STOP_TIMED"
#define JLINKARM_EMU_CAP_EX_GET_LICENSE_INFO        (15)            // Supports command "EMU_CMD_GET_LICENSE_INFO"
#define JLINKARM_EMU_CAP_EX_MEASURE_RTCK_REACT      (16)            // Supports command "EMU_CMD_MEASURE_RTCK_REACT"
#define JLINKARM_EMU_CAP_EX_SELECT_IF               (17)            // Supports command "EMU_CMD_HW_SELECT_IF"
#define JLINKARM_EMU_CAP_EX_RW_MEM_ARM79            (18)            // Supports command "EMU_CMD_WRITE_MEM_ARM79", "CMD_READ_MEM_ARM79"
#define JLINKARM_EMU_CAP_EX_GET_COUNTERS            (19)            // Supports command "EMU_CMD_GET_COUNTERS"
#define JLINKARM_EMU_CAP_EX_READ_DCC                (20)            // Supports command "EMU_CMD_READ_DCC"
#define JLINKARM_EMU_CAP_EX_GET_CPU_CAPS            (21)            // Supports command "EMU_CMD_GET_CPU_CAPS"
#define JLINKARM_EMU_CAP_EX_EXEC_CPU_CMD            (22)            // Supports command "EMU_CMD_EXEC_CPU_CMD"
#define JLINKARM_EMU_CAP_EX_SWO                     (23)            // Supports command "EMU_CMD_SWO"
#define JLINKARM_EMU_CAP_EX_WRITE_DCC_EX            (24)            // Supports command "EMU_CMD_WRITE_DCC_EX"
#define JLINKARM_EMU_CAP_EX_UPDATE_FIRMWARE_EX      (25)            // Supports command "EMU_CMD_UPDATE_FIRMWARE_EX"
#define JLINKARM_EMU_CAP_EX_FILE_IO                 (26)            // Supports command "EMU_CMD_FILE_IO"
#define JLINKARM_EMU_CAP_EX_REGISTER                (27)            // Supports command "EMU_CMD_REGISTER"
#define JLINKARM_EMU_CAP_EX_INDICATORS              (28)            // Supports command "EMU_CMD_INDICATORS"
#define JLINKARM_EMU_CAP_EX_TEST_NET_SPEED          (29)            // Supports command "EMU_CMD_TEST_NET_SPEED"
#define JLINKARM_EMU_CAP_EX_RAWTRACE                (30)            // Supports command "EMU_CMD_RAWTRACE"
#define JLINKARM_EMU_CAP_EX_GET_CAPS_EX             (31)            // Supports command "EMU_CMD_GET_CAPS_EX"
#define JLINKARM_EMU_CAP_EX_HW_JTAG_WRITE           (32)            // Supports command "EMU_CMD_HW_JTAG_WRITE"
#define JLINKARM_EMU_CAP_EX_EMUCOM                  (33)            // Supports command "EMU_CMD_EMUCOM"
#define JLINKARM_EMU_CAP_EX_EXEC_CPU2_CMD           (34)            // Supports command "EMU_CMD_EXEC_CPU2_CMD"
#define JLINKARM_EMU_CAP_EX_GET_CPU2_CAPS           (35)            // Supports command "EMU_CMD_GET_CPU2_CAPS"
#define JLINKARM_EMU_CAP_EX_TEST_NET                (36)            // Supports command "EMU_CMD_TEST_NET"
#define JLINKARM_EMU_CAP_EX_POWERTRACE              (37)            // Supports command "EMU_CMD_POWERTRACE"
#define JLINKARM_EMU_CAP_EX_ETHERNET                (38)            // Supports communication via ethernet
#define JLINKARM_EMU_CAP_EX_HW_SET_SWDIO_DIR        (39)            // Supports commands "EMU_CMD_HW_SWD_SET_DIR_OUT",  "EMU_CMD_HW_SWD_SET_DIR_IN"
#define JLINKARM_EMU_CAP_EX_HW_ENABLE_DISABLE_SWCLK (40)            // Supports commands "EMU_CMD_HW_SWD_ENABLE_SWCLK", "EMU_CMD_HW_SWD_DISABLE_SWCLK"
#define JLINKARM_EMU_CAP_EX_HW_ENABLE_DISABLE_JTAG_IF (41)          // Supports commands "EMU_CMD_HW_JTAG_ENABLE_IF",   "EMU_CMD_HW_JTAG_DISABLE_IF"
#define JLINKARM_EMU_CAP_EX_HW_SET_CLEAR_TCK        (42)            // Supports commands "EMU_CMD_HW_TCK0",             "EMU_CMD_HW_TCK1"
// (43) is for internal use only
#define JLINKARM_EMU_CAP_EX_PCODE_EXEC              (44)            // Supports command "EMU_CMD_PCODE_EXEC"
// (45) is for internal use only
// (46) is for internal use only
// (47) is for internal use only
#define JLINKARM_EMU_CAP_EX_CDC_EXEC                (48)            // Supports command "EMU_CMD_CDC_EXEC"
#define JLINKARM_EMU_CAP_EX_CDC_SET_HOOK_FUNCS      (49)            // Supports command "EMU_CMD_CDC_SET_HOOK_FUNCS"
#define JLINKARM_EMU_CAP_EX_HANDLE_BMI              (50)            // Supports JLINKARM_SetBMI() / JLINKARM_GetBMI()
#define JLINKARM_EMU_CAP_EX_HANDLE_GPIO             (51)            // Supports command "EMU_CMD_HANDLE_GPIO"
// (52) is for internal use only
#define JLINKARM_EMU_CAP_EX_HSS                     (53)            // Supports command "EMU_CMD_HSS"
#define JLINKARM_EMU_CAP_EX_FILE_IO_EX              (54)            // Supports extended command set of FILE IO commands
// (55) is for internal use only
#define JLINKARM_EMU_CAP_EX_SPI                     (56)            // Supports command "EMU_CMD_SPI"
#define JLINKARM_EMU_CAP_EX_SUPPORT_OTS_CONFIG_X    (57)            // Supports command "EMU_CMD_HANDLE_OTSX_CONFIGX"
#define JLINKARM_EMU_CAP_EX_HANDLE_SECURITY         (58)            // Supports command "EMU_SECURE"
#define JLINKARM_EMU_CAP_EX_PIN_OVERRIDE            (59)            // Supports command "EMU_CMD_PIN_OVERRIDE"

#define JLINKARM_EMUCOM_CHANNEL_TIME                (0x0000)        // Always returns 4 bytes: OS_Time
#define JLINKARM_EMUCOM_CHANNEL_USER                (0x10000)       // The first 64k channels are reserved

#define JLINKARM_EMUCOM_ERR                         (0x80000000UL)  // Generic error
#define JLINKARM_EMUCOM_ERR_CHANNEL_NOT_SUPPORTED   (0x80000001UL)  // Selected channel is not supported by emulator
#define JLINKARM_EMUCOM_ERR_BUFFER_TOO_SMALL        (0x81000000UL)  // The lower 24 bytes contain required buffer size

/*********************************************************************
*
*       Supported host interfaces for JLINKARM_EMU_GetList()
*/
#define JLINKARM_HOSTIF_USB   (1 << 0)
#define JLINKARM_HOSTIF_IP    (1 << 1)

//
// Flags for JLINKARM_BP_
//
#define JLINKARM_BP_MODE0                 (0 << 0)      // Meaning depends on CPU type
#define JLINKARM_BP_MODE1                 (1 << 0)      // Meaning depends on CPU type
#define JLINKARM_BP_MODE2                 (2 << 0)      // Meaning depends on CPU type
#define JLINKARM_BP_MODE3                 (3 << 0)      // Meaning depends on CPU type
#define JLINKARM_BP_MODE_MASK             (0x0000000F)

#define JLINKARM_BP_IMP_SW_RAM            (1 << 4)
#define JLINKARM_BP_IMP_SW_FLASH          (1 << 5)
#define JLINKARM_BP_IMP_SW                (0x000000F0)
#define JLINKARM_BP_IMP_HW                (0xFFFFFF00)
#define JLINKARM_BP_IMP_ANY               (JLINKARM_BP_IMP_HW | JLINKARM_BP_IMP_SW)
#define JLINKARM_BP_IMP_MASK              (JLINKARM_BP_IMP_ANY)

#define JLINKARM_BP_HANDLE_ALL            (0xFFFFFFFF)
#define JLINKARM_WP_HANDLE_ALL            (0xFFFFFFFF)

//
// Flags for ARM7/9 cores only
//
#define JLINKARM_BP_TYPE_ARM              JLINKARM_BP_MODE1
#define JLINKARM_BP_TYPE_THUMB            JLINKARM_BP_MODE2

//
// Flags for JLINKARM_BP_INFO (ImpFlags member)
//

#define JLINKARM_BP_IMP_FLAG_HARD         (1 << 0)      // Hardware breakpoint ?
#define JLINKARM_BP_IMP_FLAG_SOFT         (1 << 1)      // Software breakpoint ?
#define JLINKARM_BP_IMP_FLAG_PENDING      (1 << 2)      // Breakpoint not yet written to memory (SWBPs only)
#define JLINKARM_BP_IMP_FLAG_FLASH        (1 << 4)      // FlashBP ?

/*********************************************************************
*
*       Flags for JLINKARM_SetDataEvent()
*/

#define JLINKARM_EVENT_TYPE_DATA_BP             (1 << 0)

#define JLINKARM_EVENT_ERR_UNKNOWN              (0x80000000)
#define JLINKARM_EVENT_ERR_NO_MORE_EVENTS       (0x80000001)
#define JLINKARM_EVENT_ERR_NO_MORE_ADDR_COMP    (0x80000002)
#define JLINKARM_EVENT_ERR_NO_MORE_DATA_COMP    (0x80000004)
#define JLINKARM_EVENT_ERR_INVALID_ADDR_MASK    (0x80000020)
#define JLINKARM_EVENT_ERR_INVALID_DATA_MASK    (0x80000040)
#define JLINKARM_EVENT_ERR_INVALID_ACCESS_MASK  (0x80000080)

#define JLINK_EVENT_HANDLE_ALL                  (0xFFFFFFFF)

#define JLINK_EVENT_DATA_BP_DIR_RD              (0 << 0)
#define JLINK_EVENT_DATA_BP_DIR_WR              (1 << 0)
#define JLINK_EVENT_DATA_BP_PRIV                (1 << 4)
#define JLINK_EVENT_DATA_BP_SIZE_8BIT           (0 << 1)
#define JLINK_EVENT_DATA_BP_SIZE_16BIT          (1 << 1)
#define JLINK_EVENT_DATA_BP_SIZE_32BIT          (2 << 1)
//
// Flags for the parameter TypeMask
//
#define JLINK_EVENT_DATA_BP_MASK_SIZE           (3 << 1)
#define JLINK_EVENT_DATA_BP_MASK_DIR            (1 << 0)
#define JLINK_EVENT_DATA_BP_MASK_PRIV           (1 << 4)

/*********************************************************************
*
*       Flags for JLINKARM_SetWP()
*/
//
// Flags for the parameter AccessType
//
#define JLINKARM_WP_DIR_RD      (0 << 0)
#define JLINKARM_WP_DIR_WR      (1 << 0)
#define JLINKARM_WP_PRIV        (1 << 4)
#define JLINKARM_WP_SIZE_8BIT   (0 << 1)
#define JLINKARM_WP_SIZE_16BIT  (1 << 1)
#define JLINKARM_WP_SIZE_32BIT  (2 << 1)
//
// Flags for the parameter TypeMask
//
#define JLINKARM_WP_MASK_SIZE   (3 << 1)
#define JLINKARM_WP_MASK_DIR    (1 << 0)
#define JLINKARM_WP_MASK_PRIV   (1 << 4)

#define JLINKARM_SPEED_AUTO         0
#define JLINKARM_SPEED_INVALID      0xFFFE
#define JLINKARM_SPEED_ADAPTIVE     0xFFFF

#define JLINKARM_INDICATOR_ID_LED_GREEN  0
#define JLINKARM_INDICATOR_ID_LED_RED    1
#define JLINKARM_INDICATOR_ID_BUSY       2
#define JLINKARM_INDICATOR_ID_OK         3

//
// Flags for JLINKARM_GoEx()
//
#define JLINKARM_GO_FLAG_OVERSTEP_BP        (1 << 0)    // Overstep the current instruction if it is breakpointed

#define JLINKARM_GO_MAX_EMUL_INSTS_DEFAULT  (-1)        // Use the same settings as JLINKARM_Go()

//
// Flags for JLINKARM_BeginDownload()
//
#define JLINKARM_ALLOW_FLASH_DOWNLOAD           (1 << 0)
#define JLINKARM_ALLOW_BUFFERED_RAM_DOWNLOAD    (1 << 1)

/*********************************************************************
*
*       Flags for JLINKARM_WriteVectorCatch()
*/
#define JLINKARM_CORTEX_R4_VCATCH_RESET    (1 << 0)
#define JLINKARM_CORTEX_R4_VCATCH_UNDEF    (1 << 1)
#define JLINKARM_CORTEX_R4_VCATCH_SVC      (1 << 2)
#define JLINKARM_CORTEX_R4_VCATCH_SWI      JLINKARM_CORTEX_R4_VCATCH_SVC
#define JLINKARM_CORTEX_R4_VCATCH_PREFETCH (1 << 3)
#define JLINKARM_CORTEX_R4_VCATCH_ABORT    (1 << 4)
#define JLINKARM_CORTEX_R4_VCATCH_IRQ      (1 << 6)
#define JLINKARM_CORTEX_R4_VCATCH_FIQ      (1 << 7)

//
// ARM DDI0337G (Cortex-M3 TRM): 10.2.4 Debug Exception and Monitor Control Register (DEMCR)
//
// [0]   Read/write     VC_CORERESET Reset Vector Catch. Halt running system if Core reset occurs.
// [3:1] -              -            Reserved, SBZP
// [4]   Read/write     VC_MMERR     Debug trap on Memory Management faults.
// [5]   Read/write     VC_NOCPERR   Debug trap on Usage Fault access to Coprocessor that is not present or marked as not present in CAR register.
// [6]   Read/write     VC_CHKERR    Debug trap on Usage Fault enabled checking errors.
// [7]   Read/write     VC_STATERR   Debug trap on Usage Fault state errors.
// [8]   Read/write     VC_BUSERR    Debug Trap on normal Bus error.
// [9]   Read/write     VC_INTERR    Debug Trap on interrupt/exception service errors. These are a subset of other faults and catches before BUSERR or HARDERR.
// [10]  Read/write     VC_HARDERR   Debug trap on Hard Fault.
//
#define JLINKARM_CORTEX_M3_VCATCH_CORERESET (1 <<  0)
#define JLINKARM_CORTEX_M3_VCATCH_MMERR     (1 <<  4)
#define JLINKARM_CORTEX_M3_VCATCH_NOCPERR   (1 <<  5)
#define JLINKARM_CORTEX_M3_VCATCH_CHKERR    (1 <<  6)
#define JLINKARM_CORTEX_M3_VCATCH_STATERR   (1 <<  7)
#define JLINKARM_CORTEX_M3_VCATCH_BUSERR    (1 <<  8)
#define JLINKARM_CORTEX_M3_VCATCH_INTERR    (1 <<  9)
#define JLINKARM_CORTEX_M3_VCATCH_HARDERR   (1 << 10)

#define JLINKARM_MAX_NUM_CPU_REGS           (256)

/*********************************************************************
*
*       Flags for JLINK_EMU_GPIO_SetState(), parameter *paState
*/

#define JLINK_EMU_GPIO_STATE_UNCHANGE                   0
#define JLINK_EMU_GPIO_STATE_INPUT                      1   // Set to input
#define JLINK_EMU_GPIO_STATE_CLR                        2   // Set to output + LOW level
#define JLINK_EMU_GPIO_STATE_SET                        3   // Set to output + HIGH level

/*********************************************************************
*
*       Group Ids for JLINK_MRU_GetList()
*/
#define JLINK_MRU_GROUP_DEVICE          0
#define JLINK_MRU_GROUP_SETTINGSFILE    1
#define JLINK_MRU_GROUP_SCRIPTFILE      2

/*********************************************************************
*
*       Flags for external dialogs that are called via hook functions
*/
#define JLINK_DLG_BUTTON_YES                  (1 << 0)
#define JLINK_DLG_BUTTON_NO                   (1 << 1)
#define JLINK_DLG_BUTTON_OK                   (1 << 2)
#define JLINK_DLG_BUTTON_CANCEL               (1 << 3)

#define JLINK_DLG_TYPE_PROT_READ              (1 << 28)
#define JLINK_DLG_TYPE_PROT_WRITE             (2 << 28)


/*********************************************************************
*
*       Types required for API
*
**********************************************************************
*/

//
// ARM_REG
//
typedef enum {
  ARM_REG_R0,                         // Index  0
  ARM_REG_R1,                         // Index  1
  ARM_REG_R2,                         // Index  2
  ARM_REG_R3,                         // Index  3
  ARM_REG_R4,                         // Index  4
  ARM_REG_R5,                         // Index  5
  ARM_REG_R6,                         // Index  6
  ARM_REG_R7,                         // Index  7
  ARM_REG_CPSR,                       // Index  8
  ARM_REG_R15,                        // Index  9
  ARM_REG_R8_USR,                     // Index 10
  ARM_REG_R9_USR,                     // Index 11
  ARM_REG_R10_USR,                    // Index 12
  ARM_REG_R11_USR,                    // Index 13
  ARM_REG_R12_USR,                    // Index 14
  ARM_REG_R13_USR,                    // Index 15
  ARM_REG_R14_USR,                    // Index 16
  ARM_REG_SPSR_FIQ,                   // Index 17
  ARM_REG_R8_FIQ,                     // Index 18
  ARM_REG_R9_FIQ,                     // Index 19
  ARM_REG_R10_FIQ,                    // Index 20
  ARM_REG_R11_FIQ,                    // Index 21
  ARM_REG_R12_FIQ,                    // Index 22
  ARM_REG_R13_FIQ,                    // Index 23
  ARM_REG_R14_FIQ,                    // Index 24
  ARM_REG_SPSR_SVC,                   // Index 25
  ARM_REG_R13_SVC,                    // Index 26
  ARM_REG_R14_SVC,                    // Index 27
  ARM_REG_SPSR_ABT,                   // Index 28
  ARM_REG_R13_ABT,                    // Index 29
  ARM_REG_R14_ABT,                    // Index 30
  ARM_REG_SPSR_IRQ,                   // Index 31
  ARM_REG_R13_IRQ,                    // Index 32
  ARM_REG_R14_IRQ,                    // Index 33
  ARM_REG_SPSR_UND,                   // Index 34
  ARM_REG_R13_UND,                    // Index 35
  ARM_REG_R14_UND,                    // Index 36
  ARM_REG_FPSID,                      // Index 37
  ARM_REG_FPSCR,                      // Index 38
  ARM_REG_FPEXC,                      // Index 39
  ARM_REG_FPS0,                       // Index 40
  ARM_REG_FPS1,                       // Index 41
  ARM_REG_FPS2,                       // Index 42
  ARM_REG_FPS3,                       // Index 43
  ARM_REG_FPS4,                       // Index 44
  ARM_REG_FPS5,                       // Index 45
  ARM_REG_FPS6,                       // Index 46
  ARM_REG_FPS7,                       // Index 47
  ARM_REG_FPS8,                       // Index 48
  ARM_REG_FPS9,                       // Index 49
  ARM_REG_FPS10,                      // Index 50
  ARM_REG_FPS11,                      // Index 51
  ARM_REG_FPS12,                      // Index 52
  ARM_REG_FPS13,                      // Index 53
  ARM_REG_FPS14,                      // Index 54
  ARM_REG_FPS15,                      // Index 55
  ARM_REG_FPS16,                      // Index 56
  ARM_REG_FPS17,                      // Index 57
  ARM_REG_FPS18,                      // Index 58
  ARM_REG_FPS19,                      // Index 59
  ARM_REG_FPS20,                      // Index 60
  ARM_REG_FPS21,                      // Index 61
  ARM_REG_FPS22,                      // Index 62
  ARM_REG_FPS23,                      // Index 63
  ARM_REG_FPS24,                      // Index 64
  ARM_REG_FPS25,                      // Index 65
  ARM_REG_FPS26,                      // Index 66
  ARM_REG_FPS27,                      // Index 67
  ARM_REG_FPS28,                      // Index 68
  ARM_REG_FPS29,                      // Index 69
  ARM_REG_FPS30,                      // Index 70
  ARM_REG_FPS31,                      // Index 71
  ARM_REG_R8,                         // Index 72
  ARM_REG_R9,                         // Index 73
  ARM_REG_R10,                        // Index 74
  ARM_REG_R11,                        // Index 75
  ARM_REG_R12,                        // Index 76
  ARM_REG_R13,                        // Index 77
  ARM_REG_R14,                        // Index 78
  ARM_REG_SPSR,                       // Index 79
  ARM_NUM_REGS
} ARM_REG;

//
// JLINKARM_CM3_REG
//
typedef enum {
  JLINKARM_CM3_REG_R0,                // Index  0: HW-Reg
  JLINKARM_CM3_REG_R1,                // Index  1: HW-Reg
  JLINKARM_CM3_REG_R2,                // Index  2: HW-Reg
  JLINKARM_CM3_REG_R3,                // Index  3: HW-Reg
  JLINKARM_CM3_REG_R4,                // Index  4: HW-Reg
  JLINKARM_CM3_REG_R5,                // Index  5: HW-Reg
  JLINKARM_CM3_REG_R6,                // Index  6: HW-Reg
  JLINKARM_CM3_REG_R7,                // Index  7: HW-Reg
  JLINKARM_CM3_REG_R8,                // Index  8: HW-Reg
  JLINKARM_CM3_REG_R9,                // Index  9: HW-Reg
  JLINKARM_CM3_REG_R10,               // Index 10: HW-Reg
  JLINKARM_CM3_REG_R11,               // Index 11: HW-Reg
  JLINKARM_CM3_REG_R12,               // Index 12: HW-Reg
  JLINKARM_CM3_REG_R13,               // Index 13: Pseudo reg! It needs to be mapped to SP_MSP or SP_PSP, depending on current Controlregister:
  JLINKARM_CM3_REG_R14,               // Index 14: HW-Reg
  JLINKARM_CM3_REG_R15,               // Index 15: HW-Reg
  JLINKARM_CM3_REG_XPSR,              // Index 16: HW-Reg
  JLINKARM_CM3_REG_MSP,               // Index 17: HW-Reg  Main stack pointer
  JLINKARM_CM3_REG_PSP,               // Index 18: HW-Reg  Process stack pointer
  JLINKARM_CM3_REG_RAZ,               // Index 19: HW-Reg, Unused/Reserved
  JLINKARM_CM3_REG_CFBP,              // Index 20: HW-Reg, CONTROL/FAULTMASK/BASEPRI/PRIMASK (packed into 4 bytes of word. CONTROL = CFBP[31:24], FAULTMASK = CFBP[16:23], BASEPRI = CFBP[15:8], PRIMASK = CFBP[7:0]
  JLINKARM_CM3_REG_APSR,              // Index 21: Pseudo reg. (Part of XPSR)
  JLINKARM_CM3_REG_EPSR,              // Index 22: Pseudo reg. (Part of XPSR)
  JLINKARM_CM3_REG_IPSR,              // Index 23: Pseudo reg. (Part of XPSR)
  //
  // Data for CFBP pseudo regs is expected & returned
  // in same byte lane as when reading/writing the CFBP.
  // This means when reading
  //  PRIMASK,     the data of PRIMASK is expected/returned at     bits [7:0]
  //  BASEPRI,     the data of BASEPRI is expected/returned at     bits [15:8]
  //  BASEPRI_MAX, the data of BASEPRI_MAX is expected/returned at bits [15:8]
  //  FAULTMASK,   the data of FAULTMASK is expected/returned at   bits [23:16]
  //  CONTROL,     the data of CONTROL is expected/returned at     bits [31:24]
  //
  // Sample CFBP = 0xx11223344
  // Read PRIMASK   will return 0x00000044
  // Read BASEPRI   will return 0x00003300
  // Read FAULTMASK will return 0x00220000
  // Read CONTROL   will return 0x11000000
  // When writing, data is expected at the same position
  //
  JLINKARM_CM3_REG_PRIMASK,           // Index 24: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI,           // Index 25: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_FAULTMASK,         // Index 26: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_CONTROL,           // Index 27: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_MAX,       // Index 28: Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_IAPSR,             // Index 29: Pseudo reg. (Part of XPSR)
  JLINKARM_CM3_REG_EAPSR,             // Index 30: Pseudo reg. (Part of XPSR)
  JLINKARM_CM3_REG_IEPSR,             // Index 31: Pseudo reg. (Part of XPSR)
  //
  // Pseudo registers (not really CPU registers but handled as such ones
  //
  JLINKARM_CM3_REG_DWT_CYCCNT=65,     // Index 65
  //
  // New regs introduced with ARMv8M architecture
  //
  JLINKARM_CM3_REG_MSP_NS,
  JLINKARM_CM3_REG_PSP_NS,
  JLINKARM_CM3_REG_MSP_S,
  JLINKARM_CM3_REG_PSP_S,
  JLINKARM_CM3_REG_MSPLIM_S,
  JLINKARM_CM3_REG_PSPLIM_S,
  JLINKARM_CM3_REG_MSPLIM_NS,
  JLINKARM_CM3_REG_PSPLIM_NS,
  JLINKARM_CM3_REG_CFBP_S,
  JLINKARM_CM3_REG_CFBP_NS,
  //
  // Data for CFBP pseudo regs is expected & returned
  // in same byte lane as when reading/writing the CFBP.
  // This means when reading
  //  PRIMASK,     the data of PRIMASK is expected/returned at     bits [7:0]
  //  BASEPRI,     the data of BASEPRI is expected/returned at     bits [15:8]
  //  BASEPRI_MAX, the data of BASEPRI_MAX is expected/returned at bits [15:8]
  //  FAULTMASK,   the data of FAULTMASK is expected/returned at   bits [23:16]
  //  CONTROL,     the data of CONTROL is expected/returned at     bits [31:24]
  //
  // Sample CFBP = 0xx11223344
  // Read PRIMASK   will return 0x00000044
  // Read BASEPRI   will return 0x00003300
  // Read FAULTMASK will return 0x00220000
  // Read CONTROL   will return 0x11000000
  // When writing, data is expected at the same position
  //
  JLINKARM_CM3_REG_PRIMASK_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_FAULTMASK_NS,      // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_CONTROL_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_MAX_NS,    // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_PRIMASK_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_FAULTMASK_S,       // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_CONTROL_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_REG_BASEPRI_MAX_S,     // Pseudo reg. (Part of CFBP)
  JLINKARM_CM3_NUM_REGS
} JLINKARM_CM3_REG;

//
// JLINKARM_CM4_REG
//
typedef enum {
  JLINKARM_CM4_REG_R0,                // Index  0
  JLINKARM_CM4_REG_R1,                // Index  1
  JLINKARM_CM4_REG_R2,                // Index  2
  JLINKARM_CM4_REG_R3,                // Index  3
  JLINKARM_CM4_REG_R4,                // Index  4
  JLINKARM_CM4_REG_R5,                // Index  5
  JLINKARM_CM4_REG_R6,                // Index  6
  JLINKARM_CM4_REG_R7,                // Index  7
  JLINKARM_CM4_REG_R8,                // Index  8
  JLINKARM_CM4_REG_R9,                // Index  9
  JLINKARM_CM4_REG_R10,               // Index 10
  JLINKARM_CM4_REG_R11,               // Index 11
  JLINKARM_CM4_REG_R12,               // Index 12
  JLINKARM_CM4_REG_R13,               // Index 13: Pseudo reg! It needs to be mapped to SP_MSP or SP_PSP, depending on current Controlregister:
  JLINKARM_CM4_REG_R14,               // Index 14
  JLINKARM_CM4_REG_R15,               // Index 15
  JLINKARM_CM4_REG_XPSR,              // Index 16
  JLINKARM_CM4_REG_MSP,               // Index 17
  JLINKARM_CM4_REG_PSP,               // Index 18
  JLINKARM_CM4_REG_RAZ,               // Index 19: Reserved
  JLINKARM_CM4_REG_CFBP,              // Index 20: CONTROL/FAULTMASK/BASEPRI/PRIMASK (packed into 4 bytes of word. CONTROL = CFBP[31:24], FAULTMASK = CFBP[16:23], BASEPRI = CFBP[15:8], PRIMASK = CFBP[7:0]
  JLINKARM_CM4_REG_APSR,              // Index 21: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_EPSR,              // Index 22: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_IPSR,              // Index 23: Pseudo reg. (Part of XPSR)
  //
  // Data for CFBP pseudo regs is expected & returned
  // in same byte lane as when reading/writing the CFBP.
  // This means when reading
  //  PRIMASK,     the data of PRIMASK is expected/returned at     bits [7:0]
  //  BASEPRI,     the data of BASEPRI is expected/returned at     bits [15:8]
  //  BASEPRI_MAX, the data of BASEPRI_MAX is expected/returned at bits [15:8]
  //  FAULTMASK,   the data of FAULTMASK is expected/returned at   bits [23:16]
  //  CONTROL,     the data of CONTROL is expected/returned at     bits [31:24]
  //
  // Sample CFBP = 0xx11223344
  // Read PRIMASK   will return 0x00000044
  // Read BASEPRI   will return 0x00003300
  // Read FAULTMASK will return 0x00220000
  // Read CONTROL   will return 0x11000000
  // When writing, data is expected at the same position
  //
  JLINKARM_CM4_REG_PRIMASK,           // Index 24: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI,           // Index 25: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_FAULTMASK,         // Index 26: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_CONTROL,           // Index 27: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_MAX,       // Index 28: Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_IAPSR,             // Index 29: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_EAPSR,             // Index 30: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_IEPSR,             // Index 31: Pseudo reg. (Part of XPSR)
  JLINKARM_CM4_REG_FPSCR,             // Index 32
  JLINKARM_CM4_REG_FPS0,              // Index 33
  JLINKARM_CM4_REG_FPS1,              // Index 34
  JLINKARM_CM4_REG_FPS2,              // Index 35
  JLINKARM_CM4_REG_FPS3,              // Index 36
  JLINKARM_CM4_REG_FPS4,              // Index 37
  JLINKARM_CM4_REG_FPS5,              // Index 38
  JLINKARM_CM4_REG_FPS6,              // Index 39
  JLINKARM_CM4_REG_FPS7,              // Index 40
  JLINKARM_CM4_REG_FPS8,              // Index 41
  JLINKARM_CM4_REG_FPS9,              // Index 42
  JLINKARM_CM4_REG_FPS10,             // Index 43
  JLINKARM_CM4_REG_FPS11,             // Index 44
  JLINKARM_CM4_REG_FPS12,             // Index 45
  JLINKARM_CM4_REG_FPS13,             // Index 46
  JLINKARM_CM4_REG_FPS14,             // Index 47
  JLINKARM_CM4_REG_FPS15,             // Index 48
  JLINKARM_CM4_REG_FPS16,             // Index 49
  JLINKARM_CM4_REG_FPS17,             // Index 50
  JLINKARM_CM4_REG_FPS18,             // Index 51
  JLINKARM_CM4_REG_FPS19,             // Index 52
  JLINKARM_CM4_REG_FPS20,             // Index 53
  JLINKARM_CM4_REG_FPS21,             // Index 54
  JLINKARM_CM4_REG_FPS22,             // Index 55
  JLINKARM_CM4_REG_FPS23,             // Index 56
  JLINKARM_CM4_REG_FPS24,             // Index 57
  JLINKARM_CM4_REG_FPS25,             // Index 58
  JLINKARM_CM4_REG_FPS26,             // Index 59
  JLINKARM_CM4_REG_FPS27,             // Index 60
  JLINKARM_CM4_REG_FPS28,             // Index 61
  JLINKARM_CM4_REG_FPS29,             // Index 62
  JLINKARM_CM4_REG_FPS30,             // Index 63
  JLINKARM_CM4_REG_FPS31,             // Index 64
  //
  // Pseudo registers (not really CPU registers but handled as such ones
  //
  JLINKARM_CM4_REG_DWT_CYCCNT,        // Index 65
  //
  // New regs introduced with ARMv8M architecture
  //
  JLINKARM_CM4_REG_MSP_NS,
  JLINKARM_CM4_REG_PSP_NS,
  JLINKARM_CM4_REG_MSP_S,
  JLINKARM_CM4_REG_PSP_S,
  JLINKARM_CM4_REG_MSPLIM_S,
  JLINKARM_CM4_REG_PSPLIM_S,
  JLINKARM_CM4_REG_MSPLIM_NS,
  JLINKARM_CM4_REG_PSPLIM_NS,
  JLINKARM_CM4_REG_CFBP_S,
  JLINKARM_CM4_REG_CFBP_NS,
  //
  // Data for CFBP pseudo regs is expected & returned
  // in same byte lane as when reading/writing the CFBP.
  // This means when reading
  //  PRIMASK,     the data of PRIMASK is expected/returned at     bits [7:0]
  //  BASEPRI,     the data of BASEPRI is expected/returned at     bits [15:8]
  //  BASEPRI_MAX, the data of BASEPRI_MAX is expected/returned at bits [15:8]
  //  FAULTMASK,   the data of FAULTMASK is expected/returned at   bits [23:16]
  //  CONTROL,     the data of CONTROL is expected/returned at     bits [31:24]
  //
  // Sample CFBP = 0xx11223344
  // Read PRIMASK   will return 0x00000044
  // Read BASEPRI   will return 0x00003300
  // Read FAULTMASK will return 0x00220000
  // Read CONTROL   will return 0x11000000
  // When writing, data is expected at the same position
  //
  JLINKARM_CM4_REG_PRIMASK_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_FAULTMASK_NS,      // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_CONTROL_NS,        // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_MAX_NS,    // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_PRIMASK_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_FAULTMASK_S,       // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_CONTROL_S,         // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_REG_BASEPRI_MAX_S,     // Pseudo reg. (Part of CFBP)
  JLINKARM_CM4_NUM_REGS
} JLINKARM_CM4_REG;

//
// JLINKARM_CORTEX_R4_REG
//
typedef enum {
  JLINKARM_CORTEX_R4_REG_R0,          // Index  0
  JLINKARM_CORTEX_R4_REG_R1,          // Index  1
  JLINKARM_CORTEX_R4_REG_R2,          // Index  2
  JLINKARM_CORTEX_R4_REG_R3,          // Index  3
  JLINKARM_CORTEX_R4_REG_R4,          // Index  4
  JLINKARM_CORTEX_R4_REG_R5,          // Index  5
  JLINKARM_CORTEX_R4_REG_R6,          // Index  6
  JLINKARM_CORTEX_R4_REG_R7,          // Index  7
  JLINKARM_CORTEX_R4_REG_CPSR,        // Index  8
  JLINKARM_CORTEX_R4_REG_R15,         // Index  9
  JLINKARM_CORTEX_R4_REG_R8_USR,      // Index 10
  JLINKARM_CORTEX_R4_REG_R9_USR,      // Index 11
  JLINKARM_CORTEX_R4_REG_R10_USR,     // Index 12
  JLINKARM_CORTEX_R4_REG_R11_USR,     // Index 13
  JLINKARM_CORTEX_R4_REG_R12_USR,     // Index 14
  JLINKARM_CORTEX_R4_REG_R13_USR,     // Index 15
  JLINKARM_CORTEX_R4_REG_R14_USR,     // Index 16
  JLINKARM_CORTEX_R4_REG_SPSR_FIQ,    // Index 17
  JLINKARM_CORTEX_R4_REG_R8_FIQ,      // Index 18
  JLINKARM_CORTEX_R4_REG_R9_FIQ,      // Index 19
  JLINKARM_CORTEX_R4_REG_R10_FIQ,     // Index 20
  JLINKARM_CORTEX_R4_REG_R11_FIQ,     // Index 21
  JLINKARM_CORTEX_R4_REG_R12_FIQ,     // Index 22
  JLINKARM_CORTEX_R4_REG_R13_FIQ,     // Index 23
  JLINKARM_CORTEX_R4_REG_R14_FIQ,     // Index 24
  JLINKARM_CORTEX_R4_REG_SPSR_SVC,    // Index 25
  JLINKARM_CORTEX_R4_REG_R13_SVC,     // Index 26
  JLINKARM_CORTEX_R4_REG_R14_SVC,     // Index 27
  JLINKARM_CORTEX_R4_REG_SPSR_ABT,    // Index 28
  JLINKARM_CORTEX_R4_REG_R13_ABT,     // Index 29
  JLINKARM_CORTEX_R4_REG_R14_ABT,     // Index 30
  JLINKARM_CORTEX_R4_REG_SPSR_IRQ,    // Index 31
  JLINKARM_CORTEX_R4_REG_R13_IRQ,     // Index 32
  JLINKARM_CORTEX_R4_REG_R14_IRQ,     // Index 33
  JLINKARM_CORTEX_R4_REG_SPSR_UND,    // Index 34
  JLINKARM_CORTEX_R4_REG_R13_UND,     // Index 35
  JLINKARM_CORTEX_R4_REG_R14_UND,     // Index 36
  JLINKARM_CORTEX_R4_REG_FPSID,       // Index 37
  JLINKARM_CORTEX_R4_REG_FPSCR,       // Index 38
  JLINKARM_CORTEX_R4_REG_FPEXC,       // Index 39
  JLINKARM_CORTEX_R4_REG_FPS0,        // Index 40
  JLINKARM_CORTEX_R4_REG_FPS1,        // Index 41
  JLINKARM_CORTEX_R4_REG_FPS2,        // Index 42
  JLINKARM_CORTEX_R4_REG_FPS3,        // Index 43
  JLINKARM_CORTEX_R4_REG_FPS4,        // Index 44
  JLINKARM_CORTEX_R4_REG_FPS5,        // Index 45
  JLINKARM_CORTEX_R4_REG_FPS6,        // Index 46
  JLINKARM_CORTEX_R4_REG_FPS7,        // Index 47
  JLINKARM_CORTEX_R4_REG_FPS8,        // Index 48
  JLINKARM_CORTEX_R4_REG_FPS9,        // Index 49
  JLINKARM_CORTEX_R4_REG_FPS10,       // Index 50
  JLINKARM_CORTEX_R4_REG_FPS11,       // Index 51
  JLINKARM_CORTEX_R4_REG_FPS12,       // Index 52
  JLINKARM_CORTEX_R4_REG_FPS13,       // Index 53
  JLINKARM_CORTEX_R4_REG_FPS14,       // Index 54
  JLINKARM_CORTEX_R4_REG_FPS15,       // Index 55
  JLINKARM_CORTEX_R4_REG_FPS16,       // Index 56
  JLINKARM_CORTEX_R4_REG_FPS17,       // Index 57
  JLINKARM_CORTEX_R4_REG_FPS18,       // Index 58
  JLINKARM_CORTEX_R4_REG_FPS19,       // Index 59
  JLINKARM_CORTEX_R4_REG_FPS20,       // Index 60
  JLINKARM_CORTEX_R4_REG_FPS21,       // Index 61
  JLINKARM_CORTEX_R4_REG_FPS22,       // Index 62
  JLINKARM_CORTEX_R4_REG_FPS23,       // Index 63
  JLINKARM_CORTEX_R4_REG_FPS24,       // Index 64
  JLINKARM_CORTEX_R4_REG_FPS25,       // Index 65
  JLINKARM_CORTEX_R4_REG_FPS26,       // Index 66
  JLINKARM_CORTEX_R4_REG_FPS27,       // Index 67
  JLINKARM_CORTEX_R4_REG_FPS28,       // Index 68
  JLINKARM_CORTEX_R4_REG_FPS29,       // Index 69
  JLINKARM_CORTEX_R4_REG_FPS30,       // Index 70
  JLINKARM_CORTEX_R4_REG_FPS31,       // Index 71
  JLINKARM_CORTEX_R4_REG_MVFR0,       // Index 72
  JLINKARM_CORTEX_R4_REG_MVFR1,       // Index 73
  JLINKARM_CORTEX_R4_REG_R8,          // Index 74
  JLINKARM_CORTEX_R4_REG_R9,          // Index 75
  JLINKARM_CORTEX_R4_REG_R10,         // Index 76
  JLINKARM_CORTEX_R4_REG_R11,         // Index 77
  JLINKARM_CORTEX_R4_REG_R12,         // Index 78
  JLINKARM_CORTEX_R4_REG_R13,         // Index 79
  JLINKARM_CORTEX_R4_REG_R14,         // Index 80
  JLINKARM_CORTEX_R4_REG_SPSR,        // Index 81
  JLINKARM_CORTEX_R4_NUM_REGS
} JLINKARM_CORTEX_R4_REG;

//
// JLINKARM_RX_REG
//
typedef enum {
  JLINKARM_RX_REG_R0,                 // Index 0
  JLINKARM_RX_REG_R1,                 // Index 1
  JLINKARM_RX_REG_R2,                 // Index 2
  JLINKARM_RX_REG_R3,                 // Index 3
  JLINKARM_RX_REG_R4,                 // Index 4
  JLINKARM_RX_REG_R5,                 // Index 5
  JLINKARM_RX_REG_R6,                 // Index 6
  JLINKARM_RX_REG_R7,                 // Index 7
  JLINKARM_RX_REG_R8,                 // Index 8
  JLINKARM_RX_REG_R9,                 // Index 9
  JLINKARM_RX_REG_R10,                // Index 10
  JLINKARM_RX_REG_R11,                // Index 11
  JLINKARM_RX_REG_R12,                // Index 12
  JLINKARM_RX_REG_R13,                // Index 13
  JLINKARM_RX_REG_R14,                // Index 14
  JLINKARM_RX_REG_R15,                // Index 15
  JLINKARM_RX_REG_ISP,                // Index 16
  JLINKARM_RX_REG_USP,                // Index 17
  JLINKARM_RX_REG_INTB,               // Index 18
  JLINKARM_RX_REG_PC,                 // Index 19
  JLINKARM_RX_REG_PSW,                // Index 20
  JLINKARM_RX_REG_BPC,                // Index 21
  JLINKARM_RX_REG_BPSW,               // Index 22
  JLINKARM_RX_REG_FINTV,              // Index 23
  JLINKARM_RX_REG_FPSW,               // Index 24
  JLINKARM_RX_REG_CPEN,               // Index 25
  JLINKARM_RX_REG_ACCUH,              // Index 26  ACCU0[63:32]  (On RXv1 ACC[63:32] as there is only one ACC)
  JLINKARM_RX_REG_ACCUL,              // Index 27  ACCU0[31:0]   (On RXv1 ACC[31:0] as there is only one ACC)
  JLINKARM_RX_REG_ACCUE,              // Index 28  For RXv2: ACCU0[95:64], not present for RXv1
  JLINKARM_RX_REG_ACCU1H,             // Index 29  For RXv2: ACCU1[63:32], not present for RXv1
  JLINKARM_RX_REG_ACCU1L,             // Index 30  For RXv2: ACCU1[31:0], not present for RXv1
  JLINKARM_RX_REG_ACCU1E,             // Index 31  For RXv2: ACCU1[95:64], not present for RXv1
  JLINKARM_RX_REG_EXTB,               // Index 32
  JLINKARM_RX_NUM_REGS
} JLINKARM_RX_REG;

typedef enum {
  //
  //  General purpose registers:
  //
  JLINK_MIPS_REG_R0,            //    r0 (zero)      Always 0 (hard-wired)
  JLINK_MIPS_REG_R1,            //    r1 (at)        Assembler Temporary
  JLINK_MIPS_REG_R2,            //    r2 (v0)        Function Return Values
  JLINK_MIPS_REG_R3,            //    r3 (v1)        Function Return Values
  JLINK_MIPS_REG_R4,            //    r4 (a0)        Function Arguments
  JLINK_MIPS_REG_R5,            //    r5 (a1)        Function Arguments
  JLINK_MIPS_REG_R6,            //    r6 (a2)        Function Arguments
  JLINK_MIPS_REG_R7,            //    r7 (a3)        Function Arguments
  JLINK_MIPS_REG_R8,            //    r8 (t0)        Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R9,            //    r9 (t1)        Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R10,           //    r10 (t2)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R11,           //    r11 (t3)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R12,           //    r12 (t4)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R13,           //    r13 (t5)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R14,           //    r14 (t6)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R15,           //    r15 (t7)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R16,           //    r16 (s0)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R17,           //    r17 (s1)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R18,           //    r18 (s2)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R19,           //    r19 (s3)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R20,           //    r20 (s4)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R21,           //    r21 (s5)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R22,           //    r22 (s6)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R23,           //    r23 (s7)       Saved Temporary - Caller must preserve contents
  JLINK_MIPS_REG_R24,           //    r24 (t8)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R25,           //    r25 (t9)       Temporary - Caller does not need to preserve contents
  JLINK_MIPS_REG_R26,           //    r26 (k0)       Kernel temporary - Used for interrupt and exception handling
  JLINK_MIPS_REG_R27,           //    r27 (k1)       Kernel temporary - Used for interrupt and exception handling
  JLINK_MIPS_REG_R28,           //    r28 (gp)       Global Pointer - Used for fast-access common data
  JLINK_MIPS_REG_R29,           //    r29 (sp)       Stack Pointer - Software stack
  JLINK_MIPS_REG_R30,           //    r30 (s8 or fp) Saved Temporary - Caller must preserve contents OR Frame Pointer - Pointer to procedure frame on stack
  JLINK_MIPS_REG_R31,           //    r31 (ra)       Return Address (hard-wired)
  //
  // CP0 registers
  //
  JLINK_MIPS_REG_HWRENA,        // Index 32: HWREna        (CP0R7,  Sel0)
  JLINK_MIPS_REG_BADVADDR,      // Index 33: BadVAddr      (CP0R8,  Sel0)
  JLINK_MIPS_REG_COUNT,         // Index 34: Count         (CP0R9,  Sel0)
  JLINK_MIPS_REG_COMPARE,       // Index 35: Compare       (CP0R11, Sel0)
  JLINK_MIPS_REG_STATUS,        // Index 36: Status        (CP0R12, Sel0)
  JLINK_MIPS_REG_INTCTL,        // Index 37: IntCtl        (CP0R12, Sel1)
  JLINK_MIPS_REG_SRSCTL,        // Index 38: SRSCtl        (CP0R12, Sel2)
  JLINK_MIPS_REG_SRSMAP,        // Index 39: SRSMap        (CP0R12, Sel3)
  JLINK_MIPS_REG_CAUSE,         // Index 40: Cause         (CP0R13, Sel0)
  JLINK_MIPS_REG_EPC,           // Index 41: EPC           (CP0R14, Sel0)
  JLINK_MIPS_REG_PRID,          // Index 42: PRId          (CP0R15, Sel0)
  JLINK_MIPS_REG_EBASE,         // Index 43: EBASE         (CP0R15, Sel1)
  JLINK_MIPS_REG_CONFIG,        // Index 44: Config        (CP0R16, Sel0)
  JLINK_MIPS_REG_CONFIG1,       // Index 45: Config1       (CP0R16, Sel1)
  JLINK_MIPS_REG_CONFIG2,       // Index 46: Config2       (CP0R16, Sel2)
  JLINK_MIPS_REG_CONFIG3,       // Index 47: Config3       (CP0R16, Sel3)
  JLINK_MIPS_REG_DEBUG,         // Index 48: Debug         (CP0R23, Sel0)
  JLINK_MIPS_REG_TRACECONTROL,  // Index 49: TraceControl  (CP0R23, Sel1)
  JLINK_MIPS_REG_TRACECONTROL2, // Index 50: TraceControl2 (CP0R23, Sel2)
  JLINK_MIPS_REG_USERTRACEDATA, // Index 51: UserTraceData (CP0R23, Sel3)
  JLINK_MIPS_REG_TRACEBPC,      // Index 52: TraceBPC      (CP0R23, Sel4)
  JLINK_MIPS_REG_DEBUG2,        // Index 53: Debug2        (CP0R23, Sel5) CP0R23, Sel 6 on microAptiv core
  JLINK_MIPS_REG_PC,            // Index 54: DEPC          (CP0R24, Sel0)
  JLINK_MIPS_REG_ERROR_PC,      // Index 55: ErrorEPC      (CP0R30, Sel0)
  JLINK_MIPS_REG_DESAVE,        // Index 56: DESAVE        (CP0R31, Sel0)
  //
  // Special purpose registers
  //
  JLINK_MIPS_REG_HI,            // Index 57: HI
  JLINK_MIPS_REG_LO,            // Index 58: LO
  //
  // Additional registers (available on microAptiv core only)
  //
  JLINK_MIPS_REG_LO1,              // Index 59
  JLINK_MIPS_REG_LO2,              // Index 60
  JLINK_MIPS_REG_LO3,              // Index 61
  JLINK_MIPS_REG_HI1,              // Index 62
  JLINK_MIPS_REG_HI2,              // Index 63
  JLINK_MIPS_REG_HI3,              // Index 64
  JLINK_MIPS_REG_INDEX,            // Index 65: Index                 (CP0R0, Sel0)                 Index into the TLB array (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_RANDOM,           // Index 66: Random                (CP0R1, Sel0)                 Randomly generated index into the TLB array (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_ENTRY_LO0,        // Index 67: EntryLo0              (CP0R2, Sel0)                 Low-order portion of the TLB entry for even-numbered virtual pages (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_ENTRY_LO1,        // Index 68: EntryLo1              (CP0R3, Sel0)                 Low-order portion of the TLB entry for odd-numbered virtual pages (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_CONTEXT,          // Index 69: Context               (CP0R4, Sel0)                 Pointer to the page table entry in memory (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_USER_LOCAL,       // Index 70: UserLocal             (CP0R4, Sel2)                 User information that can be written by privileged software and read via the RDHWR instruction.
  JLINK_MIPS_REG_PAGE_MASK,        // Index 71: PageMask              (CP0R5, Sel0)                 PageMask controls the variable page sizes in TLB entries.
  JLINK_MIPS_REG_PAGE_GRAIN,       // Index 72: PageGrain             (CP0R5, Sel1)                 PageGrain enables support of 1 KB pages in the TLB (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WIRED,            // Index 73: Wired                 (CP0R6, Sel0)                 Controls the number of fixed (i.e., wired) TLB entries (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_BADINSTR,         // Index 74: BadInstr              (CP0R8, Sel1)                 Reports the instruction that caused the most recent exception
  JLINK_MIPS_REG_BADINSTRP,        // Index 75: BadInstrP             (CP0R8, Sel2)                 Reports the branch instruction if a delay slot caused the most recent exception
  JLINK_MIPS_REG_ENTRYHI,          // Index 76: EntryHi               (CP0R10, Sel0)                High-order portion of the TLB entry (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_VIEWIPL,          // Index 77: View_IPL              (CP0R12, Sel4)                Allows the Priority Level to be read/written without extracting or inserting that bit from/to the Status register.
  JLINK_MIPS_REG_SRSMAP2,          // Index 78: SRSMAP2               (CP0R12, Sel5)                Contains two 4-bit fields that provide the mapping from a vector number to the shadow set number to use when servicing such an interrupt.
  JLINK_MIPS_REG_VIEW_RIPL,        // Index 79: View_RIPL             (CP0R13, Sel4)                Enables read access to the RIPL bit that is available in the Cause register.
  JLINK_MIPS_REG_NESTEDEXC,        // Index 80: NestedExc             (CP0R13, Sel5)                Contains the error and exception level status bit values that existed prior to the current exception.
  JLINK_MIPS_REG_NESTEDEPC,        // Index 81: NestedEPC             (CP0R14, Sel2)                Contains the exception program counter that existed prior to the current exception.
  JLINK_MIPS_REG_CDMMBASE,         // Index 82: CDMMBase              (CP0R15, Sel2)                Common device memory map base.
  JLINK_MIPS_REG_CONFIG4,          // Index 83: Config4               (CP0R16, Sel4)                Configuration register 4.
  JLINK_MIPS_REG_CONFIG5,          // Index 84: Config5               (CP0R16, Sel5)                Configuration register 5.
  JLINK_MIPS_REG_CONFIG7,          // Index 85: Config7               (CP0R16, Sel7)                Configuration register 7.
  JLINK_MIPS_REG_LLADDR,           // Index 86: LLAddr                (CP0R17, Sel0)                Load link address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_USERTRACEDATA2,   // Index 87: UserTraceData2        (CP0R24, Sel3)                EJTAG user trace data 2 register.
  JLINK_MIPS_REG_PERFCTL0,         // Index 88: PerfCtl0              (CP0R25, Sel0)                Performance counter 0 control.
  JLINK_MIPS_REG_PERFCNT0,         // Index 89: PerfCnt0              (CP0R25, Sel1)                Performance counter 0.
  JLINK_MIPS_REG_PERFCTL1,         // Index 90: PerfCtl1              (CP0R25, Sel2)                Performance counter 1 control.
  JLINK_MIPS_REG_PERFCNT1,         // Index 91: PerfCnt1              (CP0R25, Sel3)                Performance counter 1.
  JLINK_MIPS_REG_ERRCTL,           // Index 92: ErrCtl                (CP0R26, Sel0)                Software test enable of way-select and data RAM arrays for I-Cache and D-Cache (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_CACHEERR,         // Index 93: CacheErr              (CP0R27, Sel0)                Records of information about cache/SPRAM parity errors
  JLINK_MIPS_REG_TAGLO,            // Index 94: TagLo                 (CP0R28, Sel0)                Low-order portion of cache tag interface (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_DATALO,           // Index 95: DataLo                (CP0R28, Sel1)                Low-order portion of cache tag interface (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_KSCRATCH1,        // Index 96: KScratch1             (CP0R31, Sel2)                Scratch Register for Kernel Mode
  JLINK_MIPS_REG_KSCRATCH2,        // Index 97: KScratch2             (CP0R31, Sel3)                Scratch Register for Kernel Mode
  JLINK_MIPS_REG_WATCHLO0,         // Index 98: WatchLo0              (CP0R18, Sel0)                Low-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHLO1,         // Index 99: WatchLo1              (CP0R18, Sel1)                Low-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHLO2,         // Index 100: WatchLo2              (CP0R18, Sel2)                Low-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHLO3,         // Index 101: WatchLo3              (CP0R18, Sel3)                Low-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHLO4,         // Index 102: WatchLo4              (CP0R18, Sel4)                Low-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHLO5,         // Index 103: WatchLo5              (CP0R18, Sel5)                Low-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHLO6,         // Index 104: WatchLo6              (CP0R18, Sel6)                Low-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHLO7,         // Index 105: WatchLo7              (CP0R18, Sel7)                Low-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHHI0,         // Index 106: WatchHi0              (CP0R19, Sel0)                High-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHHI1,         // Index 107: WatchHi1              (CP0R19, Sel1)                High-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHHI2,         // Index 108: WatchHi2              (CP0R19, Sel2)                High-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHHI3,         // Index 109: WatchHi3              (CP0R19, Sel3)                High-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHHI4,         // Index 110: WatchHi4              (CP0R19, Sel4)                High-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHHI5,         // Index 111: WatchHi5             (CP0R19, Sel5)                High-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHHI6,         // Index 112: WatchHi6             (CP0R19, Sel6)                High-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_REG_WATCHHI7,         // Index 113: WatchHi7             (CP0R19, Sel7)                High-order watchpoint address (microAptiv(TM) MPU only).
  JLINK_MIPS_NUM_REGS
} JLINK_MIPS_REG;

//
// Do NEVER change the numeric value of the register description, as certain parts of the DLL rely on this
//
typedef enum {
  //
  // R0-R7 of each bank
  //
  JLINK_EFM8_R0_B0,     // Index 0: R0, bank 0
  JLINK_EFM8_R1_B0,
  JLINK_EFM8_R2_B0,
  JLINK_EFM8_R3_B0,
  JLINK_EFM8_R4_B0,
  JLINK_EFM8_R5_B0,
  JLINK_EFM8_R6_B0,
  JLINK_EFM8_R7_B0,
  JLINK_EFM8_R0_B1,     // Index 8: R0, bank 1
  JLINK_EFM8_R1_B1,
  JLINK_EFM8_R2_B1,
  JLINK_EFM8_R3_B1,
  JLINK_EFM8_R4_B1,
  JLINK_EFM8_R5_B1,
  JLINK_EFM8_R6_B1,
  JLINK_EFM8_R7_B1,
  JLINK_EFM8_R0_B2,     // Index 16: R0, bank 2
  JLINK_EFM8_R1_B2,
  JLINK_EFM8_R2_B2,
  JLINK_EFM8_R3_B2,
  JLINK_EFM8_R4_B2,
  JLINK_EFM8_R5_B2,
  JLINK_EFM8_R6_B2,
  JLINK_EFM8_R7_B2,
  JLINK_EFM8_R0_B3,     // Index 24: R0, bank 3
  JLINK_EFM8_R1_B3,
  JLINK_EFM8_R2_B3,
  JLINK_EFM8_R3_B3,
  JLINK_EFM8_R4_B3,
  JLINK_EFM8_R5_B3,
  JLINK_EFM8_R6_B3,
  JLINK_EFM8_R7_B3,
  //
  // Special registers like PC, stackpointer etc.
  //
  JLINK_EFM8_PC,        // Index 32: PC, 16-bit
  JLINK_EFM8_A,         // Accumulator
  JLINK_EFM8_B,
  JLINK_EFM8_DPTR,      // Data pointer register (16-bit)
  JLINK_EFM8_SP,
  JLINK_EFM8_PSW,
  //
  // Pseudo regs. Mapped to currenttly selected register bank
  // Bank is selected via PSW
  //
  JLINK_EFM8_R0,        // Index 38: Current R0 (pseudo reg)
  JLINK_EFM8_R1,
  JLINK_EFM8_R2,
  JLINK_EFM8_R3,
  JLINK_EFM8_R4,
  JLINK_EFM8_R5,
  JLINK_EFM8_R6,
  JLINK_EFM8_R7,
  //
  // End of list
  //
  JLINK_EFM8_NUM_REGS
} JLINK_EFM8_REG;

//
// JLINK_CF_REG
//
typedef enum {
  //
  // CPU registers
  //
  JLINK_CF_REG_D0,
  JLINK_CF_REG_D1,
  JLINK_CF_REG_D2,
  JLINK_CF_REG_D3,
  JLINK_CF_REG_D4,
  JLINK_CF_REG_D5,
  JLINK_CF_REG_D6,
  JLINK_CF_REG_D7,
  JLINK_CF_REG_A0,
  JLINK_CF_REG_A1,
  JLINK_CF_REG_A2,
  JLINK_CF_REG_A3,
  JLINK_CF_REG_A4,
  JLINK_CF_REG_A5,
  JLINK_CF_REG_A6,
  JLINK_CF_REG_A7,
  JLINK_CF_REG_OTHER_A7,
  JLINK_CF_REG_USP,                   // User Stack Pointer
  JLINK_CF_REG_SSP,                   // Supervisor Stack Pointer
  JLINK_CF_REG_SR,                    // Status Register
  JLINK_CF_REG_PC,                    // Program Counter
  JLINK_CF_REG_PC_CORRECTED,          // Program Counter Corrected
  //
  // Misc. CPU registers
  //
  JLINK_CF_REG_VBR,                   // Vector Base Register
  JLINK_CF_REG_CACR,                  // Cache Control Register
  JLINK_CF_REG_ACR0,                  // Access Control Register 0
  JLINK_CF_REG_ACR1,                  // Access Control Register 1
  JLINK_CF_REG_ACR2,                  // Access Control Register 2
  JLINK_CF_REG_ACR3,                  // Access Control Register 3
  //
  // MMU registers (only available if MMU is present)
  //
  JLINK_CF_REG_ASID,                  // Address Space Identifier
  JLINK_CF_REG_MMUBAR,                // MMU Base Address Register
  //
  // MAC registers (only available if MAC is present)
  //
  JLINK_CF_REG_MACSR,                 // MAC Status Register
  JLINK_CF_REG_MASK,                  // MAC Address Mask Register
  JLINK_CF_REG_ACC0,                  // MAC Accumulator 0
  //
  // EMAC registers (only available if EMAC is present)
  //
  JLINK_CF_REG_ACC1,                  // MAC Accumulator 1
  JLINK_CF_REG_ACC2,                  // MAC Accumulator 2
  JLINK_CF_REG_ACC3,                  // MAC Accumulator 3
  JLINK_CF_REG_ACCext01,              // MAC Accumulator 0,1 extension bytes
  JLINK_CF_REG_ACCext23,              // MAC Accumulator 2,3 extension bytes
  //
  // FPU registers (only available if FPU is present)
  //
  JLINK_CF_REG_FPU0,                  // FPU Data Register 0 (MSB)
  JLINK_CF_REG_FPL0,                  // FPU Data Register 0 (LSB)
  JLINK_CF_REG_FPU1,                  // FPU Data Register 1 (MSB)
  JLINK_CF_REG_FPL1,                  // FPU Data Register 1 (LSB)
  JLINK_CF_REG_FPU2,                  // FPU Data Register 2 (MSB)
  JLINK_CF_REG_FPL2,                  // FPU Data Register 2 (LSB)
  JLINK_CF_REG_FPU3,                  // FPU Data Register 3 (MSB)
  JLINK_CF_REG_FPL3,                  // FPU Data Register 3 (LSB)
  JLINK_CF_REG_FPU4,                  // FPU Data Register 4 (MSB)
  JLINK_CF_REG_FPL4,                  // FPU Data Register 4 (LSB)
  JLINK_CF_REG_FPU5,                  // FPU Data Register 5 (MSB)
  JLINK_CF_REG_FPL5,                  // FPU Data Register 5 (LSB)
  JLINK_CF_REG_FPU6,                  // FPU Data Register 6 (MSB)
  JLINK_CF_REG_FPL6,                  // FPU Data Register 6 (LSB)
  JLINK_CF_REG_FPU7,                  // FPU Data Register 7 (MSB)
  JLINK_CF_REG_FPL7,                  // FPU Data Register 7 (LSB)
  JLINK_CF_REG_FPIAR,                 // FPU Instruction Address Register
  JLINK_CF_REG_FPSR,                  // FPU Status Register
  JLINK_CF_REG_FPCR,                  // FPU Control Register
  JLINK_CF_NUM_REGS
} JLINK_CF_REG;

//
// JLINK_POWER_PC_REG
//
typedef enum {
  //
  // General Purpose Registers
  //
  JLINK_POWER_PC_REG_R0,              // Index 0
  JLINK_POWER_PC_REG_R1,              // Index 1
  JLINK_POWER_PC_REG_R2,              // Index 2
  JLINK_POWER_PC_REG_R3,              // Index 3
  JLINK_POWER_PC_REG_R4,              // Index 4
  JLINK_POWER_PC_REG_R5,              // Index 5
  JLINK_POWER_PC_REG_R6,              // Index 6
  JLINK_POWER_PC_REG_R7,              // Index 7
  JLINK_POWER_PC_REG_R8,              // Index 8
  JLINK_POWER_PC_REG_R9,              // Index 9
  JLINK_POWER_PC_REG_R10,             // Index 10
  JLINK_POWER_PC_REG_R11,             // Index 11
  JLINK_POWER_PC_REG_R12,             // Index 12
  JLINK_POWER_PC_REG_R13,             // Index 13
  JLINK_POWER_PC_REG_R14,             // Index 14
  JLINK_POWER_PC_REG_R15,             // Index 15
  JLINK_POWER_PC_REG_R16,             // Index 16
  JLINK_POWER_PC_REG_R17,             // Index 17
  JLINK_POWER_PC_REG_R18,             // Index 18
  JLINK_POWER_PC_REG_R19,             // Index 19
  JLINK_POWER_PC_REG_R20,             // Index 20
  JLINK_POWER_PC_REG_R21,             // Index 21
  JLINK_POWER_PC_REG_R22,             // Index 22
  JLINK_POWER_PC_REG_R23,             // Index 23
  JLINK_POWER_PC_REG_R24,             // Index 24
  JLINK_POWER_PC_REG_R25,             // Index 25
  JLINK_POWER_PC_REG_R26,             // Index 26
  JLINK_POWER_PC_REG_R27,             // Index 27
  JLINK_POWER_PC_REG_R28,             // Index 28
  JLINK_POWER_PC_REG_R29,             // Index 29
  JLINK_POWER_PC_REG_R30,             // Index 30
  JLINK_POWER_PC_REG_R31,             // Index 31
  JLINK_POWER_PC_REG_CR,              // Index 32  Condition Register
  JLINK_POWER_PC_REG_CTR,             // Index 33  Count Register
  JLINK_POWER_PC_REG_LR,              // Index 34  Link
  JLINK_POWER_PC_REG_XER,             // Index 35  XER
  JLINK_POWER_PC_REG_PC,              // Index 36
  //
  // Processor Control Registers
  //
  JLINK_POWER_PC_REG_MSR,             // Index 37  Machine State
  JLINK_POWER_PC_REG_PVR,             // Index 38  Processor Version
  JLINK_POWER_PC_REG_PIR,             // Index 39  Processor ID
  JLINK_POWER_PC_REG_SVR,             // Index 40  System Version
  JLINK_POWER_PC_REG_HID0,            // Index 41  Hardware Implementation Dependent
  JLINK_POWER_PC_REG_HID1,            // Index 42  Hardware Implementation Dependent
  //
  // Exception Handling/Control Registers
  //
  JLINK_POWER_PC_REG_SPRG0,           // Index 43  SPR General 0
  JLINK_POWER_PC_REG_SPRG1,           // Index 44  SPR General 1
  JLINK_POWER_PC_REG_SRR0,            // Index 45  Save and Restore
  JLINK_POWER_PC_REG_SRR1,            // Index 46  Save and Restore
  JLINK_POWER_PC_REG_CSRR0,           // Index 47  Save and Restore
  JLINK_POWER_PC_REG_CSRR1,           // Index 48  Save and Restore
  JLINK_POWER_PC_REG_DSRR0,           // Index 49  Save and Restore
  JLINK_POWER_PC_REG_DSRR1,           // Index 50  Save and Restore
  JLINK_POWER_PC_REG_ESR,             // Index 51  Exception Syndrome
  JLINK_POWER_PC_REG_MCSR,            // Index 52  Machine Check Syndrome Register
  JLINK_POWER_PC_REG_DEAR,            // Index 53  Data Exception Address
  JLINK_POWER_PC_REG_IVPR,            // Index 54  Interrupt Vector Prefix
  //
  // Memory Management Registers
  //
  JLINK_POWER_PC_REG_PID0,            // Index 55  Process ID
  JLINK_POWER_PC_REG_MMUCFG,          // Index 56
  //
  // Cache Registers
  //
  JLINK_POWER_PC_REG_L1CFG0,          // Index 57
  //
  // BTB Registers
  //
  JLINK_POWER_PC_REG_BUCSR,           // Index 58
  //
  // Debug Registers
  //
  JLINK_POWER_PC_REG_DBCR0,           // Index 59
  JLINK_POWER_PC_REG_DBCR1,           // Index 60
  JLINK_POWER_PC_REG_DBCR2,           // Index 61
  JLINK_POWER_PC_REG_DBSR,            // Index 62
  JLINK_POWER_PC_REG_IAC1,            // Index 63
  JLINK_POWER_PC_REG_IAC2,            // Index 64
  JLINK_POWER_PC_REG_IAC3,            // Index 65
  JLINK_POWER_PC_REG_IAC4,            // Index 66
  JLINK_POWER_PC_REG_DAC1,            // Index 67
  JLINK_POWER_PC_REG_DAC2,            // Index 68
  JLINK_POWER_PC_NUM_REGS
} JLINK_POWER_PC_REG;

typedef struct {
  U32 SizeofStruct;                   // Filled in by user
  U8 Type;                            // Of type JLINK_STRACE_EVENT_TYPE_
  U8 Op;                              // Of type JLINK_STRACE_OP_
  U8 AccessSize;
  U8 Reserved0;
  U64 Addr;
  U64 Data;
  U64 DataMask;
  U32 AddrRangeSize;
  //
  // Everything from here on was not present in the initial version of this struct and needs to be checked to be present via SizeofStruct
  //
} JLINK_STRACE_EVENT_INFO;

//
// Indices for JLINKARM_GetDebugInfo()
// Coresight infos start at index 0x100
//
#define JLINKARM_ROM_TABLE_ADDR_INDEX                       (0x100)
#define JLINKARM_DEBUG_INFO_ETM_ADDR_INDEX                  (0x101)
#define JLINKARM_DEBUG_INFO_MTB_ADDR_INDEX                  (0x102)
#define JLINKARM_DEBUG_INFO_TPIU_ADDR_INDEX                 (0x103)
#define JLINKARM_DEBUG_INFO_ITM_ADDR_INDEX                  (0x104)
#define JLINKARM_DEBUG_INFO_DWT_ADDR_INDEX                  (0x105)
#define JLINKARM_DEBUG_INFO_FPB_ADDR_INDEX                  (0x106)
#define JLINKARM_DEBUG_INFO_NVIC_ADDR_INDEX                 (0x107)
#define JLINKARM_DEBUG_INFO_TMC_ADDR_INDEX                  (0x108)
#define JLINKARM_DEBUG_INFO_TF_ADDR_INDEX                   (0x109)
#define JLINKARM_DEBUG_INFO_PTM_ADDR_INDEX                  (0x10A)
#define JLINKARM_DEBUG_INFO_ETB_ADDR_INDEX                  (0x10B)
#define JLINKARM_DEBUG_INFO_DBG_ADDR_INDEX                  (0x10C)
#define JLINKARM_DEBUG_INFO_APBAP_INDEX                     (0x10D)
#define JLINKARM_DEBUG_INFO_AHBAP_INDEX                     (0x10E)
#define JLINKARM_DEBUG_INFO_HAS_CORTEX_M_SECURITY_EXT_INDEX (0x10F)

//
// ARM_ENDIAN
//
typedef enum {
  ARM_ENDIAN_LITTLE, ARM_ENDIAN_BIG
} ARM_ENDIAN;

//
// ARM_RESET_TYPE
//
typedef enum {
  JLINKARM_RESET_TYPE_NORMAL = 0,    // Resets core + peripherals. Reset pin is avoided where possible and reset via SFR access is preferred.
  //
  // --- Start ---
  // Do NOT use anymore
  //
  JLINKARM_RESET_TYPE_BP0,
  JLINKARM_RESET_TYPE_ADI,
  JLINKARM_RESET_TYPE_NO_RESET,
  JLINKARM_RESET_TYPE_HALT_WP,
  JLINKARM_RESET_TYPE_HALT_DBGRQ,
  JLINKARM_RESET_TYPE_SOFT,
  JLINKARM_RESET_TYPE_HALT_DURING,
  JLINKARM_RESET_TYPE_SAM7,
  JLINKARM_RESET_TYPE_LPC,
  //
  // --- End ---
  //
  //
  // Generic J-Link reset types (core independent)
  // CPU-specific reset types are still in the header for backward compatibility but should not be used anymore
  // All reset types halt the CPU before executing the first instruction of the user application, after reset release
  // If the CPU incorporates a ROM bootloader, J-Link makes sure that this bootloader is executed and the CPU is halted as soon as it jumps into the user application code
  //
  // Note:
  // If a specific reset type also resets the debug logic, it may happen that the CPU cannot be halted immediately after reset
  // so it may have already executed some instructions before J-Link has a chance to halt it
  //
  JLINK_RESET_TYPE_CORE=100,         // Resets core only
  JLINK_RESET_TYPE_RESET_PIN         // Toggles reset pin in order to issue a reset. Requires reset pin to be connected, otherwise result will be unpredictable
} JLINKARM_RESET_TYPE;

//
// SWO Conversion Mode
//
//
#define JLINK_SWO_CONVERSION_MODE_WINDOWS  0    // If only '\n' is received, make it "\r\n" to make the line end Windows-compliant. (Default behavior)
#define JLINK_SWO_CONVERSION_MODE_NONE     1    // Leave everything as it is, do not add any characters

//
// JLINKARM_INDICATOR_CTRL
//
typedef struct {
  U16 IndicatorId;
  U16 Override;               // 1: Controlled by Host (PC), 0: controlled by emulator (default)
  U16 InitialOnTime;          // [1ms]
  U16 OnTime;                 // [1ms]
  U16 OffTime;                // [1ms]
} JLINKARM_INDICATOR_CTRL;

//
// JTAG_ID_DATA
//
typedef struct {
  int NumDevices;             // Number of devices in this scan chain
  U16 ScanLen;                // Total number of bits in all scan chain select register
  U32 aId      [3];
  U8  aScanLen [3];           // Number of bits in individual scan chain select registers
  U8  aIrRead  [3];           // Data read back from instruction register
  U8  aScanRead[3];           // Data read back from scan chain select register
} JTAG_ID_DATA;

//
// JLINKARM_BP_INFO
//
typedef struct {
  U32   SizeOfStruct;         // In
  U32   Handle;               // Out (also "In" if Index is invalid)
  U32   Addr;                 // Out
  U32   Type;                 // Out
  U32   ImpFlags;             // Out
  U32   UseCnt;               // Out
  U8    Internal;             // Out
  U8    Disabled;             // Out
} JLINKARM_BP_INFO;

//
// JLINKARM_WP_INFO
//
typedef struct {
  U32   SizeOfStruct;
  U32   Handle;
  U32   Addr;
  U32   AddrMask;
  U32   Data;
  U32   DataMask;
  U32   Ctrl;
  U32   CtrlMask;
  U8    WPUnit;
  U8    Internal;
  U8    Disabled;
  U8    Dummy;
  U32   UnitMask;
} JLINKARM_WP_INFO;

//
// JLINKARM_DATA_EVENT
//
typedef struct {
  int   SizeOfStruct;
  int   Type;
  U32   Addr;
  U32   AddrMask;
  U32   Data;
  U32   DataMask;
  U8    Access;
  U8    AccessMask;
} JLINKARM_DATA_EVENT;

//
// JLINKARM_HALT_REASON
//
#define JLINKARM_HALT_REASON_DBGRQ                (0)
#define JLINKARM_HALT_REASON_CODE_BREAKPOINT      (1)
#define JLINKARM_HALT_REASON_DATA_BREAKPOINT      (2)
#define JLINKARM_HALT_REASON_VECTOR_CATCH         (3)

typedef struct {
  int HaltReason;
  int Index;
} JLINKARM_MOE_INFO;

typedef struct {
  U32 Addr;
  U32 Size;
} JLINK_FLASH_AREA_INFO;

typedef struct {
  U32 Addr;
  U32 Size;
} JLINK_RAM_AREA_INFO;

typedef struct {
  const char*             sBankName;
  const char*             sAlgoFile;
  U32                     AlgoType;        // Really of type MAIN_FLASH_ALGO_TYPE but to avoid mutual inclusion, we choose U32 here for now...
  U32                     BaseAddr;
  const void*             paBlockInfo;     // For some algos, e.g. for SPIFI, this is just a default block info that may vary from target to target, so we need to request the actual one from the target via the RAMCode, at runtime
  const void*             pAlgoInfo;
} JLINK_FLASH_BANK_INFO;

//
// JLINKARM_DEVICE_INFO
//
typedef struct {
  U32                     SizeOfStruct;           // Required. Use SizeofStruct = sizeof(JLINKARM_DEVICE_INFO)
  const char*             sName;
  U32                     CoreId;
  U32                     FlashAddr;              // Start address of first flash area
  U32                     RAMAddr;                // Start address of first RAM area
  char                    EndianMode;             // 0=Little, 1=Big, 2=Both
  U32                     FlashSize;              // Total flash size in bytes (flash may contain gaps. For exact address & size of each region, please refer to aFlashArea)
  U32                     RAMSize;                // Total RAM size in bytes  (RAM may contain gaps. For exact address & size of each region, please refer to aRAMArea)
  const char*             sManu;                  // Device manufacturer
  JLINK_FLASH_AREA_INFO   aFlashArea[32];         // Region size of 0 bytes marks the end of the list
  JLINK_RAM_AREA_INFO     aRAMArea[32];           // Region size of 0 bytes marks the end of the list
  U32                     Core;                   // JLINKARM_CORE_... value
  JLINK_FLASH_BANK_INFO   aFlashBank[16];
} JLINKARM_DEVICE_INFO;

//
// JLINKARM_DEVICE_SELECT_INFO
//
typedef struct {
  U32 SizeOfStruct;           // Required. Use SizeofStruct = sizeof(JLINKARM_DEVICE_SELECT_INFO)
} JLINKARM_DEVICE_SELECT_INFO;

//
// JLINKARM_JTAG_DEVICE_CONF
//
typedef struct {
  U32 SizeOfStruct;           // Required. Use SizeofStruct = sizeof(JTAG_DEVICE_CONF)
  U32 IRLen;                  // Required
  U32 IRPrint;                // Optional. 0 means invalid.
  U32 Id;                     // Optional. 0 means invalid.
  const char* sName;          // Optional. NULL means invalid.
} JLINKARM_JTAG_DEVICE_CONF;

//
// JLINKARM_JTAG_DEVICE_INFO
//
typedef struct {
  const char* sName;
  U32         IRLen;
  U32         IRPrint;
} JLINKARM_JTAG_DEVICE_INFO;

//
// JLINKARM_RTCK_REACT_INFO
//
typedef struct {
  U32 SizeOfStruct;
  U32 Min;
  U32 Max;
  U32 Average;
} JLINKARM_RTCK_REACT_INFO;

//
// JLINKARM_SPEED_INFO
//
typedef struct {
  U32 SizeOfStruct;
  U32 BaseFreq;               // Clock frequency in Hz
  U16 MinDiv;                 // Minimum divider.  Max JTAG speed is BaseFreq / MinDiv.
  U16 SupportAdaptive;
} JLINKARM_SPEED_INFO;

//
// JLINKARM_EMU_INFO
//
typedef struct {
  U32 SizeOfStruct;
  U32 USBAddr;
  U32 SerialNo;
} JLINKARM_EMU_INFO;

//
// JLINKARM_EMU_CONNECT_INFO
//
typedef struct {
  U32       SerialNumber;             // This is the serial number reported in the discovery process, which is the "true serial number" for newer J-Links and 123456 for older J-Links.
  unsigned  Connection;               // Either JLINKARM_HOSTIF_USB = 1 or JLINKARM_HOSTIF_IP = 2
  U32       USBAddr;                  // USB Addr. Default is 0, values of 0..3 are permitted (Only filled if for J-Links connected via USB)
  U8        aIPAddr[16];              // IP Addr. in case emulator is connected via IP. For IP4 (current version), only the first 4 bytes are used.
  int       Time;                     // J-Link via IP only: Time period [ms] after which we have received the UDP discover answer from emulator (-1 if emulator is connected over USB)
  U64       Time_us;                  // J-Link via IP only: Time period [us] after which we have received the UDP discover answer from emulator (-1 if emulator is connected over USB)
  U32       HWVersion;                // J-Link via IP only: Hardware version of J-Link
  U8        abMACAddr[6];             // J-Link via IP only: MAC Addr
  char      acProduct[32];            // Product name
  char      acNickName[32];           // J-Link via IP only: Nickname of J-Link
  char      acFWString[112];          // J-Link via IP only: Firmware string of J-Link
  char      IsDHCPAssignedIP;         // J-Link via IP only: Is J-Link configured for IP address reception via DHCP?
  char      IsDHCPAssignedIPIsValid;  // J-Link via IP only
  char      NumIPConnections;         // J-Link via IP only: Number of IP connections which are currently established to this J-Link
  char      NumIPConnectionsIsValid;  // J-Link via IP only
  U8        aPadding[34];             // Pad struct size to 264 bytes
} JLINKARM_EMU_CONNECT_INFO;          // In general, unused fields are zeroed.

//
// JLINKARM_HW_STATUS
//
typedef struct {
  U16 VTarget;                // Target supply voltage
  U8  tck;                    // Measured state of TCK pin:  0 or 1 or 255 (unsupported/unknown)
  U8  tdi;                    // Measured state of TDI pin:  0 or 1 or 255 (unsupported/unknown)
  U8  tdo;                    // Measured state of TDO pin:  0 or 1 or 255 (unsupported/unknown)
  U8  tms;                    // Measured state of TMS pin:  0 or 1 or 255 (unsupported/unknown)
  U8  tres;                   // Measured state of TRES pin: 0 or 1 or 255 (unsupported/unknown)
  U8  trst;                   // Measured state of TRST pin: 0 or 1 or 255 (unsupported/unknown)
} JLINKARM_HW_STATUS;

//
// JLINKARM_TRACE_DATA
//
typedef struct {
  U8 PipeStat;
  U8 Sync;
  U16 Packet;
} JLINKARM_TRACE_DATA;

//
// JLINKARM_TRACE_REGION_PROPS
//
typedef struct {
  U32 SizeofStruct;      // Size of this structure (allows extension in future versions)
  U32 RegionIndex;       // 0- based Index of region, where index 0 means oldest region.
  U32 NumSamples;        // Number of samples in region
  U32 Off;               // Offset in trace buffer
} JLINKARM_TRACE_REGION_PROPS;

//
// JLINKARM_TRACE_REGION_PROPS
//
typedef struct {
  U32 SizeofStruct;      // Size of this structure (allows extension in future versions)
  U32 RegionIndex;       // 0- based Index of region, where index 0 means oldest region.
  U32 NumSamples;        // Number of samples in region
  U32 Off;               // Offset in trace buffer
  U32 RegionCnt;
  U32 Dummy;
  U64 TimeStamp;
} JLINKARM_TRACE_REGION_PROPS_EX;

//
// JLINKARM_SWO_START_INFO
//
typedef struct {
  U32 SizeofStruct;         // Size of this structure (allows extension in future versions)
  U32 Interface;            // 0: UART, 1: Manchester
  U32 Speed;                // 0: Auto, else [Hz]
} JLINKARM_SWO_START_INFO;

//
// JLINKARM_SWO_SPEED_INFO
//
typedef struct {
  U32 SizeofStruct;         // Size of this structure (allows extension in future versions)
  U32 Interface;            // 0: UART, 1: Manchester
  U32 BaseFreq;
  U32 MinDiv;
  U32 MaxDiv;
  U32 MinPrescale;          // 0: 1, 1: 2, 2: 4, 3: 8, ...
  U32 MaxPrescale;          // 0: 1, 1: 2, 2: 4, 3: 8, ...
} JLINKARM_SWO_SPEED_INFO;

//
// JLINK_SPI_COMMAND_DESC
//
typedef struct {
  U32       Command;
  const U8* pDataDown;
  U8*       pDataUp;
  U32       NumBits;
  U32       Flags;
  U8*       pCmdArgs;
  U32       NumBytesArgs;
  int       Result;
} JLINK_SPI_COMMAND_DESC;

//
// JLINK_WRITE_MEM_DESC
//
typedef struct {
  U32 Addr;
  U32 NumBytes;
  U8* pData;
  U8* pStatus;
  U32 AccessWidth;
  U32 Flags;
  int Result;
} JLINK_WRITE_MEM_DESC;

//
// JLINK_POWERTRACE_DATA_ITEM
//
typedef struct {
  U32 RefValue;
  U32 Data;
} JLINK_POWERTRACE_DATA_ITEM;

//
// JLINK_POWERTRACE_SETUP
//
typedef struct {
  int SizeOfStruct;
  U32 ChannelMask;      // 0: Int0, 1: Int1, ... 8: Ext0, 9: Ext1
  U32 SampleFreq;
  int RefSelect;        // 0: No reference, 1: SWOByteCnt
  int EnableCond;       // 0: Always, 1: RUN_ONLY
} JLINK_POWERTRACE_SETUP;

//
// JLINK_POWERTRACE_CAPS
//
typedef struct {
  int SizeOfStruct;
  U32 ChannelMask;      // 0: Int0, 1: Int1, ... 8: Ext0, 9: Ext1
} JLINK_POWERTRACE_CAPS;

//
// JLINK_POWERTRACE_CHANNEL_CAPS_OUT
//
typedef struct {
  int SizeOfStruct;
  U32 BaseFreq;
  U32 MinDiv;
} JLINK_POWERTRACE_CHANNEL_CAPS_OUT;

//
// JLINK_POWERTRACE_CHANNEL_CAPS_IN
//
typedef struct {
  int SizeOfStruct;
  U32 ChannelMask;      // 0: Int0, 1: Int1, ... 8: Ext0, 9: Ext1
} JLINK_POWERTRACE_CHANNEL_CAPS_IN;

//
// JLINK_RTTERMINAL_START
//
typedef struct {
  U32 ConfigBlockAddress;
  U32 Dummy0;         // Future use. SBZ
  U32 Dummy1;         // Future use. SBZ
  U32 Dummy2;         // Future use. SBZ
} JLINK_RTTERMINAL_START;

//
// JLINK_RTTERMINAL_STOP
//
typedef struct {
  U8  InvalidateTargetCB;
  U8  acDummy[3];     // Future use. SBZ
  U32 Dummy0;         // Future use. SBZ
  U32 Dummy1;         // Future use. SBZ
  U32 Dummy2;         // Future use. SBZ
} JLINK_RTTERMINAL_STOP;

typedef struct {
  int   BufferIndex;
  U32   Direction;
  char  acName[32];
  U32   SizeOfBuffer;
  U32   Flags;
} JLINK_RTTERMINAL_BUFDESC;

typedef struct {
  U32         NumBytesTransferred;  // Total number of bytes sent to client application(s)
  U32         NumBytesRead;         // Total number of bytes read from the target
  int         HostOverflowCount;    // Indicates a buffer overflow on the host system
  int         IsRunning;          
  int         NumUpBuffers;         // Number of channels from the target which are handled by DLL.
  int         NumDownBuffers;       // Number of channels to the target which are handled by DLL.
  U32         Dummy0;               // Future use. SBZ
  U32         Dummy1;               // Future use. SBZ
} JLINK_RTTERMINAL_STATUS;

//
// JLINK_HSS_MEM_BLOCK_DESC
//
typedef struct {
  U32 Addr;
  U32 NumBytes;
  U32 Flags;          // Future use. SBZ.
  U32 Dummy;          // Future use. SBZ.
} JLINK_HSS_MEM_BLOCK_DESC;

//
// JLINK_HSS_CAPS
//
typedef struct {
  U32 MaxBlocks;
  U32 MaxFreq;
  U32 Caps;
  U32 aDummy[5];      // Future use. SBZ.
} JLINK_HSS_CAPS;

//
// JLINK_EMU_GPIO_DESC
//
typedef struct {
  char  acName[32];
  U32   Caps;
} JLINK_EMU_GPIO_DESC;

//
// JLINKARM_EMU_PCODE_STATUS_INFO
//
typedef struct {
  U32 PCodeStat;                   // 0: Code has been executed properly
  U32 S32_PC;                      // PC of target system. Typically PC starts at 4 and last PC is the offset of a BRK instruction.
  U32 S32_R0;                      // R0 of target system. Typically used as return value.
  U32 S32_SP;                      // Stack pointer of target system. Not normally of interest.
  U32 S32_StatusReg;               // Status register of target system. Not normally of interest.
} JLINKARM_EMU_PCODE_STATUS_INFO;

//
// JLINKARM_FLASH_EVENT
//
typedef enum {
  JLINKARM_FLASH_EVENT_ERASE,
  JLINKARM_FLASH_EVENT_PROGRAM,
  JLINKARM_FLASH_EVENT_VERIFY
} JLINKARM_FLASH_EVENT;

//
// JLINKARM_FLASH_NOTIFY_INFO
//
typedef struct {
  U32 SizeofStruct;
  U32 Addr;
  U32 NumBytesTotal;
  U32 NumBytesCurrent;
} JLINKARM_FLASH_NOTIFY_INFO;

typedef struct {
  const char* sName;
  const char* sDesc;
  U64         VirtAddr;
  U8 abDummy[16];       // Round up to 32 bytes. Future use. Currently 0
} JLINK_MEM_ZONE_INFO;

typedef enum {
  JLINK_PCODE_OP_RESET=0,
  JLINK_PCODE_OP_CONNECT,
  JLINK_PCODE_OP_UNSECURE,
  JLINK_PCODE_OP_SETUP
} JLINK_PCODE_OPERATIONS;

#define JLINK_DISASSEMBLY_MODE_CURRENT  (0)
#define JLINK_DISASSEMBLY_MODE_ARM      (1)
#define JLINK_DISASSEMBLY_MODE_THUMB    (2)

typedef struct {
  U32 SizeOfStruct; // Required. Use SizeofStruct = sizeof(JLINK_DISASSEMBLY_INFO)
  U8  Mode;         // Mode to use for disassembling instruction. 0: Current CPU Mode; 1: ARM Mode; 2: Thumb Mode
  U8  aDummy[3];
} JLINK_DISASSEMBLY_INFO;
//
// Types, flash programming API
//
typedef unsigned BANK_HANDLE;
typedef unsigned DATA_FILE_HANDLE;
typedef void (JLINK_FLASH_PROGRESS_CB_FUNC)(const char * sAction, const char * sProg, int Percentage);

//
// Types, callback functions
//
typedef void STDCALL    JLINK_LOG                       (const char* sErr);
typedef void            JLINKARM_LOG                    (const char* sErr);
typedef void            JLINKARM_FLASH_NOTIFY_FUNC      (JLINKARM_FLASH_EVENT Event, const JLINKARM_FLASH_NOTIFY_INFO * pNotifyInfo);
typedef int             JLINKARM_WAIT_FUNC              (void* pContext);                                                                   // Returns 0 if o.k., else != 0
//
//
//  Flags:
//    Or combination of JLINK_DLG_BUTTON_ values.
//
//
//  Return value
//    Or combination of JLINK_DLG_BUTTON_ values.
//    Note: Only the values that have been passed in <Flags> can be returned.
//    Sample: <Flags> == JLINK_DLG_BUTTON_YES | JLINK_DLG_BUTTON_NO
//    Possible return values:
//                            JLINK_DLG_BUTTON_YES
//                            JLINK_DLG_BUTTON_NO
//

#define JLINK_UNSECURE_DIALOG_CB_FUNC  JLINK_HOOK_DIALOG_UNSECURE  // Define for backward compatability

typedef int JLINK_HOOK_DIALOG_UNSECURE       (const char* sTitle, const char* sMsg, U32 Flags);
typedef int JLINK_HOOK_DIALOG_UNLOCK_IDCODE  (const char* sTitle, const char* sMsg, U32 Flags, void* pIDCODE, int MaxNumBytesIDCODE);

//
// Typedefs for JLINK_GetpFunc()
//
typedef enum {
  JLINK_IFUNC_SET_HOOK_DIALOG_UNLOCK_IDCODE=0,
  JLINK_IFUNC_SPI_TRANSFER_MULTIPLE,
  JLINK_IFUNC_PIN_OVERRIDE,                             // Internal use. Not explained in SDK manual
  JLINK_IFUNC_PIN_OVERRIDE_GET_PIN_CAPS,                // Internal use. Not explained in SDK manual
  JLINK_IFUNC_MRU_GETLIST,
  //
  // There was one version which by accident added some defines
  // for new functions here even though the functions have never been implemented.
  // To stay binary compatible, define them as dummies
  //
  JLINK_IFUNC_RESERVED3,
  JLINK_IFUNC_RESERVED4,
  JLINK_IFUNC_RESERVED5,
  JLINK_IFUNC_GET_SESSION_ID,
  JLINK_IFUNC_CORESIGHT_TRIGGER_READ_APDP_REG,
  JLINK_IFUNC_CAN_ACC_MEM_WHILE_RUNNING,
  JLINK_IFUNC_UPDATE_BTL,
  JLINK_IFUNC_GET_CURRENT_ENDIANESS,
  JLINK_IFUNC_ALGODB_GET_PALGO_INFO,      // Only needed for SEGGER internal use (J-Flash etc.) to get add. info from the DLL. Type therefore not public, to not make it necessary to make multiple other struct types public etc.
  JLINK_IFUNC_ALGODB_GET_PALGO_INFO_CFI,  // Only needed for SEGGER internal use (J-Flash etc.) to get add. info from the DLL. Type therefore not public, to not make it necessary to make multiple other struct types public etc.
  JLINK_IFUNC_ALGODB_GET_ALGO_NO,         // Only needed for SEGGER internal use (J-Flash etc.) to get add. info from the DLL. Type therefore not public, to not make it necessary to make multiple other struct types public etc.
  JLINK_NUM_FUNC_INDEXES
} JLINK_FUNC_INDEX;

typedef int STDCALL JLINK_FUNC_SET_HOOK_DIALOG_UNLOCK_IDCODE(JLINK_HOOK_DIALOG_UNLOCK_IDCODE* pfHook);        // JLINK_IFUNC_SET_HOOK_DIALOG_UNLOCK_IDCODE
typedef int STDCALL JLINK_FUNC_SPI_TRANSFER_MULTIPLE(JLINK_SPI_COMMAND_DESC* paDesc, U32 NumCommands);        // JLINK_IFUNC_SPI_TRANSFER_MULTIPLE
typedef int STDCALL JLINK_FUNC_MRU_GETLIST(U32 Group, char* sList, U32 BufferSize);                           // JLINK_IFUNC_MRU_GETLIST
typedef int STDCALL JLINK_FUNC_PIN_OVERRIDE(const U32* paMode, U32* paState);
typedef int STDCALL JLINK_FUNC_PIN_OVERRIDE_GET_PIN_CAPS(U32* paCaps);
typedef int STDCALL JLINK_FUNC_GET_SESSION_ID(void);
typedef int STDCALL JLINK_FUNC_CORESIGHT_TRIGGER_READ_APDP_REG(U8 RegIndex, U8 APnDP, U32* pData);
typedef int STDCALL JLINK_FUNC_CAN_ACC_MEM_WHILE_RUNNING(void);
typedef int STDCALL JLINK_FUNC_UPDATE_BTL(void);
typedef int STDCALL JLINK_FUNC_GET_CURRENT_ENDIANESS(void);
typedef int STDCALL JLINK_FUNC_ALGODB_GET_PALGO_INFO(unsigned AlgoNo, unsigned Variant, const void** ppAlgoInfo);
typedef int STDCALL JLINK_FUNC_ALGODB_GET_PALGO_INFO_CFI(unsigned AlgoNo, unsigned Variant, unsigned NumChips, unsigned NumBits, unsigned CoreType, const void** ppAlgoInfo);
typedef int STDCALL JLINK_FUNC_ALGODB_GET_ALGO_NO(const void* pAlgoInfo, unsigned* pAlgoNo, unsigned* pVariant);

/*********************************************************************
*
*       Config mask
*/
#define JLINK_CONFIG_MASK_HOST_IF         (1 << 0)
#define JLINK_CONFIG_MASK_TARGET_DEVICE   (1 << 1)
#define JLINK_CONFIG_MASK_TARGET_IF       (1 << 2)

/*********************************************************************
*
*       Trace source defines
*/
#define JLINKARM_TRACE_SOURCE_ETB  0
#define JLINKARM_TRACE_SOURCE_ETM  1
#define JLINKARM_TRACE_SOURCE_MTB  2

/*********************************************************************
*
*       TRACE Commands (for JLINKARM_TRACE_Control())
*/
#define JLINKARM_TRACE_CMD_START                0
#define JLINKARM_TRACE_CMD_STOP                 1
#define JLINKARM_TRACE_CMD_FLUSH                2
#define JLINKARM_TRACE_CMD_GET_NUM_SAMPLES      0x10
#define JLINKARM_TRACE_CMD_GET_CONF_CAPACITY    0x11
#define JLINKARM_TRACE_CMD_SET_CAPACITY         0x12
#define JLINKARM_TRACE_CMD_GET_MIN_CAPACITY     0x13
#define JLINKARM_TRACE_CMD_GET_MAX_CAPACITY     0x14
#define JLINKARM_TRACE_CMD_SET_FORMAT           0x20
#define JLINKARM_TRACE_CMD_GET_FORMAT           0x21
#define JLINKARM_TRACE_CMD_GET_NUM_REGIONS      0x30
#define JLINKARM_TRACE_CMD_GET_REGION_PROPS     0x31
#define JLINKARM_TRACE_CMD_GET_REGION_PROPS_EX  0x32

/*********************************************************************
*
*       TRACE Format flags
*/
#define JLINKARM_TRACE_FORMAT_4BIT              0x00000001ul  // 4 bit data
#define JLINKARM_TRACE_FORMAT_8BIT              0x00000002ul  // 8 bit data
#define JLINKARM_TRACE_FORMAT_16BIT             0x00000004ul  // 16 bit data
#define JLINKARM_TRACE_FORMAT_MULTIPLEXED       0x00000008ul  // muliplexing on ETM/buffer link
#define JLINKARM_TRACE_FORMAT_DEMULTIPLEXED     0x00000010ul  // demultiplexing on ETM/buffer link
#define JLINKARM_TRACE_FORMAT_DOUBLE_EDGE       0x00000020ul  // clock data on both edges on ETM/buffer link
#define JLINKARM_TRACE_FORMAT_ETM7_9            0x00000040ul  // Use ETM7/ETM9 protocol
#define JLINKARM_TRACE_FORMAT_ETM10             0x00000080ul  // Use ETM10 protocol
#define JLINKARM_TRACE_FORMAT_1BIT              0x00000100ul  // 1 bit data
#define JLINKARM_TRACE_FORMAT_2BIT              0x00000200ul  // 2 bit data

/*********************************************************************
*
*       RAWTRACE Commands (for JLINKARM_RAWTRACE_Control())
*/
#define JLINKARM_RAWTRACE_CMD_START             0
#define JLINKARM_RAWTRACE_CMD_STOP              1
#define JLINKARM_RAWTRACE_CMD_GET_TRACE_FREQ    2
#define JLINKARM_RAWTRACE_CMD_SET_BUFF_SIZE     3

/*********************************************************************
*
*       RAWTRACE format flags
*/
#define JLINKARM_RAWTRACE_FORMAT_1BIT           0x00000003ul  // 1 bit data
#define JLINKARM_RAWTRACE_FORMAT_2BIT           0x0000000Cul  // 2 bit data
#define JLINKARM_RAWTRACE_FORMAT_4BIT           0x00000030ul  // 4 bit data

/*********************************************************************
*
*       RTTERMINAL Commands
*/
#define JLINKARM_RTTERMINAL_CMD_START           0
#define JLINKARM_RTTERMINAL_CMD_STOP            1
#define JLINKARM_RTTERMINAL_CMD_GETDESC         2
#define JLINKARM_RTTERMINAL_CMD_GETNUMBUF       3
#define JLINKARM_RTTERMINAL_CMD_GETSTAT         4

#define JLINKARM_RTTERMINAL_BUFFER_DIR_UP       0
#define JLINKARM_RTTERMINAL_BUFFER_DIR_DOWN     1

/*********************************************************************
*
*       POWERTRACE Commands (for JLINK_POWERTRACE_Control())
*/
#define JLINK_POWERTRACE_CMD_SETUP              0
#define JLINK_POWERTRACE_CMD_START              1
#define JLINK_POWERTRACE_CMD_STOP               2
#define JLINK_POWERTRACE_CMD_FLUSH              3
#define JLINK_POWERTRACE_CMD_GET_CAPS           4
#define JLINK_POWERTRACE_CMD_GET_CHANNEL_CAPS   5
#define JLINK_POWERTRACE_CMD_GET_NUM_ITEMS      6

/*********************************************************************
*
*       SWO Commands
*/
#define JLINKARM_SWO_CMD_START                  0         // Parameter: JLINKARM_SWO_START_INFO *
#define JLINKARM_SWO_CMD_STOP                   1
#define JLINKARM_SWO_CMD_FLUSH                  2         // Parameter: U32* Number of bytes to flush
#define JLINKARM_SWO_CMD_GET_SPEED_INFO         3
#define JLINKARM_SWO_CMD_GET_NUM_BYTES          10
#define JLINKARM_SWO_CMD_SET_BUFFERSIZE_HOST    20
#define JLINKARM_SWO_CMD_SET_BUFFERSIZE_EMU     21

/*********************************************************************
*
*       SWO Interfaces
*/
#define JLINKARM_SWO_IF_UART                    0
#define JLINKARM_SWO_IF_MANCHESTER              1         // Not supported in current version

/*********************************************************************
*
*       Firmware update results
*/
#define JLINK_FW_UPDATE_RESULT_FW_INVALID             (6)
#define JLINK_FW_UPDATE_RESULT_EMU_FW_OLDER           (5)
#define JLINK_FW_UPDATE_RESULT_EMU_FW_NEWER           (4)
#define JLINK_FW_UPDATE_RESULT_EMU_FW_CURRENT         (3)
#define JLINK_FW_UPDATE_RESULT_NO_EMBEDDED_FW         (2)
#define JLINK_FW_UPDATE_RESULT_CANCELED_BY_USER       (1)
#define JLINK_FW_UPDATE_RESULT_OK                     (0)
#define JLINK_FW_UPDATE_RESULT_ERR_GENERIC            (-1)
#define JLINK_FW_UPDATE_RESULT_ERR_UNKNOWN_FW_ID      (-2)
#define JLINK_FW_UPDATE_RESULT_ERR_NO_TIMESTAMP       (-3)

/*********************************************************************
*
*       Obsolete, only for compatibility with older versions
*                 Do not use anymore
*/
#define JLINKARM_TIF_2_WIRE_JTAG_PIC32  JLINKARM_TIF_ICSP
#define JLINKARM_GO_OVERSTEP_BP         (1 << 0)      // Obsolete, use JLINKARM_GO_FLAG_OVERSTEP_BP instead
#define JLINKARM_EMU_CAP_TRACE          JLINKARM_EMU_CAP_TRACE_ARM79
#define JLINKARM_BP_TYPE_SW_RAM         JLINKARM_BP_IMP_SW_RAM
#define JLINKARM_BP_TYPE_SW_FLASH       JLINKARM_BP_IMP_SW_FLASH
#define JLINKARM_BP_TYPE_SW             JLINKARM_BP_IMP_SW
#define JLINKARM_BP_TYPE_HW             JLINKARM_BP_IMP_HW
#define JLINKARM_BP_TYPE_ANY            JLINKARM_BP_IMP_ANY
#define JLINKARM_BP_IMP_HW_ICE          (1 << 8)
#define JLINKARM_BP_IMP_HW_ETM          (1 << 9)
#define JLINKARM_BP_IMP_HW_NEXUS        (1 << 10)
typedef U32 JLINKARM_BP_TYPE;
#define JLINKARM_TIF_UART               JLINKARM_TIF_FINE
#define JLINK_MIPS_REG_DEPC             JLINK_MIPS_REG_PC
#define JLINKARM_CM3_RESET_TYPE_NORMAL                  ((JLINKARM_RESET_TYPE)(0))      // Use RESET pin. If this fails, try to reset core.
#define JLINKARM_CM3_RESET_TYPE_CORE                    ((JLINKARM_RESET_TYPE)(1))      // Resets the core only, not peripherals.
#define JLINKARM_CM3_RESET_TYPE_RESETPIN                ((JLINKARM_RESET_TYPE)(2))      // Resets core & peripherals using RESET pin.
#define JLINKARM_CM3_RESET_TYPE_CONNECT_UNDER_RESET     ((JLINKARM_RESET_TYPE)(3))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_HALT_AFTER_BTL          ((JLINKARM_RESET_TYPE)(4))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_HALT_BEFORE_BTL         ((JLINKARM_RESET_TYPE)(5))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_KINETIS                 ((JLINKARM_RESET_TYPE)(6))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_ADI_HALT_AFTER_KERNEL   ((JLINKARM_RESET_TYPE)(7))
#define JLINKARM_CM3_RESET_TYPE_CORE_AND_PERIPHERALS    ((JLINKARM_RESET_TYPE)(8))      // Deprecated. Should no longer be used. Internally handled as JLINKARM_CM3_RESET_TYPE_NORMAL.
#define JLINKARM_CM3_RESET_TYPE_LPC1200                 ((JLINKARM_RESET_TYPE)(9))
#define JLINKARM_CM3_RESET_TYPE_S3FN60D                 ((JLINKARM_RESET_TYPE)(10))     // Performs a normal reset and disables the watchdog of the Samsung S3FN60D after the reset has been performed
#define JLINKARM_CM3_RESET_TYPE_LPC11A                  ((JLINKARM_RESET_TYPE)(11))
#define JLINKARM_CM3_RESET_TYPE_LPC176X                 ((JLINKARM_RESET_TYPE)(257))    // Used DLL-internally only. Automatically selected if LPC176x device is selected
#define JLINK_PPC_RESET_TYPE_NORMAL                     ((JLINKARM_RESET_TYPE)(0))      // Resets core & peripherals using RESET pin.

typedef JLINK_FLASH_AREA_INFO   FLASH_AREA_INFO;
typedef JLINK_RAM_AREA_INFO     RAM_AREA_INFO;

//
// JLINKARM_READ_DESC
//
typedef struct JLINKARM_READ_DESC {
  U64 Addr;
  U32 NumBytes;
  U32 Flags;
} JLINKARM_READ_DESC;

#endif

/************************** end of file *****************************/
