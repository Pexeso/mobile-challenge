/* Copyright 2021 Pexeso Inc. All rights reserved. */

#ifndef PX_PX_H_
#define PX_PX_H_

#include <stddef.h>

#include "px/frame.h"
#include "px/video.h"

/**
 * This function initializes the library. It should be called only once, at the
 * start of the program.
 */
void PX_Init();

/**
 * This function initializes the library. It should be called only once, at the
 * start of the program.
 *
 * \param errnum is an error code we want to turn into text.
 * \param buf is where the text representation of the error will be stored.
 * \param bufsize size of the output buffer.
 */
void PX_Error(int errnum, char *buf, size_t bufsize);

#endif /* PX_PX_H_ */
