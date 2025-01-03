#include "x86_64.h"

enum x86_64_OPERANDS_TYPES {
	X86_64_REGISTER,
	X86_64_IMMEDIATE,
};

static void x86_64_WriteInstruction(x86_64_AssemblerContext *c);
static void force_inline x86_64_WriteByte(x86_64_AssemblerContext *c, u8 byte) {
	assert(c->assember_output_memory_cursor < c->assembler_output_memory_size);
	c->assembler_output_memory[c->assember_output_memory_cursor] = byte;
	c->assember_output_memory_cursor += 1;
}

void x86_64_StartInstruction(x86_64_AssemblerContext *c, u16 instrunction) {
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
	c->current_operand = 0;
	c->remaining_operands = n_params;
	if (n_params == 0) {
		x86_64_WriteInstruction(c);
	}
}


void x86_64_AddReg(x86_64_AssemblerContext *c, u8 reg) {
	assert(c->remaining_operands > 0);
	c->operands[c->current_operand].kind = X86_64_REGISTER;
	c->operands[c->current_operand].value = reg;
	if (x86_64_IsReg64(reg)) {
		c->operands[c->current_operand].size = X86_64_QWORD;
	} else if (x86_64_IsReg32(reg)) {
		c->operands[c->current_operand].size = X86_64_DWORD;
	} else if (x86_64_IsReg16(reg)) {
		c->operands[c->current_operand].size = X86_64_WORD;
	} else {
		assert(!"Invalid register.");
	}

	c->current_operand += 1;
	c->remaining_operands -= 1;
	if (c->remaining_operands == 0) {
		x86_64_WriteInstruction(c);
	}
} 


void x86_64_AddImmediate(x86_64_AssemblerContext *c, u64 immediate, u8 immediate_width) {
	assert(c->remaining_operands > 0);
	c->operands[c->current_operand].kind = X86_64_IMMEDIATE;
	c->operands[c->current_operand].value = immediate;
	c->operands[c->current_operand].size = immediate_width;
	c->current_operand += 1;
	c->remaining_operands -= 1;
	if (c->remaining_operands == 0) {
		x86_64_WriteInstruction(c);
	}
}


static void x86_64_WriteInstruction(x86_64_AssemblerContext *c) {
	switch (c->current_instruction) {
		case X86_64_PUSH: {
			if (c->operands[0].kind == X86_64_REGISTER) {
				usize reg = c->operands[0].value;
				usize reg_size = c->operands[0].size;
				if (reg_size == X86_64_WORD || reg_size == X86_64_QWORD) {
					if (reg_size == X86_64_WORD) {
						x86_64_WriteByte(c, 0x66);
						reg -= (X86_64_AX - X86_64_RAX);
					}
					if (reg >= X86_64_R8) {
						x86_64_WriteByte(c, 0x41);
						reg -= (X86_64_R8 - X86_64_RAX);
					}
					x86_64_WriteByte(c, (u8) (0x50+reg));
				} else {
					assert(!"Invalid register to push on x86_64.");
				}

			} else if (c->operands[0].kind == X86_64_IMMEDIATE) {
				if (c->operands[0].size == X86_64_WORD) {
					x86_64_WriteByte(c, 0x66);
				} else if (c->operands[0].size != X86_64_DWORD) {
					assert(!"Invalid size of this instrunction.");
				}

				u8 can_be_byte_1 = c->operands[0].value < ((u64)1 << 7);
				u8 can_be_byte_2;
				if (c->operands[0].size == X86_64_WORD) {
					can_be_byte_2 = c->operands[0].value > ((u64)((u16)-127));
				} else {
					can_be_byte_2 = c->operands[0].value > ((u64)((u32)-127));
				}
				if (can_be_byte_1 || can_be_byte_2) {
					x86_64_WriteByte(c, 0x6A);
					x86_64_WriteByte(c, (u8)c->operands[0].value);
				} else if (c->operands[0].size == X86_64_WORD) {
					x86_64_WriteByte(c, 0x68);
					x86_64_WriteByte(c, (u8)c->operands[0].value);
					x86_64_WriteByte(c, (u8)(c->operands[0].value >> 8));
				} else {
					x86_64_WriteByte(c, 0x68);
					x86_64_WriteByte(c, (u8)c->operands[0].value);
					x86_64_WriteByte(c, (u8)(c->operands[0].value >> 8));
					x86_64_WriteByte(c, (u8)(c->operands[0].value >> 16));
					x86_64_WriteByte(c, (u8)(c->operands[0].value >> 24));
				}
			} else {
				assert(!"Invalid parameter kind.");
			}
			break;
		}
		
		case X86_64_POP: {
			if (c->operands[0].kind == X86_64_REGISTER) {
				usize reg = c->operands[0].value;
				usize reg_size = c->operands[0].size;
				if (reg_size == X86_64_WORD || reg_size == X86_64_QWORD) {
					if (reg_size == X86_64_WORD) {
						x86_64_WriteByte(c, 0x66);
						reg -= (X86_64_AX - X86_64_RAX);
					}
					if (reg >= X86_64_R8) {
						x86_64_WriteByte(c, 0x41);
						reg -= (X86_64_R8 - X86_64_RAX);
					}
					x86_64_WriteByte(c, (u8) (0x58+reg));
				} else {
					assert(!"Invalid register to pop on x86_64.");
				}
			} else {
				assert(!"Invalid parameter kind.");
			}
			break;
		}

		case X86_64_RET: {
			x86_64_WriteByte(c, 0xC3);
			break;
		}

		default: {
			assert(!"Invalid Instrunction");
		}
	}
}
