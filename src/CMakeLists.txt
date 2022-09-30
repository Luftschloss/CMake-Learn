cmake_minimum_required(VERSION 2.8.12)

project(cmake_test)

# 添加编译根目录
aux_source_directory(. DIRSRCS)

set(CMAKE_CXX_STANDARD 11)

message(STATUS ${CMAKE_CURRENT_LIST_FILE})
message(${CMAKE_BINARY_DIR})
message("Project Dir: ${PROJECT_BINARY_DIR}")
  
# set macro "USE_MACRO" in main.cpp
option(-DIS_USE_MACRO "is Use Macro")
if(IS_USE_MACRO)
	add_definitions(-DUSE_MACRO)
endif()

if(ANDROID)
	message(STATUS "Android")
	if(DEFINED ENV{GoLand})
		message(STATUS "Defined GoLand: $ENV{GoLand}")
	else()
		message(STATUS "Not Defined GoLand")
	endif()
else()
	message(STATUS "Not Adnroid")
endif()

# 添加头文件Include的目录
include_directories(./Hello ./World)

# 添加编译子目录
add_subdirectory(Hello)
add_subdirectory(World)

# Target
add_executable(cmake_test ${DIRSRCS})
target_link_libraries(cmake_test hello world)

# 手动添加编译文件
# set(HELLO_SRC Hello/hello.cpp)
# set(WORLD_SRC World/world.cpp)
# add_executable(cmake_test main.cpp ${HELLO_SRC} ${WORLD_SRC})