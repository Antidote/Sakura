#include "Sakura/Core/SSpriteFileWriter.hpp"

namespace Sakura
{
namespace Core
{

SSpriteFileWriter::SSpriteFileWriter(Uint8 *data, Uint64 length)
    : base(data, length)
{
}

SSpriteFileWriter::SSpriteFileWriter(const std::string& filepath)
    : base(filepath)
{
}

void SSpriteFileWriter::writeFile(SSpriteFile* file)
{
    ((void)file);
}

}
}
