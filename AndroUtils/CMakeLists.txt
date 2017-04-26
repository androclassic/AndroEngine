# Create a library called "AndroUtils" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 3.1.0 FATAL_ERROR)


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


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


add_definitions(-D_CRT_RAND_S -DANDRO_UTILS )
source_group(EventManager  FILES  ${EM_CPPSOURCE}  ${EM_CSOURCE}  ${EM_HEADERS})
source_group(Utils 		   FILES  ${UTILS_CPPSOURCE}  ${UTILS_CSOURCE}  ${UTILS_HEADERS})
source_group(Introspection FILES  ${I_CPPSOURCE}  ${I_CSOURCE}  ${I_HEADERS})

add_library (AndroUtils ${HEADERS} ${CSOURCE} ${CPPSOURCE} ${EM_CPPSOURCE} ${EM_CSOURCE} ${EM_HEADERS} ${UTILS_CPPSOURCE} ${UTILS_CSOURCE} ${UTILS_HEADERS} ${I_CPPSOURCE} ${I_CSOURCE} ${I_HEADERS} ) 

SET_TARGET_PROPERTIES(AndroUtils PROPERTIES LINKER_LANGUAGE C)
