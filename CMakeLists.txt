cmake_minimum_required(VERSION 3.18)

project(im_editor C CXX)

add_executable(${PROJECT_NAME})
add_subdirectory(src)

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
    glfw
    imgui::imgui
    ${OPENGL_LIBRARIES}
)