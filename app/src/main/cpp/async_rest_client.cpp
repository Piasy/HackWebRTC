//
// Created by Piasy on 03/09/2017.
//

#include <restclient-cpp/restclient.h>
#include <rtc_base/bind.h>
#include <modules/utility/include/helpers_android.h>
#include <sdk/android/src/jni/jni_helpers.h>

#include "async_rest_client.h"

static JNIEnv* jni() {
    return webrtc_jni::AttachCurrentThreadIfNeeded();
}

AsyncRestClient::AsyncRestClient()
        : signal_thread(rtc::Thread::Create()),
          network_thread(rtc::Thread::Create()),
          invoker(std::make_unique<rtc::AsyncInvoker>()) {
    signal_thread->Start();
    network_thread->Start();
}

AsyncRestClient::~AsyncRestClient() {
    // no need to call stop, ~Thread will call stop automatically.
    //signal_thread->Stop();
}

void AsyncRestClient::get(const std::string& url, AsyncRestClientCallback* callback) {
    invoker->AsyncInvoke<void>(RTC_FROM_HERE, network_thread.get(),
                               rtc::Bind(&AsyncRestClient::invokeGet, this, url, callback));
}

void AsyncRestClient::invokeGet(const std::string& url, AsyncRestClientCallback* callback) {
    LOG(LS_INFO) << "get " << url;

    RestClient::Response response = RestClient::get(url);
    if (200 <= response.code && response.code <= 300) {
        invoker->AsyncInvoke<void>(RTC_FROM_HERE, signal_thread.get(),
                                   rtc::Bind(&AsyncRestClient::notifySuccess, this,
                                             callback, response.code, response.body));
    } else {
        invoker->AsyncInvoke<void>(RTC_FROM_HERE, signal_thread.get(),
                                   rtc::Bind(&AsyncRestClient::notifyError, this,
                                             callback, response.code));
    }
}

void AsyncRestClient::notifySuccess(AsyncRestClientCallback* callback, int code,
                                    const std::string& body) {
    callback->onSuccess(code, body);
    delete callback;
}

void AsyncRestClient::notifyError(AsyncRestClientCallback* callback, int code) {
    callback->onError(code);
    delete callback;
}

AsyncRestClientCallback::AsyncRestClientCallback(jobject callback_)
        : callback(jni()->NewGlobalRef(callback_)) {
    LOG(LS_INFO) << "AsyncRestClientCallback " << (long) this;
    jclass callback_class = webrtc::FindClass(
            jni(), "com/github/piasy/hack_webrtc/rest_client/AsyncRestClient$Callback");
    on_error = webrtc::GetMethodID(jni(), callback_class, "onError", "(I)V");
    on_success = webrtc::GetMethodID(jni(), callback_class, "onSuccess", "(ILjava/lang/String;)V");
}

AsyncRestClientCallback::~AsyncRestClientCallback() {
    jni()->DeleteGlobalRef(callback);
    LOG(LS_INFO) << "~AsyncRestClientCallback " << (long) this;
}

void AsyncRestClientCallback::onSuccess(int code, const std::string& body) {
    jni()->CallVoidMethod(callback, on_success, code, jni()->NewStringUTF(body.c_str()));
}

void AsyncRestClientCallback::onError(int code) {
    LOG(LS_INFO) << "AsyncRestClientCallback::onError " << code;
    jni()->CallVoidMethod(callback, on_error, code);
}
