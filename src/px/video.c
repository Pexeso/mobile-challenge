/* Copyright 2021 Pexeso Inc. All rights reserved. */

#include <libavformat/avformat.h>
#include <libavutil/mem.h>
#include <libavutil/opt.h>

#include <string.h>

#include "px/video_internal.h"
#include "px/frame_internal.h"

static int InitVideoStream(PX_Video* video) {
  int ret;
  AVStream* stream;
  AVCodec* dec;

  ret = avformat_find_stream_info(video->fmt_ctx, NULL);
  if (ret < 0) {
    return ret;
  }

  ret = av_find_best_stream(video->fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
  if (ret < 0) {
    return ret;
  }
  stream = video->fmt_ctx->streams[ret];

  video->dec_ctx = avcodec_alloc_context3(dec);
  if (!video->dec_ctx) {
    return AVERROR(ENOMEM);
  }
  avcodec_parameters_to_context(video->dec_ctx, stream->codecpar);

  ret = avcodec_open2(video->dec_ctx, dec, NULL);
  if (ret < 0) {
    return ret;
  }

  video->stream_index = stream->index;
  return 0;
}

static int InitVideo(PX_Video* video, const char* path) {
  int ret;

  if (!(video->fmt_ctx = avformat_alloc_context())) {
    return AVERROR(ENOMEM);
  } else if (!video->fmt_ctx->av_class) {
    return AVERROR(EINVAL);
  }

  if (!(video->frame = av_frame_alloc())) {
    return AVERROR(ENOMEM);
  }
  if (!(video->packet = (AVPacket*)av_malloc(sizeof(AVPacket)))) {
    return AVERROR(ENOMEM);
  }

  av_init_packet(video->packet);

  ret = avformat_open_input(&video->fmt_ctx, path, NULL, NULL);
  if (ret < 0) {
    return ret;
  }

  ret = InitVideoStream(video);
  if (ret < 0) {
    return ret;
  }

  switch (video->dec_ctx->pix_fmt) {
    case AV_PIX_FMT_YUV410P:
    case AV_PIX_FMT_YUV411P:
    case AV_PIX_FMT_YUV420P:
    case AV_PIX_FMT_YUV422P:
    case AV_PIX_FMT_YUV440P:
    case AV_PIX_FMT_YUV444P:
    case AV_PIX_FMT_YUVA420P:
    case AV_PIX_FMT_YUVA422P:
    case AV_PIX_FMT_YUVA444P:
      /* These are fine. */
      break;
    default:
      av_log(NULL, AV_LOG_ERROR, "Unsupported pixel format\n");
      return AVERROR_INVALIDDATA;
  }

  video->time_base = av_q2d(video->fmt_ctx->streams[video->stream_index]->time_base);
  return 0;
}

static void FreeVideo(PX_Video* video) {
  avcodec_free_context(&video->dec_ctx);
  avformat_close_input(&video->fmt_ctx);

  av_frame_free(&video->frame);
  av_freep(&video->packet);
}

int PX_Video_New(PX_Video** video_p, const char* path) {
  // Set to null in case of an error.
  *video_p = NULL;

  PX_Video* video = (PX_Video*)av_mallocz(sizeof(PX_Video));
  if (!video) {
    return AVERROR(ENOMEM);
  }

  int ret = InitVideo(video, path);
  if (ret < 0) {
    FreeVideo(video);
    av_free(video);
    return ret;
  }

  *video_p = video;
  return 0;
}

void PX_Video_Free(PX_Video** video_p) {
  if (!video_p || !*video_p) {
    return;
  }

  FreeVideo(*video_p);
  av_freep(video_p);
}

int PX_Video_NextFrame(PX_Video* video, PX_Frame* frame) {
  int ret;
  bool terminate = false;

  if (!video) {
    return AVERROR(EINVAL);
  }

  for (;;) {
    for (;;) {
      ret = avcodec_receive_frame(video->dec_ctx, video->frame);
      if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
        /* No more frames in the decoding queue, need to feed some more packets. */
        break;
      } else if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to read the decoder\n");
        return ret;
      }

      video->frame->pts = video->frame->best_effort_timestamp;
      frame->timestamp = video->frame->pts * video->time_base;

      av_frame_unref(frame->av_frame);
      av_frame_move_ref(frame->av_frame, video->frame);
      return 1;
    }

    /* We terminate the loop here so that we're not reading more packets
     * anymore but we still process the once in the decoder queue. */
    if (terminate) {
      break;
    }

    ret = av_read_frame(video->fmt_ctx, video->packet);
    if (ret < 0) {
      /* Send flush packet instructing decoder to return buffered frames. */
      avcodec_send_packet(video->dec_ctx, NULL);
      terminate = true;
      continue;
    }

    /* We're only interested in the packet if it contains video data. */
    if (video->packet->stream_index == video->stream_index) {
      /* And only if it's a keyframe. */
      if (video->packet->flags & AV_PKT_FLAG_KEY) {
        ret = avcodec_send_packet(video->dec_ctx, video->packet);
        /* Don't want to check the ret value right away because we want to
         * unref the packet first, even in case of a failure. */
      }
    }

    av_packet_unref(video->packet);

    if (ret < 0) {
      av_log(NULL, AV_LOG_ERROR, "Failed to feed the decoder\n");
      return ret;
    }
  }

  return 0;
}
