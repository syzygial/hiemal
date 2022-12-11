#include <alsa/asoundlib.h>
#include "core.h"
#include "fmt.h"

#ifdef __linux__
void playChunkALSA(chunk_t *chunk);
#endif