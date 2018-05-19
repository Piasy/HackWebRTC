# HackWebRTC

Let's hack into WebRTC :)

## Use WebRTC static library

+ https://blog.piasy.com/2017/09/03/Use-WebRTC-Static-Library/
+ Copy header files:

``` bash
# CP='gcp' # mac
# CP='cp'  # linux

find api audio base call common_audio common_video logging media \
modules ortc p2p pc rtc_base rtc_tools sdk stats system_wrappers \
video -name "*.h" \
-exec $CP --parents '{}' <target dir> ';'
find . -maxdepth 1 -name "*.h" -exec $CP --parents '{}' <target dir> ';'
```

Tips:

+ Due to the file size limit of GitHub, I ignore `libwebrtc.a`, please extract it and put it under `app/libs/webrtc/lib/armeabi-v7a/` before building.

## WebRTC classes and data flow

![](WebRTC_classes_23261.svg)
