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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include <cpu.h>

/* helper for mallocs */
#define malloc_reg() (uint16_t *)malloc(2);
#define MAX_SIZE 0x200

/*
 * initialize cpu_s structure.
 */
int
cpu_s_init(struct cpu_s *cpu, int mem_size)
{
	cpu = (struct cpu_s *)malloc(sizeof(struct cpu_s));
	if (!cpu)
		return -1;

	cpu->r1 = malloc_reg();
	cpu->r2 = malloc_reg();
	cpu->r3 = malloc_reg();
	cpu->r4 = malloc_reg();
	cpu->r5 = malloc_reg();
	cpu->r6 = malloc_reg();
	cpu->r7 = malloc_reg();

	if (!cpu->r1 || !cpu->r2 || !cpu->r3 ||
			!cpu->r4 || !cpu->r5 ||
			!cpu->r6 || !cpu->r7)
	{
		goto	cleanup_err;
	}

	cpu->pc = 0;
	cpu->mem = (uint16_t *)malloc(mem_size);
	if (!cpu->mem)
		goto	cleanup_err;
	memset(cpu->mem, 0, 0x200);

	return 0;
cleanup_err:
	if (cpu->r7) free(cpu->r7);
	if (cpu->r6) free(cpu->r6);
	if (cpu->r5) free(cpu->r5);
	if (cpu->r4) free(cpu->r4);
	if (cpu->r3) free(cpu->r3);
	if (cpu->r2) free(cpu->r2);
	if (cpu->r1) free(cpu->r1);
	if (cpu) free(cpu);
	return -1;
}

void
cpu_s_fini(struct cpu_s *cpu)
{
	free(cpu->r7);
	free(cpu->r6);
	free(cpu->r5);
	free(cpu->r4);
	free(cpu->r3);
	free(cpu->r2);
	free(cpu->r1);
	free(cpu->mem);
	free(cpu);
}

int
read_file(char *path, uint16_t *mem)
{
	FILE *fptr;
	char *bin;
	int size;

	bin = (char *)mem;
	
	fptr = fopen(path, "r");
	if (!fptr) {
		fprintf(stderr, "Unable to read program file.\n");
		return -1;
	}

	fseek(fptr, 0, SEEK_END);
	size = ftell(fptr);

	if (size > MAX_SIZE) {
		fprintf(stderr, "File too huge.\n");
		fclose(fptr);
		return -1;
	}

	fread(bin, 0, size, fptr);
	fclose(fptr);
	return 0;
}

int
usage()
{
	printf("./path/to/emulator <program to run>\n");
	return 0;
}

int
main(int argc, char **argv)
{
	struct cpu_s *cpu;
	uint16_t *bin;

	cpu = 0;
	bin = 0;

	bin = (uint16_t *)malloc(0x200);
	if (!bin) {
		fprintf(stderr, "main(): malloc() failed\n");
		return -1;
	}
	memset(bin, 0, 0x200);

	if (argc != 2) {
		free(bin);
		return usage();
	}

	if (cpu_s_init(cpu, 0x200)) {
		fprintf(stderr, "cpu_s_init(cpu) failed: Malloc()\n");
		free(bin);
		return -1;
	}

	if (read_file(argv[1], bin)) {
		cpu_s_fini(cpu);
		free(bin);
		return 0;
	}

	cpu_main(cpu, bin);
	cpu_s_fini(cpu);
	free(bin);

	return 0;
}


