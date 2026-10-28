cmake_minimum_required(VERSION 3.8)
project(robot_speed_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)



#新增发布节点
add_executable(speed_publisher src/speed_publisher.cpp)
ament_target_dependencies(speed_publisher std_msgs rclcpp)
#新增监测节点
add_executable(speed_monitor src/speed_monitor.cpp)
ament_target_dependencies(speed_monitor std_msgs rclcpp)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS
  speed_monitor
  speed_publisher
  DESTINATION lib/${PROJECT_NAME}
  )
ament_package()
