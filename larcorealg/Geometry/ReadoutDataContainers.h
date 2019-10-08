/**
 * @file   larcorealg/Geometry/ReadoutDataContainers.h
 * @brief  Containers to hold one datum per TPC set or readout plane.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   September 7, 2019
 * @ingroup Geometry
 *
 * This is a header-only library.
 */

#ifndef LARCOREALG_GEOMETRY_READOUTDATACONTAINERS_H
#define LARCOREALG_GEOMETRY_READOUTDATACONTAINERS_H

// LArSoft libraries
#include "larcorealg/Geometry/GeometryDataContainers.h"
#include "larcoreobj/SimpleTypesAndConstants/readout_types.h"


namespace readout {
  
  template <typename T>
  class TPCsetDataContainer;
  
  template <typename T>
  class ROPDataContainer;
  
} // namespace geo


// --- BEGIN Readout data containers -------------------------------------------
/// @name Readout data containers
/// @ingroup Geometry
/// @{

//------------------------------------------------------------------------------
/** 
 * @brief Container with one element per readout TPC set.
 * @tparam T type of the contained datum
 * @see `geo::GeometryCore::makeTPCsetData`
 *
 * The container is of fixed size and can't be neither resized nor freed
 * before destruction.
 *
 * Assumptions
 * ============
 *
 * The following assumptions should be considered unchecked, and the behavior
 * when they are violated undefined (but note that in debug mode some of them
 * might be actually checked):
 * * the container assumes the same number of TPC sets in each cryostat. While
 *   this is not effectively a necessary condition, keep in mind that this
 *   container has no notion whether a given TPC set actually exists in the
 *   geometry or not
 * * at least one element is expected to be present
 *
 */
template <typename T>
class readout::TPCsetDataContainer
  : public geo::GeoIDdataContainer<T, readout::TPCsetID>
{
  
  using BaseContainer_t = geo::GeoIDdataContainer<T, readout::TPCsetID>;
  
    public:
  
  using value_type = typename BaseContainer_t::value_type;
  
  /**
   * @brief Prepares the container with default-constructed data.
   * @param nCryo number of cryostats
   * @param nTPCsets number of TPC sets
   *
   * The container is sized to host data for `nCryo` cryostats, each with
   * `nTPCsets` TPC sets. Each element in the container is default-constructed.
   */
  TPCsetDataContainer(unsigned int nCryo, unsigned int nTPCsets)
    : BaseContainer_t({ nCryo, nTPCsets })
    {}

  /**
   * @brief Prepares the container with copies of the specified default value.
   * @param nCryo number of cryostats
   * @param nTPCsets number of TPC sets
   * @param defValue the value to be replicated
   *
   * The container is sized to host data for `nCryo` cryostats, each with
   * `nTPCsets` TPC sets. Each element in the container is a copy of defValue.
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * auto const* geom = lar::providerFrom<geo::GeometryCore>();
   * readout::TPCsetDataContainer<unsigned int> ROPsPerTPCset
   *   (geom->NCryostats(), geom->MaxTPCsets(), 3U);
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  TPCsetDataContainer
    (unsigned int nCryo, unsigned int nTPCsets, value_type const& defValue)
    : BaseContainer_t({ nCryo, nTPCsets }, defValue)
    {}


  // --- BEGIN Container status query ------------------------------------------
  /// @name Container status query
  /// @{

  /// Returns whether this container hosts data for the specified cryostat.
  bool hasCryostat(readout::CryostatID const& cryoid) const
    { return BaseContainer_t::hasElement(cryoid); }

  /// Returns whether this container hosts data for the specified TPC set.
  bool hasTPCset(readout::TPCsetID const& tpcsetid) const
    { return BaseContainer_t::hasElement(tpcsetid); }

  /// @}
  // --- END Container status query --------------------------------------------


}; // class readout::TPCsetDataContainer<>


//------------------------------------------------------------------------------
/** 
 * @brief Container with one element per readout plane.
 * @tparam T type of the contained datum
 * @see `geo::GeometryCore::makeROPdata`
 *
 * The container is of fixed size and can't be neither resized nor freed
 * before destruction.
 *
 * Assumptions
 * ============
 *
 * The following assumptions should be considered unchecked, and the behavior
 * when they are violated undefined (but note that in debug mode some of them
 * might be actually checked):
 * * the container assumes the same number of readout planes in all TPC set,
 *   and of TPC sets on all cryostats. While this is not effectively a necessary
 *   condition, keep in mind that this container has no notion whether a given
 *   readout plane or TPC set actually exists in the geometry or not
 * * at least one element is expected to be present
 *
 */
template <typename T>
class readout::ROPDataContainer
  : public geo::GeoIDdataContainer<T, readout::ROPID>
{

  /// Base class.
  using BaseContainer_t = geo::GeoIDdataContainer<T, readout::ROPID>;
  
    public:

  /**
   * @brief Prepares the container with default-constructed data.
   * @param nCryo number of cryostats
   * @param nTPCsets number of TPC sets per cryostat
   * @param nROPs number of readout planes per TPC set
   *
   * The container is sized to host data for `nCryo` cryostats, each with
   * `nTPCsets` TPC sets, each one with `nROPs` readout planes. Each element
   * in the container is default-constructed.
   */
  ROPDataContainer
    (unsigned int nCryo, unsigned int nTPCsets, unsigned int nROPs)
    : BaseContainer_t({ nCryo, nTPCsets, nROPs })
    {}

  /**
   * @brief Prepares the container with copies of the specified default value.
   * @param nCryo number of cryostats
   * @param nTPCsets number of TPC sets
   * @param nROPs number of readout planes per TPC set
   * @param defValue the value to be replicated
   *
   * The container is sized to host data for `nCryo` cryostats, each with
   * `nTPCsets` TPC sets, and each of them with `nROPs` readout planes.
   * Each element in the container is a copy of `defValue`.
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * auto const* geom = lar::providerFrom<geo::GeometryCore>();
   * readout::ROPDataContainer<unsigned int> countPerROP
   *   (geom->NCryostats(), geom->MaxTPCsets(), geom->MaxROPs(), 0U);
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  ROPDataContainer(
    unsigned int nCryo, unsigned int nTPCsets, unsigned int nROPs,
    T const& defValue
    )
    : BaseContainer_t{ { nCryo, nTPCsets, nROPs }, defValue }
    {}


  // --- BEGIN Container status query ------------------------------------------
  /// @name Container status query
  /// @{

  /// Returns whether this container hosts data for the specified cryostat.
  bool hasCryostat(readout::CryostatID const& cryoid) const
    { return BaseContainer_t::hasElement(cryoid); }

  /// Returns whether this container hosts data for the specified TPC set.
  bool hasTPCset(readout::TPCsetID const& tpcsetid) const
    { return BaseContainer_t::hasElement(tpcsetid); }

  /// Returns whether this container hosts data for the specified readout plane.
  bool hasROP(readout::ROPID const& ropid) const
    { return BaseContainer_t::hasElement(ropid); }

  /// @}
  // --- END Container status query --------------------------------------------


}; // class readout::ROPDataContainer<>


/// @}
// --- END Readout data containers ---------------------------------------------
//------------------------------------------------------------------------------


#endif // LARCOREALG_GEOMETRY_READOUTDATACONTAINERS_H
