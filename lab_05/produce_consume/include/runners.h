#ifndef __RUNNERS__
#define __RUNNERS__

#include <time.h>
#include <stdio.h>
#include <sys/sem.h>

#include "buffer.h"
#include "constants.h"

int run_producer(CBuffer* const buffer, const int sid, const int prod_id);
int run_consumer(CBuffer* const buffer, const int sid, const int cons_id);

#endif // __RUNNERS__
