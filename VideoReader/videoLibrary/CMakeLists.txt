
# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/videoreader)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/callback)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src/main/cpp)



file(GLOB native_srcs ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/*.cpp
                      ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/videoreader/*.cc
                      ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/callback/*.cpp )



find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )
find_library( android-lib android )
find_library( # Sets the name of the path variable.
        zlib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        z )

find_library( # Sets the name of the path variable.
        graphicslib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        jnigraphics)



add_library( # Sets the name of the library.
        videoreader
        # Sets the library as a shared library.
        SHARED
        ${native_srcs}

        )




set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}")

target_link_libraries( # Specifies the target library.
        videoreader

        opencv_videoio
        opencv_calib3d
        opencv_video
        opencv_imgcodecs
        opencv_imgproc
        opencv_core
        opencv_ml
        opencv_objdetect
        opencv_shape
        opencv_stitching
        opencv_superres
        opencv_videostab
        opencv_features2d
        opencv_flann
        opencv_highgui
        opencv_photo
        IlmImf
        tegra_hal
        libjasper
        libtiff
        libwebp
        libpng
        cpufeatures
        libjpeg-turbo
        avcodec
        avformat
        swscale
        avutil

        ${log-lib}
        ${graphicslib}
        ${zlib}
        )
