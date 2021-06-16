/* Copyright 2021 Pexeso Inc. All rights reserved. */

#ifndef PX_FRAME_INTERNAL_H_
#define PX_FRAME_INTERNAL_H_

#include <libavformat/avformat.h>

#include "px/frame.h"

struct PX_Frame {
  AVFrame* av_frame;
  float timestamp;
};

#endif /* PX_FRAME_INTERNAL_H_ */
