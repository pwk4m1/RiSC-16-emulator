/*
 * BSD 3-Clause License
 * 
 * Copyright (c) 2019, k4m1 <k4m1@protonmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#define __USE_BSD
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>

#include <cpu.h>
#include <opcodes.h>

#define OP_OFF		1110000000000000
#define REG1_OFF	0001110000000000
#define REG2_OFF	0000001110000000
#define REG3_OFF	0000000000000111
#define PRI_IMM_OFF	0000000001111111
#define RI_IMM_OFF	0000001111111111

#define OPC		0
#define R1		1
#define R2		2
#define R3		3
#define PRI_IMM		4
#define RI_IMM		5

/*
 * do_XXX functions are used to perform operations supported by
 * the RiSC-16 cpu.
 *
 * They do _not_ return any values, but set cpu_s registers.
 */

void
do_add(uint16_t *r1, uint16_t *r2, uint16_t *r3)
{
	r1[0] = (uint16_t)(r2[0] + r3[0]);
}

void
do_addi(uint16_t *r1, uint16_t *r2, uint16_t imm)
{
	r1[0] = (uint16_t)(r2[0] + imm);
}

void
do_nand(uint16_t *r1, uint16_t *r2, uint16_t *r3)
{
	if (r2[0] == 0 || r3[0] == 0) {
		r1[0] = 1;
	} else {
		r1[0] = 0;
	}
}

void
do_lui(uint16_t *r1, uint16_t imm)
{
	imm = imm >> 6;
	r1[0] = imm;
}

void
do_sw(struct cpu_s *cpu, uint16_t *r1, uint16_t *r2, uint16_t imm)
{
	uint16_t ptr;
	ptr = r2[0];
	ptr = ptr << 8;
	ptr += imm;
	cpu->mem[ptr] = r1[0];
}

void
do_lw(struct cpu_s *cpu, uint16_t *r1, uint16_t *r2, uint16_t imm)
{
	uint16_t ptr;
	ptr = r2[0];
	ptr = ptr << 8;
	ptr += imm;
	r1[0] = cpu->mem[ptr];
}

void
do_beq(struct cpu_s *cpu, uint16_t *r1, uint16_t *r2, uint16_t imm)
{
	uint16_t addr;
	if (r1[0] == r2[0]) {
		addr = cpu->pc + 1;
		addr += imm;
		cpu->pc = addr;
	}
}

void
do_jalr(struct cpu_s *cpu, uint16_t *r1, uint16_t *r2)
{
	r1[0] = (cpu->pc + 1);
	cpu->pc = r2[0];
}

/* Helper function for cpu_main to get opcode/r1/r2/r3/imm */
int
get_part(uint16_t chunk, int type)
{
	if (type == OPC) return (chunk & OP_OFF);
	if (type == R1) return (chunk & REG1_OFF);
	if (type == R2) return (chunk & REG2_OFF);
	if (type == R3) return (chunk & REG3_OFF);
	if (type == PRI_IMM) return (chunk & PRI_IMM_OFF);
	if (type == RI_IMM) return (chunk & RI_IMM_OFF);
	return 0xff;
}

/* Helper function for cpu_main to get cpu_s register based on input. */
uint16_t *
get_reg(struct cpu_s *cpu, int part)
{
	uint16_t *ret;
	switch (part) {
		case (reg0):
			ret = 0;
			break;
		case (reg1):
			ret = cpu->r1;
			break;
		case (reg2):
			ret = cpu->r2;
			break;
		case (reg3):
			ret = cpu->r3;
			break;
		case (reg4):
			ret = cpu->r4;
			break;
		case (reg5):
			ret = cpu->r5;
			break;
		case (reg6):
			ret = cpu->r6;
			break;
		case (reg7):
			ret = cpu->r7;
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}

/*
 * CPU main loop
 */
void
cpu_main(struct cpu_s *cpu, uint16_t *code)
{
	uint16_t *r1, *r2, *r3;
	int opcode;
	uint16_t pri_imm;
	uint16_t ri_imm;
	uint16_t chunk;

	while (cpu->pc < 0x200) {
		chunk = code[cpu->pc];
		opcode = get_part(chunk, OPC);
		r1 = get_reg(cpu, get_part(chunk, R1));
		r2 = get_reg(cpu, get_part(chunk, R2));
		r3 = get_reg(cpu, get_part(chunk, R3));
		pri_imm = get_part(chunk, PRI_IMM);
		ri_imm = get_part(chunk, RI_IMM_OFF);

		switch (opcode) {
			case (op_add):
				do_add(r1, r2, r3);
				break;
			case (op_addi):
				do_addi(r1, r2, pri_imm);
				break;
			case (op_nand):
				do_nand(r1, r2, r3);
				break;
			case (op_lui):
				do_lui(r1, ri_imm);
				break;
			case (op_sw):
				do_sw(cpu, r1, r2, pri_imm);
				break;
			case (op_lw):
				do_lw(cpu, r1, r2, pri_imm);
				break;
			case (op_beq):
				do_beq(cpu, r1, r2, pri_imm);
				break;
			case (op_jalr):
				do_jalr(cpu, r1, r2);
				break;
			default:
				goto sig_ill;
				break;
		}
		cpu->pc++;
	}
sig_ill:
	printf("SIG-ILL\n");
	return;

}


