#include "x86_64.c"

#define NO_MINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

int main(void) {
	x86_64_AssemberContext c = {0};
	c.assembler_output_memory_size = 4096 * 8;
	c.assembler_output_memory = VirtualAlloc(NULL, c.assembler_output_memory_size, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	/*
	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddReg(&c, X86_64_RCX);
	
	x86_64_StartInstruction(&c, X86_64_POP);
		x86_64_AddReg(&c, X86_64_RAX);

	x86_64_StartInstruction(&c, X86_64_RET);
	*/
	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddImmediate(&c, 65534, X86_64_DWORD);
	
	x86_64_StartInstruction(&c, X86_64_RET);

	for (usize i = 0; i < 20; i += 1) {
		printf("0x%02hhx ", c.assembler_output_memory[i]);
	}
	printf("\n");

	/*
	long long (*identity)(long long) = (long long (*)(long long))c.assembler_output_memory;
	printf("%lld\n", identity(100));
	*/
}
