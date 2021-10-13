#include "drivers/rtc.h"
#include "drivers/cmos.h"
#include "stdlib/string.h"
#include <stdbool.h>

#define RTC_SECREG     (0x00)
#define RTC_MINREG     (0x02)
#define RTC_HOURREG    (0x04)
#define RTC_WKDAYREG   (0x06)
#define RTC_MDAYREG    (0x07)
#define RTC_MONTHREG   (0x08)
#define RTC_YEARREG    (0x09)
#define RTC_CENTREG    (0x32)
#define RTC_STATREGA   (0x0A)
#define RTC_STATREGB   (0x0B)

#define RTC_IN_PROGRESS(REGA) (REGA&0x80)
#define RTC_FMT_BCD( REGB ) (!(REGB&0x04))
const char* rtc_month_names[12] =
{
    "Jan", 
    "Feb", 
    "Mar", 
    "Apr", 
    "May",
    "Jun", 
    "Jul", 
    "Aug", 
    "Sep", 
    "Oct", 
    "Nov", 
    "Dec"
};

static void await_rtc_ready()
{
    while ( RTC_IN_PROGRESS(cmos_read( RTC_STATREGA )) );
}


static uint8_t bcd_to_bin( uint8_t bcd )
{
    return ((bcd&0x0f))+((bcd>>4)*10);
}

static uint8_t rtc_convert_field( uint8_t regb, uint8_t field )
{
    if (RTC_FMT_BCD(regb))
    {
        return bcd_to_bin(field);
    }
    return field;
} 

static void populate_rtct( struct rtc_timepoint* dest )
{
    dest->m_sec = cmos_read( RTC_SECREG );
    dest->m_min = cmos_read( RTC_MINREG );
    dest->m_hour = cmos_read( RTC_HOURREG );
    dest->m_mday = cmos_read( RTC_MDAYREG );
    dest->m_month = cmos_read( RTC_MONTHREG );
    dest->m_year = cmos_read( RTC_YEARREG );
}

err_t rtc_get_time( struct rtc_timepoint* dest )
{
    struct rtc_timepoint try_a, try_b;

    do {
        await_rtc_ready();
        populate_rtct( &try_a );
        populate_rtct( &try_b );
    } while ( 0 );

    uint8_t regb = cmos_read( RTC_STATREGB );
    dest->m_sec = rtc_convert_field( regb, try_a.m_sec );
    dest->m_min = rtc_convert_field( regb, try_a.m_min );
    dest->m_hour = rtc_convert_field( regb, try_a.m_hour );
    dest->m_mday = rtc_convert_field( regb, try_a.m_mday );
    dest->m_month = rtc_convert_field( regb, try_a.m_month );
    dest->m_year = rtc_convert_field( regb, try_a.m_year );

    return OS32_SUCCESS;

}
err_t rtc_set_time( const struct rtc_timepoint* src );
