# Create a library called "TakeOneRenderer" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 2.8)


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB HEADERS "*.h")
FILE(GLOB HEADERS_INL "*.inl")
FILE(GLOB CPPHEADERS "*.hpp")


include_directories( ${ANDRO_SDK}/GLEW/include  ${ANDRO_SDK}/soil/include  ${ANDRO_SDK}/GLFW/include  ${ANDRO_SDK}/glm/)
add_definitions(-DANDRO_EVENT_MANAGER -DENABLE_DEBUG)


add_library (TakeOneRenderer ${HEADERS} ${HEADERS_INL} ${CPPHEADERS} ${CSOURCE}  ${CPPSOURCE}) 

SET_TARGET_PROPERTIES(TakeOneRenderer PROPERTIES LINKER_LANGUAGE C)

