cmake_minimum_required(VERSION 3.20) #设置cmake最低版本

if (WIN32)
  set(CMAKE_PREFIX_PATH "D:\\Qt\\6.8.0\\msvc2022_64\\lib\\cmake")
  if (DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT "E:/love-yuri/github/vcpkg")
    message("VCPKG_ROOT 使用: $ENV{VCPKG_ROOT}")
  else ()
    set(VCPKG_ROOT "E:/love-yuri/github/vcpkg")
    message(WARNING "VCPKG_ROOT 没有设置, 使用默认: ${VCPKG_ROOT}")
  endif ()
  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif ()

# 设置项目名称 版本 语言
project(yuri VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON) # 自动编译ui文件
set(CMAKE_AUTOMOC ON) # 自动编译moc文件
set(CMAKE_AUTORCC ON) # 自动编译资源文件
set(CMAKE_CXX_STANDARD 23) # 设置c++ 最低版本
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 要求使用指定版本号
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 开启生成 compile_commands.json
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "TRUE")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

# 添加api工程
add_subdirectory(api)

# 寻找qt依赖库
find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Svg)

# 包含ui文件
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/include/ui)

# 搜索源码、头文件、资源文件
file(GLOB IXX_MODULES
    ${PROJECT_SOURCE_DIR}/include/modules/*.ixx
)

file(GLOB_RECURSE PROJECT_SOURCES
  CONFIGURE_DEPENDS
  main.cpp
  ${IXX_MODULES}
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/components/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/components/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/qrc/*.qrc"
)

# 构建可执行文件
qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})

target_sources(${PROJECT_NAME}
  PRIVATE
  FILE_SET cxx_modules TYPE CXX_MODULES FILES
  ${IXX_MODULES}
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_SCAN_FOR_MODULES ON)

# 包含include目录
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 链接文件
target_link_libraries(
  ${PROJECT_NAME} 
  PRIVATE 
  Qt6::Widgets
  Qt6::Svg
)

# 链接api库
target_link_libraries(${PROJECT_NAME} PRIVATE qq_music_api)

qt_finalize_executable(${PROJECT_NAME})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC /Zc:preprocessor /utf-8)
endif()

# 拷贝js
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/api/script"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/script"
    COMMENT "Copying script directory to executable output directory..."
)

if (WIN32)
  get_target_property(QtCore_location Qt6::Core LOCATION)
  get_filename_component(QT_BIN_DIR "${QtCore_location}" DIRECTORY)
  set(QT_PLUGIN_DIR "${QT_BIN_DIR}/../plugins")
  message(STATUS "Qt bin path: ${QT_BIN_DIR}")

  # 平台插件所在路径

  # 拷贝 Qt 平台插件
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${QT_PLUGIN_DIR}/platforms/qwindows$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:d>.dll"
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms"
  )

  add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${QT_PLUGIN_DIR}/imageformats"
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats"
  )

  foreach (QT_LIB Core Gui Widgets Svg)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${QT_BIN_DIR}/Qt6${QT_LIB}$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:d>.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
  endforeach ()

endif ()