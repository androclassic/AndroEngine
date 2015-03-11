# Create a library called "Force" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
cmake_minimum_required (VERSION 2.8)

FILE(GLOB CPPSOURCEP "ParticlePhysics/*.cpp")
FILE(GLOB CSOURCEP "ParticlePhysics/*.c")
FILE(GLOB HEADERSP "ParticlePhysics/*.h")
SOURCE_GROUP(ParticlePhysics FILES  ${HEADERSP}  ${CSOURCEP}  ${CPPSOURCEP})


FILE(GLOB CPPSOURCER "RigidBodyPhysics/*.cpp")
FILE(GLOB CSOURCER "RigidBodyPhysics/*.c")
FILE(GLOB HEADERSR "RigidBodyPhysics/*.h")
SOURCE_GROUP(RigidBodyPhysics FILES  ${HEADERSR}  ${CSOURCER}  ${CPPSOURCER})

FILE(GLOB CPPSOURCEC "core/*.cpp")
FILE(GLOB CSOURCEC "core/*.c")
FILE(GLOB HEADERSC "core/*.h")
SOURCE_GROUP(Core FILES  ${HEADERSC}  ${CSOURCEC}  ${CPPSOURCEC})


FILE(GLOB CPPSOURCE "*.cpp")
FILE(GLOB CSOURCE "*.c")
FILE(GLOB HEADERS "*.h")

include_directories("./core")
include_directories("./RigidBodyPhysics")
include_directories("./ParticlePhysics")
include_directories("./")

add_library (Force ${HEADERS} ${HEADERSC} ${HEADERSP} ${HEADERSR} 
				   ${CSOURCE} ${CSOURCEC} ${CSOURCER} ${CSOURCE}
				   ${CPPSOURCEC} ${CPPSOURCER} ${CPPSOURCEP} ${CPPSOURCE} ${CPPSOURCE}) 

SET_TARGET_PROPERTIES(Force PROPERTIES LINKER_LANGUAGE C)

