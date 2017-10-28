package com.github.piasy.hack_webrtc;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.nodes.Node;
import org.jsoup.select.Elements;

public class MainActivity extends AppCompatActivity {

    private TextView mTvResult;
    private AsyncRestClient mAsyncRestClient;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        mTvResult = findViewById(R.id.mTvResult);

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
                try {
                    showResult(body);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onError(final int code) {
                Toast.makeText(MainActivity.this,
                        "onError<" + hashCode() + "> " + code, Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void showResult(String body) throws Exception {
        StringBuilder result = new StringBuilder();
        Document document = Jsoup.parse(body);
        for (Node node : document.childNodes()) {
            if (node.nodeName().equals("#comment")) {
                String comment = node.toString();
                Document hero = Jsoup.parse(comment.substring(5, comment.length() - 3));
                Elements heroUnit = hero.getElementsByClass("hero-unit");
                for (Element p : heroUnit.get(0).getElementsByTag("p")) {
                    result.append(p.text())
                            .append("\n");
                }
            }
        }

        mTvResult.setText(result);
    }
}
