# Find and link against ws2_32 library
find_library(WS2_32_LIBRARY NAMES ws2_32)

file(GLOB_RECURSE SRC_SIMULATOR *.c *.cc *.cpp *.cxx)
# set(SRC_SIMULATOR ${CMAKE_CURRENT_SOURCE_DIR}/communication/communication.cpp ${CMAKE_CURRENT_SOURCE_DIR}/log/log.cpp ${CMAKE_CURRENT_SOURCE_DIR}/NanoLog/NanoLog.cpp ${CMAKE_CURRENT_SOURCE_DIR}/time/times.cpp ${CMAKE_CURRENT_SOURCE_DIR}/event/event.cpp)

set(SRC_SIMULATOR ${SRC_SIMULATOR} ${SRC_USER})
add_library(simulator_share SHARED ${SRC_SIMULATOR})
target_include_directories(simulator_share PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(simulator_share PUBLIC ${CMAKE_SOURCE_DIR}/include/NanoLog)
target_include_directories(simulator_share PUBLIC ${CMAKE_SOURCE_DIR}/include/nlohmann)
target_include_directories(simulator_share PUBLIC ${INCLUDE_USER})
target_link_libraries(simulator_share ${WS2_32_LIBRARY})
set_target_properties(simulator_share PROPERTIES  OUTPUT_NAME simulator)
add_library(simulator_static STATIC ${SRC_SIMULATOR})
target_include_directories(simulator_static PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(simulator_static PUBLIC ${CMAKE_SOURCE_DIR}/include/NanoLog)
target_include_directories(simulator_static PUBLIC ${CMAKE_SOURCE_DIR}/include/nlohmann)
target_include_directories(simulator_static PUBLIC ${INCLUDE_USER})
target_link_libraries(simulator_static ${WS2_32_LIBRARY})
set_target_properties(simulator_static PROPERTIES  OUTPUT_NAME simulator)


install(TARGETS simulator_share simulator_static
        EXPORT SimuCoreLib
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/lib
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin
        PUBLIC_HEADER DESTINATION ${CMAKE_SOURCE_DIR}/include)
