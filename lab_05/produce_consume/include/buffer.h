#ifndef __CBUFFER__
#define __CBUFFER__

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

typedef struct CycleBuffer {
    size_t rpos;
    size_t wpos;
    char data[N];
} CBuffer;

int init_cbuffer(CBuffer* const buffer);
int write_cbuffer(CBuffer* const buffer, const char elem);
int read_cbuffer(CBuffer* const buffer, char* const dest);

#endif // __CBUFFER__
