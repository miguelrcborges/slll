#define SLLL_ASSEMBLER_X86_64_NO_MACROS
#include "x86_64.h"


force_inline void x86_64_WriteByte(x86_64_AssemblerContext *c, u8 byte) {
	assert(c->assember_output_memory_cursor < c->assembler_output_memory_size);
	c->assembler_output_memory[c->assember_output_memory_cursor] = byte;
	c->assember_output_memory_cursor += 1;
}

static void x86_64_WriteRM64Instruction(
		x86_64_AssemblerContext *c, 
		u8 first_byte, u8 opcode, 
		i32 offset, Reg64_Optional base, Reg64_Optional index, DisplacementScale scale) 
{
	assert(index.Value != RSP.Value);
	if (base.Value == Reg64_None.Value && index.Value == Reg64_None.Value) {
		x86_64_WriteByte(c, first_byte);
		x86_64_WriteByte(c, (opcode << 3) | 0b100);
		x86_64_WriteByte(c, 0x25);
		x86_64_WriteByte(c, (u8) ((u32)offset));
		x86_64_WriteByte(c, (u8) ((u32)offset >> 8));
		x86_64_WriteByte(c, (u8) ((u32)offset >> 16));
		x86_64_WriteByte(c, (u8) ((u32)offset >> 24));
	} else {
		// Small optimization if the user provides silly input
		if (unlikely(base.Value == Reg64_None.Value && index.Value != Reg64_None.Value && scale.Value == Scale1x.Value)) {
			Reg64_Optional tmp = base;
			base = index;
			index = tmp;
		}
		bool can_offset_be_single_byte = offset >= -128 && offset <= 127;
		bool is_base_none = base.Value == Reg64_None.Value;
		bool is_index_none = index.Value == Reg64_None.Value;
		bool is_base_extension = (base.Value >= R8.Value) && (base.Value <= R15.Value);
		bool is_index_extension = (index.Value >= R8.Value) && (index.Value <= R15.Value);
		bool is_rbp_base = base.Value == RBP.Value;
		bool write_offset = is_rbp_base || offset != 0;
		bool write_single_byte = can_offset_be_single_byte && !is_base_none;
		if (is_base_extension || is_index_extension) {
			u8 byte_to_write = is_base_extension ? 0x41 : 0x42;
			byte_to_write = is_base_extension && is_index_extension ? 0x43 : byte_to_write;
			base.Value = is_base_extension ? base.Value - R8.Value : base.Value;
			index.Value = is_index_extension ? index.Value - R8.Value : index.Value;
			x86_64_WriteByte(c, byte_to_write);
		}
		x86_64_WriteByte(c, first_byte);
		if (!is_index_none) {
			u8 rm_byte = (opcode << 3) | 0b100;
			if (write_offset && !is_base_none) {
				if (write_single_byte) {
					rm_byte |= 0b01 << 6;
				} else {
					rm_byte |= 0b10 << 6;
				}
			}
			x86_64_WriteByte(c, rm_byte);
			u8 sib_byte = scale.Value << 6;
			sib_byte |= (index.Value << 3); // Note that regs >= R8 are set back to RAX-RDI range
			sib_byte |= is_base_none ? 0b101 : base.Value;
			x86_64_WriteByte(c, sib_byte);
		} else {
			u8 modrm_byte = opcode << 3;
			if (write_offset) {
				if (write_single_byte) {
					modrm_byte |= 0b01 << 6;
				} else {
					modrm_byte |= 0b10 << 6;
				}
			}
			modrm_byte |= base.Value;
			x86_64_WriteByte(c, modrm_byte);
			if (base.Value == 0b100) {         // R12 alone encodes also SIB, even with 0x41 prefix.
				x86_64_WriteByte(c, 0x24);      // Has to be set as a base of the SIB byte
			}
		}
		if (write_offset) {
			if (write_single_byte) {
				x86_64_WriteByte(c, (u8) ((u32)offset));
			} else {
				x86_64_WriteByte(c, (u8) ((u32)offset));
				x86_64_WriteByte(c, (u8) ((u32)offset >> 8));
				x86_64_WriteByte(c, (u8) ((u32)offset >> 16));
				x86_64_WriteByte(c, (u8) ((u32)offset >> 24));
			}
		}
	}
}


void x86_64_PushQReg64(x86_64_AssemblerContext *c, Reg64 r) {
	u8 op_byte = r.Value + 0x50;
	if (r.Value >= R8.Value) {
		x86_64_WriteByte(c, 0x41);
		op_byte -= R8.Value;
	}
	x86_64_WriteByte(c, op_byte);
}

void x86_64_PushWReg16(x86_64_AssemblerContext *c, Reg16 r) {
	x86_64_WriteByte(c, 0x66);
	Reg64 new_reg = { r.Value };
	x86_64_PushQReg64(c, new_reg);
}

void x86_64_PushDImm32(x86_64_AssemblerContext *c, u32 value) {
	u8 cond1 = value <= ((u32)127);
	u8 cond2 = value >= ((u32)-128);
	if (cond1 || cond2) {
		x86_64_WriteByte(c, 0x6A);
		x86_64_WriteByte(c, (u8) value);
	} else {
		x86_64_WriteByte(c, 0x68);
		x86_64_WriteByte(c, (u8) value);
		x86_64_WriteByte(c, (u8) (value >> 8));
		x86_64_WriteByte(c, (u8) (value >> 16));
		x86_64_WriteByte(c, (u8) (value >> 24));
	}
}

void x86_64_PushWImm16(x86_64_AssemblerContext *c, u16 value) {
	u8 cond1 = value <= ((u16)127);
	u8 cond2 = value >= ((u16)-128);
	x86_64_WriteByte(c, 0x66);
	if (cond1 || cond2) {
		x86_64_WriteByte(c, 0x6A);
		x86_64_WriteByte(c, (u8) value);
	} else {
		x86_64_WriteByte(c, 0x68);
		x86_64_WriteByte(c, (u8) value);
		x86_64_WriteByte(c, (u8) (value >> 8));
	}
}

void (x86_64_PushQRM64)(x86_64_AssemblerContext *c, i32 offset, Reg64_Optional base, Reg64_Optional index, DisplacementScale scale) {
	x86_64_WriteRM64Instruction(c, 0xFF, 6, offset, base, index, scale);
}

void (x86_64_PushWRM64)(x86_64_AssemblerContext *c, i32 offset, Reg64_Optional base, Reg64_Optional index, DisplacementScale scale) {
	x86_64_WriteByte(c, 0x66);
	x86_64_PushQRM64(c, offset, base, index, scale);
}

void (x86_64_PushQRM32)(x86_64_AssemblerContext *c, i32 offset, Reg32_Optional base, Reg32_Optional index, DisplacementScale scale) {
	x86_64_WriteByte(c, 0x67);
	Reg64_Optional new_base = { base.Value };
	Reg64_Optional new_index = { index.Value };
	x86_64_PushQRM64(c, offset, new_base, new_index, scale);
}

void (x86_64_PushWRM32)(x86_64_AssemblerContext *c, i32 offset, Reg32_Optional base, Reg32_Optional index, DisplacementScale scale) {
	x86_64_WriteByte(c, 0x67);
	Reg64 new_base = { base.Value };
	Reg64 new_index = { index.Value };
	x86_64_PushWRM64(c, offset, new_base, new_index, scale);
}


void x86_64_PopQReg64(x86_64_AssemblerContext *c, Reg64 r) {
	u8 op_byte = r.Value + 0x58;
	if (r.Value >= R8.Value) {
		x86_64_WriteByte(c, 0x41);
		op_byte -= R8.Value;
	}
	x86_64_WriteByte(c, op_byte);
}

void x86_64_PopWReg16(x86_64_AssemblerContext *c, Reg16 r) {
	Reg64 new_reg = { r.Value };
	x86_64_WriteByte(c, 0x66);
	x86_64_PopQReg64(c, new_reg);
}

void (x86_64_PopQRM64)(x86_64_AssemblerContext *c, i32 offset, Reg64_Optional base, Reg64_Optional index, DisplacementScale scale) {
	x86_64_WriteRM64Instruction(c, 0x8F, 0, offset, base, index, scale);
}

void (x86_64_PopWRM64)(x86_64_AssemblerContext *c, i32 offset, Reg64_Optional base, Reg64_Optional index, DisplacementScale scale) {
	x86_64_WriteByte(c, 0x66);
	x86_64_PopQRM64(c, offset, base, index, scale);
}

void (x86_64_PopQRM32)(x86_64_AssemblerContext *c, i32 offset, Reg32_Optional base, Reg32_Optional index, DisplacementScale scale) {
	Reg64_Optional new_base = { base.Value };
	Reg64_Optional new_index = { index.Value };
	x86_64_WriteByte(c, 0x67);
	x86_64_PopQRM64(c, offset, new_base, new_index, scale);
}

void (x86_64_PopWRM32)(x86_64_AssemblerContext *c, i32 offset, Reg32_Optional base, Reg32_Optional index, DisplacementScale scale) {
	x86_64_WriteByte(c, 0x66);
	x86_64_PopQRM32(c, offset, base, index, scale);
}


void x86_64_Ret(x86_64_AssemblerContext *c) {
	x86_64_WriteByte(c, 0xC3);
}
