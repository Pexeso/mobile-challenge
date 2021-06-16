/* Copyright 2021 Pexeso Inc. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>

#include <px/px.h>

const int kErrorBufSize = 100;

static void checkError(int ret) {
  if (ret < 0) {
    char buf[kErrorBufSize];

    PX_Error(ret, buf, kErrorBufSize);
    fprintf(stderr, "%d: %s\n", ret, buf);
    exit(ret);
  }
}

int main(int argc, char** argv) {
  int ret;
  PX_Video* video;
  PX_Frame* frame;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <video-file>\n", argv[0]);
    return 1;
  }

  // Initialize the library.
  PX_Init();

  // Open the video.
  ret = PX_Video_New(&video, argv[1]);
  checkError(ret);

  // Initialize the frame.
  ret = PX_Frame_New(&frame);
  checkError(ret);

  for (;;) {
    ret = PX_Video_NextFrame(video, frame);
    checkError(ret);

    if (ret == 0) {
      break;
    }

    float ts = PX_Frame_GetTimestamp(frame);
    int is_dark = PX_Frame_IsDark(frame);

    printf("ts=%f is_dark=%d\n", ts, is_dark);
  }

  PX_Frame_Free(&frame);
  PX_Video_Free(&video);

  return 0;
}
