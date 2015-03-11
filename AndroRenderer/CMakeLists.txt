# Create a library called "AndroRenderer" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 2.8)


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB HEADERS "*.h")


include_directories( ${ANDRO_SDK}/GLEW/include  ${ANDRO_SDK}/soil/include  ${ANDRO_SDK}/GLFW/include)
add_definitions(-DANDRO_EVENT_MANAGER)


add_library (AndroRenderer ${HEADERS}  ${CSOURCE}  ${CPPSOURCE}) 

SET_TARGET_PROPERTIES(AndroRenderer PROPERTIES LINKER_LANGUAGE C)

