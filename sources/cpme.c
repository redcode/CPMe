#include <Z80.h>
#include <Z/constants/pointer.h>
#include <stdio.h>
#include <string.h>

static Z80 cpu;
static zuint8 memory[65536];


static zuint8 cpu_read(void *context, zuint16 address)
	{
	return 0;
	}


static void cpu_write(void *context, zuint16 address, zuint8 value)
	{
	}


static zuint8 cpu_in(void *context, zuint16 port)
	{
	return 0;
	}


static void cpu_out(void *context, zuint16 port, zuint8 value)
	{
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
	cpu.hook	 = Z_NULL;
	cpu.illegal      = Z_NULL;
	cpu.options      = Z80_MODEL_ZILOG_NMOS;

	z80_power(&cpu, TRUE);

	return 0;
	}


/* cpme.c EOF */
