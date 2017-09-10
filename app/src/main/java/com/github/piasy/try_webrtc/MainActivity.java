package com.github.piasy.try_webrtc;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("curl");
        System.loadLibrary("z");
        System.loadLibrary("try_webrtc");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        findViewById(R.id.mBtnGet).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(final View v) {
                startPost();
            }
        });
    }

    private void startPost() {
        new Thread() {
            @Override
            public void run() {
                directGet("ip.gs");
            }
        }.start();
    }

    private native void directGet(String url);
}
