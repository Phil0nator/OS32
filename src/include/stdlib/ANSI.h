#ifndef OS32_ANSI
#define OS32_ANSI

//https://gist.github.com/Anastasiiaq/8f84951d754a1b2f62eefb5a5efb5618
#define ANSI_ESC ((char)27)
#define ANSI_CSI ((char)'[')

#define ASCII_BEL   ((char)7)
#define ASCII_BS    ((char)8)
#define ASCII_HT    ((char)9)
#define ASCII_LF    ((char)10)
#define ASCII_VT    ((char)11)
#define ASCII_FF    ((char)12)
#define ASCII_CR    ((char)13)
#define ASCII_ESC   ANSI_ESC
#define ASCII_DEL   ((char)127)

#define ANSI_HOME       'H'
#define ANSI_SETC       'f'
#define ANSI_SETC_ALT   ANSI_HOME
#define ANSI_MVUL       'A'
#define ANSI_MVDL       'B'
#define ANSI_MVRC       'C'
#define ANSI_MVLC       'D'
#define ANSI_MVBGLD     'E'
#define ANSI_MVBGPRV    'F'
#define ANSI_MVC        'G'

#define ANSI_RQP        "\x1b[6n"

#define ANSI_SAVE_DEC   '7'
#define ANSI_REST_DEC   '8'

#define ANSI_SAVE_SCO   's'
#define ANSI_REST_SCO   'u'

#define ANSI_CLS        "\x1b[J"
#define ANSI_CLS_C2E    "\x1b[0J"
#define ANSI_CLS_C2B    "\x1b[1J"
#define ANSI_CLS_ALL    "\x1b[2J"
#define ANSI_CLL        "\x1b[K"
#define ANSI_CLL_C2E    "\x1b[0K"
#define ANSI_CLL_C2B    "\x1b[1K"
#define ANSI_CLL_ALL    "\x1b[2K"

#define ANSI_RESET      "\x1b[0m"
#define ANSI_BOLD       "\x1b[1m"
#define ANSI_BOLD_R     "\x1b[22m"
#define ANSI_DIM        "\x1b[2m"
#define ANSI_DIM_R      "\x1b[22m"
#define ANSI_ITALIC     "\x1b[3m"
#define ANSI_ITALIC_R   "\x1b[23m"
#define ANSI_ULINE      "\x1b[4m"
#define ANSI_ULINE_R    "\x1b[24m"
#define ANSI_BLNK       "\x1b[5m"
#define ANSI_BLNK_R     "\x1b[25m"
#define ANSI_INV        "\x1b[7m"
#define ANSI_INV_R      "\x1b[27m"
#define ANSI_HIDE       "\x1b[8m"
#define ANSI_HIDE_R     "\x1b[28m"
#define ANSI_STRK       "\x1b[9m"
#define ANSI_STRK_R     "\x1b[29m"

#define ANSI_COL_RESET  '0'

#define ANSI_BLACK_FG   "30"
#define ANSI_BLACK_BG   "40"
#define ANSI_RED_FG     "31"
#define ANSI_RED_BG     "41"
#define ANSI_GREEN_FG   "32"
#define ANSI_GREEN_BG   "42"
#define ANSI_YELLOW_FG  "33"
#define ANSI_YELLOW_BG  "43"
#define ANSI_BLUE_FG    "34"
#define ANSI_BLUE_BG    "44"
#define ANSI_MAGENTA_FG "35"
#define ANSI_MAGENTA_BG "45"
#define ANSI_CYAN_FG    "36"
#define ANSI_CYAN_BG    "46"
#define ANSI_WHITE_FG   "37"
#define ANSI_WHITE_BG   "47"
#define ANSI_DEFAULT_FG "39"
#define ANSI_DEFUALT_BG "49"

#define ANSI_BRIGHT_BLACK_FG   "90"
#define ANSI_BRIGHT_BLACK_BG   "100"
#define ANSI_BRIGHT_RED_FG     "91"
#define ANSI_BRIGHT_RED_BG     "101"
#define ANSI_BRIGHT_GREEN_FG   "92"
#define ANSI_BRIGHT_GREEN_BG   "102"
#define ANSI_BRIGHT_YELLOW_FG  "93"
#define ANSI_BRIGHT_YELLOW_BG  "103"
#define ANSI_BRIGHT_BLUE_FG    "94"
#define ANSI_BRIGHT_BLUE_BG    "104"
#define ANSI_BRIGHT_MAGENTA_FG "95"
#define ANSI_BRIGHT_MAGENTA_BG "105"
#define ANSI_BRIGHT_CYAN_FG    "96"
#define ANSI_BRIGHT_CYAN_BG    "106"
#define ANSI_BRIGHT_WHITE_FG   "97"
#define ANSI_BRIGHT_WHITE_BG   "107"

#define ANSI_FMT_GRAPHIC_CMD3( fg, bg, mode ) ("\x1b[" mode ";" fg ";" bg "m")
#define ANSI_FMT_GRAPHIC_CMD2( fg, mode ) ("\x1b[" mode ";" fg "m" )


#endif