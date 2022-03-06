#define __USE_XOPEN
#define _GNU_SOURCE

#define MAX_SIZE 5000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../includes/fetch_commits.h"

bool confirm_date(int year, int month, int day)
{
    if (year == 2005)
    {
        if ((month <= 4) && (day < 7)) return false;
    }
    else if (year < 2005) return false;

    if (year == 2021)
    {
        if ((month >= 11) && (day > 9)) return false;
    }
    else if (year > 2021) return false;

    return true;
}

bool update_date(char* string)
{
    int year, month, day, hour, min, sec, tz_min, tz_secs;
    char tz_sym;

    sscanf(string, 
           "%4d-%2d-%2d %2d:%2d:%2d%c%2d:%2d", 
           &year, &month, &day, &hour, &min, &sec, &tz_sym, &tz_min, &tz_secs);

    printf("Da string foi lido :\n%d-%d-%d %d:%d:%d%c%d:%d\n", 
            year, month, day, hour, min, sec, tz_sym, tz_min, tz_secs);

    struct tm ct = {0};
    strptime(string, "%Y-%m-%d %H:%M:%S %z", &ct);

    ct.tm_year += 1900;
    ct.tm_mon += 1;

    int tz_time = tz_min * 60 + tz_secs;

    printf("%d == %d ?\n", tz_time, ct.tm_gmtoff);
    printf("Da struct criada pela strptime foi lido:\n%d-%d-%d %d:%d:%d %ld (%d))\n\n", 
            ct.tm_year, ct.tm_mon, ct.tm_mday,
            ct.tm_hour, ct.tm_min, ct.tm_sec,
            ct.tm_gmtoff, tz_secs);

    /* O tempo da timezone dado pelo parametro tm_gmtoff é nos dado em segundos, por isso temos de fazer a conversão. */
    if (tz_sym == '-') ct.tm_gmtoff = ct.tm_gmtoff;
    else if (tz_sym == '-') ct.tm_gmtoff = -(ct.tm_gmtoff);
    else {return false;}

    if (year != ct.tm_year || month != ct.tm_mon || day != ct.tm_mday ||
        hour != ct.tm_hour || min != ct.tm_min   || sec != ct.tm_sec  ||
        tz_time != ct.tm_gmtoff)
        { return false; }

    if (!confirm_date(ct.tm_year, ct.tm_mon, ct.tm_mday)) return false;

    return true;

}

int main()
{
    char list[20][100] = {"2012-02-13 21:30"}
}