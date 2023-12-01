#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_REPLACEMENTS_COUNT 5

int extractNumber(char* buffer) {
	//ignoring error checking. this is not a robust program
	int number = 0;
	for (int j = 0; j < strlen(buffer); j++) {
		if (isdigit(buffer[j])) {
			number += (buffer[j] - '0') * 10;
			break;
		}
	}

	for (int j = strlen(buffer) - 1; j >= 0; j--) {
		if (isdigit(buffer[j])) {
			number += (buffer[j] - '0');
			break;
		}
	}
	return number;
}

void part1(FILE* data) {
	int sum = 0;
	for (int i = 0;; i++) {
		char buffer[256] = {};
		fgets(buffer, 256, data);

		sum += extractNumber(buffer);
		if (feof(data)) {break;}
	}

	printf("part 1 calibration number: %d\n", sum);
}

//i wish this was odin for maps. we're doing this C style.
char* testStrings[17] = {
	"twone", "eightwo", "eighthree", "oneight", "threeight", "fiveight", "nineight", "sevenine",
	"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

char* replacementStrings[17] = {
	"21", "82", "83", "18", "38", "58", "98", "79",
	"1", "2", "3", "4", "5", "6", "7", "8", "9"
};
// teststrings[i] is replaced with replacementstrings[i].

void part2(FILE* data) {
	int sum = 0;
	for (int i = 0;; i++) {
		char buffer[256] = {};
		fgets(buffer, 256, data);
		//printf("%d: pre-convert: \"%s\"\n", i, buffer);
		for (int k = 0; k < MAX_REPLACEMENTS_COUNT; k++) {
			for (int j = 0; j < 17; j++) { //17 tests, 9 singular and 8 combined.
				char* location = strstr(buffer, testStrings[j]);
				if (location == NULL) {continue;}
				//we now copy in the data from replacement strings, and move everything down.
				memcpy(location, replacementStrings[j], strlen(replacementStrings[j]));
				memmove((location + strlen(replacementStrings[j])), (location + strlen(testStrings[j])), strlen(location) - strlen(replacementStrings[j]));
				//printf("%d: %s\n", i, buffer);
			}
		}
		printf("%d: post-convert: \"%s\", number: %d\n", i, buffer, extractNumber(buffer));
		sum += extractNumber(buffer);
		if (feof(data)) {break;}
	}
	printf("part 2 calibration number: %d\n", sum);
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

//54450