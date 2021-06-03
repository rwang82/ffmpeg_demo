extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/frame.h>
}

#include <libswscale/swscale.h>
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

static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame);
static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);

JNIEXPORT void JNICALL
Java_com_daskks_ffmpeg_MainActivity_video_1decode(JNIEnv *env, jobject instance, jstring input_,
                                                  jstring output_) {
    LOGD("ROM_DEBUG;fa");
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
    AVPacket* pPacket = NULL;
    int frameFinished;
    int numBytes;
    uint8_t *buffer;
    int resp = 0;

    // 打开文件.
    pFmtCtx = avformat_alloc_context();
    if (!pFmtCtx) {
        LOGE("[failed] avformat_alloc_context NULL");
        return;
    }

    resp = avformat_open_input(&pFmtCtx, input, NULL, NULL);
    if (resp != 0) {
        LOGE("[failed][%d] avformat_open_input", resp);
        return;
    }
    LOGI("long_name :%s, duration :%lld us", pFmtCtx->iformat->long_name, pFmtCtx->duration);
    //
    if (avformat_find_stream_info(pFmtCtx, NULL) < 0) {
        LOGE("[failed] avformat_find_stream_info");
        return;
    }
    //
    av_dump_format(pFmtCtx, -1, input, 0);
    //
    AVCodecParameters *pCodecParameters =  NULL;
    videoStream = -1;
    for (i = 0; i<pFmtCtx->nb_streams; i++) {
        AVCodecParameters *pLocalCodecParameters =  NULL;
        pLocalCodecParameters = pFmtCtx->streams[i]->codecpar;

        LOGI("codecpar type:%d", (int)pLocalCodecParameters->codec_type);
        LOGI("AVStream->time_base before open coded %d/%d", pFmtCtx->streams[i]->time_base.num, pFmtCtx->streams[i]->time_base.den);
        LOGI("AVStream->r_frame_rate before open coded %d/%d", pFmtCtx->streams[i]->r_frame_rate.num, pFmtCtx->streams[i]->r_frame_rate.den);
        LOGI("AVStream->start_time %" PRId64, pFmtCtx->streams[i]->start_time);
        LOGI("AVStream->duration %" PRId64, pFmtCtx->streams[i]->duration);

        if (pFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }
    if (videoStream == -1) {
        LOGE("no video stream. so exit.");
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
    if (pCodec->long_name != NULL) {
        LOGI("codec:%s, id:%d", pCodec->long_name, (int)pCodec->id);
    } else {
        LOGI("pCodec->long_name == NULL");
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (pCodecCtx == NULL) {
        return;
    }
    // 新版本的 FFmpeg 需要这个函数把音视频流信息拷贝到新的ACCodecContext结构体中.
    if ( avcodec_parameters_to_context(pCodecCtx, param4AVCodec) < 0) {
        LOGE("[failed] avcodec_parameters_to_context");
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

    // https://ffmpeg.org/doxygen/trunk/structAVPacket.html
    pPacket = av_packet_alloc();
    if (!pPacket) {
        LOGI("failed to allocated memory for AVPacket");
        return;
    }
    int response = 0;
    int how_many_packets_to_process = 8;
    //获取图像
    i = 0;
    while( av_read_frame(pFmtCtx, pPacket) >= 0 ) {
        if( pPacket->stream_index == videoStream ) {
            LOGI("start to decode... %" PRId64, pPacket->pts);

            response = decode_packet(pPacket, pCodecCtx, pFrame);
            if (response < 0)
                break;
            if (--how_many_packets_to_process <= 0) break;

            // https://ffmpeg.org/doxygen/trunk/group__lavc__packet.html#ga63d5a489b419bd5d45cfd09091cbcbc2
            av_packet_unref(pPacket);
        }
    }
    av_packet_free(&pPacket);

    av_free(buffer);
    av_frame_free(&pFrameRGB);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFmtCtx);

    //
    env->ReleaseStringUTFChars(input_, input);
    env->ReleaseStringUTFChars(output_, output);
}


static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame)
{
    // Supply raw packet data as input to a decoder
    // https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3
    int response = avcodec_send_packet(pCodecContext, pPacket);

    if (response < 0) {
        LOGE("Error while sending a packet to the decoder: %s", av_err2str(response));
        return response;
    }

    while (response >= 0)
    {
        // Return decoded output data (into a frame) from a decoder
        // https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c
        response = avcodec_receive_frame(pCodecContext, pFrame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            break;
        } else if (response < 0) {
            LOGE("Error while receiving a frame from the decoder: %s", av_err2str(response));
            return response;
        }

        if (response >= 0) {
            LOGI(
                    "Frame %d (type=%c, size=%d bytes, format=%d) pts %d key_frame %d [DTS %d]",
                    pCodecContext->frame_number,
                    av_get_picture_type_char(pFrame->pict_type),
                    pFrame->pkt_size,
                    pFrame->format,
                    pFrame->pts,
                    pFrame->key_frame,
                    pFrame->coded_picture_number
            );

            char frame_filename[1024];
            snprintf(frame_filename, sizeof(frame_filename), "/sdcard/%s-%d.pgm", "frame", pCodecContext->frame_number);
            // Check if the frame is a planar YUV 4:2:0, 12bpp
            // That is the format of the provided .mp4 file
            // RGB formats will definitely not give a gray image
            // Other YUV image may do so, but untested, so give a warning
            if (pFrame->format != AV_PIX_FMT_YUV420P)
            {
                LOGE("Warning: the generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB");
            }
            // save a grayscale frame into a .pgm file
            save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, frame_filename);
        }
    }
    return 0;
}

static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename)
{
    FILE *f;
    int i;
    f = fopen(filename,"w");
    if (f == NULL) {
        LOGE("fopen %s failed.", filename);
        return;
    }
    // writing the minimal required header for a pgm file format
    // portable graymap format -> https://en.wikipedia.org/wiki/Netpbm_format#PGM_example
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

    // writing line by line
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}


