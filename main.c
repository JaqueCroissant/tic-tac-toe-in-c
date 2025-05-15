#include "include/raylib.h"
#include<stdio.h>

enum Player 
{
    Cross = 0,
    Circle = 1
};

enum GameState 
{
    Running = 0,
    GameOver = 1
};

const int screenWidth = 305;
const int screenHeight = 305;
int xMoves[3] = {-1, -1, -1};
int oMoves[3] = {-1, -1, -1};

enum Player currentPlayer = Cross;
enum GameState gameState = Running;


struct Rectangle board[] = 
{
    { 5, 5, 95, 95 },
    { 105, 5, 95, 95 },
    { 205, 5, 95, 95 },
    { 5, 105, 95, 95 },
    { 105, 105, 95, 95 },
    { 205, 105, 95, 95 },
    { 5, 205, 95, 95 },
    { 105, 205, 95, 95 },
    { 205, 205, 95, 95 },
};



void DrawSquares()
{
    for(int i = 0; i < 9; i++)
    {
        DrawRectangleRec(board[i], LIGHTGRAY);
    }
}

int GetSquareIndex(Vector2 *position)
{
    for(int i = 0; i < 9; i++)
    {
        if( position->x > board[i].x && 
            position->x < board[i].x + board[i].width &&
            position->y > board[i].y && 
            position->y < board[i].y + board[i].height)
            {
                return i;
            }
    }

    return -1;
}

int IsSquareAvailable(int index)
{
    for(int i = 0; i < 3; i++)
    {
        if (xMoves[i] == index || oMoves[i] == index)
        {
            return 0;
        }
    }

    return 1;
}

int AssignNewMove(int index, enum Player player)
{
    if(!IsSquareAvailable(index))
    {
        return 0;
    }

    int *moves;

    if(player == Cross)
    {
        moves = xMoves;
    }
    else
    {
        moves = oMoves;
    }

    moves[2] = moves[1];
    moves[1] = moves[0];
    moves[0] = index;

    return 1;
}

void DrawX(Rectangle rectangle, float thickness, Color color) 
{
    Vector2 topLeft     = { rectangle.x + thickness, rectangle.y + thickness };
    Vector2 topRight    = { rectangle.x + rectangle.width - thickness, rectangle.y + thickness };
    Vector2 bottomLeft  = { rectangle.x + thickness, rectangle.y + rectangle.height - thickness };
    Vector2 bottomRight = { rectangle.x + rectangle.width - thickness, rectangle.y + rectangle.height - thickness };

    DrawLineEx(topLeft, bottomRight, thickness, color);
    DrawLineEx(topRight, bottomLeft, thickness, color);
}

void DrawO(Rectangle rectangle, float thickness, Color color) 
{
    float centerX = rectangle.x + rectangle.width / 2;
    float centerY = rectangle.y + rectangle.height / 2;
    float radius = (rectangle.width < rectangle.height ? rectangle.width : rectangle.height) / 2 - thickness;

    DrawCircle(centerX, centerY, radius, color);
    DrawCircle(centerX, centerY, radius - thickness, LIGHTGRAY);
}

void DrawMoves()
{
    for(int i = 0; i < 3; i++)
    {
        int x = xMoves[i];

        if(x > -1)
        {
            DrawX(board[x], 10, BLACK);            
        }

        int o = oMoves[i];

        if(o > -1)
        {
            DrawO(board[o], 10, BLACK);            
        }
    }
}

int ContainsAll(int arr[], int values[]) 
{
    for (int i = 0; i < 3; i++) 
    {
        int found = 0;
        for (int j = 0; j < 3; j++) 
        {
            if (arr[j] == values[i]) 
            {
                found = 1;
                break;
            }
        }
        if (!found) return 0;
    }
    return 1;
}

int IsGameOver()
{
    int winningCombinations[8][3] = 
    {
        {0, 1, 2}, // Top row
        {3, 4, 5}, // Middle row
        {6, 7, 8}, // Bottom row
        {0, 3, 6}, // Left column
        {1, 4, 7}, // Middle column
        {2, 5, 8}, // Right column
        {0, 4, 8}, // Diagonal top-left to bottom-right
        {2, 4, 6}  // Diagonal top-right to bottom-left
    };

    int *player_moves;

    if(currentPlayer == Cross)
    {
        player_moves = xMoves;
    }
    else{
        player_moves = oMoves;
    }

    for(int i = 0; i < 8; i++)
    {
        if(ContainsAll(player_moves, winningCombinations[i]))
        {
            return 1; 
        }
    }

    return 0;
}


void GameStateRunning()
{
    if(IsMouseButtonReleased(0))
    {
        Vector2 mousePosition = GetMousePosition();
        int index = GetSquareIndex(&mousePosition);

        if(index > -1 && AssignNewMove(index, currentPlayer))
        {
            if(IsGameOver(currentPlayer))
            {
                gameState = GameOver;
            }
            else if(currentPlayer == Cross)
            {
                currentPlayer = Circle;
            }
            else
            {
                currentPlayer = Cross;
            }
        }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawSquares();
    DrawMoves();
    EndDrawing();
}

void GameStateGameOver()
{
    char message[8] = "  wins!\0";
    char winner = 'X';

    if(currentPlayer != Cross)
    {
        winner = 'O';
    }

    message[0] = winner;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(message, 100, 120, 40, RED);
    EndDrawing();
}


int main(void)
{
    InitWindow(screenWidth, screenHeight, "Tic Tac Toe");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if(gameState == Running)
        {
            GameStateRunning();
        }

        if(gameState == GameOver)
        {
            GameStateGameOver();
        }
    }

    CloseWindow();

    return 0;
}