cmake_minimum_required(VERSION 3.10)
project(MyRaylibProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/include)
link_directories(/usr/lib)

add_executable(Pong State.cpp main.cpp menu.cpp button.cpp)

target_link_libraries(Pong PRIVATE raylib GL m pthread dl X11)
