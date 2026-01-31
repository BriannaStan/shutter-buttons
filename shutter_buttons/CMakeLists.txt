cmake_minimum_required(VERSION 3.8)
project(shutter_buttons)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)

# install python package
ament_python_install_package(${PROJECT_NAME})

# install launch files
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
  PATTERN "__pycache__" EXCLUDE
)

# install entry point script
install(PROGRAMS
  src/button_board.py
  DESTINATION lib/${PROJECT_NAME}
  RENAME button_board
)

ament_package()
