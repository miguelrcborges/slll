#include "c.h"

bool StringEquals(String s1, String s2) {
	if (s1.length != s2.length) return false;
	for (usize i = 0; i < s1.length; i += 1) {
		if (s1.buffer[i] != s2.buffer[i]) {
			return false;
		}
	}

	return true;
}


i8 StringCompare(String s1, String s2) {
	usize min_length = min(s1.length, s2.length);
	for (usize i = 0; i < min_length; i += 1) {
		if (s1.buffer[i] != s2.buffer[i]) {
			return s1.buffer[i] - s2.buffer[i];
		}
	}

	return (s1.length > s2.length) - (s1.length < s2.length);
}


bool u8IsSpace(u8 chr) {
#define m(c) (chr == c)
	return m(' ') || m('\n') || m('\t') || m('\v') || m('\r') || m('\f');
#undef m
}
