#include "SoundResource.hpp"
#include <iostream>

SoundResource::SoundResource(const std::string &filename)
{
    m_filename = filename;
    m_isLoaded = false;
    m_data = new sf::SoundBuffer;
    if (m_data->loadFromFile(filename))
        m_isLoaded = true;
}

SoundResource::~SoundResource()
{
    std::cout << "Sound destroyed " << m_filename << std::endl;
    delete m_data;
}

std::string SoundResource::filename() const
{
    return m_filename;
}

sf::SoundBuffer *SoundResource::data()
{
    return m_data;
}

bool SoundResource::isLoaded() const
{
    return m_isLoaded;
}

void SoundResource::setData(sf::SoundBuffer *data)
{
    m_data = data;
}


