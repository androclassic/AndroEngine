# Create a library called "AndroRenderer" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 2.8)


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB HEADERS "*.h")

SET(EXECUTABLE_OUTPUT_PATH "${ANDRO_SDK}/../bin")

set(LINK_DIRS
    ${ANDRO_SDK}/GLFW/lib/${PLATFORM}
    ${ANDRO_SDK}/GLEW/lib/${PLATFORM}
    ${ANDRO_SDK}/soil/lib/${PLATFORM}
    )

link_directories(${LINK_DIRS})
include_directories(${ANDRO_SDK}/GLEW/include  ${ANDRO_SDK}/soil/include  ${ANDRO_SDK}/GLFW/include )
include_directories(${ANDRO_SDK}/../)
include_directories(${ANDRO_SDK}/../Force)

if(PLATFORM STREQUAL "Win32")
    set(LINK_FILES
        SOIL
        glfw3dll
        glew32.dll
		GlU32.lib
        opengl32
        )
else(PLATFORM STREQUAL "Win32")
    set(LINK_FILES
        SOIL
        glfw
        GLEW
        GL
        )
endif(PLATFORM STREQUAL "Win32")
	
add_executable (Platform WIN32 ${HEADERS} ${CSOURCE}  ${CPPSOURCE} "../Window.h" "../Window.cpp" ) 
target_link_libraries(Platform Force EventManager  AndroRenderer ${LINK_FILES})
