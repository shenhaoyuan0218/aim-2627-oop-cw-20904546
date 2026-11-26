cmake_minimum_required(VERSION 3.16)
project(aim_oop_coursework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(aim_oop_core
    src/warehouse/domain.cpp
    src/dispatch/policy.cpp
    src/dispatch/scheduler.cpp
    src/audit/trace.cpp
    src/app/cli.cpp
)
target_include_directories(aim_oop_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(aim_oop_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(core_demo src/demo/main.cpp)
target_link_libraries(core_demo PRIVATE aim_oop_core)

enable_testing()
add_executable(dispatch_cli src/app/main.cpp)
target_link_libraries(dispatch_cli PRIVATE aim_oop_core)
add_executable(public_tests tests/public_tests.cpp)
target_link_libraries(public_tests PRIVATE aim_oop_core)
add_test(NAME public_tests COMMAND public_tests)

add_executable(debug_task src/debug/main.cpp)
add_test(NAME debug_task COMMAND debug_task)

if(CW_HIDDEN_DIR)
    add_executable(hidden_tests ${CW_HIDDEN_DIR}/hidden_tests.cpp)
    target_link_libraries(hidden_tests PRIVATE aim_oop_core)
    add_test(NAME hidden_tests COMMAND hidden_tests)
endif()
