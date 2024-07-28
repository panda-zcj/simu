cmake_minimum_required(VERSION 3.7.2)

project(simu VERSION 0.1.0)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CLANG_TIDY_COMMAND ${CMAKE_SOURCE_DIR}/tool/LLVM/bin/clang-tidy.exe;
#                             -header-filter=.;
#                             -checks=*;)
# set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND};-p=${CMAKE_BINARY_DIR};)

set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(user)
add_subdirectory(src)
add_subdirectory(test)

set(SIMULATOR_RUN_STATE_SRC "${CMAKE_SOURCE_DIR}/user/simulator_run_state.json")
set(SIMULATOR_RUN_STATE_DST "${CMAKE_SOURCE_DIR}/bin/simulator_run_state.json")
add_custom_command(
    OUTPUT ${SIMULATOR_RUN_STATE_DST}
    COMMAND ${CMAKE_COMMAND} -E copy ${SIMULATOR_RUN_STATE_SRC} ${SIMULATOR_RUN_STATE_DST}
    DEPENDS ${SIMULATOR_RUN_STATE_SRC}
    COMMENT "Copying ${SIMULATOR_RUN_STATE_SRC} to ${SIMULATOR_RUN_STATE_DST}"
)
add_custom_target(SIMULATOR_RUN_STATE ALL DEPENDS ${SIMULATOR_RUN_STATE_DST})

set(NODE_RUN_STATE_SRC "${CMAKE_SOURCE_DIR}/user/node_run_state.json")
set(NODE_RUN_STATE_DST "${CMAKE_SOURCE_DIR}/bin/node_run_state.json")
add_custom_command(
    OUTPUT ${NODE_RUN_STATE_DST}
    COMMAND ${CMAKE_COMMAND} -E copy ${NODE_RUN_STATE_SRC} ${NODE_RUN_STATE_DST}
    DEPENDS ${NODE_RUN_STATE_SRC}
    COMMENT "Copying ${NODE_RUN_STATE_SRC} to ${NODE_RUN_STATE_DST}"
)
add_custom_target(NODE_RUN_STATE ALL DEPENDS ${NODE_RUN_STATE_DST})

add_executable(simu_simulator main.cpp)
target_link_libraries(simu_simulator simulator_static)
target_compile_definitions(simu_simulator PRIVATE SIMULATOR)
set_target_properties(simu_simulator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_dependencies(simu_simulator SIMULATOR_RUN_STATE)

add_executable(simu_node main.cpp)
target_link_libraries(simu_node simulator_static)
set_target_properties(simu_node PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_dependencies(simu_simulator NODE_RUN_STATE)

# 生成注释文档
find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_RECURSIVE YES)
set(DOXYGEN_CLASS_GRAPH YES)
set(DOXYGEN_COLLABORATION_GRAPH NO)
set(DOXYGEN_GROUP_GRAPHS NO)
set(DOXYGEN_INCLUDE_GRAPH YES)
set(DOXYGEN_INCLUDED_BY_GRAPH YES)
set(DOXYGEN_CALL_GRAPH NO)
set(DOXYGEN_CALLER_GRAPH NO)
set(DOXYGEN_GRAPHICAL_HIERARCHY NO)
set(DOXYGEN_DIRECTORY_GRAPH NO)
set(DOXYGEN_PROJECT_LOGO doc/icon.svg)
set(DOXYGEN_EXCLUDE_PATTERNS */gtest/* */gmock/* */nlohmann/* ${DOXYGEN_EXCLUDE_PATTERNS})
doxygen_add_docs(simu_doc
    include, src, user, doc/description.h
    ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generate  html"
)