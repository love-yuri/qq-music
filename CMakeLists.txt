cmake_minimum_required(VERSION 3.16)

project(yuri VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 26) # 设置c++ 最低版本
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 开启生成 compile_commands.json
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "TRUE")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

# 寻找qt依赖库
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.8)

# 搜索源码、头文件、资源文件
file(GLOB IXX_MODULES ${PROJECT_SOURCE_DIR}/include/modules/*.ixx)
file(GLOB_RECURSE PROJECT_SOURCES
  CONFIGURE_DEPENDS
  main.cpp
  "${CMAKE_SOURCE_DIR}/include/modules/*.ixx"
  "${CMAKE_SOURCE_DIR}/include/components/*.hpp"
  "${CMAKE_SOURCE_DIR}/include/*.hpp"

  "${CMAKE_SOURCE_DIR}/src/components/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/qrc/*.qrc"
)

# 构建可执行文件
qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})

qt_add_qml_module(
  ${PROJECT_NAME}
  URI main
  QML_FILES
  Main.qml
)

target_sources(${PROJECT_NAME}
  PRIVATE
  FILE_SET cxx_modules TYPE CXX_MODULES FILES
  ${IXX_MODULES}
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_SCAN_FOR_MODULES ON)


# 包含include目录
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 链接文件
target_link_libraries(yuri
  PRIVATE 
  Qt6::Quick
)