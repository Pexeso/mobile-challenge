/* Copyright 2021 Pexeso Inc. All rights reserved. */

#ifndef PX_VIDEO_H_
#define PX_VIDEO_H_

#include <stddef.h>

#include "px/frame.h"

typedef struct PX_Video PX_Video;

/*!
 * Open the file specified in the path argument, allocate a new PX_Video object
 * and associate it with the open file. PX_Video_Free() must be called to close
 * the file and free allocated resources.
 *
 * \param video_p will be filled with a pointer to the newly allocated PX_Video object.
 * \param path is a path to the file that we want to process.
 * \return 0 on success and <0 in case of a failure.
 *
 * You can use the PX_Error() function to get string representation of the
 * returned error.
 */
int PX_Video_New(PX_Video** video_p, const char* path);

/*!
 * Close the file and free allocated resources. This function must be called
 * when the video object is no longer needed.
 *
 * \param video_p a pointer to the PX_Video pointer that should be freed. The
 *                pointer itself will be set to NULL.
 */
void PX_Video_Free(PX_Video** video_p);

/*!
 * Grabs the next available video keyframe from the file.
 *
 * \param video this is the video object from which we want to obtain the frame.
 * \param frame the obtained frame will be stored here.
 * \return 1 if a frame was grabbed and there's more frames to be grabbed 0 if there
 *         are no more frames, the video is finished -1 on error
 *
 * You can use the PX_Error() function to get string representation of the
 * returned error.
 */
int PX_Video_NextFrame(PX_Video* video, PX_Frame* frame);

#endif /* PX_VIDEO_H_ */
