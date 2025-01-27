#ifndef SLLL_ASSEMBERS_X86_64
#include "../c.h"

typedef struct {
	u8 *assembler_output_memory;
	usize assembler_output_memory_size;
	usize assember_output_memory_cursor;
} x86_64_AssemblerContext;


typedef struct {
	u8 Value;
} Reg64;

typedef struct {
	u8 Value;
} Reg64_Optional;

#define Reg64OrNone(x) _Generic((x), \
	Reg64: ((Reg64_Optional) { x.Value }), \
	default: (x))

typedef struct {
	u8 Value;
} Reg32;

typedef struct {
	u8 Value;
} Reg32_Optional;

#define Reg32OrNone(x) _Generic((x), \
	Reg32: ((Reg32_Optional) { x.Value }), \
	default: (x))

typedef struct {
	u8 Value;
} Reg16;

typedef struct {
	u8 Value;
} Reg16_Optional;

#define Reg16OrNone(x) _Generic((x), \
	Reg16_Optional: ((Reg16_Optional) { x.Value }), \
	default: (x))

typedef struct {
	u8 Value;
} Reg8;

typedef struct {
	u8 Value;
} Reg8_Optional;

#define Reg8OrNone(x) _Generic((x), \
	Reg8_Optional: ((Reg8_Optional) { x.Value }), \
	default: (x))


typedef struct {
	u8 Value;
} DisplacementScale;


static const Reg64 RAX = {  0 };
static const Reg64 RCX = {  1 };
static const Reg64 RDX = {  2 };
static const Reg64 RBX = {  3 };
static const Reg64 RSP = {  4 };
static const Reg64 RBP = {  5 };
static const Reg64 RSI = {  6 };
static const Reg64 RDI = {  7 };
static const Reg64 R8  = {  8 };
static const Reg64 R9  = {  9 };
static const Reg64 R10 = { 10 };
static const Reg64 R11 = { 11 };
static const Reg64 R12 = { 12 };
static const Reg64 R13 = { 13 };
static const Reg64 R14 = { 14 };
static const Reg64 R15 = { 15 };
static const Reg64_Optional Reg64_None = { 16 };


static const Reg32 EAX  = {  0 };
static const Reg32 ECX  = {  1 };
static const Reg32 EDX  = {  2 };
static const Reg32 EBX  = {  3 };
static const Reg32 ESP  = {  4 };
static const Reg32 EBP  = {  5 };
static const Reg32 ESI  = {  6 };
static const Reg32 EDI  = {  7 };
static const Reg32 R8D  = {  8 };
static const Reg32 R9D  = {  9 };
static const Reg32 R10D = { 10 };
static const Reg32 R11D = { 11 };
static const Reg32 R12D = { 12 };
static const Reg32 R13D = { 13 };
static const Reg32 R14D = { 14 };
static const Reg32 R15D = { 15 };
static const Reg32_Optional Reg32_None = { 16 };


static const Reg16 AX   = {  0 };
static const Reg16 CX   = {  1 };
static const Reg16 DX   = {  2 };
static const Reg16 BX   = {  3 };
static const Reg16 SP   = {  4 };
static const Reg16 BP   = {  5 };
static const Reg16 SI   = {  6 };
static const Reg16 DI   = {  7 };
static const Reg16 R8W  = {  8 };
static const Reg16 R9W  = {  9 };
static const Reg16 R10W = { 10 };
static const Reg16 R11W = { 11 };
static const Reg16 R12W = { 12 };
static const Reg16 R13W = { 13 };
static const Reg16 R14W = { 14 };
static const Reg16 R15W = { 15 };
static const Reg16_Optional Reg16_None = { 16 };


static const Reg8 AL   = {  0 };
static const Reg8 CL   = {  1 };
static const Reg8 DL   = {  2 };
static const Reg8 BL   = {  3 };
static const Reg8 AH   = {  4 };
static const Reg8 CH   = {  5 };
static const Reg8 DH   = {  6 };
static const Reg8 BH   = {  7 };
static const Reg8 R8B  = {  8 };
static const Reg8 R9B  = {  9 };
static const Reg8 R10B = { 10 };
static const Reg8 R11B = { 11 };
static const Reg8 R12B = { 12 };
static const Reg8 R13B = { 13 };
static const Reg8 R14B = { 14 };
static const Reg8 R15B = { 15 };
static const Reg8_Optional Reg8_None = { 16 };


static const DisplacementScale Scale1x = { 0 };
static const DisplacementScale Scale2x = { 1 };
static const DisplacementScale Scale4x = { 2 };
static const DisplacementScale Scale8x = { 3 };


void x86_64_MovQReg64Reg64(x86_64_AssemblerContext *c, Reg64 dest, Reg64 from);
void x86_64_MovDReg32Reg32(x86_64_AssemblerContext *c, Reg32 dest, Reg32 from);
void x86_64_MovWReg16Reg16(x86_64_AssemblerContext *c, Reg16 dest, Reg16 from);
void x86_64_MovBReg8Reg8(x86_64_AssemblerContext *c, Reg8 dest, Reg8 from);

void x86_64_PopQReg64(x86_64_AssemblerContext *c, Reg64 r);
void x86_64_PopWReg16(x86_64_AssemblerContext *c, Reg16 r);
void x86_64_PopQRM64(x86_64_AssemblerContext *c, i32 offset, Reg64_Optional base, Reg64_Optional index, DisplacementScale scale);
#define x86_64_PopQRM64(c, off, base, index, scale) x86_64_PopQRM64(c, off, Reg64OrNone(base), Reg64OrNone(index), scale)
void x86_64_PopWRM64(x86_64_AssemblerContext *c, i32 offset, Reg64_Optional base, Reg64_Optional index, DisplacementScale scale);
#define x86_64_PopWRM64(c, off, base, index, scale) x86_64_PopWRM64(c, off, Reg64OrNone(base), Reg64OrNone(index), scale)
void x86_64_PopQRM32(x86_64_AssemblerContext *c, i32 offset, Reg32_Optional base, Reg32_Optional index, DisplacementScale scale);
#define x86_64_PopQRM32(c, off, base, index, scale) x86_64_PopQRM32(c, off, Reg32OrNone(base), Reg32OrNone(index), scale)
void x86_64_PopWRM32(x86_64_AssemblerContext *c, i32 offset, Reg32_Optional base, Reg32_Optional index, DisplacementScale scale);
#define x86_64_PopWRM32(c, off, base, index, scale) x86_64_PopWRM32(c, off, Reg32OrNone(base), Reg32OrNone(index), scale)

void x86_64_PushQReg64(x86_64_AssemblerContext *c, Reg64 r);
void x86_64_PushWReg16(x86_64_AssemblerContext *c, Reg16 r);
void x86_64_PushQImm32(x86_64_AssemblerContext *c, u32 value);
void x86_64_PushWImm16(x86_64_AssemblerContext *c, u16 value);
void x86_64_PushQRM64(x86_64_AssemblerContext *c, i32 offset, Reg64_Optional base, Reg64_Optional index, DisplacementScale scale);
#define x86_64_PushQRM64(c, off, base, index, scale) x86_64_PushQRM64(c, off, Reg64OrNone(base), Reg64OrNone(index), scale)
void x86_64_PushWRM64(x86_64_AssemblerContext *c, i32 offset, Reg64_Optional base, Reg64_Optional index, DisplacementScale scale);
#define x86_64_PushWRM64(c, off, base, index, scale) x86_64_PushWRM64(c, off, Reg64OrNone(base), Reg64OrNone(index), scale)
void x86_64_PushQRM32(x86_64_AssemblerContext *c, i32 offset, Reg32_Optional base, Reg32_Optional index, DisplacementScale scale);
#define x86_64_PushQRM32(c, off, base, index, scale) x86_64_PushQRM32(c, off, Reg32OrNone(base), Reg32OrNone(index), scale)
void x86_64_PushWRM32(x86_64_AssemblerContext *c, i32 offset, Reg32_Optional base, Reg32_Optional index, DisplacementScale scale);
#define x86_64_PushWRM32(c, off, base, index, scale) x86_64_PushWRM32(c, off, Reg32OrNone(base), Reg32OrNone(index), scale)

void x86_64_Ret(x86_64_AssemblerContext *c);

#define SLLL_ASSEMBERS_X86_64
#endif
