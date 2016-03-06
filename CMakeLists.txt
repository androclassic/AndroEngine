# Top-Level CmakeLists.txt
cmake_minimum_required (VERSION 2.8)

PROJECT( DemoProject )

if(UNIX)
  if(APPLE)
    set(PLATFORM "Apple")
  else(APPLE)
    set(PLATFORM "Linux")
  endif(APPLE)
else(UNIX)
  if(WIN32)
    set(PLATFORM "Win32")
  else(WIN32)
    set(PLATFORM "Unknown")
  endif(WIN32)
endif(UNIX)

set(ANDRO_SDK "${CMAKE_CURRENT_SOURCE_DIR}/AndroSDK")

ADD_SUBDIRECTORY( TakeOneRenderer )
ADD_SUBDIRECTORY( Force )
ADD_SUBDIRECTORY( Demos )
ADD_SUBDIRECTORY( AndroUtils )
ADD_SUBDIRECTORY( Lua )
ADD_SUBDIRECTORY( External )
