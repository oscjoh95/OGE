# git clone git@github.com:oscjoh95/OGE.git
# cd OGE
# mkdir build && cd build
# cmake .. -DCMAKE_TOOLCHAIN_FILE=~/Toolchain-Ubuntu-mingw-w64.cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
# cmake --build . 
CMAKE_MINIMUM_REQUIRED( VERSION 3.14 )


project(OGE VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")

# Apparently good for generated folder structure
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(OGE_BUILD_SAMPLES
   "If the sample program should be built."
   OFF
)


set(OGE_LIBRARY_TYPE "${OGE_LIBRARY_TYPE}" CACHE STRING
    "Library type override for OGE (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")

if (OGE_LIBRARY_TYPE)
    if (OGE_LIBRARY_TYPE STREQUAL "SHARED")
        set(OGE_BUILD_SHARED_LIBRARY ON)
    else()
        set(OGE_BUILD_SHARED_LIBRARY OFF)
    endif()
else()
    set(OGE_BUILD_SHARED_LIBRARY ${BUILD_SHARED_LIBS})
endif()

# simplifies downloading dependencies
INCLUDE(FetchContent)
#SET(FETCHCONTENT_QUIET OFF) # no need to print all information

### assimp
message(STATUS "\r\n-----------assimp-----------\r\n")
# set assimp build options that are non-default see list,
# see https://github.com/assimp/assimp/blob/master/Build.md#cmake-build-options
# and
# https://github.com/assimp/assimp/blob/master/CMakeLists.txt

set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_COVERALLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

set(FETCHCONTENT_SOURCE_DIR_ASSIMP "${CMAKE_CURRENT_SOURCE_DIR}/assimp")

FetchContent_Declare(
   assimp
   GIT_REPOSITORY https://github.com/assimp/assimp
)

FetchContent_GetProperties(assimp)

if(NOT assimp_POPULATED)
   FetchContent_Populate(assimp)
   add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
   message(STATUS "Assimp source dir: ${assimp_SOURCE_DIR}")
   message(STATUS "Assimp binary dir: ${assimp_BINARY_DIR}")
endif()
### end Assimp

### glad
message(STATUS "\r\n------------GLAD------------\r\n")

FETCHCONTENT_SOURCE_DIR_GLAD="${CMAKE_CURRENT_SOURCE_DIR}/glad"

FetchContent_Declare(
   glad
   GIT_REPOSITORY https://github.com/Dav1dde/glad
   # Branch master (i.e. v1.0), latest commit hash
   GIT_TAG ea756f7cc5e11dcef3cafdab87d45b3b528c875d
)

FetchContent_GetProperties(glad)

if(NOT glad_POPULATED)
   FetchContent_Populate(glad)
   add_subdirectory("${glad_SOURCE_DIR}" "${glad_BINARY_DIR}")
   message(STATUS "glad source dir: ${glad_SOURCE_DIR}")
   message(STATUS "glad binary dir: ${glad_BINARY_DIR}")
endif()

### end glad

### GLFW
message(STATUS "\r\n------------GLFW------------\r\n")
# set GLFW build options that are non-default see
# https://www.glfw.org/docs/latest/build_guide.html#build_link_cmake_source
# and
# https://github.com/glfw/glfw/blob/master/CMakeLists.txt
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FETCHCONTENT_SOURCE_DIR_GLFW="${CMAKE_CURRENT_SOURCE_DIR}/glfw"

FetchContent_Declare(
   glfw
   GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_GetProperties(glfw)

if(NOT glfw_POPULATED)
   FetchContent_Populate(glfw)
   add_subdirectory("${glfw_SOURCE_DIR}" "${glfw_BINARY_DIR}")
   message(STATUS "GLFW source dir: ${glfw_SOURCE_DIR}")
   message(STATUS "GLFW binary dir: ${glfw_BINARY_DIR}")
endif()

### end GLFW

### OGE
message(STATUS "\r\n-------------OGE------------\r\n")
add_subdirectory(src)

if(OGE_BUILD_SAMPLES)
    add_subdirectory(sample_program)
endif()

### end OGE