# Top-Level CmakeLists.txt
cmake_minimum_required (VERSION 2.8)

PROJECT( DemoProject )

if(UNIX)
  if(APPLE)
    set(PLATFORM "Apple")
  else(APPLE)
    SET(PLATFORM "Linux")
    add_definitions(-std=gnu++0x)
  endif(APPLE)
else(UNIX)
  if(WIN32)
    set(PLATFORM "Win32")
  else(WIN32)
    set(PLATFORM "Unknown")
  endif(WIN32)
endif(UNIX)

set(ANDRO_SDK "${CMAKE_CURRENT_SOURCE_DIR}/AndroSDK")


MESSAGE(${PLATFORM})

ADD_SUBDIRECTORY( Renderer )
ADD_SUBDIRECTORY( Force )
ADD_SUBDIRECTORY( Demos )
ADD_SUBDIRECTORY( AndroUtils )
ADD_SUBDIRECTORY( External )
