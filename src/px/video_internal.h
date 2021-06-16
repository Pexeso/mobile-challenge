/* Copyright 2021 Pexeso Inc. All rights reserved. */

#ifndef PX_VIDEO_INTERNAL_H_
#define PX_VIDEO_INTERNAL_H_

#include <stddef.h>

#include <libavformat/avformat.h>

#include "px/video.h"

struct PX_Video {
  AVFormatContext* fmt_ctx;
  AVCodecContext* dec_ctx;
  int stream_index;
  double time_base;

  AVFrame* frame;
  AVPacket* packet;
};

#endif /* PX_VIDEO_INTERNAL_H_ */
