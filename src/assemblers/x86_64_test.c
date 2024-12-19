#include "x86_64.c"

#define NO_MINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int main(void) {
	x86_64_AssemberContext c = {0};
	c.assembler_output_memory_size = 4096 * 8;
	c.assembler_output_memory = VirtualAlloc(NULL, c.assembler_output_memory_size, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	x86_64_StartInstruction(&c, X86_64_RET);

	void (*fun)(void) = (void (*)(void))c.assembler_output_memory;
	fun();
}
