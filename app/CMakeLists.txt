cmake_minimum_required(VERSION 3.4.1)

set(CWD ${CMAKE_CURRENT_LIST_DIR})

add_library(try-webrtc SHARED
            src/main/cpp/try-webrtc.cpp
            )

include_directories(libs/webrtc/include)
add_definitions(-DWEBRTC_POSIX)

# Include libraries needed for try-webrtc lib
target_link_libraries(try-webrtc
                      android
                      log
                      ${CWD}/libs/webrtc/libwebrtc.a
                      )
