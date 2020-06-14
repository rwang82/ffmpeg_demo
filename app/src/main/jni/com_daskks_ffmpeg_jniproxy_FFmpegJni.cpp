#include "com_daskks_ffmpeg_jniproxy_FFmpegJni.h"
/*
 * Class:     com_daskks_ffmpeg_jniproxy_FFmpegJni
 * Method:    onInit
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_daskks_ffmpeg_jniproxy_FFmpegJni_onInit
  (JNIEnv *, jclass, jstring, jstring) {
  }

/*
 * Class:     com_daskks_ffmpeg_jniproxy_FFmpegJni
 * Method:    onUnInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_daskks_ffmpeg_jniproxy_FFmpegJni_onUnInit
  (JNIEnv *, jclass) {
  }


JNIEXPORT void JNICALL
Java_com_daskks_ffmpeg_MainActivity_video_1decode(JNIEnv *env, jobject instance, jstring input_,
                                                  jstring output_) {
  const char *input = env->GetStringUTFChars(input_, 0);
  const char *output = env->GetStringUTFChars(output_, 0);

  // TODO

  env->ReleaseStringUTFChars(input_, input);
  env->ReleaseStringUTFChars(output_, output);
}
