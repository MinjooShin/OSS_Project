#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <conio.h> //to use the kbhit() function
#include "console.h"

#define MAX 30
#define BACKCOLOR 0 //black

int g_iScore = 0;

struct ST_OBJECT
{
    int x = 0; //Enemy x,y coordinates
    int y = 0;
    bool bActive = false; //enemy missiles
};

ST_OBJECT g_stEnemy[MAX];
ST_OBJECT g_stPlayer;
ST_OBJECT g_stABM;

void Spawn(void)
{
    for(int i=0; i<MAX; i++)
    {
        if(!g_stEnemy[i].bActive) //Make it when enemy is not exist
        {
            g_stEnemy[i].x = rand()%29;
            g_stEnemy[i].y = rand()%21; //If the interceptor is directly above the plane, it's hard to throw, so leave about eight.
            g_stEnemy[i].bActive=true;
            break; 
            //When one is made -> break;
            //Create one enemy each time while while loop.
        }
    }
}

int EnemyProcess(void)
{
    int count = 0;
    for(int i=0; i<MAX; i++)//Turn 20 and print out all the true ones out of 20.
    {
        if(g_stEnemy[i].bActive)
        {
            SetColor(BACKCOLOR, i%15); //It prints enemy missiles in various colors.
            count++;
            GotoXY(g_stEnemy[i].x, g_stEnemy[i].y);
            printf("¡Ü");
            g_stEnemy[i].y++; //Make enemy missiles come down.

            //In the event of a collision with an enemy missile and a player.
            if(g_stEnemy[i].y == g_stPlayer.y)
            {
                if(g_stEnemy[i].x == g_stPlayer.x)
                    g_stPlayer.bActive = false;
            }
            if(g_stEnemy[i].y > 28) 
            {
                g_stEnemy[i].bActive = false;
            }
            //In the event of a collision with an enemy missile and an ABM. 
            if(g_stEnemy[i].y == g_stABM.y)
            {
                if(g_stEnemy[i].x == g_stABM.x)
                {
                    g_stABM.bActive = false; //Get rid of interceptors
                    g_stEnemy[i].bActive = false; //Get rid of enemy missile
                    g_iScore+=5; //Five more points and three seconds to rest.
                    SetColor(BACKCOLOR, 10);
                    GotoXY(0, 0);
                    printf("Score: %d \n!!You intercepted the missile!! +5", g_iScore);
                    Sleep(3000);
                }
            }
        }
    }
    return count;  
}

void InputProcess(void)
{
    //A function that receives what state a particular key is in.
    //return value -> 0 (unpressed state)
    //-> 0x8000(Never pressed before calling, pressed at the time of calling)
    //-> 0x8001(Pressed before call and pressed at call point)
    //-> 1(Pressed before calling and not at the time of calling)
    //=> 0,1 (unpressed state) / Ox8000 or Ox8001 (pressed state)
    if(GetAsyncKeyState(VK_LEFT) & 0x8000) //When the key is pressed, GetAsyncKeyState(VK_LEFT) & 0x8000 = 8000 ->True because nonzero, i.e. true when key is pressed.
    {    
        g_stPlayer.x-=2;//move the x to left direction
        if(g_stPlayer.x<0) g_stPlayer.x=0; //To prevent errors that occur when the index exceeds the specified range.
    }
    if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        g_stPlayer.x+=2;
        if(g_stPlayer.x>28) g_stPlayer.x=28;
    }     
    if(GetAsyncKeyState(VK_UP) & 0x8000) 
    {    
        g_stPlayer.y--;
        if(g_stPlayer.y<0) g_stPlayer.y=0; 
    }
    if(GetAsyncKeyState(VK_DOWN) & 0x8000) 
    {    
        g_stPlayer.y++;
        if(g_stPlayer.y>28) g_stPlayer.y=28; 
    }
    if(GetAsyncKeyState(VK_SPACE) & 0x8000) //When space is pressed, change coordinates and make ABM true.
    {
        if(!g_stABM.bActive) 
        //When the x-coordinate is moved and the space is pressed again, 
        //the previous interceptor does not disappear and the interceptor continues to shoot on the same x-axis.
        {
        g_stABM.x=g_stPlayer.x;
        g_stABM.y=g_stPlayer.y-1;
        g_stABM.bActive=true;
        }
    } 
}

void ABMProcess(void)
{
    if(g_stABM.bActive) //Run only when ABM is true.
    {
        SetColor(BACKCOLOR, 12);
        GotoXY(g_stABM.x,g_stABM.y);//Move cursor to interceptor position
        printf("¡è");
        g_stABM.y--; 
        if(g_stABM.y < 0) g_stABM.bActive=false; //Prevent interceptors from exceeding the y-axis.
    }
} 

void update(void)
{
    SetColor(BACKCOLOR, 12);
    if(g_stPlayer.bActive)
    {
        //move the cursor to print out the plane
        GotoXY(g_stPlayer.x, g_stPlayer.y); 

        //and then at that position, print out the plane character.
        printf("¡ã");
    }
    else //If conflict occurs, output other player characters
    {
        //move the cursor to print out the plane
        GotoXY(g_stPlayer.x, g_stPlayer.y); 

        //and then at that position, print out the plane character.
        SetColor(BACKCOLOR, 14);
        printf("¡Ø");   
    }
}

void Init(void)
{
    //set to plane coordination
    g_stPlayer.x = 14; 
    g_stPlayer.y = 28;
    g_stPlayer.bActive = true;

    srand(time(NULL)); 

    //change size of the console to do compact game
    SetConsoleSize(30, 30); 

    SetTitle("Avoid enemy missiles"); 
}

//int g_iScore = 0;
void Score(void) //If the missile is avoided, the score goes up as many times as the missile. Scores don't go up when they collide.
{
    SetColor(BACKCOLOR, 15);
    if(g_stPlayer.bActive) g_iScore++;
    GotoXY(0, 0);
    printf("Score: %d", g_iScore);
}

void StartMenu(void) //Create a game start screen
{
    SetColor(BACKCOLOR, 15);
    while(_kbhit()) getch();
  
    while(1)
    {
       Clear();
       GotoXY(10, 9);
       printf("Avoid enemy missiles");
       GotoXY(10, 20);
       printf("Press and any key"); 
       if(_kbhit()) break;
       Sleep(50); 
    }
}
void GameMain(void)
{
    SetColor(BACKCOLOR, 15);
    int state=0;
    g_stPlayer.x = 14; 
    g_stPlayer.y = 28;
    g_stPlayer.bActive = true;
    g_iScore=0;

    while(1)
    {
        //Clear afterimage
        Clear();

        if(g_stPlayer.bActive) 
        {
        //Create enemy missiles
        Spawn();

        //Input Processing
        InputProcess();
        }
        //enemy missile handling
        state=EnemyProcess();

        //ABM handling
        ABMProcess();

        //print the player 
        update();

        Score();

        //1000ms->Move the plane every second. /If it's too fast, it's a mistake.
        Sleep(50);
        if(state == 0) break;
    }  
}
bool ResultMenu(void)
{
    SetColor(BACKCOLOR, 15);
    bool bRet = false;
    while(1)
    {
        Clear();
        GotoXY(10, 9);
        printf("Result Menu");
        GotoXY(10, 15);
        printf("SCORE : %d", g_iScore);
        
        GotoXY(0, 24);
        printf("continue....Y/N");
        if(GetAsyncKeyState('Y') & 0x8000)
        {
            bRet =false;
            break;
        }
        if(GetAsyncKeyState('N') & 0x8000)
        {
            bRet = true;
            break;
        }
        Sleep(50);
    }
    return bRet;
}

int main(void)
{
    int state=0;
    Init();
    while(1)
    {
    StartMenu();
    GameMain();
    if(ResultMenu()) break;
    }
    system("pause");
 
    return 0;
}