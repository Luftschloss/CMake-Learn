cmake_minimum_required(VERSION 3.1.0)

project(cmake_demo VERSION 2.0.1)

# 是否使用自己的 MathFunc 库
option (USE_MYMATH
       "Use provided math implementation" OFF)

set(MYTEST 7)

# 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  @ONLY
  )

  # 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file (
  "${PROJECT_SOURCE_DIR}/CPack.xml.in"
  "${PROJECT_BINARY_DIR}/CPack.xml"
  )

  message("dir:${PROJECT_BINARY_DIR}")

# 是否加入 MathFunc 库
if (USE_MYMATH)
  message("Use Math Library")
  include_directories ("${PROJECT_SOURCE_DIR}/Math")
  add_subdirectory (Math)  
  set (EXTRA_LIBS MathFunc)
endif (USE_MYMATH)

include_directories ("${PROJECT_SOURCE_DIR}/Thread")
add_subdirectory (Thread)
list (APPEND EXTRA_LIBS ThreadTest)

include_directories ("${PROJECT_SOURCE_DIR}/Template")
add_subdirectory (Template)
list (APPEND EXTRA_LIBS Template)

include_directories ("${PROJECT_SOURCE_DIR}/C++11Test")
add_subdirectory (C++11Test)
list (APPEND EXTRA_LIBS CPP11)

include_directories ("${PROJECT_SOURCE_DIR}/LeetCode")
add_subdirectory (LeetCode)
list (APPEND EXTRA_LIBS LeetCode)

include_directories("${PROJECT_SOURECE_DIR}/Protobuf")
add_subdirectory(Protobuf)
list (APPEND EXTRA_LIBS PBTest)

add_subdirectory(PETest)
add_subdirectory(WindowsCoding)

# 添加编译根目录
aux_source_directory(. DIRSRCS)
list (APPEND DIRSRCS "${PROJECT_SOURCE_DIR}/CPack.xml.in")

message(STATUS ${CMAKE_CURRENT_LIST_FILE})
message(${CMAKE_BINARY_DIR})
message("Project Dir: ${PROJECT_BINARY_DIR}")


# Target
add_executable(cmake_test ${DIRSRCS})
target_link_libraries(cmake_test ${EXTRA_LIBS})

set(package_name "MSIDemo" CACHE STRING "package name")

set(CPACK_PACKAGE_NAME ${package_name})
set(CPACK_PACKAGE_VENDOR "UWA")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${package_name})
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 2)
set(CPACK_PACKAGE_VERSION_PATCH 1)
set(CPACK_PACKAGE_EXECUTABLES ${package_name};${package_name})
set(CPACK_PACKAGE_ICON resource/icon/UWA-Gears.ico)
set(CPACK_CREATE_DESKTOP_LINKS ${package_name})
set(CPACK_GENERATOR WIX)
set(CPACK_WIX_UPGRADE_GUID 978C614F-708E-4E1A-B201-565925725DBA)
set(CPACK_WIX_CULTURES zh-CN)
set(CPACK_WIX_EXTENSIONS WixUIExtension WixUtilExtension)
set(CPACK_WIX_PRODUCT_ICON resource/icon/UWA-Gears.ico)
