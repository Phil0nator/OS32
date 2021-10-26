#include "stdlib/format.h"
#include "stdlib/string.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#define FMT_SPEC    '%'
#define FMT_INT     'i'
#define FMT_INT2    'd'
#define FMT_CHAR    'c'
#define FMT_STR     's'
#define FMT_UINT    'u'
#define FMT_HEX     'x'


static const char* multibase_chars = "0123456789abcdef";
static size_t fmt_int( uint32_t num, bool sign, uint32_t base, char* dest )
{

    char digits[22];
    
    if (num == 0){
        *(dest++)=('0');
        return 1;
    } 
    else if (((int)num) < 0 && sign)
    {
        *(dest++)=('-');
        num = -(int)num;
    }
    
    
    memset(digits, 0, sizeof(digits));
    size_t counter = 0;
    while (num)
    {
        digits[counter] = multibase_chars[ ( num % base ) ];
        num /= base;
        counter ++;
    }
    size_t reverse_counter = 0;
    do
    {
        counter--;
        dest[reverse_counter] = (digits[counter]);
        reverse_counter++;
    }while (counter);
    return reverse_counter;
}


static size_t defmt_int( uint32_t* dest, const char* src, bool sign, uint32_t base )
{
    
}

int snprintf( char* dest, size_t n, const char* fmt, ... )
{
    va_list args;
    va_start(args, fmt);
    char* final_dest = dest+n;
    char int_fmt_temp[22];
    while (*fmt && dest < final_dest)
    {
        if (*fmt == FMT_SPEC && final_dest-dest > 1)
        {
            fmt++;
            switch (*fmt)
            {
            case FMT_INT:
            case FMT_INT2:
                bzero(int_fmt_temp, 22);
                fmt_int( va_arg(args, int), true, 10, int_fmt_temp );
                dest+=strncpy(dest, final_dest-dest, int_fmt_temp);
                break;
            case FMT_HEX:
                bzero(int_fmt_temp, 22);
                fmt_int( va_arg( args, int ), false, 16,int_fmt_temp );
                dest+=strncpy(dest, final_dest-dest, int_fmt_temp);
                break;
            case FMT_UINT:
                bzero(int_fmt_temp, 22);
                fmt_int( va_arg( args, int ), false, 10,int_fmt_temp );
                dest+=strncpy(dest, final_dest-dest, int_fmt_temp);
                break;
            case FMT_STR:
                dest+=strncpy( dest, final_dest-dest, va_arg(args, const char*) );
                break;
            case FMT_SPEC:
                *(dest++)=('%');
                break;
            case FMT_CHAR:
                *(dest++)=(char)( va_arg( args, int ) );
                break;
            case 't':
                {
                    bzero(int_fmt_temp, 22);
                    int num = va_arg(args, int);
                    if ( num < 10 )
                    {
                        int_fmt_temp[0] = '0';
                        fmt_int( num, true, 10, &int_fmt_temp[1] );
                    }
                    else
                    {
                        fmt_int( num, true, 10, int_fmt_temp );
                    }
                    dest+=strncpy(dest, final_dest-dest, int_fmt_temp);
                }
            default:
                break;
            }
        }
        else
        {
            *(dest++)=(*fmt);
        }
        fmt++;
    }
    return final_dest-dest;
}


int sscanf( const char* source, const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    int args_copied = 0;
    while (*source && *fmt)
    {
        if (*fmt == FMT_SPEC)
        {
            fmt++;
            switch (*fmt)
            {
            case FMT_INT:
            case FMT_INT2:

                break;
            
            default:
                break;
            }
        }
        else
        {
            if (*source != *fmt)
            {
                return args_copied;
            }
        }
    }
}