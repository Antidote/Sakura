#ifndef PHYSFSSTREAM_HPP
#define PHYSFSSTREAM_HPP

#include <SFML/System.hpp>
#include <physfs.h>

class PHYSFSStream : public sf::InputStream
{
public:
    PHYSFSStream(const std::string& filepath);
    ~PHYSFSStream();

    sf::Int64 read(void *data, sf::Int64 size);
    sf::Int64 seek(sf::Int64 position);
    sf::Int64 tell();
    sf::Int64 getSize();
    bool hasError() const;
private:
    PHYSFS_File* m_file;
    bool         m_error;
};

#endif // PHYSFSSTREAM_HPP
