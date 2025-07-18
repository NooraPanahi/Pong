cmake_minimum_required(VERSION 3.10)
project(MyRaylibProject)

set(CMAKE_CXX_STANDARD 17)

# مسیرهای کتابخانه و هدر را مشخص کن
include_directories(/usr/include)
link_directories(/usr/lib)

add_executable(MyExecutable State.cpp main.cpp menu.cpp button.cpp)

# لینک کردن کتابخانه‌های لازم
target_link_libraries(MyExecutable PRIVATE raylib GL m pthread dl X11)
