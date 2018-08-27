cmake_minimum_required(VERSION 3.1.0)

project(MatchWatcher)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
file(GLOB SRC "*.h" "*.hpp" "*.cpp" "*.ui")
add_executable(matchwatcher ${SRC})
target_link_libraries(matchwatcher Qt5::Widgets  Eigen3::Eigen)