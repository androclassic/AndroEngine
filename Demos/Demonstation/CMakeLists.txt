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
    )

link_directories(${LINK_DIRS})
include_directories(${ANDRO_SDK}/GLEW/include  ${ANDRO_SDK}/GLFW/include ${ANDRO_SDK}/glm/)
include_directories(${ANDRO_SDK}/../)
include_directories(${ANDRO_SDK}/../Force)
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
		
add_executable (Demonstation WIN32 ${HEADERS} ${GL_HEADERS} ${GL_CPPSOURCE} ${CSOURCE}  ${CPPSOURCE}) 
target_link_libraries(Demonstation Force  AndroUtils Renderer Lua TinyObj SOIL ${LINK_FILES})
