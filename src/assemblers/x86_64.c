#include "x86_64.h"

static void x86_64_WriteInstruction(x86_64_AssemberContext *c);

void x86_64_StartInstruction(x86_64_AssemberContext *c, u16 instrunction) {
	static const u8 ParamsPerInstruction[X86_64_INSTRUCTIONS_COUNT] = {
		[X86_64_PUSH] = 1,
		[X86_64_POP] = 1,
		[X86_64_RET] = 0,
		[X86_64_MOV] = 2,
	};

	assert(c->remaining_operands == 0);
	assert(instrunction < X86_64_INSTRUCTIONS_COUNT);
	u8 n_params = ParamsPerInstruction[instrunction];

	c->current_instruction = instrunction;
	c->remaining_operands = n_params;
	if (n_params == 0) {
		x86_64_WriteInstruction(c);
	}
}


void x86_64_AddReg(x86_64_AssemberContext *c, u8 reg) {

}


void x86_64_AddImmediate(x86_64_AssemberContext *c, u64 immediate) {

}


static void x86_64_WriteInstruction(x86_64_AssemberContext *c) {
	switch (c->current_instruction) {
		case X86_64_RET: {
			c->assembler_output_memory[c->assember_output_memory_cursor] = 0xC3;
			c->assember_output_memory_cursor += 1;
		}
	}
}
