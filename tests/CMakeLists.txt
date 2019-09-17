add_executable(pedal_test main.cpp)

set_target_properties(pedal_test PROPERTIES
    DEBUG_POSTFIX d
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(pedal_test PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(pedal_test pedal)
