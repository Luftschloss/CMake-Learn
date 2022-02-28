cmake_minimum_required(VERSION 3.6)

project(NDK_Slib CXX)

#设置NDK交叉编译toolchain_cmake路径
set(CMAKE_TOOLCHAIN_FILE
        "D:/Envs/RDCAndroidSDK/android-ndk-r14b/build/cmake/android.toolchain.cmake"
        CACHE STRING
        "The Android toolchain file")

set(ANDROID_NDK, "D:/Envs/RDCAndroidSDK/android-ndk-r14b")
set(CMAKE_BUILD_TYPE, "Release")
set(ANDROID_ABI, "armeabi-v7a")

file(GLOB native_srcs "${CMAKE_SOURCE_DIR}/src/*.cpp")
include_directories(${CMAKE_SOURCE_DIR}/include)

#STATIC表示编译结果为静态库.a,如果想为动态库.so,可改为SHARED
add_library(Math STATIC ${native_srcs}) 
target_link_libraries(Math)