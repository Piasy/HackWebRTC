package com.github.piasy.hack_webrtc;

import android.os.Handler;
import android.os.Looper;

/**
 * Created by Piasy{github.com/Piasy} on 03/09/2017.
 */

public class AsyncRestClient {

    private static final Handler UI_HANDLER = new Handler(Looper.getMainLooper());

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("z");
        System.loadLibrary("curl");
        //System.loadLibrary("restclient-cpp");
        System.loadLibrary("hack_webrtc");
    }

    private long mNativeClient;

    private static native long nativeInit();

    private static native void nativeGet(long nativeClient, String url, Callback callback);

    private static native void nativeDestroy(long nativeClient);

    public synchronized void init() {
        mNativeClient = nativeInit();
    }

    public synchronized void get(String url, final Callback callback) {
        if (mNativeClient != 0) {
            nativeGet(mNativeClient, url, new Callback() {
                @Override
                public void onSuccess(final int code, final String responseBody) {
                    UI_HANDLER.post(new Runnable() {
                        @Override
                        public void run() {
                            callback.onSuccess(code, responseBody);
                        }
                    });
                }

                @Override
                public void onError(final int code) {
                    UI_HANDLER.post(new Runnable() {
                        @Override
                        public void run() {
                            callback.onError(code);
                        }
                    });
                }
            });
        }
    }

    public synchronized void destroy() {
        if (mNativeClient != 0) {
            mNativeClient = 0;
            nativeDestroy(mNativeClient);
        }
    }

    public interface Callback {
        void onSuccess(int code, String body);

        void onError(int code);
    }
}
