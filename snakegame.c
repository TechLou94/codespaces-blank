#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define BOARD_WIDTH 30
#define BOARD_HEIGHT 20
#define INITIAL_SNAKE_LENGTH 1
#define TICK_RATE 100000   // microseconds between frames //These are all my constants

// BIGMAMA letters for snake body growth
char *letterCycle = "BIGMAMA"; // BigMama is spelled each time a food is eaten
int letterIndex = 1; //This is so that it will start at B and then spell out BigMama as it grows


typedef struct SnakeSegment {
    int posX;           // Node Structure of the snake
    int posY;
    char displayChar;    // letter of this segment
    struct SnakeSegment *next;
} SnakeSegment;


SnakeSegment *snakeHead = NULL;  // All my global variables
SnakeSegment *snakeTail = NULL;
int directionX = 1;  // Start moving right
int directionY = 0;
int foodX, foodY;
int score = 0;


void initializeGame();
void generateFood();    // These are all the function prototypes
void moveSnake();
int checkCollision();
void growSnake();
void drawGame();
void endGame();


void addSnakeSegment(int x, int y, char bodyChar) {
    SnakeSegment *newSeg = (SnakeSegment*)malloc(sizeof(SnakeSegment)); // To be able to add a new segment to the tail
    newSeg->posX = x;
    newSeg->posY = y;
    newSeg->displayChar = bodyChar;
    newSeg->next = NULL;

    if (snakeHead == NULL) {
        snakeHead = newSeg;
        snakeTail = newSeg;
    } else {
        snakeTail->next = newSeg;
        snakeTail = newSeg;
    }
}


void initializeGame() { // Initializing the game
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(0);

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);    // snake color
    init_pair(2, COLOR_RED, COLOR_BLACK);     // wall color
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   // food color

    
    addSnakeSegment(BOARD_WIDTH / 2, BOARD_HEIGHT / 2, 'B'); // Create the snake starting with letter - B
}


void generateFood() {
    foodX = (rand() % (BOARD_WIDTH - 2)) + 1;  // Generate food at random within the board
    foodY = (rand() % (BOARD_HEIGHT - 2)) + 1;
}


void moveSnake() {
    int newX = snakeHead->posX + directionX; //The movement of the snake 
    int newY = snakeHead->posY + directionY;

    
    if (newX == foodX && newY == foodY) {   // Check food collision (must happen before movement)
        growSnake();
        generateFood();
    }

    
    SnakeSegment *current = snakeTail;
    while (current != snakeHead) {          // This is to be able to move each segment to the position of the previous one
        SnakeSegment *prev = snakeHead;
        while (prev->next != current) {
            prev = prev->next;
        }
        current->posX = prev->posX;
        current->posY = prev->posY;
        current = prev;
    }

    // Move head last
    snakeHead->posX = newX;
    snakeHead->posY = newY;
}


void growSnake() {   // This grows the  nake by one new segment
    
    char newLetter = letterCycle[letterIndex]; // This makes the program cycle through the bigmama letters 
    letterIndex = (letterIndex + 1) % 7;

    
    addSnakeSegment(snakeTail->posX, snakeTail->posY, newLetter); // This places new segment at tail's position

    score++;
}


int checkCollision() {
   
    if (snakeHead->posX <= 0 || snakeHead->posX >= BOARD_WIDTH - 1 ||       //This is for collsions if the snake hits the wall
        snakeHead->posY <= 0 || snakeHead->posY >= BOARD_HEIGHT - 1) {
        return 1;
    }

    
    SnakeSegment *current = snakeHead->next;
    while (current != NULL) {
        if (current->posX == snakeHead->posX &&         //This is for self collision
            current->posY == snakeHead->posY) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}


void drawGame() {   // This to draw eveyrthing for the game
    clear();

    
    attron(COLOR_PAIR(2));
    for (int x = 0; x < BOARD_WIDTH; x++) {      // This is to draw the border box
        mvprintw(0, x, "#");
        mvprintw(BOARD_HEIGHT - 1, x, "#");
    }
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        mvprintw(y, 0, "#");
        mvprintw(y, BOARD_WIDTH - 1, "#");
    }
    attroff(COLOR_PAIR(2));

    
    SnakeSegment *current = snakeHead;
    while (current != NULL) {
        attron(COLOR_PAIR(1));          
        mvprintw(current->posY, current->posX, "%c", current->displayChar);     // This creates the snake
        attroff(COLOR_PAIR(1));
        current = current->next;
    }

    
    attron(COLOR_PAIR(3));                  
    mvprintw(foodY, foodX, "O");        // This is to create the food 
    attroff(COLOR_PAIR(3));

    
    mvprintw(BOARD_HEIGHT, 0, "Score: %d", score);  //This prints the score at the top

    refresh();
}



void endGame() {     //This is for the end of the game and prints game over with your final score
    endwin();
    printf("GAME OVER! Final Score: %d\n", score);
}


int main() {   // This is the main function
    srand(time(NULL));
    initializeGame();
    generateFood();

    while (1) {
        int key = getch();
        switch (key) {
            case KEY_UP:    directionX = 0; directionY = -1; break;
            case KEY_DOWN:  directionX = 0; directionY = 1;  break;
            case KEY_LEFT:  directionX = -1; directionY = 0; break;
            case KEY_RIGHT: directionX = 1; directionY = 0; break;
        }

        moveSnake();

        if (checkCollision()) break;
        
        drawGame();
        usleep(80000); // This slows the snake down 
    }

    endGame();
    return 0;
}