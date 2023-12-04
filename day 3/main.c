#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//this code is ugly and bad and full of awful practices.
//this should NOT be repeated, i just did not want to
//rewrite all my code for part 1.

typedef struct {
	int width;
	int height;
	char** data;
} engine;

typedef struct {
	int xGear;
	int yGear;
	int num;
} gearRatio;

int gearRatioCount = 0;

gearRatio* ratios;

void viewEngine(engine intEngine) {
	for (int i = 0; i < (intEngine.height + 2); i++) {
		printf("%s\n", intEngine.data[i]);
	}
}

void appendPotentialRatio(int xGear, int yGear, int num) {
	if (ratios == NULL) {
		ratios = malloc(sizeof(*ratios));
		gearRatioCount = 1;
	}
	ratios = realloc(ratios, sizeof(*ratios)*gearRatioCount);
	ratios[gearRatioCount - 1] = (gearRatio){xGear, yGear, num};
	gearRatioCount++;
}

void clearRatios() {
	if (ratios != NULL) {
		free(ratios);
		ratios = NULL;
		gearRatioCount = 0;
	}
}

engine generateEngine(FILE* data) {
	//determine dimensions of engine, assuming (n + 2)x(m + 2).
	char buffer[1024];
	fgets(buffer, 1024, data);
	engine intEngine;
	intEngine.width = strlen(buffer) - 1; //remove \n
	
	fseek(data, 0, SEEK_END);
	int length = ftell(data);
	intEngine.height = length / intEngine.width;
	rewind(data);

	printf("width: %d, height: %d\n", intEngine.width, intEngine.height);
	//generate memory allocation for engine
	//add a ring around engine to facilitate the literal edge case.
	intEngine.data = malloc(sizeof(*intEngine.data) * (intEngine.height + 2));
	if (intEngine.data == NULL) {
		printf("malloc failed!\n");
		exit(-1);
	}
	intEngine.data[0]                    = calloc(intEngine.width + 2, sizeof(**intEngine.data));
	intEngine.data[intEngine.height + 1] = calloc(intEngine.width + 2, sizeof(**intEngine.data));
	for (int i = 1; i < (intEngine.height + 1); i++) {
		intEngine.data[i] = calloc(intEngine.width + 2, sizeof(**intEngine.data));
		if (intEngine.data[i] == NULL) {
			printf("malloc failed!\n");
			exit(-1);
		}
		fgets(buffer, 1024, data);
		if (i == (intEngine.height - 1)) {
			memcpy((intEngine.data[i] + 1), buffer, strlen(buffer));
		} else {
			memcpy((intEngine.data[i] + 1), buffer, strlen(buffer) - 1);
		}
		intEngine.data[i][0] = '.';
		intEngine.data[i][intEngine.width + 1] = '.';
		//printf("%s\n", intEngine.data[i]);
	}
	for (int i = 0; i < (intEngine.width + 2); i++) {

		intEngine.data[0][i] = '.';
		intEngine.data[intEngine.height + 1][i] = '.';
	}

	//viewEngine(intEngine);

	return intEngine;
}

char* symbolTable = "*#+$@/=&-%";

int locationTableLength3[12][2] = {
	{-1, -1}, {-1,  0}, {-1,  1}, {-1,  2}, {-1,  3},
	{0,  -1}, /*origin*/                    {0,   3},
	{1,  -1}, {1,   0}, {1,   1}, {1,   2}, {1,   3}
};

int locationTableLength2[10][2] = {
	{-1, -1}, {-1,  0}, {-1,  1}, {-1,  2},
	{0,  -1}, /*origin*/          {0,   2},
	{1,  -1}, {1,   0}, {1,   1}, {1,   2} 
};

int locationTableLength1[8][2] = {
	{-1, -1}, {-1,  0}, {-1,  1},
	{0,  -1}, /*origin*/{0,   1},
	{1,  -1}, {1,   0}, {1,   1},
};

int extractNumber(engine intEngine, int x, int y) {
	int sum = 0;
	//find length of number. we test 2 spaces in front
	if (isdigit(intEngine.data[y][x + 2]) && isdigit(intEngine.data[y][x + 1])) {
		//length is 3! we need to test all 12 locations.
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < strlen(symbolTable); j++) {
				if (intEngine.data[y + locationTableLength3[i][0]][x + locationTableLength3[i][1]] == symbolTable[j]) {
					//we have detected a symbol around the ring! we make the sum, and delete the number.
					sum += (intEngine.data[y][x]     - '0') * 100;
					sum += (intEngine.data[y][x + 1] - '0') * 10;
					sum += (intEngine.data[y][x + 2] - '0') * 1;
					intEngine.data[y][x] = '.';
					intEngine.data[y][x + 1] = '.';
					intEngine.data[y][x + 2] = '.';
					appendPotentialRatio(x + locationTableLength3[i][1], y + locationTableLength3[i][0], sum);
					//printf("extracted num of len 3: %d at %d:%d\n", sum, x, y);
					return sum;
				}
			}	
		}
	} else if (isdigit(intEngine.data[y][x+1])) {
		//length is 2! we need to test all 10 locations.
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < strlen(symbolTable); j++) {
				if (intEngine.data[y + locationTableLength2[i][0]][x + locationTableLength2[i][1]] == symbolTable[j]) {
					//we have detected a symbol around the ring! we make the sum, and delete the number.
					sum += (intEngine.data[y][x]     - '0') * 10;
					sum += (intEngine.data[y][x + 1] - '0') * 1;
					intEngine.data[y][x] = '.';
					intEngine.data[y][x + 1] = '.';
					appendPotentialRatio(x + locationTableLength2[i][1], y + locationTableLength2[i][0], sum);
					//printf("extracted num of len 2: %d at %d:%d\n", sum, x, y);
					return sum;
				}
			}	
		}
	} else { //single digit case crept up on me.
		//length is 1
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < strlen(symbolTable); j++) {
				if (intEngine.data[y + locationTableLength1[i][0]][x + locationTableLength1[i][1]] == symbolTable[j]) {
					//we have detected a symbol around the ring! we make the sum, and delete the number.
					sum += (intEngine.data[y][x] - '0') * 1;
					intEngine.data[y][x] = '.';
					appendPotentialRatio(x + locationTableLength1[i][1], y + locationTableLength1[i][0], sum);
					//printf("extracted num of len 1: %d at %d:%d\n", sum, x, y);
					return sum;
				}
			}	
		}
	}
	return 0;
}

void part1(FILE* data) {
	engine intEngine = generateEngine(data);
	int sum = 0;
	//scan left->right, top->bottom
	for (int i = 0; i < intEngine.height + 1; i++) {
		for (int j = 0; j < intEngine.width + 1; j++) {
			if (isdigit(intEngine.data[i][j])) {
				//we have LOCATED A NUMBER!
				//printf("\n\npre extract %d at %d:%d :\n", sum, i, j);
				//viewEngine(intEngine);
				int number = extractNumber(intEngine, j, i);
				//printf("extracted number: %d\n", number);
				sum += number;
				//printf("post extract %d:\n", sum);
				//viewEngine(intEngine);

				//to prevent double-scanning, we'll remove the symbol.
			}
		}
		//
	}

	viewEngine(intEngine);
	printf("sum: %d\n", sum);
}

int ratioSortInY(const void* a, const void* b) {
	gearRatio arg1 = *(gearRatio*)a;
	gearRatio arg2 = *(gearRatio*)b;
	if (arg1.yGear < arg2.yGear) {
		return -1;
	} else if (arg1.yGear > arg2.yGear) {
		return 1;
	}
	return 0;
}

int ratioSortInX(const void* a, const void* b) {
	gearRatio arg1 = *(gearRatio*)a;
	gearRatio arg2 = *(gearRatio*)b;
	if (arg1.xGear < arg2.xGear) {
		return -1;
	} else if (arg1.xGear > arg2.xGear) {
		return 1;
	}
	return 0;
}

void part2(FILE* data) {
	clearRatios();
	engine intEngine = generateEngine(data);
	int sum = 0;
	//scan left->right, top->bottom
	for (int i = 0; i < intEngine.height + 1; i++) {
		for (int j = 0; j < intEngine.width + 1; j++) {
			if (isdigit(intEngine.data[i][j])) {
				extractNumber(intEngine, j, i);
			}
		}
		//
	}

	qsort(ratios, gearRatioCount, sizeof(*ratios), ratioSortInY);
	qsort(ratios, gearRatioCount, sizeof(*ratios), ratioSortInX);
	for (int i = 0; i < gearRatioCount; i++) {
		if (ratios[i].yGear == ratios[i+1].yGear) {
			if (ratios[i].xGear == ratios[i+1].xGear) {
				//ratios are the same!
				sum += ratios[i].num * ratios[i+1].num;
			}
		}
	}
	//viewEngine(intEngine);
	printf("sum: %d\n", sum);
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
