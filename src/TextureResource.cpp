#include "TextureResource.hpp"
#include <iostream>

TextureResource::TextureResource(const std::string &filename)
{
    m_filename = filename;
    m_isLoaded = false;
    m_data = new sf::Texture;
    if (m_data->loadFromFile(filename))
        m_isLoaded = true;
}

TextureResource::~TextureResource()
{
    std::cout << "Texture destroyed " << m_filename << std::endl;
    delete m_data;
}

std::string TextureResource::filename() const
{
    return m_filename;
}

sf::Texture* TextureResource::data()
{
    return m_data;
}

bool TextureResource::isLoaded() const
{
    return m_isLoaded;
}

void TextureResource::setData(sf::Texture *data)
{
    m_data = data;
}
