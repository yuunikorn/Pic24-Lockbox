
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.
#define clrscr() putsU2( "\x1b[2J") //clears the screen
#define home() putsU2( "\x1b[H") //places cursor at upper left
void initUART2(void);
void putU2(char c);
char getU2(void);
void putsU2(char *s);


#endif 