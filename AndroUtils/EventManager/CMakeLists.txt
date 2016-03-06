# Create a library called "AndroRenderer" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 2.8)


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB HEADERS "*.h")

add_library (EventManager ${HEADERS}  ${CSOURCE}  ${CPPSOURCE}) 
SET_TARGET_PROPERTIES(EventManager PROPERTIES LINKER_LANGUAGE C)

