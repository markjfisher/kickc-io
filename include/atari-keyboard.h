// Atari Key Codes from CH
// Taken from cc65

#define CH_DELCHR       0xFE   /* delete char under the cursor */
#define CH_ENTER        0x9B
#define CH_ESC          0x1B
#define CH_CURS_UP      28
#define CH_CURS_DOWN    29
#define CH_CURS_LEFT    30
#define CH_CURS_RIGHT   31

#define CH_TAB          0x7F   /* tabulator */
#define CH_EOL          0x9B   /* end-of-line marker */
#define CH_CLR          0x7D   /* clear screen */
#define CH_BEL          0xFD   /* bell */
#define CH_DEL          0x7E   /* back space (delete char to the left) */
#define CH_RUBOUT       0x7E   /* back space (old, deprecated) */
#define CH_DELLINE      0x9C   /* delete line */
#define CH_INSLINE      0x9D   /* insert line */

/* These are defined to be Atari + NumberKey */
#define CH_F1           177
#define CH_F2           178
#define CH_F3           179
#define CH_F4           180
#define CH_F5           181
#define CH_F6           182
#define CH_F7           183
#define CH_F8           184
#define CH_F9           185
#define CH_F10          176

#define CH_ULCORNER     0x11
#define CH_URCORNER     0x05
#define CH_LLCORNER     0x1A
#define CH_LRCORNER     0x03
#define CH_TTEE         0x17
#define CH_BTEE         0x18
#define CH_LTEE         0x01
#define CH_RTEE         0x04
#define CH_CROSS        0x13
#define CH_HLINE        0x12
#define CH_VLINE        0x7C
