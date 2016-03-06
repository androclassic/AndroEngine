# Create a library called "AndroUtils" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 2.8)


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB HEADERS "*.h")

FILE(GLOB EM_CPPSOURCE "./EventManager/*.cpp")
FILE(GLOB EM_CSOURCE "./EventManager/*.c")
FILE(GLOB EM_HEADERS "./EventManager/*.h")

FILE(GLOB I_CPPSOURCE "./Introspection/*.cpp")
FILE(GLOB I_CSOURCE   "./Introspection/*.c")
FILE(GLOB I_HEADERS   "./Introspection/*.h")

FILE(GLOB  UTILS_CPPSOURCE "./Utils/*.cpp")
FILE(GLOB  UTILS_CSOURCE   "./Utils/*.c")
FILE(GLOB  UTILS_HEADERS   "./Utils/*.h")


add_definitions(-DANDRO_UTILS)
source_group(EventManager  FILES  ${EM_CPPSOURCE}  ${EM_CSOURCE}  ${EM_HEADERS})
source_group(Utils 		   FILES  ${UTILS_CPPSOURCE}  ${UTILS_CSOURCE}  ${UTILS_HEADERS})
source_group(Introspection FILES  ${I_CPPSOURCE}  ${I_CSOURCE}  ${I_HEADERS})

add_library (AndroUtils ${HEADERS} ${CSOURCE} ${CPPSOURCE} ${EM_CPPSOURCE} ${EM_CSOURCE} ${EM_HEADERS} ${UTILS_CPPSOURCE} ${UTILS_CSOURCE} ${UTILS_HEADERS} ${I_CPPSOURCE} ${I_CSOURCE} ${I_HEADERS} ) 
SET_TARGET_PROPERTIES(AndroUtils PROPERTIES LINKER_LANGUAGE C)
