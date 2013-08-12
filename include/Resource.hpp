#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

template <typename T>
class Resource
{
public:
    Resource(const std::string& filename, bool precache = false)
      : m_filename(filename),
        m_isLoaded(false),
        m_precached(precache),
        m_data(NULL)
    {}


    std::string filename() const;
    void setData(T* data);
    T* data();

    void load();
    bool exists() const;
    bool isLoaded() const;
    bool isPrecached() const;
protected:
    std::string m_filename;
    bool m_isLoaded;
    bool m_precached;
    T* m_data;
};

#endif // RESOURCE_HPP
