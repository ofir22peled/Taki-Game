#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> // in order to use the "rand" and "srand" functions
#include <time.h>	// in order to use "time" function
#include <stdbool.h> //in order to use "boolean" types

//define declartion
#define TAKE_NEW_CARD 0 //if player choose to get a card from deck
#define TURN_UP 1 //one direction of a turn change
#define TURN_DOWN -1 //second drection of a turn change
#define MAX_CARD_COLORS 4
#define BEGINNING_CARDS_AMOUNT 4
#define NUM_OF_COLORS 4
#define MAX_TYPE_LENGTH 6
#define PLUS_CARD 10
#define STOP_CARD 11
#define CHANGE_DIRECTION_CARD 12
#define TAKI_CARD 13
#define COLLOR_CARD 14
#define YELLOW 1
#define RED 2
#define BLUE 3
#define GREEN 4
#define PRINTABLE_YELLOW_CARD 'Y' //print "Y" in the card if yellow
#define PRINTABLE_GREEN_CARD 'G' //print "G" in the card if Green
#define PRINTABLE_RED_CARD 'R' //print "R" in the card if Red
#define PRINTABLE_BLUE_CARD 'B' //print "B" in the card if Blue
#define NO_COLOR ' ' //no color for the color card
#define PRINTABLE_PLUS_CARD "+" //print "+" for the plus card
#define PRINTABLE_STOP_CARD "STOP" //print "STOP" for the stop card
#define PRINTABLE_CHANGE_DIRECTION "<->" //print "<->" for the 'change direction' card
#define PRINTABLE_TAKI "TAKI" //print "TAKI" for the 'taki' card
#define PRINTABLE_COLOR "COLOR" //print "COLOR" for the 'color' card
#define MAX_CARD_TYPES 14
#define MAX_NAME_LENGTH 21
//end of defined declartion

//decleration of structs
typedef struct cards //struct to get enter info for each card
{
	char color;
	int type;
} Card;

typedef struct player //struct to get data for all the players
{
	int amountOfCards; //actual number of cards for player
	int currentCardsSize; //pysical number of cards saved to player
	char nameOfPlayer[MAX_NAME_LENGTH]; //array to hold the name of player
	Card* playerCards;
} Player;
//end of structs declartion

typedef struct game
{
	Player* player;
	Card upperCard;
	int numberOfPlayers;
	int direction;
	int color;
	bool taki;
}Game;

typedef struct statistics { //struct to hold statistics information
	int cardIndex;
	int frequency;
} stats;

void printWelcome();
int getNumOfPlayers();
Player* getPlayersNames(int numOfPlayers);
Card handOutCard();
void printCard(Card card, char str[][MAX_TYPE_LENGTH]);
void ifAllocationFailled();
void changeColor(char color);
void yellow();
void green();
void red();
void blue();
void resetColor();
void initialCardsHandOut(Player* player, int numOfPlayers);
void initialUpperCard(Card* upperCard);
void printUpperCard(Card upperCard, char str[][MAX_TYPE_LENGTH]);
void gameOperate(Player* player, int numOfPlayers, char str[][MAX_TYPE_LENGTH], Card* upperCard);
void printPlayerCards(Player player, char str[][MAX_TYPE_LENGTH]);
void playTurn(Player* player, int numOfPlayers, char* str, int turn, Card* upperCard);
void actPlayerChoice(Player* player, int playerChoice, Card* upperCard, char* strForPrint, int* playersTurn);
void GetNewCard(Player* player);
void swap(Card* usedCard, Card* lastCardInPack);
int getPlayerChoice(Player player);
bool ifGameIsOver(Player player);
//end of function declartion

void main()
{
	//srand(time(NULL)); //declartion before using the rand function
	int numberOfPlayers;
	Game game;
	Player* playersArr = NULL;
	Card upperCard;
	char strForPrint[][MAX_TYPE_LENGTH] = { "  0  ", "  1  ", "  2  ", "  3  ", "  4  ",
	"  5  ", "  6  ", "  7  ", "  8  ","  9  ", "  +  ", " STOP", " <-> ", " TAKI", "COLOR" };
	stats statisticsArr[14];
	char stats = { 0 };

	printWelcome();
	numberOfPlayers = getNumOfPlayers();
	playersArr = getPlayersNames(numberOfPlayers);
	initialCardsHandOut(playersArr, numberOfPlayers);
	initialUpperCard(&upperCard);
	printUpperCard(upperCard, strForPrint);
	gameOperate(playersArr, numberOfPlayers, strForPrint, &upperCard, game);

	/*
	for (int playersIndex = 0; playersIndex < numOfPlayers; playersIndex++) {
		//free(Player * playerCards);
	}
	free(playersArr);
	*/
}

//function to print welcome
void printWelcome()
{
	printf("************  Welcome to TAKI game !!! ***********\n");
}

//function to get all the players names
Player* getPlayersNames(int numOfPlayers)
{
	Player* playersArr = NULL;
	playersArr = (Player*)malloc(numOfPlayers * sizeof(Player));
	if (playersArr == NULL)
		ifAllocationFailled();

	for (int playersIndex = 0; playersIndex < numOfPlayers; playersIndex++) {
		printf("Please enter the first name of player #%d:\n", playersIndex + 1);
		scanf("%s", playersArr[playersIndex].nameOfPlayer);
	}
	return playersArr;
}

//function to get the number of player at the game
int getNumOfPlayers() {
	int numOfPlayers;
	printf("Please enter the number of players:\n");
	scanf("%d", &numOfPlayers);
	return numOfPlayers;
}

//function to hand out one card
Card handOutCard() {
	Card cardForHandOut;
	cardForHandOut.type = 1 + rand() % MAX_CARD_TYPES; //getting a random new card (type)
	if (cardForHandOut.type == 14)
		cardForHandOut.color = NO_COLOR;
	else {
		int numColor = rand() % MAX_CARD_COLORS; //getting a random new card (color)
		switch (numColor)
		{
		case 0:
			cardForHandOut.color = PRINTABLE_YELLOW_CARD;
			break;
		case 1:
			cardForHandOut.color = PRINTABLE_GREEN_CARD;
			break;
		case 2:
			cardForHandOut.color = PRINTABLE_RED_CARD;
			break;
		case 3:
			cardForHandOut.color = PRINTABLE_BLUE_CARD;
			break;
		}
	}
	return cardForHandOut;
}

//function to print one card
void printCard(Card cardForPrint, char str[][MAX_TYPE_LENGTH]) {
	changeColor(cardForPrint.color);
	printf("*********\n*       *\n");
	printf("* %s *\n*   %c   *\n", str[cardForPrint.type], cardForPrint.color);
	printf("*       *\n*********\n\n");
	resetColor();
}

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

//function to distribute the initial cards for the players
void initialCardsHandOut(Player* player, int numOfPlayers) {
	for (int playersIndex = 0; playersIndex < numOfPlayers; playersIndex++) {
		player[playersIndex].playerCards = (Card*)malloc(BEGINNING_CARDS_AMOUNT * sizeof(Card));
		if (player[playersIndex].playerCards == NULL)
			ifAllocationFailled();
		for (int cardsIndex = 0; cardsIndex < BEGINNING_CARDS_AMOUNT; cardsIndex++)
			player[playersIndex].playerCards[cardsIndex] = handOutCard();
		player[playersIndex].currentCardsSize = BEGINNING_CARDS_AMOUNT;
		player[playersIndex].amountOfCards = BEGINNING_CARDS_AMOUNT;
	}
}

void ifAllocationFailled() {
	printf("Memory allocation error!!!\n");
	exit(1);
}

void initialUpperCard(Card* upperCard) {
	*upperCard = handOutCard();
	while (upperCard->type > 10) {
		*upperCard = handOutCard();
	}
}

void printUpperCard(Card upperCard, char str[][MAX_TYPE_LENGTH]) {
	printf("Upper card:\n");
	printCard(upperCard, str);
}

void printPlayerCards(Player player, char str[][MAX_TYPE_LENGTH]) {
	for (int cardIndex = 0; cardIndex < player.amountOfCards; cardIndex++) {
		printf("Card #%d:\n", cardIndex + 1);
		printCard(player.playerCards[cardIndex], str);
	}
}

void gameOperate(Player* player, int numOfPlayers, char str[][MAX_TYPE_LENGTH], Card* upperCard, Game game) {
	bool gameOver = false;
	int PlayersTurn;
	while (!gameOver)
	{
		for (PlayersTurn = 0; PlayersTurn < numOfPlayers; PlayersTurn++) {
			playTurn(player, numOfPlayers, str, PlayersTurn, upperCard);
			printUpperCard(*upperCard, str);
		}
		changeTurn(game);

		gameOver = ifGameIsOver(player[PlayersTurn]);
	}
}

int changeTurn(Game game) {

	if (PlayersTurn > numOfPlayers && game.direction == TURN_UP)
		PlayersTurn = 0;
	else if (PlayersTurn == TURN_DOWN)
		PlayersTurn = numOfPlayers - 1;
}

//function to check if the player finished his cards to end game
bool ifGameIsOver(Player player) {
	if (player.amountOfCards == 0)
		return true;
	else
		return false;
}
//function to play a turn of a player
void playTurn(Player* player, int numOfPlayers, char* strForPrint, int playersTurn, Card* upperCard) {
	printf("%s's turn:\n\n", player[playersTurn].nameOfPlayer);

	printPlayerCards(player[playersTurn], strForPrint);
	int playerChoice = getPlayerChoice(player[playersTurn]);
	actPlayerChoice(&player[playersTurn], playerChoice, upperCard, strForPrint, playersTurn);
}

//function to get the player choice
int getPlayerChoice(Player player) {
	int playerChoice;
	printf("Please enter 0 if you want to take a card from the deck\n");
	printf("or 1 - %d if you want to put one of your cards in the middle:\n", player.currentCardsSize);
	scanf("%d", &playerChoice);

	return playerChoice;
}
void actPlayerChoice(Player* player, int playerChoice, Card* upperCard, char* strForPrint, int* playersTurn) {
	if (playerChoice == TAKE_NEW_CARD && game
		GetNewCard(player);
	else {
		Card chosenCard = player->playerCards[playerChoice - 1];
		swap(&player->playerCards[playerChoice - 1], &player->playerCards[player->amountOfCards - 1]);
		player->amountOfCards--;
		*upperCard = chosenCard;
		if (chosenCard.type == PLUS_CARD)
			*playersTurn--;
	}
}

void swap(Card* usedCard, Card* lastCardInPack) {
	Card tempCard = *usedCard;
	*usedCard = *lastCardInPack;
	*lastCardInPack = tempCard;
}

//function to get a new card from deck
void GetNewCard(Player* player) {
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
	player->playerCards[player->amountOfCards] = handOutCard();
	player->amountOfCards++;

}
