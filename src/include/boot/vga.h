#ifndef OS32_VGA
#define OS32_VGA
#include <stdint.h>
#include <stdbool.h>

#define VGA_BLACK           (0)
#define VGA_BLUE            (1)
#define VGA_GREEN           (2)
#define VGA_CYAN            (3)
#define VGA_RED             (4)
#define VGA_MAGENTA         (5)
#define VGA_BROWN           (6)
#define VGA_LIGHT_GRAY      (7)
#define VGA_DARK_GRAY       (8)
#define VGA_LIGHT_BLUE      (9)
#define VGA_LIGHT_GREEN     (10)
#define VGA_LIGHT_CYAN      (11)
#define VGA_LIGHT_RED       (12)
#define VGA_LIGHT_MAGENTA   (13)
#define VGA_YELLOW          (14)
#define VGA_WHITE           (15)

#define VGA_NEWLINE         ('\n')
#define VGA_CARRIAGE        ('\r')
#define VGA_WIDTH           (80)
#define VGA_HEIGHT          (25)

typedef uint8_t vgac_t;
extern struct VGACell* VGAO;

#pragma pack(1)

typedef struct VGACell
{
    char c;
    vgac_t fg: 4;
    vgac_t bg: 4;
} VGACell_t;

#pragma pack(0)
/**
 * Output a single character at the current position, and increment the position.
 * The current foreground and background colors are used.
 * @param c ASCII character to output
 */
void vgaPutc( char c );

/**
 * Output a c style string at the current position, and increment the position.
 * The current foreground and background colors are used.
 * @param msg ASCII characters to output
 */
void vgaPuts( const char* msg );
/**
 * Output a c style string followed by a newline at the current position, and increment the position.
 * The current foreground and background colors are used.
 * @param msg ASCII characters to output
 */
void vgaPutln( const char* msg );

/**
 * Print formatted output to the VGA.
 * Format parameters are marked with a '%' like standard C's printf.
 * a "%%" will output "%".
 * 
 * Options:
 *  - %+        output an [ OK ] marker
 *  - %-        output a  [ FAILED ] marker
 *  - %d|%i     output a signed 32 bit integer (decimal)
 *  - %u        output an unsigned 32 bit integer (decimal)
 *  - %x        output an unsigned 32 bit integer (hex)
 *  - %c        output a single ASCII character
 *  - %s        output a c-style string
 *  - %t        output a 2-digit decimal number (for time formatting)
 * @param format a format specifier string
 * @param ... coresponding parameters for the format string 
 */
void vgaPrintf( const char * format, ... );
/**
 * Set colors.
 * @param fg foreground
 * @param bg background
 */
void vgaSetCol( vgac_t fg, vgac_t bg );
/**
 * Get current colors.
 * @param fg foreground dest
 * @param bg background dest
 */
void vgaGetCol( vgac_t* fg, vgac_t* bg );
/**
 * Get foreground
 */
vgac_t vgaGetFg();
/**
 * Get background
 */
vgac_t vgaGetBg();
/**
 * Set foreground
 */
void vgaSetFg( vgac_t fg );
/**
 * Set background
 */
void vgaSetBg( vgac_t bg );
/**
 * Set cursor position
 * @param x columns
 * @param y rows
 */
void vgaSetCursor( int x, int y );
/**
 * Get cursor position
 * @param x columns dest
 * @param y rows dest
 */
void vgaGetCursor( int* x, int* y );
/**
 * Toggle hardware cursor
 */
void vgaShowCursor( bool show );

#endif


