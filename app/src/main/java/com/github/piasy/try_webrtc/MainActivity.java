package com.github.piasy.try_webrtc;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    private AsyncRestClient mAsyncRestClient;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        mAsyncRestClient = new AsyncRestClient();
        mAsyncRestClient.init();

        findViewById(R.id.mBtnGet).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(final View v) {
                startGet();
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        mAsyncRestClient.destroy();
    }

    private void startGet() {
        mAsyncRestClient.get("ip.gs", new AsyncRestClient.Callback() {
            @Override
            public void onSuccess(final int code, final String body) {
                Log.d("REST_TEST", "onSuccess<" + hashCode() + "> " + code + " " + body
                                   + " @ " + Thread.currentThread().getName());
            }

            @Override
            public void onError(final int code) {
                Log.d("REST_TEST", "onError<" + hashCode() + "> " + code
                                   + " @ " + Thread.currentThread().getName());
            }
        });
    }
}
