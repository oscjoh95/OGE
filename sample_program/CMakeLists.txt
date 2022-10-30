add_executable(sample_program sample_program.cpp)

target_link_libraries(sample_program PUBLIC OGE)

target_include_directories(OGE PUBLIC "${OGE_SOURCE_DIR}/include/")

# The below commands should be replaced by using install command
# not after finished build
add_custom_command(
         TARGET sample_program POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory
                 ${CMAKE_SOURCE_DIR}/resources
                 ${CMAKE_CURRENT_BINARY_DIR}/resources)
                
add_custom_command(
         TARGET sample_program POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory
                 ${CMAKE_SOURCE_DIR}/shaders
                 ${CMAKE_CURRENT_BINARY_DIR}/shaders)

if(BUILD_SHARED_LIBS)
IF(WIN32)
   set(file_ending dll)
ELSE()
   set(file_ending so)
ENDIF()
set(OGE_dll_list ${ASSIMP_RUNTIME_OUTPUT_DIRECTORY}/libassimp-5.${file_ending}
                 ${GLAD_BINARY_DIR}/libglad.${file_ending}
                 ${GLFW_BINARY_DIR}/src/glfw3.${file_ending}
                 ${OGE_BINARY_DIR}/src/libOGE.${file_ending})

foreach(dll_file ${OGE_dll_list})
   add_custom_command(
           TARGET sample_program POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E copy
                   ${dll_file}
                   ${CMAKE_CURRENT_BINARY_DIR})
endforeach(dll_file)
endif()