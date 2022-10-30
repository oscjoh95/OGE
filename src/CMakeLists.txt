set(OGE_SOURCE_FILES camera.cpp
                     mesh.cpp
                     model.cpp
                     scene.cpp
                     shader.cpp
                     stb_image.cpp)

set(OGE_HEADER_FILES "${OGE_SOURCE_DIR}/include/camera.h"
                     "${OGE_SOURCE_DIR}/include/mesh.h"
                     "${OGE_SOURCE_DIR}/include/model.h"
                     "${OGE_SOURCE_DIR}/include/scene.h"
                     "${OGE_SOURCE_DIR}/include/shader.h"
                     "${OGE_SOURCE_DIR}/include/stb_image.h")

add_library(OGE ${OGE_LIBRARY_TYPE}
                 ${OGE_HEADER_FILES}
                 ${OGE_SOURCE_FILES})

target_include_directories(OGE PUBLIC "${OGE_SOURCE_DIR}/include/")

target_link_libraries(OGE PUBLIC glfw glad assimp)