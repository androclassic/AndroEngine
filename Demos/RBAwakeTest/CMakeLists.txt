# Create a library called "AndroRenderer" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 2.8)


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB HEADERS "*.h")
FILE(GLOB GL_HEADERS "../*.h")
FILE(GLOB GL_CPPSOURCE "../*.cpp")


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
include_directories(${ANDRO_SDK}/../Demos)



if(PLATFORM STREQUAL "Win32")
    set(LINK_FILES
        SOIL
        glfw3dll
        glew32.dll
        opengl32
		GlU32.lib
        )
else(PLATFORM STREQUAL "Win32")
    set(LINK_FILES
        SOIL
        glfw
        GLEW
        GL
        )
endif(PLATFORM STREQUAL "Win32")
		
add_executable (RBAwakeTest WIN32 ${HEADERS} ${GL_HEADERS} ${GL_CPPSOURCE} ${CSOURCE}  ${CPPSOURCE}) 
target_link_libraries(RBAwakeTest Force EventManager AndroRenderer  ${LINK_FILES})
