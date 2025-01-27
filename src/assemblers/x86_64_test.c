#include "x86_64.c"

#ifdef _WIN32
	#define NO_MINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#include <stdio.h>


#define TestSlice(...) do { \
		usize len = sizeof((u8[]){__VA_ARGS__}); \
		u8 *buff =(u8*)&((u8[]){__VA_ARGS__}); \
		if (are_slices_different(c.assembler_output_memory, buff, len)) { \
			fprintf(stderr, "Test at line %d failed.\nMemory difference:\n", __LINE__); \
			fprintf(stderr, "        Gen  Exp\n"); \
			for (usize i = 0; i < len; i += 1) { \
				fprintf(stderr, "(%04llu):  %02hhx   %02hhx\n", i, c.assembler_output_memory[i], buff[i]); \
			} \
			fputs("\n\n", stderr); \
		} \
		c.assember_output_memory_cursor = 0; \
	} while (0)


static bool are_slices_different(u8 *restrict p1, u8 *restrict p2, usize count) {
	for (usize i = 0; i < count; i += 1) {
		if (p1[i] != p2[i]) return true;
	}
	return false;
}


int main(void) {
	x86_64_AssemblerContext c = {0};
	c.assembler_output_memory_size = 4096;
#if defined(_WIN32)
	c.assembler_output_memory = VirtualAlloc(NULL, c.assembler_output_memory_size, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#elif
	#error "Not implemented in this platform."
#endif

	/* PUSH */
	x86_64_PushQReg64(&c, RAX);
	TestSlice(0x50);

	x86_64_PushQReg64(&c, R10);
	TestSlice(0x41, 0x52);

	x86_64_PushQReg64(&c, RBP);
	TestSlice(0x55);

	x86_64_PushWReg16(&c, AX);
	TestSlice(0x66, 0x50);

	x86_64_PushWReg16(&c, BP);
	TestSlice(0x66, 0x55);

	x86_64_PushWReg16(&c, R15W);
	TestSlice(0x66, 0x41, 0x57);

	x86_64_PushDImm32(&c, 65534);
	TestSlice(0x68, 0xFE, 0xFF, 0x00, 0x00);

	x86_64_PushWImm16(&c, 65534);
	TestSlice(0x66, 0x6A, 0xFE);

	x86_64_PushDImm32(&c, 420);
	TestSlice(0x68, 0xA4, 0x01, 0x00, 0x00);

	x86_64_PushWImm16(&c, 420);
	TestSlice(0x66, 0x68, 0xA4, 0x01);

	x86_64_PushDImm32(&c, 69);
	TestSlice(0x6a, 0x45);

	x86_64_PushWImm16(&c, 69);
	TestSlice(0x66, 0x6a, 0x45);

	x86_64_PushQRM64(&c, 420, RAX, R9, Scale1x);
	TestSlice(0x42, 0xFF, 0xB4, 0x08, 0xA4, 0x01, 0x00, 0x00);

	x86_64_PushQRM64(&c, -42, R14, RDI, Scale2x);
	TestSlice(0x41, 0xFF, 0x74, 0x7E, 0xD6);

	x86_64_PushQRM64(&c, 125, R13, R8, Scale4x);
	TestSlice(0x43, 0xFF, 0x74, 0x85, 0x7D);
	
	x86_64_PushQRM64(&c, -69420, RSI, RBX, Scale8x);
	TestSlice(0xFF, 0xB4, 0xDE, 0xD4, 0xF0, 0xFE, 0xFF);
	
	x86_64_PushWRM64(&c, 420, RDI, R15, Scale1x);
	TestSlice(0x66, 0x42, 0xFF, 0xB4, 0x3F, 0xA4, 0x01, 0x00, 0x00);

	x86_64_PushWRM64(&c, -42, R8, RSI, Scale2x);
	TestSlice(0x66, 0x41, 0xFF, 0x74, 0x70, 0xD6);

	x86_64_PushWRM64(&c, 125, R12, Reg64_None, Scale4x);
	TestSlice(0x66, 0x41, 0xFF, 0x74, 0x24, 0x7D);

	x86_64_PushWRM64(&c, -69420, RBX, RSI, Scale8x);
	TestSlice(0x66, 0xFF, 0xB4, 0xF3, 0xD4, 0xF0, 0xFE, 0xFF);

	x86_64_PushQRM32(&c, 420, EAX, R9D, Scale1x);
	TestSlice(0x67, 0x42, 0xFF, 0xB4, 0x08, 0xA4, 0x01, 0x00, 0x00);

	x86_64_PushQRM32(&c, -42, R14D, EDI, Scale2x);
	TestSlice(0x67, 0x41, 0xFF, 0x74, 0x7E, 0xD6);

	x86_64_PushQRM32(&c, 125, R13D, R8D, Scale4x);
	TestSlice(0x67, 0x43, 0xFF, 0x74, 0x85, 0x7D);

	x86_64_PushQRM32(&c, -69420, ESI, EBX, Scale8x);
	TestSlice(0x67, 0xFF, 0xB4, 0xDE, 0xD4, 0xF0, 0xFE, 0xFF);
	
	x86_64_PushWRM32(&c, 420, EDI, R15D, Scale1x);
	TestSlice(0x67, 0x66, 0x42, 0xFF, 0xB4, 0x3F, 0xA4, 0x01, 0x00, 0x00);

	x86_64_PushWRM32(&c, -42, R8D, ESI, Scale2x);
	TestSlice(0x67, 0x66, 0x41, 0xFF, 0x74, 0x70, 0xD6);

	x86_64_PushWRM32(&c, 125, R12D, R12D, Scale4x);
	TestSlice(0x67, 0x66, 0x43, 0xFF, 0x74, 0xA4, 0x7D);
	
	x86_64_PushWRM32(&c, -69420, EBX, Reg32_None, Scale8x);
	TestSlice(0x67, 0x66, 0xFF, 0xB3, 0xD4, 0xF0, 0xFE, 0xFF);


	/* POP */
	x86_64_PopQReg64(&c, RBX);
	TestSlice(0x5B);

	x86_64_PopQReg64(&c, R9);
	TestSlice(0x41, 0x59);

	x86_64_PopWReg16(&c, SI);
	TestSlice(0x66, 0x5E);

	x86_64_PopWReg16(&c, R13W);
	TestSlice(0x66, 0x41, 0x5D);

	x86_64_PopQRM64(&c, 525, RDI, RSI, Scale4x);
	TestSlice(0x8F, 0x84, 0xB7, 0x0D, 0x02, 0x00, 0x00);

	x86_64_PopWRM64(&c, -33, R15, R15, Scale8x);
	TestSlice(0x66, 0x43, 0x8F, 0x44, 0xFF, 0xDF);

	x86_64_PopQRM32(&c, 42, Reg32_None, R10D, Scale2x);
	TestSlice(0x67, 0x42, 0x8F, 0x04, 0x55, 0x2A, 0x00, 0x00, 0x00);

	x86_64_PopWRM32(&c, -4444, ESP, ECX, Scale2x);
	TestSlice(0x66, 0x67, 0x8F, 0x84, 0x4C, 0xA4, 0xEE, 0xFF, 0xFF);

	x86_64_PopWRM32(&c, 0, ESP, EBP, Scale4x);
	TestSlice(0x66, 0x67, 0x8F, 0x04, 0xAC);
	
	/* RET */
	x86_64_Ret(&c);
	TestSlice(0xC3);

}
