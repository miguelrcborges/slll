#ifndef SLLL_ASSEMBERS_X86_64
#include "../c.h"

typedef struct {
	struct {
		u8 kind;
		u8 size;
		u64 value;
	} operands[3];
	u8 *assembler_output_memory;
	usize assembler_output_memory_size;
	usize assember_output_memory_cursor;
	u16 current_instruction;
	u8 current_operand;
	u8 remaining_operands;
} x86_64_AssemblerContext;

void x86_64_StartInstruction(x86_64_AssemblerContext *c, u16 instrunction);
void x86_64_AddReg(x86_64_AssemblerContext *c, u8 reg);
void x86_64_AddImmediate(x86_64_AssemblerContext *c, u64 immediate, u8 immediate_width);


enum x86_64_INSTRUCTIONS {
	X86_64_PUSH,
	X86_64_POP,
	X86_64_RET,
	X86_64_MOV,

	X86_64_INSTRUCTIONS_COUNT
};

enum X86_64_REGISTERS {
	X86_64_RAX,
	X86_64_RCX,
	X86_64_RDX,
	X86_64_RBX,
	X86_64_RSP,
	X86_64_RBP,
	X86_64_RSI,
	X86_64_RDI,
	X86_64_R8,
	X86_64_R9,
	X86_64_R10,
	X86_64_R11,
	X86_64_R12,
	X86_64_R13,
	X86_64_R14,
	X86_64_R15,

	X86_64_EAX,
	X86_64_ECX,
	X86_64_EDX,
	X86_64_EBX,
	X86_64_ESP,
	X86_64_EBP,
	X86_64_ESI,
	X86_64_EDI,
	X86_64_R8D,
	X86_64_R9D,
	X86_64_R10D,
	X86_64_R11D,
	X86_64_R12D,
	X86_64_R13D,
	X86_64_R14D,
	X86_64_R15D,

	X86_64_AX,
	X86_64_CX,
	X86_64_DX,
	X86_64_BX,
	X86_64_SP,
	X86_64_BP,
	X86_64_SI,
	X86_64_DI,
	X86_64_R8W,
	X86_64_R9W,
	X86_64_R10W,
	X86_64_R11W,
	X86_64_R12W,
	X86_64_R13W,
	X86_64_R14W,
	X86_64_R15W,
};

#define x86_64_IsReg64(reg) ((reg)>=X86_64_RAX && (reg)<=X86_64_R15)
#define x86_64_IsReg32(reg) ((reg)>=X86_64_EAX && (reg)<=X86_64_R15D)
#define x86_64_IsReg16(reg) ((reg)>=X86_64_AX && (reg)<=X86_64_R15W)


enum X86_64_SIZES {
	X86_64_BYTE,
	X86_64_WORD,
	X86_64_DWORD,
	X86_64_QWORD,
};

#define SLLL_ASSEMBERS_X86_64
#endif
