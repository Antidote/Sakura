#ifndef MAPFILEREADER_HPP
#define MAPFILEREADER_HPP

#include "BinaryReader.hpp"

namespace Sakura
{
namespace Core
{
class Map;

class MapFileReader : public zelda::io::BinaryReader
{
    BINARYREADER_BASE;
public:
    /*!
     * \brief MapFileReader
     * \param data
     * \param length
     */
    MapFileReader(Uint8* data, Uint64 length);

    /*!
     * \brief MapFileReader
     * \param filename
     */
    MapFileReader(const std::string& filename);

    /*!
     * \brief read
     * \return
     */
    Map* read();
};

} // Core
} // Sakura

#endif // MAPFILEREADER_HPP
