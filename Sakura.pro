TEMPLATE = lib
CONFIG += static

CONFIG(release, release|debug){
    DEFINES -= DEBUG
    OBJECTS_DIR += release.objs
}

CONFIG(debug, debug|release){
    DEFINES += DEBUG
    OBJECTS_DIR += debug.objs
}

QMAKE_CXXFLAGS += -std=c++0x

DEFINES += GLEW_STATIC\
    INTERNAL

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
        -lsfml-network \
        -lsfml-system \
        -lphysfs \
        -L../libzelda/lib \
        -lzelda\
         -lz

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
    src/Animation.cpp \
    src/AnimatedSprite.cpp \
    src/Killable.cpp \
    src/Collideable.cpp \
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
    src/JoystickManager.cpp \
    src/memorysize.c \
    src/MusicResource.cpp \
    src/x86cpuinfo.cpp \
    src/cpuinfo.cpp \
    src/Console.cpp \
    src/ConsoleCommandBase.cpp \
    src/Config.cpp \
    src/GameState.cpp \
    src/MapFileReader.cpp \
    src/Map.cpp \
    src/Layer.cpp \
    src/Player.cpp

INCLUDEPATH +=  \
    ../libzelda/include \
    include
win32:INCLUDEPATH += \
    extlibs/physfs/include \
    extlibs/SFML/include \
    extlibs/AL/include \
    extlibs/GLEW/include

HEADERS += \
    include/Engine.hpp \
    include/Camera.hpp \
    include/EntityManager.hpp \
    include/Entity.hpp \
    include/Animation.hpp \
    include/AnimatedSprite.hpp \
    include/Killable.hpp \
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
    include/memorysize.h \
    include/MusicResource.hpp \
    include/x86cpuinfo.h \
    include/cpuinfo.h \
    include/teebuf.hpp \
    include/teestream.hpp \
    include/Console.hpp \
    include/ConsoleCommandBase.hpp \
    include/Config.hpp \
    include/Keys.hpp \
    include/RunState.hpp \
    include/GameState.hpp \
    include/MapFileReader.hpp \
    include/Map.hpp \
    include/Layer.hpp \
    include/Player.hpp


