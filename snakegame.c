#include <ncurses.h>
#include <stdlib.h>    
#include <time.h>    
#include <unistd.h> 


#define BOARD_WIDTH 30     //These are all my constants   // This is the for the size of my game box
#define BOARD_HEIGHT 20       // This is the for the size of my game box
#define INITIAL_SNAKE_LENGTH 1 // This the starting size & starts the game with one segment
#define TICK_RATE 100000   // microseconds between frames but I will use Usleep instead


char *letterCycle = "BIGMAMA"; // // The snake body will add letters in order as it grows spelling out BigMama each time a food is eaten & keep repeating it
int letterIndex = 1; //This is so that it will start adding lettet I first - B is the beginning of the body so b=0 and then spell out BigMama as it grows


typedef struct SnakeSegment {    //Linked list node for the snake letters added each segment
    int posX;                      //X coordinate on the board
    int posY;                      //y coordinate on the board
    char displayChar;    // letter of this segment as it adds on
    struct SnakeSegment *next;  //Pointer to the next letter body piece
} SnakeSegment;


SnakeSegment *snakeHead = NULL;  // All my global variables that store what the whole program needs //Pointer to the first segment of the snake
SnakeSegment *snakeTail = NULL;    //Pointer to the last segment of snake but it will still grow
int directionX = 1;  // Snake start moving right
int directionY = 0;
int foodX, foodY;                //The position the food in a that current moment
int score = 0;                    //Everytime the food get eaten, it will increase the score by 1

// These are all the function prototypesm // 
void initializeGame(); //To set up the screen, colors, and set in place the first snake
void generateFood();    //Generates the food at random inside the box
void moveSnake();       //Movement of the the head, shifting the body & handling food
int checkCollision();   //Will detect it the snake hits the border wall or itself
void growSnake();        //Grow the snake piece by piece adding the letters to spell out BIGMAMA
void drawGame();        //Prints the border, the snake,food, and score
void endGame();         // ends game & prints the score


void addSnakeSegment(int x, int y, char bodyChar) { //Adds segment to the end of my tail (link list) for my snake
    SnakeSegment *newSeg = (SnakeSegment*)malloc(sizeof(SnakeSegment)); // To be able to add a new segment to the tail
    newSeg->posX = x;
    newSeg->posY = y;
    newSeg->displayChar = bodyChar;
    newSeg->next = NULL;

    if (snakeHead == NULL) {     //If the first segment of the snake is created 
        snakeHead = newSeg;
        snakeTail = newSeg;
    } else {
        snakeTail->next = newSeg;
        snakeTail = newSeg;
    }
}

        //These are to set up ncurses window, and start the snake 
void initializeGame() { // Initializing the game
    initscr();  //starts the ncurses window mode
    noecho();    //User keypresses(NOt print on screen)
    curs_set(FALSE); //hides the flashing cursor
    keypad(stdscr, TRUE); //This enables the arrow keys
    timeout(0);            

    start_color();            //These are for the colors used within the game
    init_pair(1, COLOR_BLUE, COLOR_BLACK);    // snake color
    init_pair(2, COLOR_RED, COLOR_BLACK);     // wall color
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   // food color

    
    addSnakeSegment(BOARD_WIDTH / 2, BOARD_HEIGHT / 2, 'B'); // Create the snake starting with letter - B
}


void generateFood() {
    foodX = (rand() % (BOARD_WIDTH - 2)) + 1;  // Generate food at random within the board
    foodY = (rand() % (BOARD_HEIGHT - 2)) + 1;
}

            // // These move the snake forward and grows it when the food is eaten
void moveSnake() {
    int newX = snakeHead->posX + directionX; //The movement of the snake  head based on the direction
    int newY = snakeHead->posY + directionY;

    
    if (newX == foodX && newY == foodY) {  //if the new position = to food location, then grow the snake first
        growSnake();
        generateFood();
    }

    // This is to be able to move each segment forward- the tail follow the position of the previous one
    SnakeSegment *current = snakeTail;
    while (current != snakeHead) {         //This is to move the tail forward until reaching the head
        SnakeSegment *prev = snakeHead;
        while (prev->next != current) {        //THis to to find current segment before it move again
            prev = prev->next;
        }
        current->posX = prev->posX;
        current->posY = prev->posY;
        current = prev;
    }

    
    snakeHead->posX = newX; // Move the head forward last
    snakeHead->posY = newY;
}


void growSnake() {   // This grows the snake by one new segment & increase the score increments of 1
    
    char newLetter = letterCycle[letterIndex]; // This makes the program cycle through the bigmama letters 
    letterIndex = (letterIndex + 1) % 7;

    
    addSnakeSegment(snakeTail->posX, snakeTail->posY, newLetter); // This places new segment at tail's position

    score++;
}


int checkCollision() { //These both detect if the snake hits a wall or the its own body
   
    if (snakeHead->posX <= 0 || snakeHead->posX >= BOARD_WIDTH - 1 ||       //This is for collsions if the snake hits the wall
        snakeHead->posY <= 0 || snakeHead->posY >= BOARD_HEIGHT - 1) {
        return 1;
    }

    
    SnakeSegment *current = snakeHead->next; //This is for self collision if the head hits the body
    while (current != NULL) {
        if (current->posX == snakeHead->posX &&       
            current->posY == snakeHead->posY) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}


void drawGame() {   // This to draw eveyrthing for the game which is the snake, food, and the score
    clear();

    
    attron(COLOR_PAIR(2));
    for (int x = 0; x < BOARD_WIDTH; x++) {      // This is to draw the border box & make it red
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
        mvprintw(current->posY, current->posX, "%c", current->displayChar);     // This creates the snake & its color
        attroff(COLOR_PAIR(1));
        current = current->next;
    }

    // This is to create the food & make it green
    attron(COLOR_PAIR(3));                  
    mvprintw(foodY, foodX, "O");        
    attroff(COLOR_PAIR(3));

    
    mvprintw(BOARD_HEIGHT, 0, "Score: %d", score);  //This prints the score below the board

    refresh();
}



void endGame() {     //This is for the end of the game and prints game over with your final score
    endwin();
    printf("HAHA YOU LOSE! GAME OVER! Final Score: %d\n", score);
}


int main() {   // This is the main function //The game is gonna keep going nutil it hit a wall, or crash into itself
    srand(time(NULL));     //Food placement is random
    initializeGame();        //Start the game
    generateFood();            //create the food

    while (1) {
        int key = getch(); //This is to be able to move the snake different directions w/ the arrow keys
        switch (key) {
            case KEY_UP:    directionX = 0; directionY = -1; break;
            case KEY_DOWN:  directionX = 0; directionY = 1;  break;
            case KEY_LEFT:  directionX = -1; directionY = 0; break;
            case KEY_RIGHT: directionX = 1; directionY = 0; break;
        }

        moveSnake();

        if (checkCollision()) break;
        
        drawGame();
        usleep(80000); // This slows down the speed of the snake moving 
    }

    endGame();
    return 0;
}
