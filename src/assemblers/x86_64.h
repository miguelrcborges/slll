#ifndef SLLL_ASSEMBERS_X86_64
#include "../c.h"

typedef struct {
	struct {
		u8 kind;
		u64 value;
	} operands[3];
	u8 *assembler_output_memory;
	usize assembler_output_memory_size;
	usize assember_output_memory_cursor;
	u16 current_instruction;
	u8 remaining_operands;
} x86_64_AssemberContext;

void x86_64_StartInstruction(x86_64_AssemberContext *c, u16 instrunction);
void x86_64_AddReg(x86_64_AssemberContext *c, u8 reg);
void x86_64_AddImmediate(x86_64_AssemberContext *c, u64 immediate);

enum x86_64_INSTRUCTIONS {
	X86_64_PUSH,
	X86_64_POP,
	X86_64_RET,
	X86_64_MOV,

	X86_64_INSTRUCTIONS_COUNT
};

enum X86_64_REGISTERS {
	X86_64_RAX,
	X86_64_RBX,
	X86_64_RCX,
	X86_64_RDX,
	X86_64_RDI,
	X86_64_RSI,
	X86_64_RBP,
	X86_64_RSP,
	X86_64_R8,
	X86_64_R9,
	X86_64_R10,
	X86_64_R11,
	X86_64_R12,
	X86_64_R13,
	X86_64_R14,
	X86_64_R15,

	X86_64_EAX,
	X86_64_EBX,
	X86_64_ECX,
	X86_64_EDX,
	X86_64_EDI,
	X86_64_ESI,
	X86_64_EBP,
	X86_64_ESP,
	X86_64_R8D,
	X86_64_R9D,
	X86_64_R10D,
	X86_64_R11D,
	X86_64_R12D,
	X86_64_R13D,
	X86_64_R14D,
	X86_64_R15D,
};


#define SLLL_ASSEMBERS_X86_64
#endif
