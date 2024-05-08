file(GLOB_RECURSE SRC_EVENT *.c *.cc *.cpp *.cxx)
add_library(event_share SHARED ${SRC_EVENT})
target_include_directories(event_share PUBLIC ${CMAKE_SOURCE_DIR}/include)
set_target_properties(event_share PROPERTIES  OUTPUT_NAME event)
add_library(event_static STATIC ${SRC_EVENT})
target_include_directories(event_static PUBLIC ${CMAKE_SOURCE_DIR}/include)
set_target_properties(event_static PROPERTIES  OUTPUT_NAME event)


install(TARGETS event_share event_static
        EXPORT SimuCoreLib
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/lib
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin
        PUBLIC_HEADER DESTINATION ${CMAKE_SOURCE_DIR}/include)
