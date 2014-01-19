#include "Sakura/Core/SSpriteFileWriter.hpp"
#include <InvalidOperationException.hpp>
#include <Sakura/Core/SSpriteFile.hpp>
#include <Sakura/Core/SSprite.hpp>
#include <Sakura/Core/SSpritePart.hpp>
#include <Sakura/Core/SSpriteFrame.hpp>

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
    if (!file)
        throw zelda::error::InvalidOperationException("SSpriteFileWriter::writeFile -> file cannot be NULL");

    base::writeUInt32(SSpriteFile::Magic);
    base::writeUInt32(SSpriteFile::Version);

    base::writeUInt16(file->textureCount());
    base::writeUInt32(file->width());
    base::writeUInt32(file->height());
    base::writeFloat(file->originX());
    base::writeFloat(file->originY());
    base::writeUInt16(file->spriteCount());

    base::writeUInt32(0xFFFFFFFF);

    for (STexture* texture : file->textures())
    {
        base::writeString(texture->Filepath);
        base::writeBool(texture->Preload);
    }

    for (std::pair<std::string, SSprite*> spritePair : file->sprites())
    {
        SSprite* sprite = spritePair.second;

        base::writeString(sprite->name());
        base::writeUInt16(sprite->partCount());
        base::writeUInt16(sprite->stateCount());

        for (int id : sprite->stateIds())
            base::writeUInt16(id);

        for (SSpritePart* part : sprite->parts())
        {
            base::writeString(part->name());
            base::writeBool(part->hasCollision());
            base::writeUInt32(part->frameCount());

            for (SSpriteFrame* frame : part->frames())
            {
                base::writeFloat(frame->offset().x);
                base::writeFloat(frame->offset().y);
                base::writeFloat(frame->textureOffset().x);
                base::writeFloat(frame->textureOffset().y);
                base::writeUInt32(frame->size().x);
                base::writeUInt32(frame->size().y);
                base::writeFloat(frame->frameTime());
                base::writeBool(frame->flippedHorizontally());
                base::writeBool(frame->flippedVertically());
            }
        }
    }

    save();
}

}
}
