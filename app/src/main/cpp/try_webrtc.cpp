//
// Created by Piasy on 02/09/2017.
//

#include <jni.h>
#include <android/log.h>
#include <restclient-cpp/restclient.h>

extern "C" {

JNIEXPORT void JNICALL
Java_com_github_piasy_try_1webrtc_MainActivity_directGet(JNIEnv *env, jobject instance,
                                                          jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    RestClient::Response response = RestClient::get(url);
    __android_log_print(ANDROID_LOG_INFO, "REST_CLIENT", "response %d %s", response.code,
                        response.body.c_str());

    env->ReleaseStringUTFChars(url_, url);
}

}
