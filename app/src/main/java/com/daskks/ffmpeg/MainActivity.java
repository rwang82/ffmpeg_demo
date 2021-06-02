package com.daskks.ffmpeg;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.daskks.ffmpeg.jniproxy.BuildCompat;
import com.daskks.ffmpeg.jniproxy.FFmpegJni;
import com.tbruyelle.rxpermissions.RxPermissions;

import java.io.File;

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
                File fDest = new File(input);
                if ((fDest == null) || !fDest.exists()) {
                    Log.e("ROM_DEBUG", "dest file not exist. path: " + input);
                    return;
                }
                video_decode(input, "output");
            }
        });

        requestExternalStoragePermission();
    }

    private void requestExternalStoragePermission() {

        if (BuildCompat.isR()) {
            new RxPermissions(this).request(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.MANAGE_EXTERNAL_STORAGE).subscribe(
                    grant -> {
                        File fEFD = getExternalFilesDir("");
                        Log.i("ROM_DEBUG", "[getExternalFilesDir]: " + fEFD.getAbsolutePath());
                        File fECD = getExternalCacheDir();
                        Log.i("ROM_DEBUG", "[getExternalCacheDir]: " + fECD.getAbsolutePath());
                        //

                    }
            );
        } else {
            new RxPermissions(this).request(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE).subscribe(
                    grant -> {
                        File fEFD = getExternalFilesDir("");
                        Log.i("ROM_DEBUG", "[getExternalFilesDir]: " + fEFD.getAbsolutePath());
                        File fECD = getExternalCacheDir();
                        Log.i("ROM_DEBUG", "[getExternalCacheDir]: " + fECD.getAbsolutePath());

                        if (grant) {
                            //InstallFactory.initInstaller(this, callback);
                        } else {
                            //showInstallError("请允许存储权限后重试！");
                        }
                    }
            );
        }
    }

    public native void video_decode(String input, String output);
}
