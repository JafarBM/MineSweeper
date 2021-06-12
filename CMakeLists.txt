cmake_minimum_required(VERSION 3.13)
project(AP_MAHLA)

set(CMAKE_CXX_STANDARD 14)
set(Qt5_DIR "~/Qt/5.12.0/gcc_64/lib/cmake/Qt5/")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(AP_MAHLA main.cpp Sources/Login.cpp Headers/Login.h Sources/Client.cpp Headers/Client.h Headers/BoardGame.h Sources/BoardGame.cpp)

find_package(Qt5 COMPONENTS Core Widgets Gui Multimedia REQUIRED)
qt5_use_modules(AP_MAHLA Core Widgets Gui Multimedia)
target_link_libraries(AP_MAHLA Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Multimedia)