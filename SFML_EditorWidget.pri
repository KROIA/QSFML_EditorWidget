QT += core gui
CONFIG += c++17
#CONFIG -= app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += USE_STATIC_SFML_EDITOR_LIB

inc = $$PWD/core/inc
#src = $$PWD/core/src

#CONFIG+=BUILD_WITH_EASY_PROFILER

#include(extern/SFML.pri)
#include(extern/easy_profiler.pri)

QMAKE_CFLAGS_RELEASE += -O3  -mtune=intel
QMAKE_CXXFLAGS_RELEASE += -O3  -mtune=intel

#DEFINES += BUILD_STATIC
dependenciesPath = $$PWD/Dependencies
#

# SFML library path
#    SFML_LIBS_PATH = $$PWD/bin
#
#    # Include SFML headers
#    INCLUDEPATH += $$dependenciesPath/sfml-src/include
#
#    # Debug build
#    CONFIG(debug, debug|release) {
#        # SFML debug library names
#        SFML_SYSTEM_LIBS += -lsfml-graphics-s-d
#        SFML_SYSTEM_LIBS += -lsfml-window-s-d
#        SFML_SYSTEM_LIBS += -lsfml-system-s-d
#        SFML_SYSTEM_LIBS += -lsfml-audio-s-d
#        SFML_SYSTEM_LIBS += -lsfml-network-s-d
#    } else {
#        # Release build
#        # SFML release library names
#        SFML_SYSTEM_LIBS += -lsfml-graphics-s
#        SFML_SYSTEM_LIBS += -lsfml-window-s
#        SFML_SYSTEM_LIBS += -lsfml-system-s
#        SFML_SYSTEM_LIBS += -lsfml-audio-s
#        SFML_SYSTEM_LIBS += -lsfml-network-s
#    }
#
#    # Link SFML libraries
#    #LIBS += -L$$SFML_LIBS_PATH/lib \
#    #        -Wl,-rpath,$$SFML_LIBS_PATH/lib \
#    #        $$join(SFML_SYSTEM_LIBS, " -l", "-l", "")
#    LIBS += -L"$$SFML_LIBS_PATH" \
#            $$SFML_SYSTEM_LIBS
#    #LIBS += $$SFML_SYSTEM_LIBS
#    # END SFML


# SFML Library Path
#     SFML_LIBS_PATH = $$PWD/bin
#     SFML_EXT_LIBS_PATH = $$dependenciesPath/sfml-src/extlibs
#
#     # SFML System Libraries
#     SFML_SYSTEM_LIBS = opengl32 \
#                        freetype \
#                        jpeg \
#                        winmm \
#                        gdi32 \
#                        openal32 \
#                        flac \
#                        vorbisenc \
#                        vorbisfile \
#                        vorbis \
#                        ogg
#
#     # SFML Libraries
#     SFML_LIBS_DEBUG += -lsfml-graphics-s-d \
#                        -lsfml-window-s-d \
#                        -lsfml-system-s-d \
#                        -lsfml-audio-s-d \
#                        -lsfml-network-s-d
#
#     SFML_LIBS_RELEASE += -lsfml-graphics-s \
#                          -lsfml-window-s \
#                          -lsfml-system-s \
#                          -lsfml-audio-s \
#                          -lsfml-network-s
#
#     # SFML External Dependencies
#     SFML_DEPENDENCIES += freetype \
#                          jpeg \
#                          openal-soft \
#                          libsndfile \
#                          vorbis \
#                          vorbisenc \
#                          vorbisfile
#
#     # Include directories
#     INCLUDEPATH += $$dependenciesPath/sfml-src/include
#     INCLUDEPATH += $$dependenciesPath/sfml-src/include/freetype2
#
#
#     # Link external dependencies
#     win32: LIBS += -L$$SFML_EXT_LIBS_PATH/libs-msvc/x64 \
#                   -lfreetype \
#                   #-ljpeg \
#                   -lopenal32 \
#                   #-lsndfile \
#                   -lvorbisenc \
#                   -lvorbisfile \
#                   -lvorbis \
#                   -lws2_32
#
#     unix: LIBS += -L$$SFML_EXT_LIBS_PATH/libs-macos/lib \
#                  -lfreetype \
#                 # -ljpeg \
#                  -lopenal \
#                 # -lsndfile \
#                  -lvorbisenc \
#                  -lvorbisfile \
#                  -lvorbis \
#                  -framework CoreFoundation \
#                  -framework CoreAudio \
#                  -framework AudioToolbox \
#                  -framework Carbon
#
#     # Platform-specific configuration
#     win32: CONFIG(release, debug|release) {
#         SFML_LIBS = $$SFML_LIBS_RELEASE
#     } else: CONFIG(debug, debug|release) {
#         SFML_LIBS = $$SFML_LIBS_DEBUG
#
#     } else {
#         error("Unknown build configuration!")
#     }
#     # Link libraries
#     LIBS += -L$$SFML_LIBS_PATH/lib \
#             $$SFML_LIBS
#     message($$SFML_LIBS)




  SFML_LIBS = $$PWD/bin
  message($$SFML_LIBS)
  SFML_INCLUDE = $$dependenciesPath/sfml-src/include \
                 $$dependenciesPath/sfml-src/extlibs/headers


  #DEFINES += SFML_STATIC
  INCLUDEPATH += $$SFML_INCLUDE
  DEPENDPATH += $$SFML_INCLUDE
  LIBS += -L$$SFML_LIBS
  LIBS += -L$$dependenciesPath/sfml-src/extlibs/libs-msvc/x64

################
#   Für shared libs ohne -s
#   Für static libs mit -s -> muss getestet werden
################
  #Release Configuration
CONFIG(release, debug|release){
    message("Release libs")
    #Audio Related Libs
    LIBS += -lsfml-audio          #SFML Static Module
    LIBS += -lopenal32              #Dependency
    LIBS += -lFLAC                  #Dependency
    LIBS += -lvorbisenc             #Dependency
    LIBS += -lvorbisfile            #Dependency
    LIBS += -lvorbis                #Dependency
    LIBS += -logg                   #Dependency

    #SFML-Graphics Libs
    LIBS += -lsfml-graphics       #SFML Static Module
    LIBS += -lfreetype              #Dependency
    #LIBS += -ljpeg                  #Dependency

    #SFML-Network Libs
    LIBS += -lsfml-network        #SFML Static Module
    LIBS += -lws2_32                #Dependency

    #SFML-Window Libs
    LIBS += -lsfml-window         #SFML Static Module
    LIBS += -lopengl32              #Dependency
    LIBS += -lgdi32                 #Dependency

    #SFML-System Libs
    LIBS += -lsfml-system         #SFML Static Module
    LIBS += -lwinmm                 #Dependency
}


  #Debug Configuration
  CONFIG(debug, debug|release){
      CONFIG -= release
      message("Debug libs")
      #Audio Related Libs
      LIBS += -lsfml-audio-d        #SFML Static Module
      LIBS += -lopenal32              #Dependency
      LIBS += -lFLAC                  #Dependency
      LIBS += -lvorbisenc             #Dependency
      LIBS += -lvorbisfile            #Dependency
      LIBS += -lvorbis                #Dependency
      LIBS += -logg                   #Dependency

      #SFML-Graphics Libs
      LIBS += -lsfml-graphics-d     #SFML Static Module
      LIBS += -lfreetype              #Dependency
      #LIBS += -ljpeg                  #Dependency

      #SFML-Network Libs
      LIBS += -lsfml-network-d      #SFML Static Module
      LIBS += -lws2_32                #Dependency

      #SFML-Window Libs
      LIBS += -lsfml-window-d       #SFML Static Module
      LIBS += -lopengl32              #Dependency
      LIBS += -lgdi32                 #Dependency

      #SFML-System Libs
      LIBS += -lsfml-system-d       #SFML Static Module
      LIBS += -lwinmm                 #Dependency
  }






# EasyProfiler library path
EASY_LIBS_PATH = $$dependenciesPath/easy_profiler-build/bin

# Include EasyProfiler headers
INCLUDEPATH += $$dependenciesPath/easy_profiler-src/easy_profiler_core/include

# Debug build
CONFIG(debug, debug|release) {
    # SFML debug library names
    EASY_LIBS += -leasy_profiler
} else {
    # Release build
    # SFML release library names
    EASY_LIBS += -leasy_profiler
}

# Link EasyProfiler libraries
#LIBS += -L$$EASY_LIBS/lib \
#        -Wl,-rpath,$$EASY_LIBS/lib \
#        $$join(EASY_LIBS, " -l", "-l", "")
LIBS += -L$$EASY_LIBS_PATH \
        $$EASY_LIBS
#LIBS += $$EASY_LIBS
# END EasyProfiler

DEFINES += SFML_DYNAMIC
QSFM_EditorWidget_base = $$PWD

QSFM_EditorWidget_libsPath = $$QSFM_EditorWidget_base/bin
QSFM_EditorWidget_inc = $$QSFM_EditorWidget_base/core/inc
QSFM_EditorWidget_dependenciesPath = $$QSFM_EditorWidget_base/Dependencies
CONFIG(debug, debug|release) {
    # SFML debug library names
    QSFM_EditorWidget_libs += -lSFML_EditorWidget-d
} else {
    # Release build
    # SFML release library names
    QSFM_EditorWidget_libs += -lSFML_EditorWidget
}
LIBS += -L$$QSFM_EditorWidget_libsPath
LIBS += $$QSFM_EditorWidget_libs
DEPENDPATH += $$QSFM_EditorWidget_libsPath \
              $$QSFM_EditorWidget_inc

INCLUDEPATH += \
    $$QSFM_EditorWidget_inc \
    $$QSFM_EditorWidget_dependenciesPath/sfml-src/include \
    $$QSFM_EditorWidget_dependenciesPath/easy_profiler-src\easy_profiler_core\include



#HEADERS += \
#    $$inc/utilities/Updatable.h \
#    $$inc/canvas/CanvasObjectGroup.h \
#    $$inc/canvas/CanvasThreadWorker.h \
#    $$inc/canvas/RenderLayer.h \
#    $$inc/QSFML_debugSettings.h \
#    $$inc/QSFML_EditorWidget.h \
#    $$inc/components/AbstractPressEvent.h \
#    $$inc/components/Button.h \
#    $$inc/components/KeyPressEvent.h \
#    $$inc/components/Line.h \
#    $$inc/components/MouseFollower.h \
#    $$inc/components/MousePressEvent.h \
#    $$inc/components/Text.h \
#    $$inc/objects/DefaultEditor.h \
#    $$inc/objects/DrawableBoxCollider.h \
#    $$inc/objects/LineChart.h \
#    $$inc/utilities/VectorOperations.h \
#    $$inc/canvas/Canvas.h \
#    $$inc/canvas/CanvasSettings.h \
#    $$inc/canvas/CanvasObjectContainer.h \
#    $$inc/canvas/CanvasForwardDeclaration.h \
#    $$inc/components/DrawableVector.h \
#    $$inc/components/Component.h \
#    $$inc/components/Drawable.h \
#    $$inc/components/SfEventHandle.h \
#    $$inc/objects/CanvasObject.h \
#    $$inc/objects/BackgroundGrid.h \
#    $$inc/objects/CameraController.h \
#    $$inc/objects/VisibleCamera.h \
#    $$inc/utilities/AABB.h \
#    $$inc/components/physics/Collider.h




#SOURCES += \
#    $$src/canvas/CanvasObjectGroup.cpp \
#    $$src/canvas/CanvasThreadWorker.cpp \
#    $$src/components/AbstractPressEvent.cpp \
#    $$src/components/Button.cpp \
#    $$src/components/KeyPressEvent.cpp \
#    $$src/components/Line.cpp \
#    $$src/components/MouseFollower.cpp \
#    $$src/components/MousePressEvent.cpp \
#    $$src/components/Text.cpp \
#    $$src/objects/DefaultEditor.cpp \
#    $$src/objects/DrawableBoxCollider.cpp \
#    $$src/objects/LineChart.cpp \
#    $$src/utilities/VectorOperations.cpp \
#    $$src/canvas/Canvas.cpp \
#    $$src/canvas/CanvasObjectContainer.cpp \
#    $$src/components/DrawableVector.cpp \
#    $$src/components/SfEventHandle.cpp \
#    $$src/components/Component.cpp \
#    $$src/components/Drawable.cpp \
#    $$src/objects/CanvasObject.cpp \
#    $$src/objects/BackgroundGrid.cpp \
#    $$src/objects/CameraController.cpp \
#    $$src/utilities/AABB.cpp \
#    $$src/components/physics/Collider.cpp



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


