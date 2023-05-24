#include "main.h"
#include <elf.h>

void print_osabi_more(Elf64_Ehdr h);

/**
 * print_magic - that prints the magic numbers of an ELF header.
 * @h: the ELF header struct.
 */
void print_magic(Elf64_Ehdr h)
{
	int i;

	printf(" Magic:  ");
	for (i = 0; i < EI_NIDENT; i++)
		printf("%2.2x%s", h.e_ident[i], i == EI_NIDENT - 1 ? "\n" : " ");
}

/**
 * print_class - that prints the class of an ELF header.
 * @h: the ELF header struct.
 * Return: void.
 */
void print_class(Elf64_Ehdr h)
{
	printf(" Class:  ");
	switch (h.e_ident[EI_CLASS])
	{
		case ELFCLASSNONE:
			printf("none\n");
			break;
		case ELFCLASS32:
			printf("ELF32\n");
			break;
		case ELFCLASS64:
			printf("ELF64\n");
			break;
	}
}

/**
 * print_data - that prints the data of an ELF header.
 * @h: the ELF hearder struct.
 * Return: void.
 */
void print_data(Elf64_Ehdr h)
{
	printf(" Data:  ");
	switch (h.e_ident[EI_DATA])
	{
		case ELFDATANONE:
			printf("none\n");
			break;
		case ELFDATA2LSB:
			printf("2's complement, little endian\n");
			break;
		case ELFDATA2MSB:
			printf("2's complement, big endian\n");
			break;
	}
}

/**
 * print_version - that prints the version of an ELF header.
 * @h: the ELF header struct.
 * Return: void
 */
void print_version(Elf64_Ehdr h)
{
	printf(" Version:   %d", h.e_ident[EI_VERSION]);
	switch (h.e_ident[EI_VERSION])
	{
		case EV_CURRENT:
			printf(" (current)");
			break;
		case EV_NONE:
			printf("%s", "");
			break;
			break;
	}
	printf("\n");
}

/**
 * print_osabi - that prints the OS/ABI of an ELF header.
 * @h: the ELF header struct.
 * Return: void.
 */
void print_osabi(Elf64_Ehdr h)
{
	printf(" OS/ABI:  ");
	switch (h.e_ident[EI_OSABI])
	{
		case ELFOSABI_NONE:
			printf("UNIX - System V\n");
			break;
		case ELFOSABI_HPUX:
			printf("UNIX - HP-UX\n");
			break;
		case ELFOSABI_NETBSD:
			printf("UNIX - NetBSD\n");
			break;
		case ELFOSABI_LINUX:
			printf("UNIX - Linux\n");
			break;
		case ELFOSABI_SOLARIS:
			printf("UNIX - Solaris\n");
			break;
		case ELFOSABI_IRIX:
			printf("UNIX - IRIX\n");
			break;
		case ELFOSABI_FREEBSD:
			printf("UNIX - FreeBSD\n");
			break;
		case ELFOSABI_TRU64:
			printf("UNIX - TRU64\n");
			break;
		default:
			print_osabi_more(h);
			break;
	}
}

/**
 * print_osabi_more - that prints osabi more of an ELF header.
 * @h: the ELF header struct.
 * Return: void.
 */
void print_osabi_more(Elf64_Ehdr h)
{
	switch (h.e_ident[EI_OSABI])
	{
		case ELFOSABI_MODESTO:
			printf("Novell - modesto\n");
			break;
		case ELFOSABI_OPENBSD:
			printf("UNIX - OpenBSD\n");
			break;
		case ELFOSABI_ARM:
			printf("ARM\n");
			break;
		case ELFOSABI_STANDALONE:
			printf("Standalone App\n");
			break;
		default:
			printf("<unknown: %x>\n", h.e_ident[EI_OSABI]);
			break;
	}
}

/**
 * print_abiversion - that prints the ABI version of an ELF header.
 * @h: the ELF header struct.
 * Return: void.
 */
void print_abiversion(Elf64_Ehdr h)
{
	printf(" ABI Version: %d\n", h.e_ident[EI_ABIVERSION]);
}

/**
 * print_type - that prints the type of an ELF header.
 * @h: the ELF header struct.
 * Return: void.
 */
void print_type(Elf64_Ehdr h)
{
	char *p = (char *)&h.e_type;
	int i = 0;

	printf(" Type:  ");
	if (h.e_ident[EI_DATA] == ELFDATA2MSB)
		i = 1;
	switch (p[i])
	{
		case ET_NONE:
			printf("NONE (None)\n");
			break;
		case ET_REL:
			printf("REL (Relocatable file)\n");
			break;
		case ET_EXEC:
			printf("EXEC (Executable file)\n");
			break;
		case ET_DYN:
			printf("DYN (Shared object file)\n");
			break;
		case ET_CORE:
			printf("CORE (Core file)\n");
			break;
		default:
			printf("<unknown: %x>\n", p[i]);
			break;
	}
}

/**
 * print_entry - that prints the entry point of an ELF header.
 * @h: the ELF header struct.
 * Return: void.
 */
void print_entry(Elf64_Ehdr h)
{
	int i = 0, len = 0;
	unsigned char *p = (unsigned char *)&h.e_entry;

	printf(" Entry point address:  0x");
	if (h.e_ident[EI_DATA] != ELFDATA2MSB)
	{
		i = h.e_ident[EI_CLASS] == ELFCLASS64 ? 7 : 3;
		while (!p[i])
			i--;
		printf("%x", p[i--]);
		for (; i >= 0; i--)
			printf("%02x", p[i]);
		printf("\n");
	}
	else
	{
		i = 0;
		len = h.e_ident[EI_CLASS] == ELFCLASS64 ? 7 : 3;
		while (!p[i])
			i++;
		printf("%x", p[i++]);
		for (; i <= len; i++)
				printf("%02x", p[i]);
		printf("\n");
	}
}

/**
 * main - that displays the information contained in the ELF header.
 * @argc: parameter of the number of arguments supplied to the program.
 * @argv: array of pointers to the arguments.
 * Return: 1 on success, 0 on failure.
 */
int main(int argc, char *argv[])
{
	int fd;
	ssize_t b;
	Elf64_Ehdr h;

	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "Usage: elf_header elf_filename\n");
		exit(98);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		dprintf(STDERR_FILENO, "Can't open file: %s\n", argv[1]);
		exit(98);
	}
	b = read(fd, &h, sizeof(h));
	if (b < 1 || b != sizeof(h))
	{
		dprintf(STDERR_FILENO, "Can't read from file: %s\n", argv[1]);
		exit(98);
	}
	if (h.e_ident[0] == 0x7f && h.e_ident[1] == 'E' && h.e_ident[2] == 'L' &&
			h.e_ident[3] == 'F')
	{
		printf("ELF Header:\n");
	}
	else
	{
		dprintf(STDERR_FILENO, "No ELF file: %s\n", argv[1]);
		exit(98);
	}
	print_magic(h);
	print_class(h);
	print_data(h);
	print_version(h);
	print_osabi(h);
	print_abiversion(h);
	print_type(h);
	print_entry(h);
	if (close(fd))
	{
		dprintf(STDERR_FILENO, "Error closing file descriptor: %d\n", fd);
		exit(98);
	}
	return (EXIT_SUCCESS);
}
