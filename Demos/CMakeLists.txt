cmake_minimum_required (VERSION 2.8)
ADD_SUBDIRECTORY( Demonstation)
ADD_SUBDIRECTORY( RayTracing)

find_package(CUDA QUIET)
if(CUDA_FOUND)
	ADD_SUBDIRECTORY( RayTracingCuda)
endif()