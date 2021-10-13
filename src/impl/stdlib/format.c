#include "stdlib/format.h"
#include "stdlib/string.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>


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


int snprintf( char* dest, size_t n, const char* fmt, ... )
{
    va_list args;
    va_start(args, fmt);
    char* final_dest = dest+n;
    char int_fmt_temp[22];
    while (*fmt && dest < final_dest)
    {
        if (*fmt == '%' && final_dest-dest > 1)
        {
            fmt++;
            switch (*fmt)
            {
            case 'd':
            case 'i':
                bzero(int_fmt_temp, 22);
                fmt_int( va_arg(args, int), true, 10, int_fmt_temp );
                dest+=strncpy(dest, final_dest-dest, int_fmt_temp);
                break;
            case 'x':
                bzero(int_fmt_temp, 22);
                fmt_int( va_arg( args, int ), false, 16,int_fmt_temp );
                dest+=strncpy(dest, final_dest-dest, int_fmt_temp);
                break;
            case 'u':
                bzero(int_fmt_temp, 22);
                fmt_int( va_arg( args, int ), false, 10,int_fmt_temp );
                dest+=strncpy(dest, final_dest-dest, int_fmt_temp);
                break;
            case 's':
                dest+=strncpy( dest, final_dest-dest, va_arg(args, const char*) );
                break;
            case '%':
                *(dest++)=('%');
                break;
            case 'c':
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
