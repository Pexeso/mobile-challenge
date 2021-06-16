/* Copyright 2017 Pexeso Inc. All rights reserved. */

#include "px/frame_internal.h"

#include <libavutil/mem.h>
#include <libavutil/pixdesc.h>

const int kDarknessThreshold = 0;

int PX_Frame_New(PX_Frame** frame_p) {
  PX_Frame* frame = (PX_Frame*)av_mallocz(sizeof(PX_Frame));
  if (!frame) {
    return AVERROR(ENOMEM);
  }

  frame->av_frame = av_frame_alloc();
  if (!frame->av_frame) {
    av_free(frame); /* don't leak memory */
    return AVERROR(ENOMEM);
  }

  *frame_p = frame;
  return 0;
}

void PX_Frame_Free(PX_Frame** frame_p) {
  if (!frame_p || !*frame_p) {
    return;
  }

  PX_Frame* frame = *frame_p;

  av_frame_unref(frame->av_frame);
  av_frame_free(&frame->av_frame);
  av_freep(frame_p);
}

bool PX_Frame_IsDark(PX_Frame* frame) {
  uint64_t cnt = frame->av_frame->height * frame->av_frame->width;
  uint64_t sum = 0;

  for (int i = 0; i < frame->av_frame->height; i++) {
    int offset = i * frame->av_frame->linesize[0];
    for (int j = 0; j < frame->av_frame->width; j++) {
      uint8_t pixel = frame->av_frame->data[0][offset + j];
      sum += pixel;
    }
  }

  return (cnt / sum) < kDarknessThreshold;
}

float PX_Frame_GetTimestamp(PX_Frame* frame) {
  return frame->timestamp;
}
