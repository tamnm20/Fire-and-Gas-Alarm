#include <stdio.h>
#include "stm32f10x.h" // Thay th? b?ng header phù h?p cho vi di?u khi?n c?a b?n

////struct __FILE { int handle; };
//FILE __stdout;
//FILE __stdin;

//int fputc(int ch, FILE *f) {
//    if (ch == '\n') {
//        ITM_SendChar('\r'); // G?i ký t? carriage return n?u g?p ký t? newline
//    }
//    ITM_SendChar(ch); // G?i ký t? t?i ITM
//    return ch;
//}
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

//struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}