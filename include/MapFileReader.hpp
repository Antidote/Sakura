#ifndef MAPFILEREADER_HPP
#define MAPFILEREADER_HPP

#include "BinaryReader.hpp"

class Map;

namespace zelda
{
namespace io
{

class MapFileReader : public BinaryReader
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


}
}
#endif // MAPFILEREADER_HPP
