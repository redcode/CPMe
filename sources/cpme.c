#include <Z80.h>
#include <Z/constants/pointer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPCODE_NOP 0x00
#define OPCODE_RET 0xC9

static Z80      cpu;
static zuint8   memory[65536];
static zboolean quit_program;

static zuint8 cpu_read(void *context, zuint16 address)
	{return memory[address];}


static void cpu_write(void *context, zuint16 address, zuint8 value)
	{memory[address] = value;}


static zuint8 cpu_in(void *context, zuint16 port)
	{return 0;}


static void cpu_out(void *context, zuint16 port, zuint8 value)
	{}


static zuint8 cpu_hook(void *context, zuint16 address)
	{
	zuint8 character;

	switch (address)
		{
		case 0: /* END */
		cpu.cycles = Z80_MAXIMUM_CYCLES;
		quit_program = TRUE;
		return OPCODE_NOP;

		case 5: /* PRINT */
		Z_UNUSED(context)
		if (address != 5) return OPCODE_NOP;

		if (Z80_C(cpu) == 2) switch ((character = Z80_E(cpu)))
			{
			case 0x0D: break;
			case 0x0A: character = '\n';

			default:
			putchar(character);
			}

		else if (Z80_C(cpu) == 9)
			{
			zuint16 i = Z80_DE(cpu);
			zuint   c = 0;

			while (memory[i] != '$')
				{
				if (c++ > 100)
					{
					putchar('\n');
					fputs("FATAL ERROR: String to print is too long!\n", stderr);
					exit(EXIT_FAILURE);
					}

				switch ((character = memory[i++]))
					{
					case 0x0D: break;
					case 0x0A: character = '\n';

					default:
					putchar(character);
					}
				}
			}

		return OPCODE_RET;
		}

	}


static zboolean load_com(char const *file_path)
	{
	zboolean ok = FALSE;
	FILE *file = fopen(file_path, "rb");

	if (file != Z_NULL)
		{
		long file_size;

		fseek(file, 0, SEEK_END);
 		file_size = ftell(file);
		fseek(file, 0, SEEK_SET);
		ok = fread(memory + 0x0100, file_size, 1, file) == 1;
		fclose(file);
		}

	return ok;
	}




int main(int argc, char **argv)
	{
	if (argc != 2)
		{
		puts("Usage: cpme <com file>");
		return -1;
		}

	memset(memory, 0, 65536);

	memory[0] =
	memory[5] = Z80_HOOK;

	if (!load_com(argv[1]))
		{
		fprintf(stderr, "Can't load file: '%s'\n", argv[1]);
		return -1;
		}

	cpu.context	 = Z_NULL;
	cpu.fetch_opcode =
	cpu.fetch	 =
	cpu.nop		 =
	cpu.read	 = (Z80Read )cpu_read;
	cpu.write	 = (Z80Write)cpu_write;
	cpu.in		 = (Z80Read )cpu_in;
	cpu.out		 = (Z80Write)cpu_out;
	cpu.halt	 = Z_NULL;
	cpu.nmia	 = Z_NULL;
	cpu.inta	 = Z_NULL;
	cpu.int_fetch    = Z_NULL;
	cpu.ld_i_a       =
	cpu.ld_r_a       =
	cpu.reti	 =
	cpu.retn	 = Z_NULL;
	cpu.hook	 = cpu_hook;
	cpu.illegal      = Z_NULL;
	cpu.options      = Z80_MODEL_ZILOG_NMOS;

	z80_power(&cpu, TRUE);

	quit_program = FALSE;
	do z80_execute(&cpu, Z80_MAXIMUM_CYCLES); while (!quit_program);

	return 0;
	}


/* cpme.c EOF */
