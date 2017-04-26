# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 3.1.0 FATAL_ERROR)


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB CUDASOURCE "kernels/*.cpp")
FILE(GLOB HEADERS "*.h")

SET(CXX_STANDARD 11)
SET(CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


SET(EXECUTABLE_OUTPUT_PATH "${ANDRO_SDK}/../bin")

set(LINK_DIRS
    ${ANDRO_SDK}/GLFW/lib/${PLATFORM}
    ${ANDRO_SDK}/GLEW/lib/${PLATFORM}
    )

link_directories(${LINK_DIRS})
include_directories(${ANDRO_SDK}/GLEW/include  ${ANDRO_SDK}/GLFW/include)
include_directories(${ANDRO_SDK}/../)
include_directories(${ANDRO_SDK}/../Demos)



if(PLATFORM STREQUAL "Win32")
    set(LINK_FILES
        glfw3dll
        glew32
		opengl32
		GlU32.lib
        )
else(PLATFORM STREQUAL "Win32")
    set(LINK_FILES
        glfw
        GLEW
        GL
        )
endif(PLATFORM STREQUAL "Win32")

add_executable (RayTracing WIN32 ${HEADERS} ${CSOURCE}  ${CPPSOURCE}) 

target_link_libraries(RayTracing  AndroUtils  Lua TinyObj   ${LINK_FILES} )
