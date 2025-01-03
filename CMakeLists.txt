cmake_minimum_required(VERSION 3.18)

project(im_editor C CXX)

add_executable(${PROJECT_NAME})
add_subdirectory(src)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    imgui::imgui
)