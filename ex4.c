/******************
Name: Thomas Kowalski
ID: 322887019
Assignment: ex4
*******************/
#include <stdio.h>
#include <string.h>

typedef struct {
    int row;
    int col;
    int len;
    char dir;
} NewSlot;

#define PYRAMID_LEVEL 5
#define ASCII_GAP1 1
#define ASCII_GAP2 2
#define NOK -1
#define SIZE4 20 // max size for task 4
#define MAX_SLOTS 100// max size for task 5
#define WORDS 100
#define GRID 30 
#define LENGTH 16
void task1RobotPaths();
void task2HumanPyramid();
void task3ParenthesisValidator();
void task4QueensBattle();
void task5CrosswordGenerator();

int pathAmount(int col, int row);
float checkWeight(int col, int row, float weightsArr[PYRAMID_LEVEL][PYRAMID_LEVEL]);
int isBalanced(char curChar);
int isClosed (char symbol);

int solutionManager(char board[SIZE4][SIZE4], char solution[SIZE4][SIZE4], int boardSize, int row, int col);
int initArray(char arr[SIZE4][SIZE4], char character, int row, int col, int length);
int isValid(char board[SIZE4][SIZE4], char solution[SIZE4][SIZE4], int boardSize, int row, int col);
int isRowUsed(char sol[SIZE4][SIZE4], int row, int col);
int isColUsed(char sol[SIZE4][SIZE4], int row, int col);
int isTouching(char sol[SIZE4][SIZE4], int row, int col, int n);
int checkBL(char sol[SIZE4][SIZE4], int row, int col, int boardSize);
int checkBR(char sol[SIZE4][SIZE4], int row, int col, int boardSize);
int checkUL(char sol[SIZE4][SIZE4], int row, int col);
int checkUR(char sol[SIZE4][SIZE4], int row, int col, int boardSize);
int isAreaUsed(char sol[SIZE4][SIZE4], char board[SIZE4][SIZE4], int row, int col, int boardSize, char areaSymbol);

int placeWord(char grid[GRID][GRID], char words[WORDS][LENGTH], NewSlot slot[MAX_SLOTS], int curWord, int curSlot,int row, int col, int i, int preserved[WORDS][LENGTH]);
int generate(int gridSize, int slotsNum, int wordsNum, char grid[GRID][GRID], char words[WORDS][LENGTH], int count, int isUsed[WORDS], NewSlot slot[MAX_SLOTS], int curWord, int curSlot, int preserved[WORDS][LENGTH]);
int clearSlot(char grid[GRID][GRID], char words[WORDS][LENGTH], NewSlot slot[MAX_SLOTS], int curWord, int curSlot,int row, int col, int i, int preserved[WORDS][LENGTH]);

int main()
{
    int task = -1;
    do
    {
        printf("Choose an option:\n"
               "1. Robot Paths\n"
               "2. The Human Pyramid\n"
               "3. Parenthesis Validation\n"
               "4. Queens Battle\n"
               "5. Crossword Generator\n"
               "6. Exit\n");

        if (scanf("%d", &task))
        {
            switch (task)
            {
            case 6:
                printf("Goodbye!\n");
                break;
            case 1:
                task1RobotPaths();
                break;
            case 2:
                task2HumanPyramid();
                break;
            case 3:
                task3ParenthesisValidator();
                break;
            case 4:
                task4QueensBattle();
                break;
            case 5:
                task5CrosswordGenerator();
                break;
            default:
                printf("Please choose a task number from the list.\n");
                break;
            }
        }
        else
        {
            scanf("%*s");
        }

    } while (task != 6);
}

void task1RobotPaths()
{
    int startX, startY;
    printf("Please enter the coordinates of the robot (column, row): \n");
    scanf(" %d %d", &startX, &startY);

    if (startX < 0 || startY < 0) { // negative coords case
        printf("The total number of paths the robot can take to reach home is: 0");
    }
    else {
        printf("The total number of paths the robot can take to reach home is: %d\n", pathAmount(startX, startY));
    }
}

int pathAmount(int col, int row) {
    if (col == 0 || row == 0) { // base case
        return 1;
    }
    // check how many paths from one down and one left
    return pathAmount(col-1, row) + pathAmount(col, row-1); 
}


void task2HumanPyramid()
{
    float weights[PYRAMID_LEVEL][PYRAMID_LEVEL] = {0}; // for storing the weights

    // user input
    printf("Please enter the weights of the cheerleaders:\n");
    for (int i = 0; i < PYRAMID_LEVEL; i++) {
        for (int j = 0; j <= i; j++) { // each row has the same number of coloumns as the row index
            float temp;
            scanf(" %f", &temp);
            if (temp < 0) {
                printf("Negative weights are not supported.\n");
                return;
            }
            weights[j][i] = temp;
        }
        
    }

    // calculate and print results
    printf("The total weight on each cheerleader is:\n");
    for (int i = 0; i < PYRAMID_LEVEL; i++) { // rows count
        for (int j = 0; j <= i; j++) { // cols count
            printf("%.2f ", checkWeight(j, i, weights));
        }
        printf("\n");
        
    }
}

float checkWeight(int col, int row, float weightsArr[PYRAMID_LEVEL][PYRAMID_LEVEL]) {

    if (col == 0 && row == 0) { // base case
        return weightsArr[col][row];
    }
    else if (col == row) { // at right wing
        return (checkWeight(col-1, row-1, weightsArr)/2 + weightsArr[col][row]);
    }
    else if (col == 0) { // at left wing
        return (checkWeight(col, row-1, weightsArr)/2 + weightsArr[col][row]);
    }
    // general step
    return (checkWeight(col-1, row-1, weightsArr)/2 + checkWeight(col, row-1, weightsArr)/2 + weightsArr[col][row]);
}



void task3ParenthesisValidator()
{
    // user first input and funtion call
    printf("Please enter a term for validation:\n");
    char firstChar;
    scanf(" %c", &firstChar);
    int isOk = isBalanced(firstChar);

    if (isOk) {
        printf("The parentheses are balanced correctly.\n");
    }
    else {
        printf("The parentheses are not balanced correctly.\n");
    }
}

int isBalanced(char curChar) {

    int allClosed = 1;

    if (curChar == '{' || curChar == '<' || curChar == '[') {
        allClosed = isClosed(curChar+ASCII_GAP2); // '{' / '[' / '<' + 2 == the closing symbol
    }
    else if (curChar == '(') {
        allClosed = isClosed(curChar+ASCII_GAP1); // '(' + 1 == ')'
    }

    if (allClosed == 0) { // base cases
        return 0;
    }
    else if (allClosed == NOK || curChar == ')' || curChar == '>' || curChar == '}' || curChar == ']') {
        scanf("%*[^ \n]"); // gets rid of the rest
        return 0;
    }
    else if (curChar == '\n') {
        return 1;
    }

    char nextChar;
    scanf("%c", &nextChar);
    return isBalanced(nextChar);
}

int isClosed (char symbol) {
    
    char nextChar;
    scanf("%c", &nextChar);
    if (nextChar == symbol) {
        return 1;
    }
    else if (nextChar == '\n') {
        return 0;
    }
    else if (nextChar == ')' || nextChar == '>' || nextChar == '}' || nextChar == ']') {
        return NOK;
    }

    int isNewClosed = 1;
    if (nextChar == '<' || nextChar == '{' || nextChar == '[') {
        isNewClosed = isClosed(nextChar+ASCII_GAP2);
    }
    else if (nextChar == '(') {
        isNewClosed = isClosed(nextChar+ASCII_GAP1);
    }
    if (isNewClosed != 1) {
        return isNewClosed;
    }

    return isClosed(symbol);
}

void task4QueensBattle()
{

    // user input
    char board[SIZE4][SIZE4]; // arr[row][col]
    char solution[SIZE4][SIZE4];
    printf("Please enter the board dimensions:\n");
    int boardSize;
    scanf(" %d", &boardSize);
    printf("Please enter the %d*%d puzzle board:\n", boardSize, boardSize);

    initArray(solution, '*', 0, 0, boardSize); // initializes the solution array with *'s
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            scanf(" %c", &board[i][j]);
        }
    }

    int results = solutionManager(board, solution, boardSize, 0, 0); // function main call
    if (results == 0) {
        printf("This puzzle cannot be solved.\n");
        return;
    }
    
    printf("Solution:\n");
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            printf("%c ", solution[i][j]);
        }
        printf("\n");
    }


}

int initArray(char arr[SIZE4][SIZE4], char character, int row, int col, int length) {
    arr[row][col] = character;
    if (row == length-1 && col == length-1) { // base case, end of the array
    return 1;
    }
    else if (row == length-1) { 
        return initArray(arr, character ,0, col+1, length);
    }
    else {
        return initArray(arr, character ,row+1, col, length);
    }
    
}

int solutionManager(char board[SIZE4][SIZE4], char solution[SIZE4][SIZE4], int boardSize, int row, int col) {
    if (isValid(board, solution, boardSize, row, col)) {
        solution[row][col] = 'X';
        if (col == boardSize-1) { // every col has a valid placed queen == done!
            return 1;
        }
        else if (solutionManager(board, solution, boardSize, 0, col+1)) { 
            return 1; // the rest of the cols has a valid placed queen!
        }
        else if (row == boardSize-1) { // no solution ahead and this is the last tile of the col :(
            solution[row][col] = '*'; // backtrack to previous col
            return 0;
        }
        else { // backtrack! try the next tile in the col
            solution[row][col] = '*';
            return solutionManager(board, solution, boardSize, row+1, col);
        }
    }
    else {
        if (row == boardSize-1) { // this is the last tile of the col and it's not valid :(
            return 0;
        }
        else { // this tile is not valid, check the next one
            return solutionManager(board, solution, boardSize, row+1, col);
        }
    }
        
}

int isValid(char board[SIZE4][SIZE4], char solution[SIZE4][SIZE4], int boardSize, int row, int col) {
    // check vertical
    if (isRowUsed(solution, row, boardSize)) {
        return 0;
    }
    // check horizontal
    if (isColUsed(solution, boardSize, col)) {
        return 0;
    }
    // check touch
    if (isTouching(solution, row, col, boardSize)) {
        return 0;
    }
    // check area
    if (isAreaUsed(solution, board, 0, 0, boardSize, board[row][col])) {
        return 0;
    }


    return 1;
}

int isRowUsed(char sol[SIZE4][SIZE4], int row, int col) {
    if (col == 0 && sol[row][col] != 'X') { // base case
        return 0;
    }
    else if (sol[row][col] == 'X') {
        return 1;
    }
    
    return isRowUsed(sol, row, col-1);

}

int isColUsed(char sol[SIZE4][SIZE4], int row, int col) {
    if (row == 0 && sol[row][col] != 'X') { // base case
        return 0;
    }
    else if (sol[row][col] == 'X') {
        return 1;
    }
    
    return isColUsed(sol, row-1, col);
}

int isTouching(char sol[SIZE4][SIZE4], int row, int col, int n) {
    return checkBL(sol,row,col,n)|| checkBR(sol,row,col,n)|| checkUL(sol,row,col)|| checkUR(sol,row,col,n);
}

int checkBL(char sol[SIZE4][SIZE4], int row, int col, int boardSize) {
    if (col > 0 && row < boardSize-1) {
        return sol[row+1][col-1] == 'X';
    }
    return 0;
}

int checkBR(char sol[SIZE4][SIZE4], int row, int col, int boardSize) {
    if (col < boardSize-1 && row < boardSize-1) {
        return sol[row+1][col+1] == 'X';
    }
    return 0;
}

int checkUL(char sol[SIZE4][SIZE4], int row, int col) {
    if (col > 0 && row > 0) {
        return sol[row-1][col-1] == 'X';
    }
    return 0;
}

int checkUR(char sol[SIZE4][SIZE4], int row, int col, int boardSize) {
    if (col < boardSize-1 && row > 0) {
        return sol[row-1][col+1] == 'X';
    }
    return 0;
}

int isAreaUsed(char sol[SIZE4][SIZE4], char board[SIZE4][SIZE4], int row, int col, int boardSize, char areaSymbol) {
    if (board[row][col] == areaSymbol && sol[row][col] == 'X') {
        return 1;
    }
    if (col == boardSize-1 && row == boardSize-1) {
        return 0;
    }
    if (col == boardSize-1) {
        return isAreaUsed(sol, board, row+1, 0, boardSize, areaSymbol);
    }

    return isAreaUsed(sol, board, row, col+1, boardSize, areaSymbol);
}



void task5CrosswordGenerator()
{
    NewSlot slot[MAX_SLOTS];
    int gridSize;
    int slotsNum;
    int wordsNum = 0;
    char grid[GRID][GRID];
    char words[WORDS][LENGTH];
    int isUsed[WORDS] = {0};
    int preservedIndexes[WORDS][LENGTH] = {0};

    // init grid
    for (int i = 0; i < GRID; i++) {
        for (int j = 0; j < GRID; j++) {
            grid[i][j] = '#';
        }
    }
    // user input
    printf("Please enter the dimensions of the crossword grid:\n");
    scanf(" %d", &gridSize);

    printf("Please enter the number of slots in the crossword:\n");
    scanf(" %d", &slotsNum);

    printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
    for (int i = 0; i < slotsNum; i++) {
        scanf(" %d %d %d %c", &slot[i].row, &slot[i].col, &slot[i].len, &slot[i].dir);
    }

    printf("Please enter the number of words in the dictionary:\n");
    scanf(" %d", &wordsNum);
    
    while(wordsNum < slotsNum) {
        printf("The dictionary must contain at least %d words. Please enter a valid dictionary size:\n", slotsNum);
        scanf(" %d", &wordsNum);
    }

    printf("Please enter the words for the dictionary:\n");
    for (int i = 0; i < wordsNum; i++) {
        scanf(" %s", words[i]);
    }

    // function main call
    int results = generate(gridSize, slotsNum, wordsNum, grid, words, 0, isUsed, slot, 0, 0, preservedIndexes);
    if (results == 0) {
        printf("This crossword cannot be solved.\n");
        return;
    }

    // print solution
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            printf("| %c ", grid[i][j]);
        }
        printf("|\n");
    }

}

int generate(int gridSize, int slotsNum, int wordsNum, char grid[GRID][GRID], char words[WORDS][LENGTH], int count, int isUsed[WORDS], NewSlot slot[MAX_SLOTS], int curWord, int curSlot, int preserved[WORDS][LENGTH]) {
    if (count == slotsNum) { // base case
        return 1;
    }

    // check length and status
    if ((int)strlen(words[curWord]) == slot[curSlot].len && !isUsed[curWord]) { 

        // if ok try to fit the word
        if (placeWord(grid, words, slot, curWord, curSlot, slot[curSlot].row, slot[curSlot].col, 0, preserved)) {
            // it fits :)
            count++;
            isUsed[curWord] = 1;

            // is there a solution ahead?
            if (generate(gridSize, slotsNum, wordsNum, grid, words, count, isUsed, slot, 0, curSlot+1, preserved)) {
                return 1;
            }
            else { // no solution ahead (backtrack!)
                clearSlot(grid, words, slot, curWord, curSlot, slot[curSlot].row, slot[curSlot].col, 0, preserved);
                count--;
                isUsed[curWord] = 0;
                if (curWord < wordsNum) { // if this is the last word
                    return generate(gridSize, slotsNum, wordsNum, grid, words, count, isUsed, slot, curWord+1, curSlot, preserved);
                } 
                return 0; // this is the last word :(
            }
        }
    }

    // word is used or have different length than slot
    if (curWord < wordsNum) {
        return generate(gridSize, slotsNum, wordsNum, grid, words, count, isUsed, slot, curWord+1, curSlot, preserved); // next word
    }

    return 0; // this is the last word 
}

int placeWord(char grid[GRID][GRID], char words[WORDS][LENGTH], NewSlot slot[MAX_SLOTS], int curWord, int curSlot,int row, int col, int i, int preserved[WORDS][LENGTH]) {
	if (i == slot[curSlot].len) { // base case
		return 1;
	}

    if (grid[row][col] != '#' && grid[row][col] != words[curWord][i]) {
        return 0;
    }
    if (grid[row][col] == words[curWord][i]) {
        preserved[curWord][i] = 1;
    }
    else {
        grid[row][col] = words[curWord][i];
    }

	if (slot[curSlot].dir == 'H') { // horizontal
        if (placeWord(grid, words, slot, curWord, curSlot, row, col+1, i+1, preserved)) { // if the rest of the word fits nextletter
            return 1;
        }
        if (preserved[curWord][i] == 0) { // if char is not preserved delete it
            grid[row][col] = '#'; // else backtrack
        }
        else {
            preserved[curWord][i] = 0; // free preservation 
        }
        return 0;
    }
    else { // vertical
        if (placeWord(grid, words, slot, curWord, curSlot, row+1, col, i+1, preserved)) { // if the rest of the word fits
            return 1;
        }

        if (preserved[curWord][i] == 0) { // if char is not preserved delete it
            grid[row][col] = '#'; // else backtrack
        }
        preserved[curWord][i] = 0; // free preservation 
        return 0;
    }
}

int clearSlot(char grid[GRID][GRID], char words[WORDS][LENGTH], NewSlot slot[MAX_SLOTS], int curWord, int curSlot,int row, int col, int i, int preserved[WORDS][LENGTH]) {
    if (i == (int)strlen(words[curWord])) {
        return 0;
    }
    if (preserved[curWord][i] == 0) {
        grid[row][col] = '#';
    }
    else {
        preserved[curWord][i] = 0;
    }

    if (slot[curSlot].dir == 'H') { // horizontal 
        return clearSlot(grid, words, slot, curWord, curSlot, row, col+1, i+1, preserved);
    }
    else { // vertical
        return clearSlot(grid, words, slot, curWord, curSlot, row+1, col, i+1, preserved);
    }
}