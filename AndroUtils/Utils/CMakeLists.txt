# Create a library called "AndroUtils" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 2.8)


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB HEADERS "*.h")

FILE(GLOB EM_CPPSOURCE "./EventManager/*.cpp")
FILE(GLOB EM_CSOURCE "./EventManager/*.c")
FILE(GLOB EM_HEADERS "./EventManager/*.h")

FILE(GLOB EM_CPPSOURCE "./Introspection/*.cpp")
FILE(GLOB EM_CSOURCE   "./Introspection/*.c")
FILE(GLOB EM_HEADERS   "./Introspection/*.h")


add_definitions(-D_CRT_RAND_S -DANDRO_UTILS )
source_group(EventManager FILES  ${EM_CPPSOURCE}  ${EM_CSOURCE}  ${EM_HEADERS})

add_library (AndroUtils ${HEADERS}   ${CSOURCE}  ${CPPSOURCE} ${EM_CPPSOURCE}  ${EM_CSOURCE}  ${EM_HEADERS}) 
SET_TARGET_PROPERTIES(AndroUtils PROPERTIES LINKER_LANGUAGE C)
