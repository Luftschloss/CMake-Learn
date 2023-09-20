cmake_minimum_required(VERSION 3.1.0)

project(cmake_demo)

# 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

# 是否使用自己的 MathFunc 库
option (USE_MYMATH
       "Use provided math implementation" ON)

# 是否加入 MathFunc 库
if (USE_MYMATH)
  include_directories ("${PROJECT_SOURCE_DIR}/Math")
  add_subdirectory (Math)  
  set (EXTRA_LIBS ${EXTRA_LIBS} MathFunc)
endif (USE_MYMATH)

# 添加编译根目录
aux_source_directory(. DIRSRCS)

message(STATUS ${CMAKE_CURRENT_LIST_FILE})
message(${CMAKE_BINARY_DIR})
message("Project Dir: ${PROJECT_BINARY_DIR}")


# Target
add_executable(cmake_test ${DIRSRCS})
target_link_libraries(cmake_test ${EXTRA_LIBS})
