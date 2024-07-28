# Set the header directory
set(INCLUDE_USER ${CMAKE_SOURCE_DIR}/user PARENT_SCOPE)

# Set the source files
file(GLOB_RECURSE SRC_USER *.c *.cc *.cpp *.cxx)
# set(SRC_USER ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(SRC_USER ${SRC_USER} CACHE STRING "Global source files")