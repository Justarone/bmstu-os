#include "buffer.h"

int init_cbuffer(CBuffer* const buffer) {
    if (!buffer) 
        return -1;
    memset(buffer, 0, sizeof(CBuffer));
    return 0;
}

int write_cbuffer(CBuffer* const buffer, const char elem) {
    if (!buffer)
        return -1;
    buffer->data[buffer->wpos++] = elem;
    buffer->wpos %= N;
    return 0;
}

int read_cbuffer(CBuffer* const buffer, char* const dest) {
    if (!buffer)
        return -1;
    *dest = buffer->data[buffer->rpos++];
    buffer->rpos %= N;
    return 0;
}
