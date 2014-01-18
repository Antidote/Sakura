// [AsmJit]
// Complete JIT Assembler for C++ Language.
//
// [License]
// Zlib - See COPYING file in this package.

// [Guard]
#ifndef _ASMJIT_CORE_CPUINFO_H
#define _ASMJIT_CORE_CPUINFO_H
#ifndef DOXYGEN
#include <SFML/Config.hpp>


namespace AsmJit {

//! @addtogroup AsmJit_Core
//! @{

// ============================================================================
// [AsmJit::CpuInfo]
// ============================================================================

//! @brief Informations about host cpu.
struct CpuInfo
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  inline CpuInfo(sf::Uint32 size = sizeof(CpuInfo)) :
    _size(size)
  {
  }

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  //! @brief Get CPU vendor string.
  inline const char* getVendorString() const { return _vendorString; }
  //! @brief Get CPU brand string.
  inline const char* getBrandString() const { return _brandString; }

  //! @brief Get CPU vendor ID.
  inline sf::Uint32 getVendorId() const { return _vendorId; }
  //! @brief Get CPU family ID.
  inline sf::Uint32 getFamily() const { return _family; }
  //! @brief Get CPU model ID.
  inline sf::Uint32 getModel() const { return _model; }
  //! @brief Get CPU stepping.
  inline sf::Uint32 getStepping() const { return _stepping; }
  //! @brief Get CPU count.
  inline sf::Uint32 getNumberOfProcessors() const { return _numberOfProcessors; }
  //! @brief Get CPU features.
  inline sf::Uint32 getFeatures() const { return _features; }
  //! @brief Get CPU bugs.
  inline sf::Uint32 getBugs() const { return _bugs; }

  //! @brief Get whether CPU has feature @a feature.
  inline bool hasFeature(sf::Uint32 feature) { return (_features & feature) != 0; }
  //! @brief Get whether CPU has bug @a bug.
  inline bool hasBug(sf::Uint32 bug) { return (_bugs & bug) != 0; }

  // --------------------------------------------------------------------------
  // [Statics]
  // --------------------------------------------------------------------------

  //! @brief Detect number of processors.
  static sf::Uint32 detectNumberOfProcessors();

  //! @brief Get global instance of @ref CpuInfo.
  static const CpuInfo* getGlobal();

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  //! @brief Size of CpuInfo structure (in bytes).
  sf::Uint32 _size;

  //! @brief Cpu short vendor string.
  char _vendorString[16];
  //! @brief Cpu long vendor string (brand).
  char _brandString[64];

  //! @brief Cpu vendor id (see @c AsmJit::CpuInfo::VendorId enum).
  sf::Uint32 _vendorId;
  //! @brief Cpu family ID.
  sf::Uint32 _family;
  //! @brief Cpu model ID.
  sf::Uint32 _model;
  //! @brief Cpu stepping.
  sf::Uint32 _stepping;
  //! @brief Number of processors or cores.
  sf::Uint32 _numberOfProcessors;
  //! @brief Cpu features bitfield, see @c AsmJit::CpuInfo::Feature enum).
  sf::Uint32 _features;
  //! @brief Cpu bugs bitfield, see @c AsmJit::CpuInfo::Bug enum).
  sf::Uint32 _bugs;
};

//! @}

} // AsmJit namespace

// [Guard]
#endif // DOXYGEN
#endif // _ASMJIT_CORE_CPUINFO_H
