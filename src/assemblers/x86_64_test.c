#include "x86_64.c"

#ifdef _WIN32
	#define NO_MINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif
#include <stdio.h>

static bool are_slices_different(u8 *restrict p1, u8 *restrict p2, usize count) {
	for (usize i = 0; i < count; i += 1) {
		if (p1[i] != p2[i]) return true;
	}
	return false;
}

#define TestSlice(...) do { \
		usize len = sizeof((u8[]){__VA_ARGS__}); \
		u8 *buff =(u8*)&((u8[]){__VA_ARGS__}); \
		if (are_slices_different(c.assembler_output_memory, buff, len)) { \
			fprintf(stderr, "Test at line %d failed.\nMemory difference:\n", __LINE__); \
			for (usize i = 0; i < len; i += 1) { \
				fprintf(stderr, "(%04llu): %hhx %hhx\n", i, c.assembler_output_memory[i], buff[i]); \
			} \
			fputs("\n\n", stderr); \
		} \
		c.assember_output_memory_cursor = 0; \
	} while (0)

int main(void) {
	x86_64_AssemblerContext c = {0};
	c.assembler_output_memory_size = 4096;
#if defined(_WIN32)
	c.assembler_output_memory = VirtualAlloc(NULL, c.assembler_output_memory_size, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
	#error "To implement on this platform"
#endif


	/* PUSH */
	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddReg(&c, X86_64_RAX);
	TestSlice(0x50);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddReg(&c, X86_64_R10);
	TestSlice(0x41, 0x52);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddReg(&c, X86_64_RBP);
	TestSlice(0x55);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddReg(&c, X86_64_AX);
	TestSlice(0x66, 0x50);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddReg(&c, X86_64_BP);
	TestSlice(0x66, 0x55);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddReg(&c, X86_64_R15W);
	TestSlice(0x66, 0x41, 0x57);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddImmediate(&c, 65534, X86_64_WORD);
	TestSlice(0x66, 0x6A, 0xFE);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddImmediate(&c, 65534, X86_64_DWORD);
	TestSlice(0x68, 0xFE, 0xFF, 0x00, 0x00);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddImmediate(&c, 420, X86_64_DWORD);
	TestSlice(0x68, 0xA4, 0x01, 0x00, 0x00);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddImmediate(&c, 420, X86_64_WORD);
	TestSlice(0x66, 0x68, 0xA4, 0x01);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddImmediate(&c, 69, X86_64_DWORD);
	TestSlice(0x6a, 0x45);

	x86_64_StartInstruction(&c, X86_64_PUSH);
		x86_64_AddImmediate(&c, 69, X86_64_WORD);
	TestSlice(0x66, 0x6a, 0x45);


	/* POP */
	x86_64_StartInstruction(&c, X86_64_POP);
		x86_64_AddReg(&c, X86_64_RAX);
	TestSlice(0x58);

	x86_64_StartInstruction(&c, X86_64_POP);
		x86_64_AddReg(&c, X86_64_R10);
	TestSlice(0x41, 0x5A);

	x86_64_StartInstruction(&c, X86_64_POP);
		x86_64_AddReg(&c, X86_64_RBP);
	TestSlice(0x5D);

	x86_64_StartInstruction(&c, X86_64_POP);
		x86_64_AddReg(&c, X86_64_AX);
	TestSlice(0x66, 0x58);

	x86_64_StartInstruction(&c, X86_64_POP);
		x86_64_AddReg(&c, X86_64_BP);
	TestSlice(0x66, 0x5D);

	x86_64_StartInstruction(&c, X86_64_POP);
		x86_64_AddReg(&c, X86_64_R15W);
	TestSlice(0x66, 0x41, 0x5F);


	/* RET */
	x86_64_StartInstruction(&c, X86_64_RET);
	TestSlice(0xC3);
}
