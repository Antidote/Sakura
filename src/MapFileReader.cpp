#include "MapFileReader.hpp"
#include "IOException.hpp"
#include "Map.hpp"
#include "Tile.hpp"
#include "Layer.hpp"
#include "Compression.hpp"
#include <iostream>

#if 1
namespace zelda
{
namespace io
{

MapFileReader::MapFileReader(Uint8 *data, Uint64 length)
    : base(data, length)
{
}

MapFileReader::MapFileReader(const std::string &filename)
    : base(filename)
{
}


Map* MapFileReader::read()
{
    Map* ret = new Map;

    Uint32 magic = base::readUInt32();

    if (magic != Map::MAGIC_NUMBER)
        throw zelda::error::IOException("MapFileReader::read -> Not a valid zmap file");

    Uint32 version = base::readUInt32();
    if (version != Map::VERSION)
        throw zelda::error::IOException("MapFileReader::read -> Unsupported version");

    Uint16 bom = readUInt16();
    if (bom == 0xFFFE)
        base::setEndianess(zelda::BigEndian);

    ret->setName(base::readString());
    int rgba = base::readUInt32();
    ret->setBackgroundColor(*((RGBA*)&rgba));

    ret->setWidth(base::readUInt32());
    ret->setHeight(base::readUInt32());

    ret->setTileWidth(base::readUInt16());
    ret->setTileHeight(base::readUInt16());

    Uint32 tilesetCount = base::readUInt32();
    Uint32 layerCount = base::readUInt32();

    base::seek((base::position() + 0x1F) & ~0x1F, base::Beginning);

    while ((tilesetCount--) > 0)
    {
        ret->addTileset(base::readString());
    }
    // Align to 32bytes for Wii/GCN support
    base::seek((base::position() + 0x1F) & ~0x1F, base::Beginning);

    int compressedSize = base::readUInt32();
    int uncompressedSize = base::readUInt32();

    // data is compressed if the compressed size is smaller than
    // uncompressedSize
    if (compressedSize < uncompressedSize)
    {
        Uint8* data = readUBytes(compressedSize);
        Uint8* uncompData = new Uint8[uncompressedSize];

        int ret = zelda::io::Compression::decompressZlib(data, compressedSize, uncompData, uncompressedSize);
        if (ret != uncompressedSize)
            throw zelda::error::IOException("MapReader::read -> Error decompressing data");

        base::setData(uncompData, uncompressedSize);

        delete[] data;
    }

    for (int i = 0; i < layerCount; i++)
    {
        std::cout << "Layer " << (i + 1) << std::endl;
        Layer* layer = new Layer;
        layer->setVisible(base::readBool());
        layer->setZOrder(base::readUInt32());
        int tileCount = base::readUInt32();
        base::seek((base::position() + 0x1F) & ~0x1F, base::Beginning);

        for (int j = 0; j < tileCount; j++)
        {
            for (int k = 0; k < ret->width()/ret->tileWidth(), j < tileCount; k++, j++)
            {
                Tile* tile = new Tile();
                tile->setId(base::readUInt32());
                tile->setTileset(base::readUInt32());
                tile->setFlippedHor(base::readBit());
                tile->setFlippedVer(base::readBit());
                tile->setFlippedDiag(base::readBit());

                tile->setPosition(base::readUInt16(), base::readUInt16());

                layer->addTile(tile);
            }
        }
        base::seek((base::position() + 0x1F) & ~0x1F, base::Beginning);
        ret->addLayer(layer);
    }
    std::unordered_map<int, std::unordered_map<int, Cell*> > collision;
    for (int y = 0; y < ret->height()/ret->tileHeight(); y++)
    {
        for (int x = 0; x < ret->width() / ret->tileWidth(); x++)
        {
            Cell* cell = new Cell;
            cell->CollisionType = base::readByte();
            cell->FlippedAndDamage = base::readByte();
            collision[x][y] = cell;
        }

    }

    ret->setCollisionData(collision);

    return ret;
}

} // io
} // zelda
#endif
