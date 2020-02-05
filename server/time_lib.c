//
// Created by wojtek on 05.02.2020.
//

#include "time_lib.h"

#include <stdlib.h>
#include <string.h>

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

struct timespec ts_sub(struct timespec a,struct timespec b)
{
    long long ans,bns,resns;
    ans = a.tv_sec*1000000000l+a.tv_nsec;
    bns = b.tv_sec*1000000000l+b.tv_nsec;
    resns = ans-bns;
    struct timespec res = {resns/1000000000l,resns%1000000000l};
    return res;
}