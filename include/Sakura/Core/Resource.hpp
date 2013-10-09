#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

namespace Sakura
{
namespace Core
{
template <typename T>
class Resource
{
public:
    /*!
     * \brief Resource
     * \param filename
     * \param precache
     */
    Resource(const std::string& filename, bool precache = false)
      : m_filename(filename),
        m_isLoaded(false),
        m_precached(precache),
        m_data(NULL)
    {}


    /*!
     * \brief filename
     * \return
     */
    std::string filename() const;

    /*!
     * \brief setData
     * \param data
     */
    void setData(T* data);

    /*!
     * \brief data
     * \return
     */
    T* data();

    /*!
     * \brief load
     */
    void load();

    /*!
     * \brief exists
     * \return
     */
    bool exists() const;

    /*!
     * \brief isLoaded
     * \return
     */
    bool isLoaded() const;

    /*!
     * \brief isPrecached
     * \return
     */
    bool isPrecached() const;
protected:
    std::string m_filename;
    bool m_isLoaded;
    bool m_precached;
    T* m_data;
};

} // Core
} // Sakura

#endif // RESOURCE_HPP
