#ifndef MULTIWRITER_TIMER_LIB_H
#define MULTIWRITER_TIMER_LIB_H

#include <time.h>

struct timespec summary_time,wait_time,kill_time,tmp,tmp1;

struct timespec dtots(double time);

void preprocess_time();

void set_killer();

void sleep_wait_time();

char *time_repr(struct timespec ts);

void add_to_summary(struct timespec ts_start,struct timespec ts_end);

#endif //MULTIWRITER_TIMER_LIB_H
