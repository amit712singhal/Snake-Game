/*----------------------------------------- SOURCE_CODE --------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/*----------------------------------------- Snake_Game ---------------------------------------*/

int i, j, height = 35, width = 35;
int gameover, score;
int fruitx, fruity, flag;

struct Node
{
    int x, y;
    struct Node *next;
} *head;
void dr_boundary();
void setup();
void add_segment();
void input();
void logic();

/*----------------------------------------- Driver_Code --------------------------------------*/

int main()
{
    int game, matrix[3][3] = {0};
    char choice;

snake:

    setup();

    while (!gameover)
    {
        dr_boundary();
        input();
        logic();
        usleep(100000);
    }

    goto end;

end:

    printf("\nWant to Play Again? (Press Y) :: ");
    scanf("%s", &choice);
    if (choice == 'y' || choice == 'Y')
    {
        main();
    }
    printf("\n");
    goto exit;

exit:

    return 0;
}

/*------------------------------------- Snake_Game_FUNCTIONS ---------------------------------*/

// Function to draw the snake game boundary

void dr_boundary()
{
    system("clear"); // Clear the screen
    printf("\n\n--- Welcome to Compiler Gamings ---\n\n");
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                // Draw the boundary walls
                printf("█"); // Use a solid block character for walls
            }
            else
            {
                int is_segment = 0;
                struct Node *curr = head;
                while (curr != NULL)
                {
                    if (curr->x == i && curr->y == j)
                    {
                        // Draw the snake's body segments
                        printf("■"); // Use a filled square for snake segments
                        is_segment = 1;
                        break;
                    }
                    curr = curr->next;
                }
                if (!is_segment && i == fruitx && j == fruity)
                {
                    // Draw the fruit
                    printf("*"); // Use '*' for the fruit
                }
                else if (!is_segment)
                {
                    // Empty space within the game area
                    printf(" "); // Use a space for empty game area
                }
            }
        }
        printf("\n"); // Move to the next line after each row
    }

    // Print the score after the game ends
    printf("\n\nCurrent Score :: %d\n\n", score);
    printf("Use arrow keys (↑, ↓, ←, →) to move\n\n");
    printf("Press X to QUIT!\n\n");
}

// function to generate fruit within the boundary

void setup()
{
    gameover = 0;

    // Stores height and width
    head = (struct Node *)malloc(sizeof(struct Node));
    head->x = height / 2;
    head->y = width / 2;
    head->next = NULL;

label1:
    fruitx = rand() % 30;
    if (fruitx == 0)
        goto label1;
label2:
    fruity = rand() % 30;
    if (fruity == 0)
        goto label2;
    score = 0;
}

// function to increase snake length

void add_segment()
{
    struct Node *new_segment = (struct Node *)malloc(sizeof(struct Node));
    new_segment->x = head->x;
    new_segment->y = head->y;
    new_segment->next = head->next;
    head->next = new_segment;
}

// function to input movements of the snake from the user

void input()
{
    int ch;
    struct termios oldt, newt;

    // Save the terminal settings
    tcgetattr(STDIN_FILENO, &oldt);

    // Disable canonical mode, and local echo
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    // Set the new terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Get the input character
    ch = getchar();

    // Restore the terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (ch != EOF)
    {
        switch (ch)
        {
        case '\033':   // ASCII value for escape key
            getchar(); // consume the [ character
            ch = getchar();
            switch (ch)
            {
            case 'A': // Up arrow key
                if (flag != 2)
                {
                    flag = 1;
                }
                break;
            case 'B': // Down arrow key
                if (flag != 1)
                {
                    flag = 2;
                }
                break;
            case 'C': // Right arrow key
                if (flag != 4)
                {
                    flag = 3;
                }
                break;
            case 'D': // Left arrow key
                if (flag != 3)
                {
                    flag = 4;
                }
                break;
            }
            break;
        case 'x':
        case 'X':
            gameover = 1;
            break;
        }
    }
}

// function to implement the logic of the game

void logic()
{
    int prevx = head->x;
    int prevy = head->y;
    int nextx, nexty;
    struct Node *curr = head->next;
    head->x += (flag == 1) ? -1 : (flag == 2) ? 1
                                              : 0;
    head->y += (flag == 3) ? 1 : (flag == 4) ? -1
                                             : 0;
    while (curr != NULL)
    {
        nextx = curr->x;
        nexty = curr->y;
        curr->x = prevx;
        curr->y = prevy;
        prevx = nextx;
        prevy = nexty;
        curr = curr->next;
    }

    if (head->x == 0 || head->x == width - 1 || head->y == 0 || head->y == height - 1)
    {
        gameover = 1;
    }

    curr = head->next;
    while (curr != NULL)
    {
        if (curr->x == head->x && curr->y == head->y)
        {
            gameover = 1;
            break;
        }
        curr = curr->next;
    }

    if (head->x == fruitx && head->y == fruity)
    {
        add_segment();
        score += 10;
    label3:
        fruitx = rand() % 30;
        if (fruitx == 0)
            goto label3;
    label4:
        fruity = rand() % 30;
        if (fruity == 0)
            goto label4;
    }
}

/* ------------------------------------------------------------------------------------------ */