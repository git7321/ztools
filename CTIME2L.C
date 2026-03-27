/* routines for converting from ASCII time to long int */
#include <time.h>
#include <stdio.h>
#include <string.h>

static int dayinmon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static char *strMon[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
    "Aug", "Sep", "Oct", "Nov", "Dec"};
static char *strDay[7] =  {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static struct tm tb;

int _days[] = {
	-1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
};
static int istr(char *p, char **q, int len);

#define DaySec  (24*60*60L)
#define YearSec (365*DaySec)
#define DecSec  315532800L
#define Day1    4
#define Day180  2

void __cdecl __tzset(void)
{
    static int first_time = 0;

    if ( !first_time ) {
        _tzset();
         first_time++;
    }
}

static int _isindst(register struct tm *tb)
{
    int mdays;
    register int yr;
    int lastsun;

    if (tb->tm_mon < 3 || tb->tm_mon > 9)
        return(0);

    if (tb->tm_mon > 3 && tb->tm_mon < 9)
        return(1);

    yr = tb->tm_year + 1900;

    if (yr > 1986 && tb->tm_mon == 3)
        mdays = 7 + _days[tb->tm_mon];
    else
        mdays = _days[tb->tm_mon+1];

    if (!(yr & 3))
        mdays++;

    yr = tb->tm_year - 70;

    lastsun = mdays - ((mdays + 365*yr + ((yr+1)/4) + Day1) % 7);

    return (tb->tm_mon==3
        ? (tb->tm_yday > lastsun ||
        (tb->tm_yday == lastsun && tb->tm_hour >= 2))
        : (tb->tm_yday < lastsun ||
        (tb->tm_yday == lastsun && tb->tm_hour < 1)));
}

static time_t _dtoxtime(int yr, int mo, int dy, int hr, int mn, int sc)
{
    int mdays;
    time_t scount;

    scount = ((yr+3)/4)*(time_t)DaySec;

    mdays = _days[mo-1];
    if (!(yr % 4) && (mo > 2))
        mdays++;
    scount += (yr*365 + dy + mdays)*(time_t)DaySec + (time_t)hr*3600L + mn*60L +
                sc + (time_t)DecSec;
    tb.tm_yday = mdays + dy;
    __tzset();
    scount += _timezone;
    tb.tm_year = yr + 80;
    tb.tm_mon = mo - 1;
    tb.tm_hour = hr;
    if (_daylight && _isindst(&tb))
        scount -= 3600L;
    return(scount);
}

static int istr(char *p, char **q, int len)
{
    int i;

    for (i=0; i < len; i++)
        if (_strcmpi(p, *q++)== 0)
            break;
    return i;
}

static leapyear(int i)
{
    return (!i%4 && i%100);
}

static int yday(int year, int mon, int day)
{
    int i, j;

    j = day -1;
    for (i=0; i < mon; i++)
        j += dayinmon[i];
    if (mon > 2 && leapyear(year))
        j++;
    return j;
}

time_t __stdcall date2l(int year, int month, int day, int hour, int min, int sec)
{
    return _dtoxtime (year - 1980, month, day, hour, min, sec);
}

struct tm * __stdcall ctime2tm(char *p)
{
    char day[4], mon[4];
    int date, year, hour, min, sec, month;
    time_t now;

    if (sscanf (p, " %3s %3s %2d %2d:%2d:%2d %4d ",
                   day, mon, &date, &hour, &min, &sec, &year) == 7) {
        tb.tm_sec = sec;
        tb.tm_min = min;
        tb.tm_hour = hour;
        tb.tm_mday = date;
        tb.tm_year = year-1900;
        tb.tm_mon  = istr(mon, strMon, 12);
        tb.tm_wday = istr(day, strDay, 7);
        tb.tm_yday = yday(tb.tm_year, tb.tm_mon, tb.tm_mday);
        tb.tm_isdst = (_daylight && _isindst(&tb) ? 1 : 0);
        return &tb;
        }

    if (*p == '+' && sscanf (p+1, " %2d:%2d:%2d ", &hour, &min, &sec) == 3) {
        time (&now);
        now += 3600L * hour + 60L * min + sec;
        tb = *localtime (&now);
        return &tb;
        }
    if (*p == '+' && sscanf (p+1, " %2d:%2d ", &hour, &min) == 2) {
        time (&now);
        now += 3600L * hour + 60L * min;
        tb = *localtime (&now);
        return &tb;
        }
    if (*p == '+' && sscanf (p+1, " %2d ", &hour) == 1) {
        time (&now);
        now += 3600L * hour;
        tb = *localtime (&now);
        return &tb;
        }

    if (sscanf (p, " %2d:%2d:%2d ", &hour, &min, &sec) == 3) {
        time (&now);
        tb = *localtime (&now);
        tb.tm_sec = sec;
        tb.tm_min = min;
        tb.tm_hour = hour;
        return &tb;
        }
    if (sscanf (p, " %2d:%2d ", &hour, &min) == 2) {
        time (&now);
        tb = *localtime (&now);
        tb.tm_sec = 0;
        tb.tm_min = min;
        tb.tm_hour = hour;
        return &tb;
        }

    if (sscanf (p, " %2d/%2d/%2d ", &month, &date, &year) == 3) {
        if (year < 70)
            year += 2000;
        if (year < 100)
            year += 1900;
        now = _dtoxtime (year - 1980, month, date, 0, 0, 0);
        tb = *localtime (&now);
        return &tb;
        }

    if (!strcmp (p, "yesterday")) {
        time (&now);
        now -= 24 * 3600L;
        tb = *localtime (&now);
        tb.tm_sec = 0;
        tb.tm_min = 0;
        tb.tm_hour = 0;
        return &tb;
        }
    if (!strcmp (p, "now")) {
        time (&now);
        tb = *localtime (&now);
        return &tb;
        }
    if (!strcmp (p, "tomorrow")) {
        time (&now);
        now += 24 * 3600L;
        tb = *localtime (&now);
        tb.tm_sec = 0;
        tb.tm_min = 0;
        tb.tm_hour = 0;
        return &tb;
        }

    return NULL;
}

time_t __stdcall ctime2l(char *p)
{
    if (ctime2tm(p) == NULL)
        return -1L;
    return date2l (tb.tm_year +1900, tb.tm_mon + 1, tb.tm_mday, tb.tm_hour,
        tb.tm_min, tb.tm_sec);
}
