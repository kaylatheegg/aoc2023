#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void part1(FILE* data) {
}

void part2(FILE* data) {
}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("please provide a file\n");
		return -1;
	}

	FILE* data = fopen(argv[1], "r");
	if (data == NULL) {
		printf("failed to open \"%s\"\n", argv[1]);
		return -1;
	}

	part1(data);
	rewind(data);
	part2(data);
	
	fclose(data);
	return 0;
}
