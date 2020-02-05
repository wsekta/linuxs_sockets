#ifndef MASSIVEREADER_TIME_LIB_H
#define MASSIVEREADER_TIME_LIB_H

#include <time.h>

char *time_repr(struct timespec ts);

struct timespec ts_sub(struct timespec a,struct timespec b);

#endif //MASSIVEREADER_TIME_LIB_H
