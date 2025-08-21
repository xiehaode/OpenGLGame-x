cmake_minimum_required(VERSION 3.28)
project(OpenGLGame2)

find_package(OpenGL REQUIRED)

# 查找 FreeType 库
find_package(Freetype REQUIRED)

# 查找GLFW3（修改后）
find_package(glfw3 REQUIRED)  # 注意：在Ubuntu中通常是小写"glfw3"
find_package(assimp REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
#file(COPY ${RESOURCE_DIR}/ DESTINATION ${BUILD_RESOURCE_DIR})
#(my_app copy_resources)

# 包含目录
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${GLFW3_INCLUDE_DIRS}  # 添加GLFW3头文件路径
        ${FREETYPE_INCLUDE_DIRS}
)

# 添加可执行文件（修正语法）
set(SOURCE_FILES
        src/main.cpp
        src/glad.c
        src/myGame.cpp
        src/model.cpp
        src/skybox.cpp
        include/skybox.h
        src/mFloor.cpp
        include/mFloor.h
        src/mModel.cpp
        include/mModel.h
        src/mShoot.cpp
        include/mShoot.h
        src/mText.cpp
        include/mText.h
        src/collisionDetector.cpp
        include/collisionDetector.h
)

add_executable(OpenGLGame2 ${SOURCE_FILES})  # 修正变量引用

# 链接库（修改后）
target_link_libraries(OpenGLGame2
        ${OPENGL_LIBRARIES}  # 修正变量名
        glfw  # Ubuntu中直接链接"glfw"
        assimp::assimp
        ${FREETYPE_LIBRARIES}
)
