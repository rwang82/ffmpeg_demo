# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
# TODO 设置构建本机库文件所需的 CMake的最小版本
cmake_minimum_required(VERSION 3.4.1)

# TODO 设置变量，方便底下使用
set(INC_DIR ${PROJECT_SOURCE_DIR}/libs/include)
set(LINK_DIR ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})


# TODO 添加so库对应的头文件目录
include_directories(${INC_DIR})

# TODO 引入so库,IMPORT代表从第三方引入的意思
add_library( avcodec SHARED IMPORTED)
# TODO 设置编译的库文件存放的目录
set_target_properties( avcodec PROPERTIES IMPORTED_LOCATION ${LINK_DIR}/libavcodec.so)

add_library( avfilter SHARED IMPORTED)
set_target_properties( avfilter PROPERTIES IMPORTED_LOCATION ${LINK_DIR}/libavfilter.so)

add_library( avformat SHARED IMPORTED)
set_target_properties( avformat PROPERTIES IMPORTED_LOCATION ${LINK_DIR}/libavformat.so)

add_library( avutil SHARED IMPORTED)
set_target_properties( avutil PROPERTIES IMPORTED_LOCATION ${LINK_DIR}/libavutil.so)

add_library( swresample SHARED IMPORTED)
set_target_properties( swresample PROPERTIES IMPORTED_LOCATION ${LINK_DIR}/libswresample.so)

add_library( swscale SHARED IMPORTED)
set_target_properties( swscale PROPERTIES IMPORTED_LOCATION ${LINK_DIR}/libswscale.so)



# TODO 添加自己写的 C/C++源文件
add_library( Hello SHARED
        src/main/jni/com_daskks_ffmpeg_jniproxy_FFmpegJni.cpp)

# TODO 依赖 NDK中的库
find_library( log-lib
        log )

# TODO 将目标库与 NDK中的库进行连接
target_link_libraries( Hello
        avcodec
        avfilter
        avformat
        avutil
        swresample
        swscale
        ${log-lib} )