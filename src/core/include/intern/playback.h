#ifndef _INTERN_PLAYBACK_H
#define _INTERN_PLAYBACK_H

#include <alsa/asoundlib.h>
#include "core.h"
#include "fmt.h"

#ifdef __linux__
void playChunkALSA(chunk_t *chunk);
#endif

#endif _INTERN_PLAYBACK_H