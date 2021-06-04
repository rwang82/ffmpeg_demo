package com.daskks.ffmpeg.helper;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.ContentUris;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.provider.DocumentsContract;
import android.provider.MediaStore;

public class FileUtils {
    @TargetApi(Build.VERSION_CODES.KITKAT)
    public static String getPath(Activity act, Uri uri) {
        /**
         * uri=content://com.android.providers.media.documents/document/video%3A293502  4.4以后
         * uri=file:///storage/emulated/0/temp_photo.jpg
         * uri=content://media/external/videos/media/193968
         *
         * uri=content://media/external/videos/media/13   4.4以前
         */
        String path = null;
        if (DocumentsContract.isDocumentUri(act, uri)) {
            // 如果是document类型的Uri，则通过document id处理
            String docId = DocumentsContract.getDocumentId(uri);
            if ("com.android.providers.media.documents".equals(uri.getAuthority())) {
                String id = docId.split(":")[1]; // 解析出数字格式的id
                String selection = MediaStore.Video.Media._ID + "=" + id;
                path = getVideoPathFromUri(act, MediaStore.Video.Media.EXTERNAL_CONTENT_URI, selection);
            } else if ("com.android.providers.downloads.documents".equals(uri.getAuthority())) {
                Uri contentUri = ContentUris.withAppendedId(Uri.parse("content://downloads/public_downloads"), Long.valueOf(docId));
                path = getVideoPathFromUri(act, contentUri, null);
            }
        } else if ("content".equalsIgnoreCase(uri.getScheme())) {
            // 如果是content类型的Uri，则使用普通方式处理
            path = getVideoPathFromUri(act, uri, null);
        } else if ("file".equalsIgnoreCase(uri.getScheme())) {
            // 如果是file类型的Uri，直接获取图片路径即可
            path = uri.getPath();
        }
        return path;
    }

    /**
     * 通过Uri和selection来获取真实的Video路径
     * @param act
     * @param uri
     * @param selection
     * @return
     */
    private static String getVideoPathFromUri(Activity act, Uri uri, String selection) {
        String path = null;
        String[] projection = { MediaStore.Video.Media.DATA };
        Cursor cursor = act.getContentResolver().query(uri, projection, selection,null,null);
        if(cursor != null){
            if(cursor.moveToFirst()){
                path = cursor.getString(cursor.getColumnIndex(MediaStore.Video.Media.DATA));
            }
            cursor.close();
        }
        return path;
    }
}
