extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/frame.h>
}
#include "com_daskks_ffmpeg_jniproxy_FFmpegJni.h"
#include "NLog.h"

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
    LOGD("asdlfjalsd;fa");
    const char *input = env->GetStringUTFChars(input_, 0);
    const char *output = env->GetStringUTFChars(output_, 0);

    // TODO
    LOGI("Enter video_decode. in:%s, out:%s", input, output);

    AVCodecParameters* param4AVCodec = NULL;
    AVFormatContext *pFmtCtx = NULL;
    int i, videoStream;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVCodecID codecId = AV_CODEC_ID_NONE;
    AVFrame *pFrame = NULL;
    AVFrame *pFrameRGB = NULL;
    AVPacket packet;
    int frameFinished;
    int numBytes;
    uint8_t *buffer;

    // 打开文件.
    if (avformat_open_input(&pFmtCtx, input, NULL, NULL) != 0) {
        return;
    }
    //
    if (avformat_find_stream_info(pFmtCtx, NULL) < 0) {
        return;
    }
    //
    av_dump_format(pFmtCtx, -1, input, 0);
    //
    videoStream = -1;
    for (i = 0; i<pFmtCtx->nb_streams; i++) {
        if (pFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }
    if (videoStream == -1) {
        return;
    }
    param4AVCodec = pFmtCtx->streams[videoStream]->codecpar;
    if (param4AVCodec == NULL) {
        return;
    }
    codecId = param4AVCodec->codec_id;
    pCodec = avcodec_find_decoder(codecId);
    if (pCodec == NULL) {
        return;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (pCodecCtx == NULL) {
        return;
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL)) {
        return;
    }
    // 分配图像缓存
    pFrame = av_frame_alloc();
    if (pFrame == NULL) {
        return;
    }
    pFrameRGB = av_frame_alloc();
    if (pFrameRGB == NULL) {
        return;
    }
    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, param4AVCodec->width, param4AVCodec->height, 1);
    buffer = (uint8_t*)av_malloc(numBytes);

//    avpicture_fill((AVPicture*)pFrameRGB, buffer, AV_PIX_FMT_RGB24, param4AVCodec->width, param4AVCodec->height);
    int nErrCode  = av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer, AV_PIX_FMT_RGB24, param4AVCodec->width, param4AVCodec->height, 1);
    if (nErrCode < 0) {
        LOGE("av_image_fill_arrays failed. errcode:%d", nErrCode);
    }
    //获取图像
//    i = 0;
//    while( av_read_frame(pFmtCtx, &packet) >= 0 ) {
//        if( packet.stream_index == videoStream ) {
//            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
//
//            if( frameFinished ) {
//                struct SwsContext *img_convert_ctx = NULL;
//                img_convert_ctx =
//                        sws_getCachedContext(img_convert_ctx, pCodecCtx->width,
//                                             pCodecCtx->height, pCodecCtx->pix_fmt,
//                                             pCodecCtx->width, pCodecCtx->height,
//                                             PIX_FMT_RGB24, SWS_BICUBIC,
//                                             NULL, NULL, NULL);
//                if( !img_convert_ctx ) {
//                    fprintf(stderr, "Cannot initialize sws conversion context\n");
//                    exit(1);
//                }
//                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data,
//                          pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
//                          pFrameRGB->linesize);
//                if( i++ < 50 )
//                    SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
//            }
//        }
//        av_free_packet(&packet);
//    }

    av_free(buffer);
    av_frame_free(&pFrameRGB);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFmtCtx);

    //
    env->ReleaseStringUTFChars(input_, input);
    env->ReleaseStringUTFChars(output_, output);
}
static void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame)
{
    FILE *pFile;
    char szFilename[32];
    int y;

    sprintf(szFilename, "frame%d.ppm", iFrame);
    pFile = fopen(szFilename, "wb");
    if( !pFile )
        return;
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    for( y = 0; y < height; y++ )
        fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);

    fclose(pFile);
}



