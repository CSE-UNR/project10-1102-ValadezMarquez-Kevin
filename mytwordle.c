//Assignment: Project 10
//by: Kevin Valadez Marquez
#include <stdio.h>
#include <stdbool.h>
#define MAX_GUESSES 6
#define WORD_LEN 5
#define SIZE 100

//funcProto
bool loadMystery(char mystery[]);
void toLower(char str[]);
int strLen(char str[]);
bool allLetters(char str[]);
void getGuess(int guessNum, char guess[]);
bool checkWin(char guess[], char mystery[]);
void formatGuess(char guess[], char mystery[], char display[], char carets[]);
void displayHistory(char guesses[][WORD_LEN + 1], char mystery[], int count);
void printWin(char mystery[], int guesses);

//main
int main(){
	char mystery[SIZE];
	char guesses[MAX_GUESSES][WORD_LEN + 1];
	int numGuesses = 0;
	bool won = false;

	//grab the mystery word from file
	if (!loadMystery(mystery)) {
		printf("Cannot open mystery file.\n");
		return 0;
	}
	toLower(mystery);

	//keep asking for guesses until they win or run out
	while (numGuesses < MAX_GUESSES && !won) {
		getGuess(numGuesses + 1, guesses[numGuesses]);
		toLower(guesses[numGuesses]);

		printf("================================\n");

		won = checkWin(guesses[numGuesses], mystery);
		numGuesses++;

		if (won) {
			printWin(mystery, numGuesses);
		} else {
			displayHistory(guesses, mystery, numGuesses);
			if (numGuesses == MAX_GUESSES) {
				printf("You lost, better luck next time!\n");
			}
		}
	}

	return 0;
}

//funcDefs
bool loadMystery(char mystery[]){
	FILE *fp = fopen("mystery.txt", "r");
	if (fp == NULL) return false;
	fgets(mystery, SIZE, fp);
	fclose(fp);
	//get rid of newline if it's there
	for (int i = 0; mystery[i] != '\0'; i++) {
		if (mystery[i] == '\n') {
			mystery[i] = '\0';
			break;
		}
	}
	return true;
}

void toLower(char str[]){
	//bump uppercase letters down by 32 in ascii
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] = str[i] + 32;
		}
	}
}

int strLen(char str[]){
	int len = 0;
	for (int i = 0; str[i] != '\0'; i++) len++;
	return len;
}

bool allLetters(char str[]){
	//returns false the moment a non letter shows up
	for (int i = 0; str[i] != '\0'; i++) {
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))) {
			return false;
		}
	}
	return true;
}

void getGuess(int guessNum, char guess[]){
	char buffer[SIZE];

	//last guess gets a different prompt
	if (guessNum == MAX_GUESSES) {
		printf("FINAL GUESS : ");
	} else {
		printf("GUESS %d! Enter your guess: ", guessNum);
	}
	fgets(buffer, SIZE, stdin);
	for (int i = 0; buffer[i] != '\0'; i++) {
		if (buffer[i] == '\n') {
			buffer[i] = '\0';
			break;
		}
	}

	//keep prompting until the input is good
	while (strLen(buffer) != WORD_LEN || !allLetters(buffer)) {
		if (strLen(buffer) != WORD_LEN) {
			printf("Your guess must be 5 letters long.\n");
		} else {
			printf("Your guess must contain only letters.\n");
		}
		printf("Please try again: ");
		fgets(buffer, SIZE, stdin);
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (buffer[i] == '\n') {
				buffer[i] = '\0';
				break;
			}
		}
	}

	//copy the cleaned input into the guess slot
	for (int i = 0; i <= WORD_LEN; i++) {
		guess[i] = buffer[i];
	}
}

bool checkWin(char guess[], char mystery[]){
	for (int i = 0; i < WORD_LEN; i++) {
		if (guess[i] != mystery[i]) return false;
	}
	return true;
}

void formatGuess(char guess[], char mystery[], char display[], char carets[]){
	bool correctPos[WORD_LEN];
	//figure out which spots are exactly right
	for (int i = 0; i < WORD_LEN; i++) {
		correctPos[i] = (guess[i] == mystery[i]);
	}

	//capitalize the matching positions, leave the rest alone
	for (int i = 0; i < WORD_LEN; i++) {
		if (correctPos[i]) {
			display[i] = guess[i] - 32;
		} else {
			display[i] = guess[i];
		}
	}
	display[WORD_LEN] = '\0';

	//start carets line as all spaces
	for (int i = 0; i < WORD_LEN; i++) {
		carets[i] = ' ';
	}
	carets[WORD_LEN] = '\0';

	//mark wrong position letters that still belong in the word
	for (int i = 0; i < WORD_LEN; i++) {
		if (correctPos[i]) continue;
		//see if this letter shows up anywhere in the mystery
		bool inMystery = false;
		for (int j = 0; j < WORD_LEN; j++) {
			if (mystery[j] == guess[i]) {
				inMystery = true;
				break;
			}
		}
		if (!inMystery) continue;
		//if the same letter is already capitalized somewhere, skip
		bool placedElsewhere = false;
		for (int j = 0; j < WORD_LEN; j++) {
			if (j != i && correctPos[j] && guess[j] == guess[i]) {
				placedElsewhere = true;
				break;
			}
		}
		if (placedElsewhere) continue;
		carets[i] = '^';
	}
}

void displayHistory(char guesses[][WORD_LEN + 1], char mystery[], int count){
	char display[WORD_LEN + 1];
	char carets[WORD_LEN + 1];
	//walk through every guess so far and print formatted version
	for (int i = 0; i < count; i++) {
		formatGuess(guesses[i], mystery, display, carets);
		printf("%s\n", display);
		printf("%s\n", carets);
	}
}

void printWin(char mystery[], int guesses){
	char upper[WORD_LEN + 1];
	for (int i = 0; i < WORD_LEN; i++) {
		upper[i] = mystery[i] - 32;
	}
	upper[WORD_LEN] = '\0';

	//tabs match the spacing the example uses
	printf("\t\t%s\n", upper);
	printf("\tYou won in %d guesses!\n", guesses);
	if (guesses <= 3) {
		printf("\t\tAmazing!\n");
	} else if (guesses <= 5) {
		printf("\t\tNice!\n");
	} else {
		printf("\t\tPhew!\n");
	}
}
