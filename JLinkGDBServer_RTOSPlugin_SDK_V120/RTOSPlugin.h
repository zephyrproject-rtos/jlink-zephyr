/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (C) 2004-2009    SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*      Internet: www.segger.com    Support:  support@segger.com      *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : RTOSPlugin.h
Purpose     : RTOS plugin header
---------------------------END-OF-HEADER------------------------------
*/

#ifndef RTOSPLUGIN_H              // Guard against multiple inclusion
#define RTOSPLUGIN_H

#include "JLINKARM_Const.h"
#include "TYPES.h"

#if defined(__cplusplus)    // Allow usage of this module from C++ files (disable name mangling)
  extern "C" {
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define RTOS_PLUGIN_BUF_SIZE_THREAD_DISPLAY  256  // Size of buffer in bytes that is passed to RTOS_GetThreadDisplay()

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

enum RTOS_PLUGIN_CPU_REGS_CORTEX_M {
  //
  // These register indexes must NOT be in line with the J-Link DLL register indexes,
  // nor the GDB register indexes (because the GDB indexes may change, depending on if J-Link GDBServer reports a different Target.xml file to GDB
  // These indexes have been determined once and stay fixed to guarantee binary compatibility between different GDBServer versions
  //
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R0=0,       // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R1,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R2,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R3,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R4,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R5,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R6,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R7,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R8,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R9,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R10,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R11,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_R12,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_SP,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_LR,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_PC,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_XPSR,       // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_MSP,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_PSP,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_PRIMASK,    // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_BASEPRI,    // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_FAULTMASK,  // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_CONTROL,    // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_FPSCR,      // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S0,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S1,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S2,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S3,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S4,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S5,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S6,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S7,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S8,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S9,         // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S10,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S11,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S12,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S13,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S14,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S15,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S16,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S17,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S18,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S19,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S20,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S21,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S22,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S23,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S24,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S25,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S26,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S27,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S28,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S29,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S30,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_S31,        // 32-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D0,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D1,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D2,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D3,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D4,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D5,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D6,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D7,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D8,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D9,         // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D10,        // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D11,        // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D12,        // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D13,        // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D14,        // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_D15,        // 64-bit reg
  RTOS_PLUGIN_CPU_REG_CORTEX_M_NUMREGS
};

//
// RTOS symbols
//
typedef struct {
  const char *name;
  int optional;
  U32 address;
} RTOS_SYMBOLS;

//
// GDB server functions that can be called by the plugin
//
typedef struct {
  //
  // API version v1.0 and higher
  //
  void  (*pfFree)        (void* p);
  void* (*pfAlloc)       (unsigned Size);
  void* (*pfRealloc)     (void* p, unsigned Size);
  void  (*pfLogOutf)     (const char* sFormat, ...);
  void  (*pfDebugOutf)   (const char* sFormat, ...);
  void  (*pfWarnOutf)    (const char* sFormat, ...);
  void  (*pfErrorOutf)   (const char* sFormat, ...);
  int   (*pfReadMem)     (U32 Addr, char* pData, unsigned int NumBytes);
  char  (*pfReadU8)      (U32 Addr, U8* pData);
  char  (*pfReadU16)     (U32 Addr, U16* pData);
  char  (*pfReadU32)     (U32 Addr, U32* pData);
  int   (*pfWriteMem)    (U32 Addr, const char* pData, unsigned NumBytes);
  void  (*pfWriteU8)     (U32 Addr, U8 Data);
  void  (*pfWriteU16)    (U32 Addr, U16 Data);
  void  (*pfWriteU32)    (U32 Addr, U32 Data);
  U32   (*pfLoad16TE)    (const U8* p);
  U32   (*pfLoad24TE)    (const U8* p);
  U32   (*pfLoad32TE)    (const U8* p);
  //
  // API version v1.1 and higher
  //
  U32   (*pfReadReg)     (U32 RegIndex);
  void  (*pfWriteReg)    (U32 RegIndex, U32 Value);
  //
  // End marker
  //
  void   *Dummy;
} GDB_API;

#if defined(__cplusplus)    // Allow usage of this module from C++ files (disable name mangling)
  }
#endif

#endif                      // Avoid multiple inclusion

/*************************** End of file ****************************/
