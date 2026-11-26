cmake_minimum_required(VERSION 3.16)
project(warehouse_dispatch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CORE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../..")

find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_library(warehouse_core
  ${CORE_SOURCE_DIR}/src/warehouse/domain.cpp
  ${CORE_SOURCE_DIR}/src/dispatch/policy.cpp
  ${CORE_SOURCE_DIR}/src/dispatch/scheduler.cpp
  ${CORE_SOURCE_DIR}/src/audit/trace.cpp
  ${CORE_SOURCE_DIR}/src/app/cli.cpp
)
target_include_directories(warehouse_core PUBLIC ${CORE_SOURCE_DIR}/include)

add_executable(simulator src/simulator_node.cpp)
target_link_libraries(simulator warehouse_core)
ament_target_dependencies(simulator rclcpp std_msgs)

add_executable(dispatcher src/dispatcher_node.cpp)
target_link_libraries(dispatcher warehouse_core)
ament_target_dependencies(dispatcher rclcpp std_msgs)

install(TARGETS simulator dispatcher DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

ament_package()
