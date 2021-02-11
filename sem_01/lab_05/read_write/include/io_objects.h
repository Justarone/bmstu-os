#ifndef __IO_OJBECTS__
#define __IO_OJBECTS__

#include <stdio.h>
#include <sys/sem.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "constants.h"

int run_reader(int* const shared_counter, const int sid, const int reader_id);
int run_writer(int* const shared_counter, const int sid, const int writer_id);

#endif // __IO_OJBECTS__
