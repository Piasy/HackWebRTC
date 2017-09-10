//
// Created by Piasy on 03/09/2017.
//

#include <restclient-cpp/restclient.h>
#include <webrtc/rtc_base/bind.h>
#include <webrtc/modules/utility/include/helpers_android.h>

#include "async_rest_client.h"

static JNIEnv *jni() {
    return webrtc_jni::AttachCurrentThreadIfNeeded();
}

AsyncRestClient::AsyncRestClient() {
    signal_thread = rtc::Thread::Create();
    signal_thread->Start();
    network_thread = rtc::Thread::Create();
    network_thread->Start();
    invoker = new rtc::AsyncInvoker();
}

AsyncRestClient::~AsyncRestClient() {
    signal_thread->Stop();
    signal_thread.release();
    network_thread->Stop();
    network_thread.release();
    delete (invoker);
}

void AsyncRestClient::get(const std::string &url, AsyncRestClientCallback *callback) {
    invoker->AsyncInvoke<void>(RTC_FROM_HERE, network_thread.get(),
                         rtc::Bind(&AsyncRestClient::invokeGet, this, url, callback));
}

void AsyncRestClient::invokeGet(const std::string &url, AsyncRestClientCallback *callback) {
    LOG(LS_INFO) << "get " << url;

    RestClient::Response response = RestClient::get(url);
    if (200 <= response.code && response.code <= 300) {
        invoker->AsyncInvoke<void>(RTC_FROM_HERE, signal_thread.get(),
                                   rtc::Bind(&AsyncRestClientCallback::onSuccess,
                                             callback, response.code, response.body));
    } else {
        invoker->AsyncInvoke<void>(RTC_FROM_HERE, signal_thread.get(),
                                   rtc::Bind(&AsyncRestClientCallback::onError,
                                             callback, response.code));
    }
}

AsyncRestClientCallback::AsyncRestClientCallback(jobject callback_)
        : callback(jni()->NewGlobalRef(callback_)) {
    jclass callback_class = webrtc::FindClass(jni(),
                                              "com/github/piasy/hack_webrtc/AsyncRestClient$Callback");
    on_error = webrtc::GetMethodID(jni(), callback_class, "onError", "(I)V");
    on_success = webrtc::GetMethodID(jni(), callback_class, "onSuccess", "(ILjava/lang/String;)V");
}

AsyncRestClientCallback::~AsyncRestClientCallback() {
    jni()->DeleteGlobalRef(callback);
}

void AsyncRestClientCallback::onSuccess(int code, const std::string &body) {
    jni()->CallVoidMethod(callback, on_success, code, jni()->NewStringUTF(body.c_str()));
}

void AsyncRestClientCallback::onError(int code) {
    LOG(LS_INFO) << "AsyncRestClientCallback::onError " << code;
    jni()->CallVoidMethod(callback, on_error, code);
}
