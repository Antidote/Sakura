#ifndef SSpRITEPART_HPP
#define SSpRITEPART_HPP

#include <Types.hpp>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <Types.hpp>

namespace Sakura
{
namespace Core
{
class SSprite;
class SSpriteFrame;

class SSpritePart
{
public:
    SSpritePart(SSprite* root);
    SSpritePart(SSprite* root, const std::string& name, bool hasCollision = false);
    virtual ~SSpritePart();

    void setName(const std::string& name);
    std::string name() const;

    void setCollision(bool col);
    bool hasCollision() const;

    void addFrame(SSpriteFrame* frame);
    SSpriteFrame* frame(int id);
    void setFrames(std::vector<SSpriteFrame*> frames);
    std::vector<SSpriteFrame*> frames() const;
    Uint32 frameCount() const;


    void setRoot(SSprite* root);
    virtual void update(const sf::Time& dt);
    virtual void draw(sf::RenderTarget& rt);
private:
    void updateTexture();
    SSprite*  m_root;
    std::string   m_name;
    bool          m_hasCollision;
    SSpriteFrame* m_currentFrame;
    Uint32        m_frameIndex;
    sf::Sprite    m_partSprite;
    sf::Time      m_currentTime;

    // The collection of frames for this part
    std::vector<SSpriteFrame*> m_frames;
};
}
}
#endif // SSpRITEPART_HPP
