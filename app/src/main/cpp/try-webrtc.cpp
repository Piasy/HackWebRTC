//
// Created by Piasy on 02/09/2017.
//

#include <jni.h>
#include <android/log.h>
#include "webrtc/rtc_base/thread.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "TRY_WEBRTC", ##__VA_ARGS__)

struct TestFunctor {
    void operator()() {
        LOGI("TestFunctor run");
    }
};

extern "C" {

JNIEXPORT void JNICALL
Java_com_github_piasy_try_1webrtc_MainActivity_testWebrtc(JNIEnv *env, jclass type) {
    LOGI("test start");

    auto thread = rtc::Thread::Create();
    thread->Start();

    thread->Invoke<void, TestFunctor>(RTC_FROM_HERE, TestFunctor());

    thread->Stop();

    LOGI("test end");
}

}
