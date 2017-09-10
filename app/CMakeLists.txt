cmake_minimum_required(VERSION 3.4.1)

set(CWD ${CMAKE_CURRENT_LIST_DIR})


add_library(restclient
            STATIC
            IMPORTED
            )
set_target_properties( # Specifies the target library.
                       restclient

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${CWD}/libs/restclient-0.4.4/lib/librestclient-cpp.a
                       )
include_directories(${CWD}/libs/restclient-0.4.4/include)


include_directories(${CWD}/libs/zlib-1.2.11/include)
include_directories(${CWD}/libs/curl-7.55.1/include)


add_library(try_webrtc SHARED
            src/main/cpp/try_webrtc.cpp
            )

# Include libraries needed for try-webrtc lib
target_link_libraries(try_webrtc
                      android
                      log

                      restclient
                      ${CWD}/libs/curl-7.55.1/lib/libcurl.a
                      ${CWD}/libs/zlib-1.2.11/lib/libz.a
                      )
