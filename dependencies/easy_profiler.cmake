## description: simple profiler for applications
include(FetchContent)

# Define the git repository and tag to download from
set(LIB_NAME easy_profiler)
set(GIT_REPO https://github.com/yse/easy_profiler.git)
set(GIT_TAG develop)

FetchContent_Declare(
    ${LIB_NAME}
    GIT_REPOSITORY ${GIT_REPO}
    GIT_TAG        ${GIT_TAG}
)

set(EASY_PROFILER_NO_SAMPLES True)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build easy_profiler as static library.")
message("Downloading dependency: ${LIB_NAME} from: ${GIT_REPO} tag: ${GIT_TAG}")
FetchContent_MakeAvailable(${LIB_NAME})
set(EASY_PROFILER_IS_AVAILABLE ON)


# Add this library to the specific profiles of this project
list(APPEND DEPENDENCIES_FOR_SHARED_LIB )
list(APPEND DEPENDENCIES_FOR_STATIC_LIB )
list(APPEND DEPENDENCIES_FOR_STATIC_PROFILE_LIB ${LIB_NAME}) # only use for static profiling profile

# Deploy the Profiler GUI
if(QT_ENABLE AND QT_DEPLOY)
    DEPLOY_QT(profiler_gui ${INSTALL_BIN_PATH})
endif()


#set(EASY_PROFILER_NO_GUI False)
set_target_properties(${LIB_NAME} PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
set_target_properties(${LIB_NAME} PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
set_target_properties(${LIB_NAME} PROPERTIES CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX ${DEBUG_POSTFIX_STR})
target_compile_definitions(${LIB_NAME} PUBLIC EASY_PROFILER_STATIC)
