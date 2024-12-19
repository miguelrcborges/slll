#ifndef SLLC_C_H

#include <mbbase.h>


#define assert(x) if (!(x)) __builtin_trap()


typedef struct String {
	u8 *buffer;
	usize length;
} String;


/* utils */
bool StringEquals(String s1, String s2);
i8 StringCompare(String s1, String s2);
bool u8IsSpace(u8 chr);


/* language */
void Parse(String source);


#define SLLC_C_H
#endif
