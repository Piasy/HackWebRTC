cmake_minimum_required(VERSION 3.4.1)

set(CWD ${CMAKE_CURRENT_LIST_DIR})


add_library(restclient
            STATIC
            #SHARED
            IMPORTED
            )
set_target_properties( # Specifies the target library.
                       restclient

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${CWD}/libs/restclient-0.4.4/lib/armeabi-v7a/librestclient-cpp.a
                       #${CWD}/src/main/jniLibs/armeabi-v7a/librestclient-cpp.so
                       )
include_directories(${CWD}/libs/restclient-0.4.4/include)


include_directories(${CWD}/libs/zlib-1.2.11/include)
include_directories(${CWD}/libs/curl-7.55.1/include)
include_directories(${CWD}/libs/webrtc/include)


add_definitions(-DWEBRTC_POSIX)

add_library(hack_webrtc SHARED
            src/main/cpp/hack_webrtc.cpp
            src/main/cpp/async_rest_client.cpp
            )

# Include libraries needed for hack_webrtc lib
target_link_libraries(hack_webrtc
                      android
                      log

                      ${CWD}/libs/webrtc/lib/armeabi-v7a/libwebrtc.a

                      restclient
                      ${CWD}/src/main/jniLibs/armeabi-v7a/libcurl.so
                      ${CWD}/src/main/jniLibs/armeabi-v7a/libz.so
                      )
