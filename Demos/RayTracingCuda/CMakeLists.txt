# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 3.1.0 FATAL_ERROR)


FILE(GLOB CPPSOURCE "../RayTracing/*.cpp")
FILE(GLOB CSOURCE "../RayTracing/*.c")
FILE(GLOB CUDASOURCE "../RayTracing/kernels/*.cpp")
FILE(GLOB HEADERS "../RayTracing/*.h")

SET(CXX_STANDARD 11)
SET(CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


SET(EXECUTABLE_OUTPUT_PATH "${ANDRO_SDK}/../bin")

set(LINK_DIRS
    ${ANDRO_SDK}/GLFW/lib/${PLATFORM}
    ${ANDRO_SDK}/GLEW/lib/${PLATFORM}
    )

link_directories(${LINK_DIRS})
include_directories(${ANDRO_SDK}/GLEW/include  ${ANDRO_SDK}/GLFW/include)
include_directories(${ANDRO_SDK}/../)
include_directories(${ANDRO_SDK}/../Demos)

if(CUDA_FOUND)
	set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};--gpu-architecture=sm_60;;-std=c++11")
	set(CUDA_SEPARABLE_COMPILATION ON CACHE BOOL "FindCUDA script variable determining whether files are compiled with -rc flag. Required for this project" FORCE)
	
	INCLUDE_DIRECTORIES("${CUDA_PATH}/common/inc")
	
	add_definitions(-D_USE_CUDA)
	add_definitions(-DGLM_FORCE_CUDA)
endif()

if(CUDA_FOUND)
	#need to set .cpp files to cuda source files so they are compiled with nvcc rather than host compiler
	set_source_files_properties(${CSOURCE}  ${CPPSOURCE} ${CUDASOURCE}  PROPERTIES CUDA_SOURCE_PROPERTY_FORMAT OBJ)
endif()


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

if(CUDA_FOUND)
CUDA_ADD_EXECUTABLE (RayTracingCuda WIN32 ${HEADERS} ${CSOURCE}  ${CPPSOURCE} ${CUDASOURCE} ) 
else()
add_executable (RayTracingCuda WIN32 ${HEADERS} ${CSOURCE}  ${CPPSOURCE}) 
endif()		

target_link_libraries(RayTracingCuda  AndroUtils  Lua TinyObj   ${LINK_FILES} )
