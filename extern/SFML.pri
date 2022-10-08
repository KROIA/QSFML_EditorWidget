
# Set the base Path to the SFML Folder relative to this file
SFML_BASE = $$PWD/SFML-2.5.1


SFML_LIBS = $$SFML_BASE/lib
SFML_INCLUDE = $$SFML_BASE/include
SFML_BIN = $$SFML_BASE/bin

DEFINES += SFML_STATIC
INCLUDEPATH += "$$SFML_INCLUDE"
DEPENDPATH += "$$SFML_INCLUDE"
LIBS += -L$$SFML_LIBS

#Release Configuration
CONFIG(release, debug|release):
{
    #Audio Related Libs
    LIBS += -lsfml-audio-s          #SFML Static Module
    LIBS += -lopenal32              #Dependency
    LIBS += -lFLAC                  #Dependency
    LIBS += -lvorbisenc             #Dependency
    LIBS += -lvorbisfile            #Dependency
    LIBS += -lvorbis                #Dependency
    LIBS += -logg                   #Dependency

    #SFML-Graphics Libs
    LIBS += -lsfml-graphics-s       #SFML Static Module
    LIBS += -lfreetype              #Dependency
    #LIBS += -ljpeg                  #Dependency

    #SFML-Network Libs
    LIBS += -lsfml-network-s        #SFML Static Module
    LIBS += -lws2_32                #Dependency

    #SFML-Window Libs
    LIBS += -lsfml-window-s         #SFML Static Module
    LIBS += -lopengl32              #Dependency
    LIBS += -lgdi32                 #Dependency

    #SFML-System Libs
    LIBS += -lsfml-system-s         #SFML Static Module
    LIBS += -lwinmm                 #Dependency
}

#Debug Configuration
CONFIG(debug, debug|release):
{
    #Audio Related Libs
    LIBS += -lsfml-audio-s-d        #SFML Static Module
    LIBS += -lopenal32              #Dependency
    LIBS += -lFLAC                  #Dependency
    LIBS += -lvorbisenc             #Dependency
    LIBS += -lvorbisfile            #Dependency
    LIBS += -lvorbis                #Dependency
    LIBS += -logg                   #Dependency

    #SFML-Graphics Libs
    LIBS += -lsfml-graphics-s-d     #SFML Static Module
    LIBS += -lfreetype              #Dependency
    #LIBS += -ljpeg                  #Dependency

    #SFML-Network Libs
    LIBS += -lsfml-network-s-d      #SFML Static Module
    LIBS += -lws2_32                #Dependency

    #SFML-Window Libs
    LIBS += -lsfml-window-s-d       #SFML Static Module
    LIBS += -lopengl32              #Dependency
    LIBS += -lgdi32                 #Dependency

    #SFML-System Libs
    LIBS += -lsfml-system-s-d       #SFML Static Module
    LIBS += -lwinmm                 #Dependency
}

# Copy dll's from SFML to the bin dir
SFML_BIN ~= s,/,\\,g # Replace "/" with "\\"
CONFIG(debug, debug|release) {
#message( "  Copy debug dll\'s: $$SFML_BIN" )
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-network-d-2.dll"  "debug\sfml-network-d-2.dll" &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-system-d-2.dll"   "debug\sfml-system-d-2.dll"  &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-window-d-2.dll"   "debug\sfml-window-d-2.dll"  &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-audio-d-2.dll"    "debug\sfml-audio-d-2.dll"   &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-graphics-d-2.dll" "debug\sfml-graphics-d-2.dll" &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\openal32.dll"          "debug\openal32.dll"
}else{
#message( "  Copy release dll\'s $$SFML_BIN" )
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-network-2.dll"  "release\sfml-network-2.dll"   &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-system-2.dll"   "release\sfml-system-2.dll"    &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-window-2.dll"   "release\sfml-window-2.dll"    &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-audio-2.dll"    "release\sfml-audio-2.dll"     &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\sfml-graphics-2.dll" "release\sfml-graphics-2.dll"  &
    QMAKE_PRE_LINK += copy "$$SFML_BIN\openal32.dll"        "release\openal32.dll"
}

