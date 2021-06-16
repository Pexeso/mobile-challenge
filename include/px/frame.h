/* Copyright 2021 Pexeso Inc. All rights reserved. */

#ifndef PX_FRAME_H_
#define PX_FRAME_H_

#include <stdbool.h>

typedef struct PX_Frame PX_Frame;

/*!
 * Allocate a new PX_Frame object and store it in the frame_p param. The object
 * must be allocated before passing it to the PX_Video_NextFrame() function.
 *
 * \param frame_p will be filled with a pointer to the newly allocated PX_Frame object.
 * \return 0 on success and <0 in case of a failure.
 *
 * You can use the PX_Error() function to get string representation of the
 * returned error.
 */
int PX_Frame_New(PX_Frame** frame_p);

/*!
 * Free allocated resources associated with the frame. This function must be
 * called when the frame object is no longer needed.
 *
 * \param frame_p a pointer to the PX_Frame pointer that should be freed. The
 *                pointer itself will be set to NULL.
 */
void PX_Frame_Free(PX_Frame** frame_p);

/*!
 * This function determines whether the frame is too dark, meaning the average
 * luminosity is less than 5.
 *
 * \param frame the frame we want to check for darkness.
 * \return true if it is dark false otherwise.
 */
bool PX_Frame_IsDark(PX_Frame* frame);

/*!
 * Get presentation timestamp of this frame, in seconds.
 *
 * \param frame is the frame we want to get the presentation timestamp from.
 * \return the presentation timestamp.
 */
float PX_Frame_GetTimestamp(PX_Frame* frame);

#endif /* PX_FRAME_H_ */
