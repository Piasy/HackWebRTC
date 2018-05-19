//
// Created by Piasy on 02/09/2017.
//

#include <jni.h>

#include <sdk/android/src/jni/jni_helpers.h>

#include "async_rest_client.h"

#define toJ(handle) webrtc::jni::jlongFromPointer((handle))
#define fromJ(type, handle) reinterpret_cast<type *>((handle))

static bool s_jvm_initialized = false;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_github_piasy_hack_1webrtc_rest_1client_AsyncRestClient_nativeInit(
        JNIEnv* env, jclass type) {
    if (!s_jvm_initialized) {
        s_jvm_initialized = true;
        JavaVM* jvm = nullptr;
        env->GetJavaVM(&jvm);
        webrtc_jni::InitGlobalJniVariables(jvm);
    }
    rtc::ThreadManager::Instance()->WrapCurrentThread();

    return toJ(new AsyncRestClient());
}

JNIEXPORT void JNICALL
Java_com_github_piasy_hack_1webrtc_rest_1client_AsyncRestClient_nativeGet(
        JNIEnv* env, jclass type, jlong nativeClient, jstring url_, jobject callback) {
    const char* url = env->GetStringUTFChars(url_, 0);

    std::unique_ptr<AsyncRestClientCallback> test
            = std::make_unique<AsyncRestClientCallback>(callback);

    AsyncRestClient* client = fromJ(AsyncRestClient, nativeClient);
    client->get(url, new AsyncRestClientCallback(callback));

    env->ReleaseStringUTFChars(url_, url);
}

JNIEXPORT void JNICALL
Java_com_github_piasy_hack_1webrtc_rest_1client_AsyncRestClient_nativeDestroy(
        JNIEnv* env, jclass type, jlong nativeClient) {
    delete fromJ(AsyncRestClient, nativeClient);
}

}