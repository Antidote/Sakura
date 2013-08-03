TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x \
                -pthread

LIBS += -lsfml-graphics\
        -lsfml-window \
        -lsfml-audio \
        -lsfml-system \
        -lpthread

SOURCES += main.cpp \
    src/Engine.cpp \
    src/Camera.cpp \
    src/EntityManager.cpp \
    src/Entity.cpp \
    src/Link.cpp \
    src/Animation.cpp \
    src/AnimatedSprite.cpp \
    src/Killable.cpp \
    src/Octorok.cpp \
    src/Hud.cpp \
    src/Collideable.cpp \
    src/HeartPickup.cpp \
    src/SoundResource.cpp \
    src/ResourceManager.cpp \
    src/TextureResource.cpp \
    src/Widget.cpp \
    src/InputManager.cpp \
    src/Label.cpp \
    src/KeyboardManager.cpp \
    src/MouseManager.cpp

INCLUDEPATH += include
HEADERS += \
    include/Engine.hpp \
    include/Camera.hpp \
    include/EntityManager.hpp \
    include/Entity.hpp \
    include/Link.hpp \
    include/Animation.hpp \
    include/AnimatedSprite.hpp \
    include/Killable.hpp \
    include/Octorok.hpp \
    include/Hud.hpp \
    include/HeartPickup.hpp \
    include/Collideable.hpp \
    include/SoundResource.hpp \
    include/Resource.hpp \
    include/ResourceManager.hpp \
    include/TextureResource.hpp \
    include/AI.hpp \
    include/Widget.hpp \
    include/UIManager.hpp \
    include/InputManager.hpp \
    include/Signal.h \
    include/Delegate.h \
    include/Label.hpp \
    include/KeyboardManager.hpp \
    include/MouseManager.hpp \
    include/InputBase.hpp

