//
// Created by Piasy on 03/09/2017.
//

#ifndef TRY_WEBRTC_ASYNCRESTCLIENT_H
#define TRY_WEBRTC_ASYNCRESTCLIENT_H

#include <string>

#include <android/log.h>

#include <rtc_base/logging.h>
#include <rtc_base/thread.h>
#include <rtc_base/asyncinvoker.h>

class AsyncRestClientCallback;

class AsyncRestClient {
public:
    AsyncRestClient();

    ~AsyncRestClient();

    void get(const std::string& url, AsyncRestClientCallback* callback);

private:
    void invokeGet(const std::string& url, AsyncRestClientCallback* callback);
    void notifySuccess(AsyncRestClientCallback* callback, int code, const std::string& body);
    void notifyError(AsyncRestClientCallback* callback, int code);

    std::unique_ptr<rtc::Thread> signal_thread;
    std::unique_ptr<rtc::Thread> network_thread;
    std::unique_ptr<rtc::AsyncInvoker> invoker;
};

class AsyncRestClientCallback {
public:
    AsyncRestClientCallback(jobject callback_);

    ~AsyncRestClientCallback();

    void onSuccess(int code, const std::string& body);

    void onError(int code);

private:
    jobject callback;
    jmethodID on_error;
    jmethodID on_success;
};

#endif //TRY_WEBRTC_ASYNCRESTCLIENT_H
