#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

char is_print(char c) {
	if (c <= 125 && c >= 33) 
		return c;
	return '.';
}

long get_fsize(FILE *fp, char* fname) {
	struct stat fstat;
	if (stat(fname, &fstat) == -1) {
		fprintf(stderr, "stat: failed to retrieve file information.");
		exit(1);
	}
	return fstat.st_size;
}

void hexdmp(FILE *fp, char* fname) {
	long fsize = get_fsize(fp, fname);
	char *buff;
	if (!(buff = malloc(fsize * sizeof(char)))) {
		fprintf(stderr, "malloc: failed to allocate memory.");
		exit(1);
	}

	if (fread(buff, sizeof(char), fsize, fp) != fsize) {
		fprintf(stderr, "fread: source file could not be read.");
		exit(1);
	}

	char str[16] = "\0";
	for (size_t i=0, offset = 0x0; i<fsize; i++) {
		if (i % 16 == 0) printf("%08lx  ", offset);
		else if (i % 16 == 8) printf("  ");
		char c = is_print((char)buff[i]);
		strncat(str, &c, 1);
		printf("%02x ", buff[i]);
		if (i % 16 == 15) {
			printf("|%s|\n", str);
			offset += 0x10;
			memset(str, 0, sizeof(str));
		}
	}
	free(buff);
	fclose(fp);
	printf("\n");
}

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: ./hexdmp [OPTION]... [FILE]");
		exit(1);
	}
	FILE *fp;
	if (!(fp = fopen(argv[1], "rb"))) {
		fprintf(stderr, "fopen: source file could not be loaded.");
		exit(1);
		// perror( "Opening data file" ), return -1;
	}
	hexdmp(fp, argv[1]);
	return 0;
}