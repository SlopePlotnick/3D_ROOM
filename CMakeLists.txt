cmake_minimum_required(VERSION 3.26)
project(3D_ROOM)

set(CMAKE_CXX_STANDARD 17)
# 在arm的MacBook上进行的调整
set(CMAKE_OSX_ARCHITECTURES  "arm64" CACHE STRING "Build architectures for Mac OS X" FORCE)

# 引入本地库 其中有glad库
set(LOCAL_H /opt/homebrew/include)
include_directories(${LOCAL_H})

# 引入glew库和glfw库
set(GLEW_H /opt/homebrew/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /opt/homebrew/Cellar/glfw/3.3.9/include/GLFW)
include_directories(${GLEW_H} ${GLFW_H})

# 链接glew库和glfw库
set(GLEW_LINK /opt/homebrew/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.dylib)
set(GLFW_LINK /opt/homebrew/Cellar/glfw/3.3.9/lib/libglfw.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

# 将文件加入编译项目
set(SOURCE_FILES "lib/glad.c" "main.cpp")
add_executable(3D_ROOM ${SOURCE_FILES})

# Mac系统下的调整
if (APPLE)
    target_link_libraries(3D_ROOM "-framework OpenGL")
    target_link_libraries(3D_ROOM "-framework GLUT")
endif ()
