#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*10, 25, 202*/

#define WIN_COUNT 10
#define NUM_COUNT 25
#define CARD_COUNT 202

int ipow(int a, int b) {
	int result = 1;
	for (int i = 0; i < b; i++) {
		result *= a;
	}
	return result;
}

typedef struct {
	int numOfMatches;
	int copies;
} card;

card cards[CARD_COUNT];

void part1(FILE* data) {
	//read in one line and scan until we hit :
	int sum = 0;
	for (int i = 0;;i++) {
		char buffer[1024];
		fgets(buffer, 1024, data);
		int location = 0;
		for (;location < strlen(buffer); location++) { if (buffer[location] == ':') break;}
		location += 2;
		//printf("%s\n", (buffer+location));
		//extract WIN_COUNT numbers.
		int winningNumbers[WIN_COUNT] = {};
		for (int j = 0; j < WIN_COUNT; j++) {
			if (buffer[location] == ' ') { //single digit.
				winningNumbers[j] = (buffer[location+1] - '0');
			} else { //two digits
				winningNumbers[j] = (buffer[location] - '0') * 10 + (buffer[location + 1] - '0');
			}
			location += 3;
			//printf("%d: %d\n", j, winningNumbers[j]);
		}
		//extract NUM_COUNT numbers.
		int ticketNumbers[NUM_COUNT] = {};

		location += 2;
		for (int j = 0; j < NUM_COUNT; j++) {
			if (buffer[location] == ' ') { //single digit.
				ticketNumbers[j] = (buffer[location+1] - '0');
			} else { //two digits
				ticketNumbers[j] = (buffer[location] - '0') * 10 + (buffer[location + 1] - '0');
			}
			location += 3;
			//printf("%d: %d\n", j, ticketNumbers[j]);
		}
		//this is O(n*m), this is bad.

		int numOfMatches = 0;
		for (int j = 0; j < NUM_COUNT; j++) {
			for (int k = 0; k < WIN_COUNT; k++) {
				if (ticketNumbers[j] == winningNumbers[k]) {
					numOfMatches++;
				}
			}
		}
		//sort the numbers

		if (numOfMatches != 0) {
			sum += ipow(2, (numOfMatches - 1));
		}


		if (feof(data)) { break; }
	}
	printf("scratchcard values: %d\n", sum);
}

void part2(FILE* data) {
	//read in one line and scan until we hit :
	int sum = 0;
	for (int i = 0;;i++) {
		char buffer[1024];
		fgets(buffer, 1024, data);
		int location = 0;
		for (;location < strlen(buffer); location++) { if (buffer[location] == ':') break;}
		location += 2;
		//printf("%s\n", (buffer+location));
		//extract WIN_COUNT numbers.
		int winningNumbers[WIN_COUNT] = {};
		for (int j = 0; j < WIN_COUNT; j++) {
			if (buffer[location] == ' ') { //single digit.
				winningNumbers[j] = (buffer[location+1] - '0');
			} else { //two digits
				winningNumbers[j] = (buffer[location] - '0') * 10 + (buffer[location + 1] - '0');
			}
			location += 3;
			//printf("%d: %d\n", j, winningNumbers[j]);
		}
		//extract NUM_COUNT numbers.
		int ticketNumbers[NUM_COUNT] = {};

		location += 2;
		for (int j = 0; j < NUM_COUNT; j++) {
			if (buffer[location] == ' ') { //single digit.
				ticketNumbers[j] = (buffer[location+1] - '0');
			} else { //two digits
				ticketNumbers[j] = (buffer[location] - '0') * 10 + (buffer[location + 1] - '0');
			}
			location += 3;
			//printf("%d: %d\n", j, ticketNumbers[j]);
		}
		//this is O(n*m), this is bad.

		int numOfMatches = 0;
		for (int j = 0; j < NUM_COUNT; j++) {
			for (int k = 0; k < WIN_COUNT; k++) {
				if (ticketNumbers[j] == winningNumbers[k]) {
					numOfMatches++;
				}
			}
		}
		//sort the numbers

		cards[i].numOfMatches = numOfMatches;
		cards[i].copies++; //adds the inital copy on.
		for (int j = 1; j < numOfMatches + 1; j++) {
			if ((i + j) > CARD_COUNT) break; //edge case
			cards[i + j].copies += cards[i].copies;
		}


		if (feof(data)) { break; }
	}
	for (int i = 0; i < CARD_COUNT; i++) {
		//printf("card %d: matches: %d, copies: %d\n", i + 1, cards[i].numOfMatches, cards[i].copies);
		sum += cards[i].copies;
	}
	printf("scratchcard values: %d\n", sum);
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
