cmake_minimum_required(VERSION 2.8)
project(simple_particle)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

################################################################################
#PACKAGES
# find_package(LM REQUIRED)
find_package(SDL REQUIRED)

################################################################################
#INCLUDES
include_directories(
    include
    # ${SDL_INCLUDE_DIR}
)

################################################################################
#SOURCES
file(GLOB_RECURSE SIMPLE_PARTICLE_SRC "src/SimpleParticle/*.c")

################################################################################
#LINK
LIST(APPEND LINK_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${SDL_LIBRARY}
	m
)

################################################################################
#EXECUTABLE
add_executable(${PROJECT_NAME} ${SIMPLE_PARTICLE_SRC} src/main.c)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
