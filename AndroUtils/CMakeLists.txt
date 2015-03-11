# Create a library called "AndroUtils" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 2.8)


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB HEADERS "*.h")



add_definitions(-DANDRO_UTILS)


add_library (AndroUtils ${HEADERS}  ${CSOURCE}  ${CPPSOURCE}) 
SET_TARGET_PROPERTIES(AndroUtils PROPERTIES LINKER_LANGUAGE C)
