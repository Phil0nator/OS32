#ifndef OS32_VGA
#define OS32_VGA
#include <stdint.h>

#define VGA_BLACK (0)
#define VGA_BLUE (1)
#define VGA_GREEN (2)
#define VGA_CYAN (3)
#define VGA_RED (4)
#define VGA_MAGENTA (5)
#define VGA_BROWN (6)
#define VGA_LIGHT_GRAY (7)
#define VGA_DARK_GRAY (8)
#define VGA_LIGHT_BLUE (9)
#define VGA_LIGHT_GREEN (10)
#define VGA_LIGHT_CYAN (11)
#define VGA_LIGHT_RED (12)
#define VGA_LIGHT_MAGENTA (13)
#define VGA_YELLOW (14)
#define VGA_WHITE (15)

#define VGA_NEWLINE ('\n')
#define VGA_CARRIAGE ('\r')
#define VGA_WIDTH (80)
#define VGA_HEIGHT (25)

typedef uint8_t vgac_t;

void vgaPutc( char c );
void vgaPuts( const char* msg );
void vgaPutln( const char* msg );
void vgaPrintf( const char * format, ... );
void vgaSetCol( vgac_t fg, vgac_t bg );
void vgaGetCol( vgac_t* fg, vgac_t* bg );
vgac_t vgaGetFg();
vgac_t vgaGetBg();
void vgaSetFg( vgac_t fg );
void vgaSetBg( vgac_t bg );


#endif


