#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define RED_CUBES 12
#define GREEN_CUBES 13
#define BLUE_CUBES 14

//note: part 2 has a weird workaround for a bug i made
//it means that it resets continually on the last line in the file
//i dont care enough to fix it.


bool isValid(char* buffer) {
	//skip "Game %d: "
	int location = 0;
	for (int i = 0; i < strlen(buffer); i++) {
		if (buffer[i] == ':') {
			location = i + 2;
			break;
		}
	}
	//extract sets of values
	int redCount = 0;
	int greenCount = 0;
	int blueCount = 0;
	for (int i = location; i < strlen(buffer); i++) {
		//printf("%d: on char: \'%c\'\n", i, buffer[i]);
		//detect if semicolon or number
		if (buffer[i] == ';' || buffer[i] == '\n') {
			//printf("counts for this round: %d red, %d green, %d blue\n", redCount, greenCount, blueCount);
			if (redCount > RED_CUBES || greenCount > GREEN_CUBES || blueCount > BLUE_CUBES) {
				//printf("invalid case. returning false\n");
				return false;
			}
			redCount = 0;
			greenCount = 0;
			blueCount = 0;
		}
		int currentNumber = 0;
		if (isdigit(buffer[i])) {
			//check if next number is digit. if so, we multiply this number by ten and add the next one,
			//otherwise we just treat it like a normal number.
			if (isdigit(buffer[i + 1])) {
				currentNumber = (buffer[i] - '0') * 10 + (buffer[i + 1] - '0');
				i += 3;
			} else {
				currentNumber = (buffer[i] - '0');
				i += 2;
			}

			if (buffer[i] == 'b') {
				blueCount = currentNumber;
				i += 3;	
			} else if (buffer[i] == 'g') {
				greenCount = currentNumber;
				i += 4;
			} else if (buffer[i] == 'r') {
				redCount = currentNumber;
				i += 2;	
			} else {
				//printf("malformed on char: \'%c\'\n", buffer[i]);
				return false;
			}

		}
	}

	return true;
}

void part2(FILE* data) {
	int sum = 0;
	for (int j = 1;; j++) {
		char buffer[1024];
		fgets(buffer, 1024, data);
		//printf("%d: \"%s\"\n", j, buffer);
		//skip "Game %d: "
		int location = 0;
		for (int i = 0; i < strlen(buffer); i++) {
			if (buffer[i] == ':') {
				location = i + 2;
				break;
			}
		}
		//extract sets of values
		int maxRed = 0;
		int maxGreen = 0;
		int maxBlue = 0;
		int redCount = 0;
		int greenCount = 0;
		int blueCount = 0;
		for (int i = location; i < strlen(buffer); i++) {
			//printf("%d: on char: \'%c\'\n", i, buffer[i]);
			//detect if semicolon or number
			if (buffer[i] == ';' || buffer[i] == '\n' || feof(data)) {
				//printf("counts for this round: %d red, %d green, %d blue\n", redCount, greenCount, blueCount);
				if (maxRed   < redCount)   {maxRed   = redCount;}
				if (maxGreen < greenCount) {maxGreen = greenCount;}
				if (maxBlue  < blueCount)  {maxBlue  = blueCount;}
				redCount = 0;
				greenCount = 0;
				blueCount = 0;
				//printf("current max: %d red, %d green, %d blue\n", maxRed, maxGreen, maxBlue);
			}
			int currentNumber = 0;
			if (isdigit(buffer[i])) {
				//check if next number is digit. if so, we multiply this number by ten and add the next one,
				//otherwise we just treat it like a normal number.
				if (isdigit(buffer[i + 1])) {
					currentNumber = (buffer[i] - '0') * 10 + (buffer[i + 1] - '0');
					i += 3;
				} else {
					currentNumber = (buffer[i] - '0');
					i += 2;
				}

				if (buffer[i] == 'b') {
					blueCount = currentNumber;
					i += 3;	
				} else if (buffer[i] == 'g') {
					greenCount = currentNumber;
					i += 4;
				} else if (buffer[i] == 'r') {
					redCount = currentNumber;
					i += 2;	
				}
			}
		}
		//printf("maxes for line %d: %d red, %d green, %d blue. power: %d\n", j, maxRed, maxGreen, maxBlue, maxRed*maxGreen*maxBlue);
		sum += maxRed*maxGreen*maxBlue;
		if (feof(data)) { break; }
	}
	printf("power total: %d\n", sum);
}

void part1(FILE* data) {
	int sum = 0;
	for (int i = 1;; i++) {
		char buffer[1024];
		fgets(buffer, 1024, data);
		//printf("%d: \"%s\"\n", i, buffer);
		if (isValid(buffer)) { sum += i; }
		if (feof(data)) { break; }
	}
	printf("sum of valid games IDs: %d\n", sum);
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