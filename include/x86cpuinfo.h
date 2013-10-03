// [AsmJit]
// Complete JIT Assembler for C++ Language.
//
// [License]
// Zlib - See COPYING file in this package.

// [Guard]
#ifndef _ASMJIT_X86_X86CPUINFO_H
#define _ASMJIT_X86_X86CPUINFO_H
#ifndef DOXYGEN

// [Dependencies - AsmJit]
#include <SFML/Config.hpp>

#include "cpuinfo.h"
namespace AsmJit {
//! @brief X86 CPU features.
enum kX86Feature
{
  //! @brief Cpu has RDTSC instruction.
  kX86FeatureRdtsc = 1U << 0,
  //! @brief Cpu has RDTSCP instruction.
  kX86FeatureRdtscP = 1U << 1,
  //! @brief Cpu has CMOV instruction (conditional move)
  kX86FeatureCMov = 1U << 2,
  //! @brief Cpu has CMPXCHG8B instruction
  kX86FeatureCmpXchg8B = 1U << 3,
  //! @brief Cpu has CMPXCHG16B instruction (64-bit processors)
  kX86FeatureCmpXchg16B = 1U << 4,
  //! @brief Cpu has CLFUSH instruction
  kX86FeatureClFlush = 1U << 5,
  //! @brief Cpu has PREFETCH instruction
  kX86FeaturePrefetch = 1U << 6,
  //! @brief Cpu supports LAHF and SAHF instrictions.
  kX86FeatureLahfSahf = 1U << 7,
  //! @brief Cpu supports FXSAVE and FXRSTOR instructions.
  kX86FeatureFXSR = 1U << 8,
  //! @brief Cpu supports FXSAVE and FXRSTOR instruction optimizations (FFXSR).
  kX86FeatureFFXSR = 1U << 9,
  //! @brief Cpu has MMX.
  kX86FeatureMmx = 1U << 10,
  //! @brief Cpu has extended MMX.
  kX86FeatureMmxExt = 1U << 11,
  //! @brief Cpu has 3dNow!
  kX86Feature3dNow = 1U << 12,
  //! @brief Cpu has enchanced 3dNow!
  kX86Feature3dNowExt = 1U << 13,
  //! @brief Cpu has SSE.
  kX86FeatureSse = 1U << 14,
  //! @brief Cpu has SSE2.
  kX86FeatureSse2 = 1U << 15,
  //! @brief Cpu has SSE3.
  kX86FeatureSse3 = 1U << 16,
  //! @brief Cpu has Supplemental SSE3 (SSSE3).
  kX86FeatureSsse3 = 1U << 17,
  //! @brief Cpu has SSE4.A.
  kX86FeatureSse4A = 1U << 18,
  //! @brief Cpu has SSE4.1.
  kX86FeatureSse41 = 1U << 19,
  //! @brief Cpu has SSE4.2.
  kX86FeatureSse42 = 1U << 20,
  //! @brief Cpu has AVX.
  kX86FeatureAvx = 1U << 22,
  //! @brief Cpu has Misaligned SSE (MSSE).
  kX86FeatureMSse = 1U << 23,
  //! @brief Cpu supports MONITOR and MWAIT instructions.
  kX86FeatureMonitorMWait = 1U << 24,
  //! @brief Cpu supports MOVBE instruction.
  kX86FeatureMovBE = 1U << 25,
  //! @brief Cpu supports POPCNT instruction.
  kX86FeaturePopCnt = 1U << 26,
  //! @brief Cpu supports LZCNT instruction.
  kX86FeatureLzCnt = 1U << 27,
  //! @brief Cpu supports PCLMULDQ set of instructions.
  kX86FeaturePclMulDQ  = 1U << 28,
  //! @brief Cpu supports multithreading.
  kX86FeatureMultiThreading = 1U << 29,
  //! @brief Cpu supports execute disable bit (execute protection).
  kX86FeatureExecuteDisableBit = 1U << 30,
  //! @brief 64-bit CPU.
  kX86Feature64Bit = 1U << 31
};

// ============================================================================
// [AsmJit::kX86Bug]
// ============================================================================

//! @brief X86 CPU bugs.
enum kX86Bug
{
  //! @brief Whether the processor contains bug seen in some
  //! AMD-Opteron processors.
  kX86BugAmdLockMB = 1U << 0
};

//! @brief Cpu vendor IDs.
//!
//! Cpu vendor IDs are specific for AsmJit library. Vendor ID is not directly
//! read from cpuid result, instead it's based on CPU vendor string.
enum kCpu
{
  //! @brief Unknown CPU vendor.
  kCpuUnknown = 0,

  //! @brief Intel CPU vendor.
  kCpuIntel = 1,
  //! @brief AMD CPU vendor.
  kCpuAmd = 2,
  //! @brief National Semiconductor CPU vendor (applies also to Cyrix processors).
  kCpuNSM = 3,
  //! @brief Transmeta CPU vendor.
  kCpuTransmeta = 4,
  //! @brief VIA CPU vendor.
  kCpuVia = 5
};
//! @addtogroup AsmJit_X86
//! @{

// ============================================================================
// [AsmJit::X86CpuId]
// ============================================================================

//! @brief X86 CpuId output.
union X86CpuId
{
  //! @brief EAX/EBX/ECX/EDX output.
  sf::Uint32 i[4];

  struct
  {
    //! @brief EAX output.
    sf::Uint32 eax;
    //! @brief EBX output.
    sf::Uint32 ebx;
    //! @brief ECX output.
    sf::Uint32 ecx;
    //! @brief EDX output.
    sf::Uint32 edx;
  };
};

// ============================================================================
// [AsmJit::X86CpuInfo]
// ============================================================================

struct X86CpuInfo : public CpuInfo
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  inline X86CpuInfo(sf::Uint32 size = sizeof(X86CpuInfo)) :
    CpuInfo(size)
  {
  }

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  //! @brief Get processor type.
  inline sf::Uint32 getProcessorType() const { return _processorType; }
  //! @brief Get brand index.
  inline sf::Uint32 getBrandIndex() const { return _brandIndex; }
  //! @brief Get flush cache line size.
  inline sf::Uint32 getFlushCacheLineSize() const { return _flushCacheLineSize; }
  //! @brief Get maximum logical processors count.
  inline sf::Uint32 getMaxLogicalProcessors() const { return _maxLogicalProcessors; }
  //! @brief Get APIC physical ID.
  inline sf::Uint32 getApicPhysicalId() const { return _apicPhysicalId; }

  // --------------------------------------------------------------------------
  // [Statics]
  // --------------------------------------------------------------------------

  //! @brief Get global instance of @ref X86CpuInfo.
  static inline const X86CpuInfo* getGlobal()
  { return static_cast<const X86CpuInfo*>(CpuInfo::getGlobal()); }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  //! @brief Processor type.
  sf::Uint32 _processorType;
  //! @brief Brand index.
  sf::Uint32 _brandIndex;
  //! @brief Flush cache line size in bytes.
  sf::Uint32 _flushCacheLineSize;
  //! @brief Maximum number of addressable IDs for logical processors.
  sf::Uint32 _maxLogicalProcessors;
  //! @brief Initial APIC ID.
  sf::Uint32 _apicPhysicalId;
};

// ============================================================================
// [AsmJit::x86CpuId]
// ============================================================================

#if defined(__GNUC__) || defined(_MSC_VER)
//! @brief Calls CPUID instruction with eax == @a in and stores output to @a out.
//!
//! @c cpuid() function has one input parameter that is passed to cpuid through
//! eax register and results in four output values representing result of cpuid
//! instruction (eax, ebx, ecx and edx registers).
void x86CpuId(sf::Uint32 in, X86CpuId* out);

// ============================================================================
// [AsmJit::x86CpuDetect]
// ============================================================================

//! @brief Detect CPU features to CpuInfo structure @a out.
//!
//! @sa @c CpuInfo.
void x86CpuDetect(X86CpuInfo* out);
#endif // ASMJIT_X86 || ASMJIT_X64

//! @}

} // AsmJit namespace


// [Guard]
#endif // DOXYGEN
#endif // _ASMJIT_X86_X86CPUINFO_H
