# Credits: https://github.com/yse/easy_profiler/releases/tag/v2.1.0
#
# You may need to build the repo first using the CMakeList.txt
# After that change the folderstructure like that:
# easy_profiler-2.1.0
#   +- bin
#   |   +- libeasy_profiler.dll
#   +- include
#   |   +- easy
#   |       +- ...
#   +- lib
#       +- libeasy_profiler.dll.a
#
# To read the profiler output file, you need to download the GUI-application:
# https://github.com/yse/easy_profiler/releases/download/v2.1.0/easy_profiler-v2.1.0-msvc15-win64.zip
# In there, you will find the profiler_gui.exe
#

profilerPath = $$PWD/easy_profiler-2.1.0


message( "----- easy_profiler.pri ----- " )
message( "  Path: $$profilerPath " )

# To enable the Profiler you can chose between two options:
# 1) "Projects" on the left side
#    Change the Build configuration to "Profile"
#    In the section "qmake: qmake.exe ProjectName.pro":
#     Add an additional parameter: "+ "CONFIG += BUILD_WITH_EASY_PROFILER""
#     without the " at the beginning and ending.
# 2) Define it in your .pro file using: CONFIG+=BUILD_WITH_EASY_PROFILER
BUILD_WITH_EASY_PROFILER{
    message( "  Profiler is active" )
    DEFINES += BUILD_WITH_EASY_PROFILER
}else{
    message( "  Profiler is not active" )
    message( "  If you want to enable the Profiler, define it in your .pro file using: CONFIG+=BUILD_WITH_EASY_PROFILER")
}


profilerBuildPath   = "$$profilerPath/bin"

profilerIncludePath = "$$profilerPath/include"


profilerLib = $$profilerPath/lib

# Copy dll from the Profiler to the bin dir
!isEmpty(QMAKE_PRE_LINK){
QMAKE_PRE_LINK += " & "
}
message( "  Copy release dll: $$profilerBuildPath " )
profilerBuildPath ~= s,/,\\,g # Replace "/" with "\\"
CONFIG(debug, debug|release){
    QMAKE_PRE_LINK += copy "$$profilerBuildPath\libeasy_profiler.dll" "debug\libeasy_profiler.dll"
}else{
    QMAKE_PRE_LINK += copy "$$profilerBuildPath\libeasy_profiler.dll" "release\libeasy_profiler.dll"
}
message("  $$QMAKE_PRE_LINK")


DEPENDPATH  += $$profilerIncludePath
INCLUDEPATH += $$profilerIncludePath

LIBS        += -L$$profilerLib
LIBS        += -llibeasy_profiler




message( "----------------------------- " )
