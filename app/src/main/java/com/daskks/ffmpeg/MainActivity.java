package com.daskks.ffmpeg;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.daskks.ffmpeg.jniproxy.FFmpegJni;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("Hello");
        System.loadLibrary("avcodec");
        System.loadLibrary("avdevice");
        System.loadLibrary("avfilter");
        System.loadLibrary("avformat");
        System.loadLibrary("avutil");
        System.loadLibrary("postproc");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        FFmpegJni.onInit("", "");

        findViewById(R.id.tv_decode).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.i("ROM_DEBUG", "devode clicked.");
                String input = "/sdcard/test.mp4";
                video_decode(input, "output");
            }
        });
    }

    public native void video_decode(String input, String output);
}
