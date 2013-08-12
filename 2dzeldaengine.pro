TEMPLATE = app
CONFIG += console


QMAKE_CXXFLAGS += -std=c++0x

DEFINES += GLEW_STATIC

win32:LIBS += \
    -Lextlibs/GLEW/lib \
    -Lextlibs/SFML/lib \
    -Lextlibs/AL/lib \
    -Lextlibs/physfs/lib \
    -Lextlibs/AL/lib \
    -lopenal32 \
    -lopengl32


LIBS += -lsfml-graphics \
        -lsfml-window \
        -lsfml-audio \
        -lsfml-system \
        -lphysfs \



unix:LIBS += \
    -lGL \
    -lGLEW \
    -lGLU \
    -lX11 \
    -lopenal


SOURCES += \
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
    src/MouseManager.cpp \
    src/Container.cpp \
    src/UIManager.cpp \
    src/FontResource.cpp \
    src/Tile.cpp \
    main.cpp \
    src/JoystickManager.cpp \
    src/Log.cpp \
    src/memorysize.c \
    src/MusicResource.cpp \
    src/x86cpuinfo.cpp \
    src/cpuinfo.cpp

INCLUDEPATH += include
win32:INCLUDEPATH += \
    extlibs/physfs/include \
    extlibs/SFML/include \
    extlibs/AL/include

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
    include/InputManager.hpp \
    include/Signal.h \
    include/Delegate.h \
    include/Label.hpp \
    include/KeyboardManager.hpp \
    include/MouseManager.hpp \
    include/InputBase.hpp \
    include/Container.hpp \
    include/UIManager.hpp \
    include/FontResource.hpp \
    include/Tile.hpp \
    include/Rupee.hpp \
    include/JoystickManager.hpp \
    include/Log.hpp \
    include/memorysize.h \
    include/MusicResource.hpp \
    include/x86cpuinfo.h \
    include/cpuinfo.h \
    include/SpriteFont.h \
    include/teebuf.hpp \
    include/teestream.hpp


