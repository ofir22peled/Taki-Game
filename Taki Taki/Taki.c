#define _CRT_SECURE_NO_WARNINGS
//Name: Ofir Peled ID: 207020405

#include <stdio.h>
#include <stdlib.h> // in order to use the "rand" and "srand" functions
#include <time.h>	// in order to use "time" function
#include <stdbool.h> //in order to use "boolean" types

//define declartion
#define  ZERO_CARDS 0 //to finish the game
#define TAKE_NEW_CARD 0 //if player choose to get a card from deck
#define FIRST_TURN 0 //initial the first turn
#define FINISH_TURN 0 //for taki card, if a player wants to finish his turn
#define TURN_UP 1 //one direction of a turn change
#define TURN_DOWN -1 //second drection of a turn change
#define TWO_PLAYERS 2 //to exclude some ruls for 2 players
#define MAX_CARD_COLORS 4 //max colors in game
#define BEGINNING_CARDS_AMOUNT 4 //amount of card for initial distribution
#define NUM_OF_COLORS 4
#define MAX_TYPE_LENGTH 6
#define PLUS_CARD 10 //to check if chosen card is plus card
#define STOP_CARD 11 //to check if chosen card is stop card
#define CHANGE_DIRECTION_CARD 12 //to check if chosen card is change direction card
#define TAKI_CARD 13 //to check if chosen card is taki card
#define COLOR_CARD 14 //to check if chosen card is color card
#define MAX_CARD_TYPES 14
#define MAX_NAME_LENGTH 21 //20 as defined + '/0'
#define YELLOW 1 //define numbers for switch cases
#define RED 2
#define BLUE 3
#define GREEN 4
#define PRINTABLE_YELLOW_CARD 'Y' //print "Y" in the card if yellow
#define PRINTABLE_GREEN_CARD 'G' //print "G" in the card if Green
#define PRINTABLE_RED_CARD 'R' //print "R" in the card if Red
#define PRINTABLE_BLUE_CARD 'B' //print "B" in the card if Blue
#define NO_COLOR ' ' //no color for the color card
//end of defined declartion


//decleration of structs
typedef struct cards { //struct to get enter info for each card
	char color;
	int type;
} Card;

typedef struct player { //struct to get data for all the players
	int amountOfCards; //actual number of cards for player
	int currentCardsSize; //pysical number of cards saved to player
	char nameOfPlayer[MAX_NAME_LENGTH]; //array to hold the name of player
	Card* playerCards;
} Player;

typedef struct statistics { //struct to hold statistics information
	int cardIndex;
	int frequency;
} Stats;


typedef struct game { //struct for game helpers
	int numberOfPlayers;
	int playersTurn;
	int playersChoice;
	int direction;
	bool isTakiOn;
	bool isChoiceValid;
	bool isGameOver;
	Player* players;
	Card upperCard;
	Card chosenCard;
	Stats statistics[MAX_CARD_TYPES];
} Game;

//end of structs declartion
void preGame(Game* game);
void printWelcome();
int getNumOfPlayers();
void getPlayersNames(Game* game);
void gameOperate(Game* game);
void gameOver(int amount);
void initialCardsHandOut(Game* game);
void initialUpperCard(Game* game);
void playTurn(Game* game);
void isChoiceValid(Game* game);
void isCardValid(Game* game);
void getPlayerChoice(Game* game);
void actPlayerChoice(Game* game);
void GetNewCard(Game* game);
void nextTurn(Game* game);
void uniqueCardsActivation(Game* game);
void plusCard(Game* game);
void stopCard(Game* game);
void changeDirectionCard(Game* game);
void takiCard(Game* game);
void colorCard(Game* game);
void printUpperCard(Card upperCard);
void printPlayerCards(Player player);
void printCard(Card card);
Card handOutCard(Game* game);
void initialStatistics(Game* game);
void gameStatistics(Game* game);
void printStatistics(Game game);
void mergeSort(Stats* arr, int size);
void merge(Stats* leftSide, int sizeLeft, Stats* rightSide, int sizeRight, Stats* tempArr);
void copyArr(Stats dest[], Stats src[], int size);
void ifAllocationFailled();
Player* getCurrentPlayer(Game* game);
void changeColor(char color);
void yellow();
void green();
void red();
void blue();
void resetColor();
//end of function declartion

//const char to help print the card types
const char typePrintHelper[][MAX_TYPE_LENGTH] = { "  0  ", "  1  ", "  2  ", "  3  ", "  4  ",
	"  5  ", "  6  ", "  7  ", "  8  ","  9  ", "  +  ", " STOP", " <-> ", " TAKI", "COLOR" };

void main() {

	srand(time(NULL)); //declartion before using the rand function

	Game game;

	preGame(&game);
	gameOperate(&game);
	gameStatistics(&game);
	freeMemoryAllocated(&game);
}

//function to print welcome, get the number of players and their names
void preGame(Game* game) {
	printWelcome();
	initialStatistics(game);
	game->numberOfPlayers = getNumOfPlayers();
	getPlayersNames(game);
}

//function to print welcome
void printWelcome()
{
	printf("************  Welcome to TAKI game !!! ***********\n");
}

//function to get the number of player at the game
int getNumOfPlayers() {
	int numOfPlayers;
	printf("Please enter the number of players:\n");
	scanf("%d", &numOfPlayers);
	return numOfPlayers;
}

//function to get all the players names
void getPlayersNames(Game* game)
{
	game->players = (Player*)malloc(game->numberOfPlayers * sizeof(Player));
	if (game->players == NULL)
		ifAllocationFailled();

	for (int playersIndex = 0; playersIndex < game->numberOfPlayers; playersIndex++) {
		printf("Please enter the first name of player #%d:\n", playersIndex + 1);
		scanf("%s", game->players[playersIndex].nameOfPlayer);
	}
}

//function to hand out one card
Card handOutCard(Game* game) {
	Card cardForHandOut;
	cardForHandOut.type = 1 + rand() % MAX_CARD_TYPES; //getting a random new card (type)
	if (cardForHandOut.type == COLOR_CARD)
		cardForHandOut.color = NO_COLOR;
	else {
		int randColor = rand() % MAX_CARD_COLORS + 1; //getting a random new card (color)
		switch (randColor)
		{
		case YELLOW:
			cardForHandOut.color = PRINTABLE_YELLOW_CARD;
			break;
		case RED:
			cardForHandOut.color = PRINTABLE_RED_CARD;
			break;
		case BLUE:
			cardForHandOut.color = PRINTABLE_BLUE_CARD;
			break;
		case GREEN:
			cardForHandOut.color = PRINTABLE_GREEN_CARD;
			break;
		}
	}
	game->statistics[cardForHandOut.type - 1].frequency++;
	return cardForHandOut;
}

//function to print one card
void printCard(Card cardForPrint) {
	changeColor(cardForPrint.color);
	printf("*********\n*       *\n");
	printf("* %s *\n*   %c   *\n", typePrintHelper[cardForPrint.type], cardForPrint.color);
	printf("*       *\n*********\n\n");
	resetColor();
}

//function to distribute the initial cards for the players
void initialCardsHandOut(Game* game) {
	for (int playersIndex = 0; playersIndex < game->numberOfPlayers; playersIndex++) {
		game->players[playersIndex].playerCards = (Card*)malloc(BEGINNING_CARDS_AMOUNT * sizeof(Card));
		if (game->players[playersIndex].playerCards == NULL)
			ifAllocationFailled();
		for (int cardsIndex = 0; cardsIndex < BEGINNING_CARDS_AMOUNT; cardsIndex++)
			game->players[playersIndex].playerCards[cardsIndex] = handOutCard(game);
		game->players[playersIndex].currentCardsSize = BEGINNING_CARDS_AMOUNT;
		game->players[playersIndex].amountOfCards = BEGINNING_CARDS_AMOUNT;
	}
}

//function for case allocation is failled (print error and exit)
void ifAllocationFailled() {
	printf("Memory allocation error!!!\n");
	exit(1);
}

//function for all initial upper card
void initialUpperCard(Game* game) {
	game->upperCard = handOutCard(game);
	while (game->upperCard.type > 10) {
		game->upperCard = handOutCard(game);
	}
}

//function to print upper card
void printUpperCard(Card upperCard) {
	printf("\nUpper card:\n");
	printCard(upperCard, typePrintHelper);
}

//function to print all player cards
void printPlayerCards(Player player) {
	for (int cardIndex = 0; cardIndex < player.amountOfCards; cardIndex++) {
		printf("Card #%d:\n", cardIndex + 1);
		printCard(player.playerCards[cardIndex], typePrintHelper);
	}
}

//game start operation function
void gameOperate(Game* game) {
	game->playersTurn = FIRST_TURN;
	game->direction = TURN_UP;
	game->isTakiOn = false;
	game->isGameOver = false;
	initialCardsHandOut(game);
	initialUpperCard(game);
	while (!game->isGameOver) {
		printUpperCard(game->upperCard);
		playTurn(game);
		gameOver(game);
		nextTurn(game);
	}
}

//function to announce winner if the game is over
void gameOver(Game* game) {
	Player* player = getCurrentPlayer(game);
	if (player->amountOfCards == ZERO_CARDS) {
		game->isGameOver = true;
		printf("The winner is... %s! Congratulations!", player->nameOfPlayer);
	}
}

//function to play a turn of a player
void playTurn(Game* game) {
	Player* player = getCurrentPlayer(game);
	printf("%s's turn:\n\n", player->nameOfPlayer);
	printPlayerCards(*player);
	getPlayerChoice(game);
	actPlayerChoice(game);
}

//function to get the player choice
void getPlayerChoice(Game* game) {
	game->isChoiceValid = false;
	Player* player = getCurrentPlayer(game);
	while (!game->isChoiceValid) {
		game->isChoiceValid = true; //exit loop if card is valid
		if (game->isTakiOn)
			printf("Please enter 0 if you want to finish your turn\n"); //if player wants to exit taki
		else
			printf("Please enter 0 if you want to take a card from the deck\n"); //if player wants new card
		printf("or 1-%d if you want to put one of your cards in the middle:\n", player->amountOfCards);
		scanf("%d", &game->playersChoice);
		isChoiceValid(game);
		if(game->playersChoice > TAKE_NEW_CARD)
		isCardValid(game);
	}
}

//function to check if choice entered was valid
void isChoiceValid(Game* game) {
	Player* player = getCurrentPlayer(game);
	if (game->playersChoice < TAKE_NEW_CARD || game->playersChoice > player->amountOfCards) {
		printf("Invalid choice! Try again.\n");
		game->isChoiceValid = false; //if card is not valid stay in loop
	}
}

//function to check if card entered by user is valid by the game rules
void isCardValid(Game* game) {
	Player* player = getCurrentPlayer(game);
		game->chosenCard = player->playerCards[game->playersChoice - 1];
		if (game->chosenCard.type != COLOR_CARD) //color card can be placed on every card
			if (game->chosenCard.type != game->upperCard.type && game->chosenCard.color != game->upperCard.color) { 
				printf("Invalid card! Try again.\n"); //if card entered's color or type not matching the upper card details
				game->isChoiceValid = false; //if card is not valid stay in loop
			}
}

//function to act the player's choice
void actPlayerChoice(Game* game) {
	Player* player = getCurrentPlayer(game);
	if (game->playersChoice == TAKE_NEW_CARD) {
		if (game->isTakiOn) {
			game->isTakiOn = false;
			uniqueCardsActivation(game);
		}
		else
			GetNewCard(game);
	}
	else {
		player->playerCards[game->playersChoice - 1] = player->playerCards[player->amountOfCards - 1];
		player->amountOfCards--;
		game->upperCard = game->chosenCard;
		uniqueCardsActivation(game);
	}
}

//function to change the turn
void nextTurn(Game* game) {
	if(!game->isTakiOn)
	game->playersTurn += game->direction;
	if (game->playersTurn == game->numberOfPlayers)
		game->playersTurn = FIRST_TURN;
	else if (game->playersTurn == TURN_DOWN)
		game->playersTurn = game->numberOfPlayers - 1;
}

//function to get a new card from deck
void GetNewCard(Game* game) {
	Player* player = getCurrentPlayer(game);
 	if (player->amountOfCards == player->currentCardsSize) { //if log size of cards array equal to pysical
		Card* temp = (Card*)malloc(player->currentCardsSize * 2 * sizeof(Card));
		if (temp == NULL)
			ifAllocationFailled();
		for (int cardIndex = 0; cardIndex < player->amountOfCards; cardIndex++)
			temp[cardIndex] = player->playerCards[cardIndex];
		free(player->playerCards);
		player->playerCards = temp;
		player->currentCardsSize *= 2;
	}
	player->playerCards[player->amountOfCards] = handOutCard(game);
	player->amountOfCards++;
}

//activate unique cards
void uniqueCardsActivation(Game* game){
	switch (game->upperCard.type) {
	case PLUS_CARD:
		plusCard(game);
		break;
	case STOP_CARD:
		stopCard(game);
		break;
	case CHANGE_DIRECTION_CARD:
		changeDirectionCard(game);
		break;
	case TAKI_CARD:
		takiCard(game);
		break;
	case COLOR_CARD:
		colorCard(game);
	}
}

//function for the plus card
void plusCard(Game* game) {
	Player* player = getCurrentPlayer(game);
	if (player->amountOfCards == ZERO_CARDS)
		GetNewCard(game);
	else
		game->playersTurn -= game->direction;
}

//function for the stop card
void stopCard(Game* game) {
	Player* player = getCurrentPlayer(game);
	if (player->amountOfCards == ZERO_CARDS && game->numberOfPlayers == TWO_PLAYERS)
		GetNewCard(game);
	else
	game->playersTurn += game->direction;
}

//function that change the direction of the game
void changeDirectionCard(Game* game) {
	if(game->numberOfPlayers == TWO_PLAYERS)
		game->playersTurn -= game->direction;
	else if (game->direction == TURN_UP)
		game->direction = TURN_DOWN;
	else
		game->direction = TURN_UP;
}

//function for the taki card
void takiCard(Game* game) {
	if(game->playersChoice > TAKE_NEW_CARD)
	game->isTakiOn = true;		
}

//function for the color card
void colorCard(Game* game) {
	int colorChoice;
	game->isTakiOn = false;
	game->upperCard.type = COLOR_CARD;
	bool isValidColor = false;
	while (!isValidColor) {
		isValidColor = true; //exit loop if choice is valid
		printf("Please enter your color choice:\n1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
		scanf("%d", &colorChoice);
		switch (colorChoice) {
		case YELLOW:
			game->upperCard.color = PRINTABLE_YELLOW_CARD;
			break;
		case RED:
			game->upperCard.color = PRINTABLE_RED_CARD;
			break;
		case BLUE:
			game->upperCard.color = PRINTABLE_BLUE_CARD;
			break;
		case GREEN:
			game->upperCard.color = PRINTABLE_GREEN_CARD;
			break;
		default:
			printf("Invalid color! Try again.");
			isValidColor = false; //return loop if color choice entered is invalid
			break;
		}
	}
}

//function to take care of all the statistics
void initialStatistics(Game* game) {
	for (int i = 0; i < MAX_CARD_TYPES; i++) {
		game->statistics[i].cardIndex = i + 1;
		game->statistics[i].frequency = 0;
	}
}

//function to reorder and print the game statistics
void gameStatistics(Game* game) {
	mergeSort(game->statistics, MAX_CARD_TYPES); //sort statistics from the second place (skip index 0 which is empty)
	printStatistics(*game);
}

//function to print the stats table
void printStatistics(Game game) {
		printf("\n************ Game Statistics ************\n");
		printf("Card # | Frequency\n");
		printf("___________________\n");
	for (int i = 0; i < MAX_CARD_TYPES; i++) 
	{
		printf("  %s |%5d\n", typePrintHelper[game.statistics[i].cardIndex], game.statistics[i].frequency);
	}
}

//function to sort the statistics array recursivly
void mergeSort(Stats* arr, int size){
	Stats tempArr[MAX_CARD_TYPES];
	if (size == 1)
		return;

	mergeSort(arr, size / 2);
	mergeSort(arr + size / 2, size - size / 2);
	merge(arr, size / 2, arr + size / 2, size - size / 2, tempArr);
	copyArr(arr, tempArr, size);
}

//function that gets the two sorted sides of the statistics array and get them into one array in a sorted manner
void merge(Stats* leftSide, int leftSize, Stats* rightSide, int rightSize, Stats* res) {
	int leftInd, rightInd, resInd;
	leftInd = rightInd = resInd = 0;

	while (leftInd < leftSize && rightInd < rightSize) {
		if (leftSide[leftInd].frequency > rightSide[rightInd].frequency) {
			res[resInd] = leftSide[leftInd];
			leftInd++;
		}
		else {
			res[resInd] = rightSide[rightInd];
			rightInd++;
		}
		resInd++;
	}
	while (leftInd < leftSize) {
		res[resInd] = leftSide[leftInd];
		resInd++;
		leftInd++;
	}
	while (rightInd < rightSize) {
		res[resInd] = rightSide[rightInd];
		resInd++;
		rightInd++;
	}
}

//function to move data from one array to another
void copyArr(Stats dest[], Stats src[], int size) {
	int i;

	for (i = 0; i < size; i++)
		dest[i] = src[i];
}

//function to free all the memory allocated in the game
freeMemoryAllocated(Game* game) {
	for (int playersIndex = 0; playersIndex < game->numberOfPlayers; playersIndex++) {
		free(game->players[playersIndex].playerCards);
	}
	free(game->players);
}

//name shortcut to use for the player that it's his turn
Player* getCurrentPlayer(Game* game) {
	return &game->players[game->playersTurn];
}

//functions to print the cards in color
void changeColor(char color) {

	switch (color)
	{
	case PRINTABLE_YELLOW_CARD:
		yellow();
		break;

	case PRINTABLE_GREEN_CARD:
		green();
		break;

	case PRINTABLE_RED_CARD:
		red();
		break;

	case PRINTABLE_BLUE_CARD:
		blue();
		break;
	}
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

void resetColor() {
	printf("\033[0m");
}