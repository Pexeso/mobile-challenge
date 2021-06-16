/* Copyright 2021 Pexeso Inc. All rights reserved. */

#include "px/px.h"

#include <libavutil/log.h>

void PX_Init() {
  av_log_set_level(AV_LOG_QUIET);
}

void PX_Error(int errnum, char* buf, size_t bufsize) {
  av_strerror(errnum, buf, bufsize);
}
