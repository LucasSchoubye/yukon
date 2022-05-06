#include <stdio.h>
#include <stdlib.h>
#include<time.h>
/**
 * Yukon: Play the traditional card-game in C.
 */

struct Card {
    char suit; // 1. Spar, 2. Ruder, 3. Klør, 4. Hjerter
    char value; // 1. Es, 11. Knægt, 12. Dame, 13. Konge
    int shown;
    struct Card* next;
    struct Card* prev;
};

// Create Test Columns
struct Card* columnArray[7];
struct Card* foundationArray[4];
struct Card* column_test = NULL;

// Create Test Cards
int card_num = 0;
int cards_placed = 0;
int cardsInColumn = 1;
int scanLoopIndex = 0;
int gameStarted = 0;
char fileName[255];
struct Card* deck[52];
struct Card* card_new = NULL;
struct Card* card_old = NULL;

// Methods
void printPrevList(struct Card *pCard);
void printGameState();
const char* checkFileInput(char* inputLine, int inputPointer);
void printAnyCard(struct Card *pCard);
void gameCommand(char fromChar, char fromNumber, char cardValue, char cardSuit, char toChar, char toNumber);
struct Card* searchSpecificCard(struct Card* n, char cardSuit, char cardValue);
struct Card* searchLastCard(struct Card* n);//, char cardSuit, char cardValue);
void mixCards();
void doCommand(char firstChar, char secChar);
int hasWon();
FILE *fopen(const char *filename, const char *mode);
int fscanf(FILE *stream, const char *format, ...);
int fputs(const char *str, FILE *stream);

// Play the game
int main()
{
    // Create Columns
    for (int i = 0; i < 7; ++i)
    {
        // Create a Column Start
        column_test = (struct Card*)malloc(sizeof(struct Card));
        column_test->value = 0; // Top
        column_test->suit = '0'; // Top
        column_test->next = NULL; // Top
        column_test->prev = NULL; // Top
        // Save card in array
        columnArray[i] = column_test;
    }

    // Create foundations
    for (int i = 0; i < 4; ++i)
    {
        // Create a Column Start
        column_test = (struct Card*)malloc(sizeof(struct Card));
        column_test->value = 0; // Top
        column_test->suit = '0'; // Top
        column_test->next = NULL; // Top
        column_test->prev = NULL; // Top
        // Save card in array
        foundationArray[i] = column_test;
    }

    // Create Deck of Cards
    for (int i = 1; i < 5; ++i)
    {
        char suit_type;
        switch (i)
        {
            case 1:
                suit_type = 'S';
                break;
            case 2:
                suit_type = 'R';
                break;
            case 3:
                suit_type = 'K';
                break;
            case 4:
                suit_type = 'H';
                break;
        }

        for (int i = 1; i < 14; ++i)
        {
            //struct Card* card_1 = NULL;
            card_new = (struct Card*)malloc(sizeof(struct Card));
            card_new->value = i; // assign data in first node
            card_new->suit = suit_type; // assign data in first node
            deck[card_num] = card_new;
            card_num++;
        }
    }

    // Shuffle deck randomly
    srand(time(0));
    for (int i = 0; i < 1000; ++i) {
        mixCards(deck);
    }

    /*
    // Create 7 columns of cards (set-up game)
    for (int i = 0; i < 7; ++i)
    {
        card_old = columnArray[i];
        for (int j = 0; j < cardsInColumn; ++j)
        {
            card_new = deck[cards_placed];
            card_new->next = card_old; // Link first node with'
            card_new->prev = NULL; // Link first node with'
            card_old->prev = card_new; // Link first node with'
            card_old = card_new;
            cards_placed++;
        }
        // Print Column
        printPrevList(columnArray[i]);
        printf("%c\n", "");
        // How big is the next column
        switch(cards_placed)
        {
            case 1:
                cardsInColumn = 6;
                break;
            case 7:
                cardsInColumn = 7;
                break;
            case 14:
                cardsInColumn = 8;
                break;
            case 22:
                cardsInColumn = 9;
                break;
            case 31:
                cardsInColumn = 10;
                break;
            case 41:
                cardsInColumn = 11;
                break;
        }
    }
     */

    // Play the game
    printf("%s\n", "WELCOME TO YUKON: PRESS 'P' TO START");
    char *input[9];
    char inputLine[255];
    int inputPointer = 1;
    while (hasWon() || gameStarted == 0)
    {
        // Reset Input array
        for (int i = 0; i < 9; ++i) {
            input[i] = NULL;
        }
        for (int i = 0; i < 255; ++i) {
            inputLine[i] = NULL;
        }

        int spaceTrue = 0;
        // Scan Next Command
        while (inputLine[inputPointer - 1] != '\n') {
            // Read until you press enter
            scanf("%c", &inputLine[inputPointer]);
            if (inputLine[inputPointer] == ' ') {
                spaceTrue = 1;
                *fileName = checkFileInput(&inputLine, inputPointer);
                //printf("%s", fileName);
                break;
            }
            inputPointer++;
        }
        int i = 0;
        while(fileName[i] != NULL) {
            i++;
        }
        i--;
        printf("%d\n", i);
        printf("%s", fileName);
        //printf("%s", "fuck mit net b");
        if(spaceTrue != 1) {
            doCommand(inputLine[inputPointer - 3], inputLine[inputPointer - 2]);
        } else {
            doCommand(inputLine[inputPointer - 2], inputLine[inputPointer - 1]);
        }

        // If game is started
        if (gameStarted  == 1)
        {
            // Do the command
            //gameCommand(input[0+1], input[1+1], input[3+1], input[4+1], input[6+1], input[7+1]);
            if (inputPointer > 8)
            {
                gameCommand(inputLine[inputPointer - 9], inputLine[inputPointer - 8], inputLine[inputPointer - 6],
                            inputLine[inputPointer - 5], inputLine[inputPointer - 3], inputLine[inputPointer - 2]);
            }
            inputPointer = 1;

            // Display Game-state
            /*
            for (int i = 0; i < 4; i++) {
                printf("F:", "");
                printPrevList(foundationArray[i]->prev);
                printf("\n", "");
            }
            for (int i = 0; i < 7; i++) {
                printf("%d: ", i);
                printPrevList(columnArray[i]);
                printf("\n", "");
            }
             */
            // Updated Game State
            printf("\n", "");
            printGameState();
        }

    }
    printf("%s\n", "YOU WON THE GAME");
}
const char* checkFileInput(char* inputLine, int inputPointer) {
    int i = 0;
    if (inputLine[inputPointer] == ' ' && inputLine[inputPointer - 2] == 'L' && inputLine[inputPointer - 1] == 'D') {
        inputPointer++;
        while(fileName[i] != '\n') {
            scanf("%c", &inputLine[inputPointer]);
            fileName[i] = inputLine[inputPointer];
            if(fileName[i] == '\n') {
                break;
            }
            inputPointer++;
            i++;
        }
    }
    return *fileName;
}
void mixCards(struct Card* deck[]){
    struct Card *mixCard;

    int random_index1 = rand() % (52 + 1 - 0) + 0;
    int random_index2 = rand() % (52 + 1 - 0) + 0;

    // Flip
    mixCard = deck[random_index1];
    deck[random_index1] = deck[random_index2];
    deck[random_index2] = mixCard;
}

void mixCardsInterleaved(struct Card* deck[]) {

    int topDeck = rand() % (52 + 1 - 0) + 0;

    int* shuffle1 [topDeck];
    int* shuffle2 [52-topDeck];

    // if topdeck > 52/2
    // switch shuffle1 og shuffle 2

    for (int i = 0; i < topDeck; i++) {
        shuffle1[i] = deck[i];
    }
    for (int i = 0; i < 52-topDeck; i++) {
        shuffle2[i] = deck[topDeck + i];
    }
    for (int i = 0; i < topDeck; i++) {

        // Insert the first deck
        if (shuffle1[i] != NULL) {
            if (topDeck >= 52/2) {
                deck[i*2] = shuffle1[i];
            } else {
                deck[i*2-1] = shuffle1[i];
            }
        }
        // Insert the second deck
        if (shuffle2[i-topDeck] != NULL) {
            if (topDeck >= 52/2) {
                deck[i*2-1] = shuffle2[i];
            } else {
                deck[i*2] = shuffle2[i];
            }
        }
    }
    if (topDeck >= 52) {
        for (int i = 0; i < 2; ++i) {
        //too kogt laver færdig i morgen:)
        }
    }
}

void printPrevList(struct Card *pCard) {
    while (pCard != NULL)
    {
        if (pCard->value < 10)
        {printf(" %d", pCard->value);}else
        {
            switch (pCard->value) {
                case 10:
                    printf(" %c", 'T');
                    break;
                case 11:
                    printf(" %c", 'J');
                    break;
                case 12:
                    printf(" %c", 'Q');
                    break;
                case 13:
                    printf(" %c", 'K');
                    break;
            }
        }
        printf("%c", pCard->suit);
        pCard = pCard->prev;
    }
}

void printAnyCard(struct Card *pCard)
{
    if (pCard->value < 10)
    {printf(" %d", pCard->value);}else
    {
        switch (pCard->value) {
            case 10:
                printf(" %c", 'T');
                break;
            case 11:
                printf(" %c", 'J');
                break;
            case 12:
                printf(" %c", 'Q');
                break;
            case 13:
                printf(" %c", 'K');
                break;
        }
    }
    printf("%c", pCard->suit);
}

void printHorList(struct Card *pCard, int rowNumber) {
    int curRow=0;
    int cardPrinted = 0;
    while (pCard != NULL)
    {
        // If the correct card
        if (curRow == rowNumber)
        {
            // If the card is shown
            if (pCard->shown == 0) {
                // Print the card
                if (pCard->value < 10) {
                    printf(" %d", pCard->value);
                } else {
                    switch (pCard->value) {
                        case 10:
                            printf(" %c", 'T');
                            break;
                        case 11:
                            printf(" %c", 'J');
                            break;
                        case 12:
                            printf(" %c", 'Q');
                            break;
                        case 13:
                            printf(" %c", 'K');
                            break;
                    }
                }
                printf("%c\t", pCard->suit);
                cardPrinted = 1;
            } else
            {
                // Print Blank square
                printf(" []\t", "");
                cardPrinted = 1;
            }
        }
        curRow++;
        pCard = pCard->prev;
    }
    if (cardPrinted == 0)
    {
        printf("%s\t", "  ");
    }
}

void gameCommand(char fromChar, char fromNumber, char cardValue, char cardSuit, char toChar, char toNumber) {

    // Variables
    int toColumnNumber = 0;
    int fromColumnNumber = 0;
    struct Card* targetCard; // The card you will place a card on

    // Read what column number you are taking from
    fromColumnNumber = (int)(fromNumber-'0');

    // Read location to move the card
    toColumnNumber = (int)(toNumber-'0');

    // Find the first card to take
    struct Card* firstCard = searchSpecificCard(columnArray[fromColumnNumber], cardSuit, cardValue); // The card you wish to move

    // What is the first card on the to column/foundation
    if (toChar == 'F')
    {
        // If moving to a foundation
        targetCard = searchLastCard(foundationArray[toColumnNumber]);
    }
    else if (toChar == 'C')
    {
        // If moving to a column
        targetCard = searchLastCard(columnArray[toColumnNumber]);
    }


    // Can you move the card?
    if (toChar == 'C') {
        if ((targetCard->suit != firstCard->suit && targetCard->value - 1 == firstCard->value) || (targetCard->value == 0 && firstCard->value == 13)) {
            // Move the card
            firstCard->next->prev = NULL;
            firstCard->next = targetCard;
            targetCard->prev = firstCard;
        } else {
            printf("%s\n", "ILLEGAL MOVE");
        }
    } else if (toChar == 'F')
    {
        if (targetCard->value == firstCard->value -1 && (targetCard->suit == '0' || targetCard->suit == firstCard->suit) && firstCard->prev == NULL)//(targetCard->suit == firstCard->suit || firstCard->suit == '0') && targetCard->value - 1 == firstCard->value) {
        {
            // Move the card
            firstCard->next->prev = NULL;
            firstCard->next = targetCard;
            targetCard->prev = firstCard;
            printf("%s\n", "CARD WAS MOVED TO FOUNDATION");
        } else {
            printf("%s\n", "YOU CANNOT MOVE TO TARGET FOUNDATION");
        }
    }
}

void printList(struct Card* n)
{
    while (n != NULL) {
        printf(" %d ", n->value);
        printf("(%c)", n->suit);
        n = n->next;
    }
}

void printDeck()
{
    // Print Deck
    for (int i = 0; i < 52; ++i)
    {
        struct Card* n = deck[i];
        printAnyCard(n);
    }
}

struct Card* searchSpecificCard(struct Card* n, char cardSuit, char cardValue)
{
    struct Card* foundCard = columnArray[6]->prev;
    int calculatedCardValue = NULL;

    // Convert Card Value to integer
    switch (cardValue) {
        case 'T':
            calculatedCardValue = 10;
            break;
        case 'J':
            calculatedCardValue = 11;
            break;
        case 'Q':
            calculatedCardValue = 12;
            break;
        case 'K':
            calculatedCardValue = 13;
            break;
    }

    // Find Card
    if (calculatedCardValue == NULL) {
        while (n != NULL) {
            if (n->value == cardValue - '0' && n->suit == cardSuit)// && n->value == cardValue)
            {
                foundCard = n;
                return foundCard;
            }
            n = n->prev;
        }
    } else
    {
        while (n != NULL) {
            if (n->value == calculatedCardValue && n->suit == cardSuit)// && n->value == cardValue)
            {
                foundCard = n;
                return foundCard;
            }
            n = n->prev;
        }
    }
    return foundCard;
}

struct Card* searchLastCard(struct Card* n)//, char cardSuit, char cardValue)
{
    struct Card* foundCard = columnArray[6]->prev;

    // Print Deck
    while (n != NULL)
    {
        if (n->prev == NULL)
        {
            foundCard = n;
            return foundCard;
        }
        n = n->prev;
    }
    return foundCard;
}

void doCommand (char firstChar, char secChar) {

    // Quit Game (QQ)
    if ('Q' == firstChar && 'Q' == secChar)
    {
        // Quit
        printf("%s", "QUITTING GAME");
        exit(0);
        scanLoopIndex = 10;
    }

    // Shuffle Randomly (SR)
    if ('S' == firstChar && 'R' == secChar) {
        // Shuffle
        //scanLoopIndex = 10;
        mixCards(deck);
        printf("%s", "Shuffled Card\n");
    }

    // Show Deck (SW)
    if ('S' == firstChar && 'W' == secChar) {
        // Shuffle
        printDeck();
    }

    // Play (P)
    if ('P' == secChar) {
        // Create 7 columns of cards (set-up game)
        for (int i = 0; i < 7; ++i) {
            card_old = columnArray[i];
            for (int j = 0; j < cardsInColumn; ++j) {
                card_new = deck[cards_placed];
                card_new->shown = 0;
                card_new->next = card_old; // Link first node with'
                card_new->prev = NULL; // Link first node with'
                card_old->prev = card_new; // Link first node with'

                // Determine which cards will be shown
                switch(cardsInColumn){

                    case 6:
                        if (j < 1){
                            card_new->shown = 1;
                        }
                        break;
                    case 7:
                        if (j < 2){
                            card_new->shown = 1;
                        }
                        break;
                    case 8:
                        if (j < 3){
                            card_new->shown = 1;
                        }
                        break;
                    case 9:
                        if (j < 4){
                            card_new->shown = 1;
                        }
                        break;
                    case 10:
                        if (j < 5){
                            card_new->shown = 1;
                        }
                        break;
                    case 11:
                        if (j < 6){
                            card_new->shown = 1;
                        }
                        break;
                }

                // Next Card
                card_old = card_new;
                cards_placed++;
            }

            // How big is the next column
            switch (cards_placed) {
                case 1:
                    cardsInColumn = 6;
                    break;
                case 7:
                    cardsInColumn = 7;
                    break;
                case 14:
                    cardsInColumn = 8;
                    break;
                case 22:
                    cardsInColumn = 9;
                    break;
                case 31:
                    cardsInColumn = 10;
                    break;
                case 41:
                    cardsInColumn = 11;
                    break;
            }
        }
        scanLoopIndex = 10;
        gameStarted = 1;
    }

    // Load (LD)
    FILE * fp;
    char readFile[150];
    if ('L' == firstChar && 'D' == secChar) {
        printf("%s", fileName);
        fp = fopen(fileName, "r");

        while(!feof(fp)) {
            fgets(readFile, 150, fp);
            puts(readFile);
        }
        fclose(fp);
    }
}

int hasWon()
{
    int result = 0;
    for (int i = 0; i < 7; ++i)
    {
        if (columnArray[i]->prev != NULL){
            result = 1;
        }
    }
    return result;
}

void printGameState()
{

    // Print Game Line
    for (int i = 0; i < 7; ++i) {
        // Print Columns
        printf(" %c%d\t", 'C', i);
    }

    // Print first row of cards
    printf("\n","");
    for (int q = 0; q < 13; ++q)
    {
        for (int i = 0; i < 7; ++i)
        {
            // Print Columns
            printHorList(columnArray[i],q+1);
        }
        // Print Foundations
        if (q > 0 && q < 5)
        {
            printf("\t %c%d",'F',q-1);
            printAnyCard(foundationArray[q-1]);
        }

        // Next Line
        printf("\n","");
    }
    printf("\n%s\n","INPUT:");

};