#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void yellow();
void green();
void red();
void blue();
void reset();

void main()
{
	red();
	printf("Hello ");
	yellow();
	printf("world\n");
	reset();
}

void yellow() {
  printf("\033[1;33m");
}

void green() {
	printf("\033[1;32m");
}

void red() {
	printf("\033[1;31m");
}

void blue() {
	printf("\033[1;34m");
}


void reset() {
	printf("\033[0m");
}
