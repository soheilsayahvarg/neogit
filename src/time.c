#include "includes.h"
#include "prototypes.h"
#include "defines.h"

// Format :
// Fri Feb  2 16:31:45 2024
int comprator_time(char date1_string[], char date2_string[])
{
    char month1_sring[4];
    char month2_sring[4];
    struct tm time1;
    struct tm time2;
    sscanf(date1_string + 4, "%s %d %d:%d:%d %d", month1_sring, &time1.tm_mday, &time1.tm_hour, &time1.tm_min, &time1.tm_sec, &time1.tm_year);
    sscanf(date2_string + 4, "%s %d %d:%d:%d %d", month2_sring, &time2.tm_mday, &time2.tm_hour, &time2.tm_min, &time2.tm_sec, &time2.tm_year);
    if (!strcmp(month1_sring, "Jan"))
    {
        time1.tm_mon = 1;
    }
    if (!strcmp(month1_sring, "Feb"))
    {
        time1.tm_mon = 2;
    }
    if (!strcmp(month1_sring, "Mar"))
    {
        time1.tm_mon = 3;
    }
    if (!strcmp(month1_sring, "Apr"))
    {
        time1.tm_mon = 4;
    }
    if (!strcmp(month1_sring, "May"))
    {
        time1.tm_mon = 5;
    }
    if (!strcmp(month1_sring, "Jun"))
    {
        time1.tm_mon = 6;
    }
    if (!strcmp(month1_sring, "Jul"))
    {
        time1.tm_mon = 7;
    }
    if (!strcmp(month1_sring, "Aug"))
    {
        time1.tm_mon = 8;
    }
    if (!strcmp(month1_sring, "Sep"))
    {
        time1.tm_mon = 9;
    }
    if (!strcmp(month1_sring, "Oct"))
    {
        time1.tm_mon = 10;
    }
    if (!strcmp(month1_sring, "Nov"))
    {
        time1.tm_mon = 11;
    }
    if (!strcmp(month1_sring, "Dec"))
    {
        time1.tm_mon = 12;
    }

    if (!strcmp(month2_sring, "Jan"))
    {
        time2.tm_mon = 1;
    }
    if (!strcmp(month2_sring, "Feb"))
    {
        time2.tm_mon = 2;
    }
    if (!strcmp(month2_sring, "Mar"))
    {
        time2.tm_mon = 3;
    }
    if (!strcmp(month2_sring, "Apr"))
    {
        time2.tm_mon = 4;
    }
    if (!strcmp(month2_sring, "May"))
    {
        time2.tm_mon = 5;
    }
    if (!strcmp(month2_sring, "Jun"))
    {
        time2.tm_mon = 6;
    }
    if (!strcmp(month2_sring, "Jul"))
    {
        time2.tm_mon = 7;
    }
    if (!strcmp(month2_sring, "Aug"))
    {
        time2.tm_mon = 8;
    }
    if (!strcmp(month2_sring, "Sep"))
    {
        time2.tm_mon = 9;
    }
    if (!strcmp(month2_sring, "Oct"))
    {
        time2.tm_mon = 10;
    }
    if (!strcmp(month2_sring, "Nov"))
    {
        time2.tm_mon = 11;
    }
    if (!strcmp(month2_sring, "Dec"))
    {
        time2.tm_mon = 12;
    }

    if (time1.tm_year > time2.tm_year)
    {
        return 1;
    }
    if (time1.tm_year < time2.tm_year)
    {
        return -1;
    }
    if (time1.tm_mon > time2.tm_mon)
    {
        return 1;
    }
    if (time1.tm_mon < time2.tm_mon)
    {
        return -1;
    }
    if (time1.tm_hour > time2.tm_hour)
    {
        return 1;
    }
    if (time1.tm_hour < time2.tm_hour)
    {
        return -1;
    }
    if (time1.tm_min > time2.tm_min)
    {
        return 1;
    }
    if (time1.tm_min < time2.tm_min)
    {
        return -1;
    }
    if (time1.tm_sec > time2.tm_sec)
    {
        return 1;
    }
    if (time1.tm_sec < time2.tm_sec)
    {
        return -1;
    }
    return 0;
}
