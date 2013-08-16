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
    BINARYREADER_BASE
public:
    MapFileReader(Uint8* data, Uint64 length);
    MapFileReader(const std::string& filename);

    Map* read();
};


}
}
#endif // MAPFILEREADER_HPP
