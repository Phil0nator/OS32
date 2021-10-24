#ifndef OS32_RTC_D
#define OS32_RTC_D
#include <stdint.h>
#include "stdlib/error.h"

/**
 * rtc_timepoint represents a point in time as given
 * by the real time clock on the CPU.
 */
typedef struct rtc_timepoint
{
    uint8_t m_sec;
    uint8_t m_min;
    uint8_t m_hour;
    uint8_t m_wkday;
    uint8_t m_mday;
    uint8_t m_month;
    uint8_t m_year;
    uint8_t m_cent;
} rtc_timepoint_t;

// The names of the months indexed in order
extern const char* rtc_month_names[12];

/**
 * Write the current rtc time into a timepoint struct
 */
err_t rtc_get_time( struct rtc_timepoint* dest );
/**
 * Set the RTC's internal time with a timepoint struct
 */
err_t rtc_set_time( const struct rtc_timepoint* src );

#endif