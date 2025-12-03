 BIGMAMA C Snake Game: 
 I completed this project solo
This is my creation of a Snake game written in C++ language 
Every time the player eats food, the snake grows using letters that will spell out "BIGMAMA" repeatedly until you crash into the border wall or into the snake body itself
The snake will grow one letter at a time as you eat the food in the following order: 
B → BI → BIG → BIGM → BIGMA → BIGMAM → BIGMAMA → BIGMAMAB → ...
The word BIGMAMA will continue to be spelled out so long you keep scoring
The snake is able to be controlled/directed by the arrow Keys
Eat the food to grow the snake  and increase your score  
The game ends if you hit wall or your own body
 How to Compile:
Make sure ncurses is installed
For Linux / Mac:
gcc snake.c -o snakegame -lncurses
To run the game:
./snakegame

My game features: 
Track your score 
Grow the snake  
Random food generation 
Linked List structure  
Ncurses for graphics  


 Files Included:
- `snakegame.c` → The full source code
- `README.md` → Information + how to run

