//
// Created by wojtek on 05.02.2020.
//

#include "timer_lib.h"
#include "tools.h"

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

struct timespec dtots(double time) {
    struct timespec ts;
    ts.tv_sec = (int) time;
    ts.tv_nsec = (long) (((double) time - ts.tv_sec) * 1000000000l);
    return ts;
}

void preprocess_time() {
    summary_time.tv_sec = 0;
    summary_time.tv_nsec = 0;
    wait_time = dtots(((double) d_arg) / 1000000l);
    kill_time = dtots(((double) T_arg) / 100);
}

void set_killer() {
    //już tylko killer
    struct sigaction sa;
    sa.sa_handler=do_at_end;
    if(sigaction(SIGUSR1,&sa,NULL)==-1)
        print_error("sigaction error");
    timer_t killer;
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    sev.sigev_value.sival_ptr = &killer;
    struct itimerspec killer_time;
    killer_time.it_interval = kill_time;
    killer_time.it_value = kill_time;
    if (timer_create(CLOCK_PROCESS_CPUTIME_ID, &sev, &killer) == -1)
        print_error("timer_create error");
    if (timer_settime(killer, 0, &killer_time, NULL) == -1)
        print_error("timer_settime error");
}

void sleep_wait_time() {
    nanosleep(&wait_time, &tmp);
    while (tmp.tv_sec != 0 || tmp.tv_nsec != 0) {
        nanosleep(&tmp, &tmp1);
        tmp = tmp1;
    }
}

char *nstostr(long ns) {
    int i;
    char *res = (char *) malloc(13);
    res[12] = 0;
    for (i = 0; i < 12; i++) {
        if(i==3||i==6||i==9)
            res[11-i++]='.';
        res[11-i] = '0' + (char)(ns%10);
        ns /= 10;
    }
    return res;
}

char *stostr(int s) {
    int i=0,ss = s % 60,mm=s/60,ii;
    char *res = (char *) malloc(13);
    char tmp;
    res[i++]=0;
    res[i++] = '0' + (char)(ss%10);
    ss/=10;
    res[i++] = '0' + (char)(ss%10);
    res[i++] = ':';
    while(i<13&&(mm!=0||i<6))
    {
        res[i++] = '0' + (char)(mm%10);
        mm/=10;
    }
    ii=i-1;
    for(i=0;i<=(ii/2);i++)
    {
        tmp=res[i];
        res[i]=res[ii-i];
        res[ii-i]=tmp;
    }
    return res;
}

char *time_repr(struct timespec ts)
{
    char *res=(char *) malloc(25);
    strcat(res,stostr(ts.tv_sec));
    strcat(res,",");
    strcat(res,nstostr(ts.tv_nsec));
    return res;
}

void add_to_summary(struct timespec ts_start,struct timespec ts_end)
{
    long nsec = summary_time.tv_nsec+ts_end.tv_nsec - ts_start.tv_nsec;
    long shift = nsec/1000000000l;
    summary_time.tv_sec += ts_end.tv_sec - ts_start.tv_sec + shift;
    summary_time.tv_nsec = nsec%1000000000l;
}