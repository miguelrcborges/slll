#include "x86_64.h"

force_inline x86_64_WriteByte(x86_64_AssemblerContext *c, u8 byte) {
	assert(c->assember_output_memory_cursor < c->assembler_output_memory_size);
	c->assembler_output_memory[c->assember_output_memory_cursor] = byte;
	c->assember_output_memory_cursor += 1;
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

void x86_64_PushQRM64(x86_64_AssemblerContext *c, i32 offset, Reg64 base, Reg64 index, DisplacementScale scale) {
	assert(index.Value != RSP.Value);
	if (base.Value == Reg64_None.Value && index.Value == Reg64_None.Value) {
		x86_64_WriteByte(c, 0xFF);
		x86_64_WriteByte(c, 0x34);
		x86_64_WriteByte(c, 0x25);
		x86_64_WriteByte(c, (u8) ((u32)offset));
		x86_64_WriteByte(c, (u8) ((u32)offset >> 8));
		x86_64_WriteByte(c, (u8) ((u32)offset >> 16));
		x86_64_WriteByte(c, (u8) ((u32)offset >> 24));
	} else {
		u8 is_base_extension = (base.Value >= R8.Value) && (base.Value <= R15.Value);
		u8 is_index_extension = (index.Value >= R8.Value) && (index.Value <= R15.Value);
		if (is_base_extension || is_index_extension) {
			u8 byte_to_write = is_base_extension ? 0x41 : 0x42;
			byte_to_write = is_base_extension && is_index_extension ? 0x43 : byte_to_write;
			base.Value = is_base_extension ? base.Value - R8.Value : base.Value;
			index.Value = is_index_extension ? index.Value - R8.Value : index.Value;
			x86_64_WriteByte(c, byte_to_write);
		}
		x86_64_WriteByte(c, 0xFF);
		u8 can_offset_be_single_byte = offset >= -128 && offset <= 127;
		if (index.Value != Reg64_None.Value) {
			if (offset == 0) {
				x86_64_WriteByte(c, 0x34);
			} else if (can_offset_be_single_byte) {
				x86_64_WriteByte(c, 0x74);
			} else {
				x86_64_WriteByte(c, 0xB4);
			}
			u8 sib_byte = scale.Value << 6;
			sib_byte |= (index.Value << 3); // Note that regs >= R8 are set back to RAX-RDI range
			sib_byte |= base.Value;
			x86_64_WriteByte(c, sib_byte);
		} else {
			u8 modrm_byte = 0b00110000; // start by setting PUSH op
			if (offset != 0) {
				if (can_offset_be_single_byte) {
					modrm_byte |= 0b01 << 6;
				} else {
					modrm_byte |= 0b10 << 6;
				}
			}
			modrm_byte |= base.Value;
		}
		if (offset != 0) {
			if (can_offset_be_single_byte) {
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

void x86_64_PushWRM64(x86_64_AssemblerContext *c, i32 offset, Reg64 base, Reg64 index, DisplacementScale scale) {
	x86_64_WriteByte(c, 0x66);
	x86_64_PushQRM64(c, offset, base, index, scale);
}

void x86_64_PushQRM32(x86_64_AssemblerContext *c, i32 offset, Reg32 base, Reg32 index, DisplacementScale scale) {
	x86_64_WriteByte(c, 0x67);
	Reg64 new_base = { base.Value };
	Reg64 new_index = { index.Value };
	x86_64_PushQRM64(c, offset, new_base, new_index, scale);
}

void x86_64_PushWRM32(x86_64_AssemblerContext *c, i32 offset, Reg32 base, Reg32 index, DisplacementScale scale) {
	x86_64_WriteByte(c, 0x67);
	Reg64 new_base = { base.Value };
	Reg64 new_index = { index.Value };
	x86_64_PushWRM64(c, offset, new_base, new_index, scale);
}


void x86_64_Ret(x86_64_AssemblerContext *c) {
	x86_64_WriteByte(c, 0xC3);
}
