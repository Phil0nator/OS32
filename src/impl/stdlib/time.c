#include "stdlib/time.h"
#include "drivers/rtc.h"
#include <stdint.h>

static const uint32_t days_in_months[] = 
{
    0,
 	31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
};
static uint32_t days_before_month( uint32_t mno )
{
    uint32_t out = 0;
    while (mno) out+=days_in_months[mno--];
    return out;
}



pxtime_t time(pxtime_t* dest)
{
    rtc_timepoint_t ct;
    rtc_get_time(&ct);
    uint32_t secs = 0;
    secs+=ct.m_sec;
    secs+=ct.m_min*60;
    secs+=ct.m_hour*60*60;
    secs+=ct.m_mday*24*60*60;
    secs+=days_before_month( ct.m_month )*24*60*60;
    secs+=ct.m_year*365*24*60*60 + (ct.m_year/4)*24*60*60;
    if (dest) *dest = secs;
    return secs;
}