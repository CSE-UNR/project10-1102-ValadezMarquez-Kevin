//Assignment: Project 10 Extra Credit
//by: Kevin Valadez Marquez
#include <stdio.h>
#include <stdbool.h>
#define MAX_GUESSES 6
#define WORD_LEN 5
#define SIZE 100
#define MAX_WORDS 100
#define TOP_N 5
#define NAME_LEN 20

//funcProto
int loadWords(char words[][WORD_LEN + 1]);
int pickIndex(int total);
bool loadMystery(char mystery[]);
void toLower(char str[]);
int strLen(char str[]);
bool allLetters(char str[]);
void getGuess(int guessNum, char guess[]);
bool checkWin(char guess[], char mystery[]);
void formatGuess(char guess[], char mystery[], char display[], char carets[]);
void displayHistory(char guesses[][WORD_LEN + 1], char mystery[], int count);
void printWin(char mystery[], int guesses);
void updateScoreboard(int score);
void showScoreboard();

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

	//only winners make the board
	if (won) {
		updateScoreboard(numGuesses);
	}
	showScoreboard();

	return 0;
}

//funcDefs
int loadWords(char words[][WORD_LEN + 1]){
	FILE *fp = fopen("words.txt", "r");
	if (fp == NULL) return 0;
	int count = 0;
	char buffer[SIZE];
	//read each line and only keep the 5 letter ones
	while (fgets(buffer, SIZE, fp) != NULL && count < MAX_WORDS) {
		//strip newline
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (buffer[i] == '\n') {
				buffer[i] = '\0';
				break;
			}
		}
		if (strLen(buffer) == WORD_LEN && allLetters(buffer)) {
			for (int i = 0; i <= WORD_LEN; i++) {
				words[count][i] = buffer[i];
			}
			count++;
		}
	}
	fclose(fp);
	return count;
}

int pickIndex(int total){
	//read counter from file, modulo wraps it around
	int counter = 0;
	FILE *cfp = fopen("counter.txt", "r");
	if (cfp != NULL) {
		fscanf(cfp, "%d", &counter);
		fclose(cfp);
	}
	int picked = counter % total;
	//bump counter so next run picks the next word
	FILE *wfp = fopen("counter.txt", "w");
	if (wfp != NULL) {
		fprintf(wfp, "%d\n", counter + 1);
		fclose(wfp);
	}
	return picked;
}

bool loadMystery(char mystery[]){
	char words[MAX_WORDS][WORD_LEN + 1];
	int total = loadWords(words);
	if (total == 0) return false;
	int idx = pickIndex(total);
	for (int i = 0; i <= WORD_LEN; i++) {
		mystery[i] = words[idx][i];
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
	bool mysteryUsed[WORD_LEN];
	//figure out which spots are exactly right and mark those mystery slots as used
	for (int i = 0; i < WORD_LEN; i++) {
		correctPos[i] = (guess[i] == mystery[i]);
		mysteryUsed[i] = correctPos[i];
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

	//for each wrong spot guess letter, claim an unused mystery slot if it has one
	for (int i = 0; i < WORD_LEN; i++) {
		if (correctPos[i]) continue;
		for (int j = 0; j < WORD_LEN; j++) {
			if (!mysteryUsed[j] && mystery[j] == guess[i]) {
				carets[i] = '^';
				mysteryUsed[j] = true;
				break;
			}
		}
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

void updateScoreboard(int score){
	char names[TOP_N + 1][NAME_LEN];
	int scores[TOP_N + 1];
	int count = 0;

	//grab the existing top scores from file
	FILE *fp = fopen("scores.txt", "r");
	if (fp != NULL) {
		while (count < TOP_N && fscanf(fp, "%s %d", names[count], &scores[count]) == 2) {
			count++;
		}
		fclose(fp);
	}

	//ask the player for their name
	printf("\nYou made it onto the board (maybe)! Enter your name: ");
	char input[SIZE];
	fgets(input, SIZE, stdin);
	for (int i = 0; input[i] != '\0'; i++) {
		if (input[i] == '\n') {
			input[i] = '\0';
			break;
		}
	}
	//cap the name length so it doesnt overflow
	int copyLen = strLen(input);
	if (copyLen >= NAME_LEN) copyLen = NAME_LEN - 1;
	for (int i = 0; i < copyLen; i++) {
		names[count][i] = input[i];
	}
	names[count][copyLen] = '\0';
	scores[count] = score;
	count++;

	//bubble sort by score, fewer guesses is better
	for (int i = 0; i < count - 1; i++) {
		for (int j = 0; j < count - 1 - i; j++) {
			if (scores[j] > scores[j + 1]) {
				int tmp = scores[j];
				scores[j] = scores[j + 1];
				scores[j + 1] = tmp;
				char tmpName[NAME_LEN];
				for (int k = 0; k < NAME_LEN; k++) tmpName[k] = names[j][k];
				for (int k = 0; k < NAME_LEN; k++) names[j][k] = names[j + 1][k];
				for (int k = 0; k < NAME_LEN; k++) names[j + 1][k] = tmpName[k];
			}
		}
	}

	//chop off anything past 5
	if (count > TOP_N) count = TOP_N;

	//write the new top list back out
	FILE *out = fopen("scores.txt", "w");
	if (out != NULL) {
		for (int i = 0; i < count; i++) {
			fprintf(out, "%s %d\n", names[i], scores[i]);
		}
		fclose(out);
	}
}

void showScoreboard(){
	FILE *fp = fopen("scores.txt", "r");
	if (fp == NULL) return;
	printf("\n=========SCOREBOARD=========\n");
	char name[NAME_LEN];
	int score;
	int rank = 1;
	while (rank <= TOP_N && fscanf(fp, "%s %d", name, &score) == 2) {
		printf("%d. %-20s %d\n", rank, name, score);
		rank++;
	}
	fclose(fp);
}
