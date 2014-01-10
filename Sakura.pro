TEMPLATE = lib
CONFIG += static
DESTDIR = lib

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
        -lz \
        -ltinyxml2

unix:LIBS += \
    -lGL \
    -lGLEW \
    -lGLU \
    -lX11 \
    -lopenal \

CONFIG(release, release|debug){
    DEFINES -= DEBUG
    OBJECTS_DIR += release.objs
    TARGET = sakura
    LIBS += -lzelda
}

CONFIG(debug, debug|release){
    DEFINES += DEBUG
    OBJECTS_DIR += debug.objs
    TARGET = sakura-d
    LIBS += -lzelda-d
}

SOURCES += \ 
    src/x86cpuinfo.cpp \
    src/memorysize.c \
    src/cpuinfo.cpp \
    src/Sakura/Core/UIManager.cpp \
    src/Sakura/Core/Tile.cpp \
    src/Sakura/Core/ResourceManager.cpp \
    src/Sakura/Core/Player.cpp \
    src/Sakura/Core/PHYSFSStream.cpp \
    src/Sakura/Core/MapFileReader.cpp \
    src/Sakura/Core/Map.cpp \
    src/Sakura/Core/Layer.cpp \
    src/Sakura/Core/Killable.cpp \
    src/Sakura/Core/InputManager.cpp \
    src/Sakura/Core/GameState.cpp \
    src/Sakura/Core/EntityManager.cpp \
    src/Sakura/Core/Entity.cpp \
    src/Sakura/Core/Engine.cpp \
    src/Sakura/Core/ConsoleCommandBase.cpp \
    src/Sakura/Core/Console.cpp \
    src/Sakura/Core/Config.cpp \
    src/Sakura/Core/Camera.cpp \
    src/Sakura/Core/CVar.cpp \
    src/Sakura/Core/CVarManager.cpp \
    src/Sakura/Gui/Widget.cpp \
    src/Sakura/Gui/Label.cpp \
    src/Sakura/Gui/Container.cpp \
    src/Sakura/Input/MouseManager.cpp \
    src/Sakura/Input/KeyboardManager.cpp \
    src/Sakura/Input/JoystickManager.cpp \
    src/Sakura/Resources/TextureResource.cpp \
    src/Sakura/Resources/SoundResource.cpp \
    src/Sakura/Resources/FontResource.cpp \
    src/Sakura/Resources/SongResource.cpp \
    src/Sakura/Core/SSprite.cpp \
    src/Sakura/Core/SSpriteFile.cpp \
    src/Sakura/Core/SSpriteFileReader.cpp \
    src/Sakura/Core/SSpriteFileWriter.cpp \
    src/Sakura/Core/SSpriteFrame.cpp \
    src/Sakura/Core/SSpritePart.cpp \
    src/Sakura/Resources/SpriteResource.cpp

INCLUDEPATH +=  \
    ../libzelda/include \
    include
win32:INCLUDEPATH += \
    extlibs/physfs/include \
    extlibs/SFML/include \
    extlibs/AL/include \
    extlibs/GLEW/include

HEADERS += \
    include/Mainpage.hpp \
    include/teestream.hpp \
    include/teebuf.hpp \
    include/x86cpuinfo.h \
    include/memorysize.h \
    include/cpuinfo.h \
    include/Sakura/Core/UIManager.hpp \
    include/Sakura/Core/Tile.hpp \
    include/Sakura/Core/RunState.hpp \
    include/Sakura/Core/ResourceManager.hpp \
    include/Sakura/Core/Player.hpp \
    include/Sakura/Core/PHYSFSStream.hpp \
    include/Sakura/Core/Map.hpp \
    include/Sakura/Core/Layer.hpp \
    include/Sakura/Core/Killable.hpp \
    include/Sakura/Core/Keys.hpp \
    include/Sakura/Core/InputManager.hpp \
    include/Sakura/Core/GameState.hpp \
    include/Sakura/Core/EntityManager.hpp \
    include/Sakura/Core/Entity.hpp \
    include/Sakura/Core/Engine.hpp \
    include/Sakura/Core/ConsoleCommandBase.hpp \
    include/Sakura/Core/Console.hpp \
    include/Sakura/Core/Config.hpp \
    include/Sakura/Core/Collideable.hpp \
    include/Sakura/Core/Camera.hpp \
    include/Sakura/Core/AI.hpp \
    include/Sakura/Core/MapFileReader.hpp \
    include/Sakura/Core/CVar.hpp \
    include/Sakura/Core/CVarManager.hpp \
    include/Sakura/Gui/Widget.hpp \
    include/Sakura/Gui/Label.hpp \
    include/Sakura/Gui/Container.hpp \
    include/Sakura/Input/MouseManager.hpp \
    include/Sakura/Input/KeyboardManager.hpp \
    include/Sakura/Input/JoystickManager.hpp \
    include/Sakura/Input/InputBase.hpp \
    include/Sakura/Resources/TextureResource.hpp \
    include/Sakura/Resources/SoundResource.hpp \
    include/Sakura/Resources/FontResource.hpp \
    include/Sakura/Core/ResourceBase.hpp \
    include/Sakura/Resources/SongResource.hpp \
    include/Sakura/Core/SSprite.hpp \
    include/Sakura/Core/SSpriteFile.hpp \
    include/Sakura/Core/SSpriteFileReader.hpp \
    include/Sakura/Core/SSpriteFileWriter.hpp \
    include/Sakura/Core/SSpriteFrame.hpp \
    include/Sakura/Core/SSpritePart.hpp \
    include/Sakura/Resources/SpriteResource.hpp

