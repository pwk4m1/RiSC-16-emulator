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

#if !defined(__OPCODES_H__)
#define __OPCODES_H__

/* regA = regB + regC */
#define op_add		00000000

/* regA = regB + imm */
#define op_addi		00000001

/* regA = regB nand regC */
#define op_nand		00000010

/* regA[6:16] = imm[0:10]. regA[0:5] = 0 */
#define op_lui		00000011

/* word [regB+imm] = regA */
#define op_sw		00000100

/* regA = word [regB+imm] */
#define op_lw		00000101

/* if (regA == regB) goto PC + 1 + imm */
#define op_beq		00000110

/* regA = PC + 1, goto regB */
#define op_jalr		00000111

/* registers */
#define reg0		op_add
#define reg1		op_addi
#define reg2		op_nand
#define reg3		op_lui
#define reg4		op_sw
#define reg5		op_lw
#define reg6		op_beq
#define reg7		op_jalr

#endif /* __OPCODES_H__*/

