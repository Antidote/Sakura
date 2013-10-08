#include "PHYSFSStream.hpp"

PHYSFSStream::PHYSFSStream(const std::string &filepath)
{
    m_file = PHYSFS_openRead(filepath.c_str());
    m_error = false;

    if (m_file == NULL)
    {
        sf::err() << PHYSFS_getLastError() << std::endl;
        m_error = true;
    }
}

PHYSFSStream::~PHYSFSStream()
{
    if (m_error)
        return;

    if (PHYSFS_close(m_file) == 0)
        sf::err() << PHYSFS_getLastError() << std::endl;
}

sf::Int64 PHYSFSStream::read(void* data, sf::Int64 size)
{
    if (m_error)
        return 0;

    sf::Int64 len = PHYSFS_read(m_file, data, 1, size);
    if (len == -1)
    {
        sf::err() << PHYSFS_getLastError() << std::endl;
        return 0;
    }

    return len;
}

sf::Int64 PHYSFSStream::seek(sf::Int64 position)
{
    if (m_error)
        return -1;

    if (PHYSFS_seek(m_file, position) == 0)
    {
        sf::err() << PHYSFS_getLastError() << std::endl;
        return -1;
    }

    return position;
}

sf::Int64 PHYSFSStream::tell()
{
    if (m_error)
        return -1;

    sf::Int64 position = PHYSFS_tell(m_file);

    if (position == -1)
    {
        sf::err() << PHYSFS_getLastError() << std::endl;
        return -1;
    }

    return position;
}

sf::Int64 PHYSFSStream::getSize()
{
    if (m_error)
        return -1;

    sf::Int64 size = PHYSFS_fileLength(m_file);

    if (size == -1)
        sf::err() << PHYSFS_getLastError() << std::endl;

    return size;
}

bool PHYSFSStream::hasError() const
{
    return m_error;
}
