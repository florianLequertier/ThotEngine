cmake_minimum_required(VERSION 2.8)

project(ThotEngine)

set(EXECUTABLE_NAME ThotEngine)

add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)

find_package(OpenGL REQUIRED)

if(WIN32)
        set(SDL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third-party/include/Windows)
        set(GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third-party/include/Windows)
        set(ASSIMP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third-party/include/Windows)
	set(BULLET_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third-party/include/Windows/Bullet)
	set(SDL_LIBRARY mingw32 ${CMAKE_SOURCE_DIR}/third-party/lib/x86/libSDLmain.a ${CMAKE_SOURCE_DIR}/third-party/lib/x86/libSDL.dll.a)
	set(GLEW_LIBRARY ${CMAKE_SOURCE_DIR}/third-party/lib/x86/libglew32.dll.a)
        set(ASSIMP_LIBRARY ${CMAKE_SOURCE_DIR}/third-party/lib/x86/libassimp.dll.a)
	set(BULLET_LIBRARY 	${CMAKE_SOURCE_DIR}/third-party/lib/x86/libBulletSoftBody.a 
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libBulletDynamics.a 
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libBulletCollision.a
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libLinearMath.a 
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libBullet3Common.a
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libBullet3Collision.a
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libBullet3OpenCL_clew.a
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libBullet3Geometry.a
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libBullet3Dynamics.a
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libBullet2FileLoader.a
				${CMAKE_SOURCE_DIR}/third-party/lib/x86/libOpenGLWindow.a)


        set(DLL_BINARIES ${CMAKE_SOURCE_DIR}/third-party/lib/x86/glew32.dll ${CMAKE_SOURCE_DIR}/third-party/lib/x86/SDL.dll ${CMAKE_SOURCE_DIR}/third-party/lib/x86/libassimp.dll)

else(WIN32)
	find_package(SDL REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(ASSIMP REQUIRED)
	find_package(BULLET REQUIRED)
endif(WIN32)

if(UPEMLV)
    # Flag a activer pour compilation à la FAC
    set(OPENGL_LIBRARIES /usr/lib/x86_64-linux-gnu/libGL.so.1)
endif(UPEMLV)

include_directories(${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${BULLET_INCLUDE_DIR} thotEngine/include third-party/include) #glimac/include

set(ALL_LIBRARIES thotEngine ${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${ASSIMP_LIBRARY} ${BULLET_LIBRARY}) #glimac

# Add the library
#add_subdirectory(glimac)
add_subdirectory(thotEngine)

# A macro to add a directory containing shaders. Each file ".glsl" is considered to be a shader and a target
# is added to the Makefile in order to copy the shader in the build directory when it is modified
macro(ADD_SHADER_DIRECTORY directory)
    file(GLOB_RECURSE relative_files RELATIVE ${directory} ${directory}/*.glsl)
    file(GLOB_RECURSE files ${directory}/*.glsl)

    if(files)
        list(LENGTH files file_count)
        math(EXPR range_end "${file_count} - 1")

        foreach(idx RANGE ${range_end})
            list(GET files ${idx} file)
            list(GET relative_files ${idx} relative_file)

            add_custom_command(OUTPUT ${EXECUTABLE_OUTPUT_PATH}/shaders/${relative_file}
                             COMMAND ${CMAKE_COMMAND} -E copy ${file} ${EXECUTABLE_OUTPUT_PATH}/shaders/${relative_file}
                             MAIN_DEPENDENCY ${file})
        endforeach()
    endif()
endmacro()

macro(ADD_ASSET_DIRECTORY directory)
    file(GLOB_RECURSE relative_files RELATIVE ${directory} ${directory}/*)
    file(GLOB_RECURSE files ${directory}/*)

    if(files)
        list(LENGTH files file_count)
        math(EXPR range_end "${file_count} - 1")

        foreach(idx RANGE ${range_end})
            list(GET files ${idx} file)
            list(GET relative_files ${idx} relative_file)

            add_custom_command(OUTPUT ${EXECUTABLE_OUTPUT_PATH}/assets/${relative_file}
                             COMMAND ${CMAKE_COMMAND} -E copy ${file} ${EXECUTABLE_OUTPUT_PATH}/assets/${relative_file}
                             MAIN_DEPENDENCY ${file})
        endforeach()
    endif()
endmacro()

file(GLOB HEADER_FILES src/*.hpp) # get all headers
file(GLOB SRC_FILES src/*.cpp) # get all sources files
file(GLOB_RECURSE SHADER_FILES shaders/*.glsl) # get all shaders
file(GLOB_RECURSE ASSET_FILES assets/*) # get all the assets

add_executable(${EXECUTABLE_NAME} ${SRC_FILES} ${HEADER_FILES} ${SHADER_FILES} ${ASSET_FILES}) # add the executable
target_link_libraries(${EXECUTABLE_NAME} ${ALL_LIBRARIES}) # and link  to the libraries

if(WIN32)
    # on windows, copy dll files to the executable directory
    file(COPY ${DLL_BINARIES} DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endif(WIN32)

ADD_SHADER_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/shaders)
ADD_ASSET_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/assets)
