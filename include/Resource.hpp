#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

template <typename T>
class Resource
{
public:
    std::string filename() const;
    void setData(T* data);
    T* data();

    bool isLoaded() const;
protected:
    std::string m_filename;
    T* m_data;
    bool m_isLoaded;
};

#endif // RESOURCE_HPP
