QT += core gui
CONFIG += c++17
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#CONFIG += USE_STATIC_SFML_EDITOR_LIB

inc = $$PWD/core/inc

QMAKE_CFLAGS_RELEASE += -O3  -mtune=intel
QMAKE_CXXFLAGS_RELEASE += -O3  -mtune=intel

################
#   Für shared libs ohne -s
#   Für static libs mit -s -> muss getestet werden
################
QSFM_EditorWidget_base = $$PWD

QSFM_EditorWidget_libsPath = $$QSFM_EditorWidget_base/bin
QSFM_EditorWidget_inc = $$QSFM_EditorWidget_base/core/inc
QSFM_EditorWidget_dependenciesPath = $$QSFM_EditorWidget_base/Dependencies

# Include EasyProfiler headers
INCLUDEPATH += \
    $$QSFM_EditorWidget_inc \
    $$QSFM_EditorWidget_dependenciesPath/sfml-src/include \
    $$QSFM_EditorWidget_dependenciesPath/easy_profiler-src\easy_profiler_core\include

LIBS += \
    -L$$QSFM_EditorWidget_dependenciesPath/easy_profiler-build/bin \
    -L$$QSFM_EditorWidget_dependenciesPath/sfml-src/extlibs/libs-msvc/x64 \
    -L$$QSFM_EditorWidget_libsPath


QSFM_EDITORWIDGET_LIBS = SFML_EditorWidget
EASY_LIBS = easy_profiler

SFML_LIBS = sfml-audio \
            sfml-graphics \
            sfml-network \
            sfml-window \
            sfml-system

SFML_DEPENDENCIES = \
            openal32 \
            FLAC \
            vorbisenc \
            vorbisfile \
            vorbis \
            ogg \
            freetype \
            ws2_32 \
            opengl32 \
            gdi32 \
            winmm

USE_STATIC_SFML_EDITOR_LIB{
    message("Use static SFML_EditorWidget LIB")
    for (lib, SFML_LIBS): SFML_LIBS_2 += $${lib}-s
    SFML_LIBS = $${SFML_LIBS_2}
    SFML_LIBS_2 = ""

    QSFM_EDITORWIDGET_LIBS = $${QSFM_EDITORWIDGET_LIBS}-s

    DEFINES += BUILD_STATIC \
               SFML_STATIC
}
else{
    DEFINES += SFML_DYNAMIC
}
CONFIG(debug, debug|release){
    message("Use debug SFML_EditorWidget LIB")
    for (lib, SFML_LIBS): SFML_LIBS_2 += $${lib}-d
    SFML_LIBS = $${SFML_LIBS_2}
    SFML_LIBS_2 = ""

    QSFM_EDITORWIDGET_LIBS = $${QSFM_EDITORWIDGET_LIBS}-d
}

EXTERNAL_LIBS += \
    $${SFML_LIBS} \
    $${SFML_DEPENDENCIES} \
    $${EASY_LIBS} \
    $${QSFM_EDITORWIDGET_LIBS}

for (lib, EXTERNAL_LIBS): EXTERNAL_LIBS_STR += -l$${lib}

for (lib, EXTERNAL_LIBS_STR): message("LIB: "$${lib})

LIBS += $${EXTERNAL_LIBS_STR}



