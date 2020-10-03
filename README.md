This project is an example of how to use opencv3.4.3 VideoCapture and VideoWriter on Android application.  

It seems that opencv does not support video capture feature on Android platform. To use video capture, you need to compile opencv library to support it or you can use the library files in project.  


Build opencv for android platform  

1.Modify CMakeLists.txt file in opencv root directory.  


(1).change IF (NOT ANDROID AND NOT IOS AND NOT WINRT) to IF (NOT IOS AND NOT WINRT) as follows.  
#OCV_OPTION(WITH_FFMPEG         "Include FFMPEG support"                      ON   IF (NOT ANDROID AND NOT IOS AND NOT WINRT) )  
OCV_OPTION(WITH_FFMPEG         "Include FFMPEG support"                      ON   IF (NOT IOS AND NOT WINRT) )  


(2).Change elseif (Win32) to elseif (Win32 or Android) as follows.  
if(WITH_FFMPEG OR HAVE_FFMPEG)  
    if(OPENCV_FFMPEG_USE_FIND_PACKAGE)  
        status("    FFMPEG:"       HAVE_FFMPEG         THEN "YES (find_package)"                       ELSE "NO (find_package)")  
    #elseif(WIN32)  
    elseif(WIN32 OR ANDROID)  
        status("    FFMPEG:"       HAVE_FFMPEG         THEN "YES (prebuilt binaries)"                  ELSE NO)  
    else()  
        status("    FFMPEG:"       HAVE_FFMPEG         THEN YES ELSE NO)  
    endif()  
    status("      avcodec:"      FFMPEG_libavcodec_FOUND    THEN "YES (ver ${FFMPEG_libavcodec_VERSION})"    ELSE NO)  
    status("      avformat:"     FFMPEG_libavformat_FOUND   THEN "YES (ver ${FFMPEG_libavformat_VERSION})"   ELSE NO)  
    status("      avutil:"       FFMPEG_libavutil_FOUND     THEN "YES (ver ${FFMPEG_libavutil_VERSION})"     ELSE NO)  
    status("      swscale:"      FFMPEG_libswscale_FOUND    THEN "YES (ver ${FFMPEG_libswscale_VERSION})"    ELSE NO)  
    status("      avresample:"   FFMPEG_libavresample_FOUND THEN "YES (ver ${FFMPEG_libavresample_VERSION})" ELSE NO)  
endif()  

2.Modify cmake/OpenCVFindLibsVideo.cmake file in opencv root directory.  

Add the following part to FFMPEG Section as below and replace FFMPEG_DIR with your own FFMPEG android library file directory.  

elseif(ANDROID)  
    set(HAVE_FFMPEG TRUE)  
    set(FFMPEG_DIR ${OpenCV_SOURCE_DIR}/../ffmpeg)  
    set(FFMPEG_INCLUDE_DIRS ${FFMPEG_DIR}/libs/${ANDROID_ABI}/include)  
    set(FFMPEG_LIBRARY_DIRS ${FFMPEG_DIR}/libs/${ANDROID_ABI}/lib)  
    set(FFMPEG_LIBRARIES avcodec avformat avutil swscale z)  
    message(STATUS "FFMPEG_INCLUDE_DIR: ${FFMPEG_INCLUDE_DIRS}")  
    message(STATUS "FFMPEG_LIBRARY_DIRS: ${FFMPEG_LIBRARY_DIRS}")  
    message(STATUS "FFMPEG_LIBRARIES: ${FFMPEG_LIBRARIES}")  


#--- FFMPEG ---  
ocv_clear_vars(HAVE_FFMPEG)  
if(WITH_FFMPEG)  # try FFmpeg autodetection  
    if(OPENCV_FFMPEG_USE_FIND_PACKAGE)  
        ...  
    elseif(WIN32 AND NOT ARM AND NOT OPENCV_FFMPEG_SKIP_DOWNLOAD)  
        ...  
    elseif(PKG_CONFIG_FOUND)  
        ...  
    elseif(ANDROID)  
        set(HAVE_FFMPEG TRUE)  
    set(FFMPEG_DIR ${OpenCV_SOURCE_DIR}/../ffmpeg)  
    set(FFMPEG_INCLUDE_DIRS ${FFMPEG_DIR}/libs/${ANDROID_ABI}/include)  
    set(FFMPEG_LIBRARY_DIRS ${FFMPEG_DIR}/libs/${ANDROID_ABI}/lib)  
    set(FFMPEG_LIBRARIES avcodec avformat avutil swscale z)  
    message(STATUS "FFMPEG_INCLUDE_DIR: ${FFMPEG_INCLUDE_DIRS}")  
    message(STATUS "FFMPEG_LIBRARY_DIRS: ${FFMPEG_LIBRARY_DIRS}")  
    message(STATUS "FFMPEG_LIBRARIES: ${FFMPEG_LIBRARIES}")  
    else()  
        ...  
    endif()  
endif()  

3. Use NDK to compile opencv  

Build command for arm64-v8a:  

cmake .. -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
-DBUILD_SHARED_LIBS=OFF \
-DANDROID_NDK="/home/public/android/AS/android-studio-ide/Ndk/android-ndk-r20b-linux-x86_64/android-ndk-r20b" \
-DANDROID_SDK="/home/public/android/AS/android-studio-ide/Sdk" \
-DCMAKE_TOOLCHAIN_FILE=/home/public/android/AS/android-studio-ide/Ndk/android-ndk-r20b-linux-x86_64/android-ndk-r20b/build/cmake/android.toolchain.cmake \
-DANDROID_NATIVE_API_LEVEL=22 \
-DANDROID_ABI=arm64-v8a \
-DANDROID_CPP_FEATURES="rtti exceptions" \
-DANDROID_ARM_NEON=TRUE \
-DANDROID_TOOLCHAIN=clang \
-DANDROID_STL=c++_shared \
-DANDROID_PLATFORM=android-22 \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX="../install_release/android/arm64-v8a/" \
-DBUILD_opencv_java=OFF \
-DBUILD_ANDROID_PROJECTS=ON \
-DBUILD_ANDROID_EXAMPLES=OFF \
-DBUILD_DOCS=OFF \
-DBUILD_PERF_TESTS=OFF \
-DBUILD_TESTS=OFF \
-DWITH_CUDA=OFF \
-DWITH_MATLAB=OFF \
-DCUDA_FAST_MATH=ON \
-DWITH_CUFFT=ON \
-DWITH_NVCUVID=ON \
-DWITH_V4L=ON \
-DWITH_LIBV4L=ON \
-DWITH_FFMPEG=ON
../

make  
make install   


Build command for armeabi-v7a:  

cmake .. -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
-DBUILD_SHARED_LIBS=OFF \
-DANDROID_NDK="/home/public/android/AS/android-studio-ide/Ndk/android-ndk-r20b-linux-x86_64/android-ndk-r20b" \
-DANDROID_SDK="/home/public/android/AS/android-studio-ide/Sdk" \
-DCMAKE_TOOLCHAIN_FILE=/home/public/android/AS/android-studio-ide/Ndk/android-ndk-r20b-linux-x86_64/android-ndk-r20b/build/cmake/android.toolchain.cmake \
-DANDROID_NATIVE_API_LEVEL=22 \
-DANDROID_ABI=armeabi-v7a \
-DANDROID_CPP_FEATURES="rtti exceptions" \
-DANDROID_ARM_NEON=TRUE \
-DANDROID_TOOLCHAIN=clang \
-DANDROID_STL=c++_shared \
-DANDROID_PLATFORM=android-22 \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX="../install_release/android/armeabi-v7a/" \
-DBUILD_opencv_java=OFF \
-DBUILD_ANDROID_PROJECTS=ON \
-DBUILD_ANDROID_EXAMPLES=OFF \
-DBUILD_DOCS=OFF \
-DBUILD_PERF_TESTS=OFF \
-DBUILD_TESTS=OFF \
-DWITH_CUDA=OFF \
-DWITH_MATLAB=OFF \
-DCUDA_FAST_MATH=ON \
-DWITH_CUFFT=ON \
-DWITH_NVCUVID=ON \
-DWITH_V4L=ON \
-DWITH_LIBV4L=ON \
-DWITH_FFMPEG=ON
../

make  
make install  
