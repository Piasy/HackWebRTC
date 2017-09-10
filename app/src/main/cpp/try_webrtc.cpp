//
// Created by Piasy on 02/09/2017.
//

#include <jni.h>
#include <restclient-cpp/restclient.h>
#include "async_rest_client.h"

static bool s_jvm_initialized = false;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_github_piasy_try_1webrtc_AsyncRestClient_nativeInit(JNIEnv *env, jclass type) {
    if (!s_jvm_initialized) {
        s_jvm_initialized = true;
        JavaVM *jvm = nullptr;
        env->GetJavaVM(&jvm);
        webrtc_jni::InitGlobalJniVariables(jvm);
    }
    rtc::ThreadManager::Instance()->WrapCurrentThread();

    return webrtc_jni::jlongFromPointer(new AsyncRestClient());
}

JNIEXPORT void JNICALL
Java_com_github_piasy_try_1webrtc_AsyncRestClient_nativeGet(JNIEnv *env, jclass type,
                                                             jlong nativeClient, jstring url_,
                                                             jobject callback) {
    const char *url = env->GetStringUTFChars(url_, 0);

    AsyncRestClient *client = AsyncRestClient::fromJ(nativeClient);
    client->get(url, new AsyncRestClientCallback(callback));

    env->ReleaseStringUTFChars(url_, url);
}

JNIEXPORT void JNICALL
Java_com_github_piasy_try_1webrtc_AsyncRestClient_nativeDestroy(JNIEnv *env, jclass type,
                                                                jlong nativeClient) {
    delete (AsyncRestClient::fromJ(nativeClient));
}

}