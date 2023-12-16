#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <string>
using namespace std;

const int MAP_HEIGHT = 30;        
const int MAP_WIDTH = 50;
bool game = true;
char map[MAP_HEIGHT][MAP_WIDTH];
int tankY, tankX;
int tankSpeed = 2;
string playerName;

int obstacles[30][2];
int obstacleBullets[10][2];
int bullets[10][2] = { {-1, -1} };

int obstacleCount = 0;
int obstacleBulletCount = 0;
int bulletCount = 0;

int gameSpeed = 1;

int score = 0;
int lives = 3;

string SCORES_FILE = "player_scores.txt";

void resetMap()     //again draw the new map in Loop
{
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            map[i][j] = ' ';
        }

        // Draw top border
        for (int i = 0; i < MAP_WIDTH; ++i) {
            map[0][i] = '-';
        }

        // Draw bottom border
        for (int i = 0; i < MAP_WIDTH; ++i) {
            map[MAP_HEIGHT - 1][i] = '-';
        }

        // Draw left and right borders
        for (int i = 1; i < MAP_HEIGHT - 1; ++i) {
            map[i][0] = '|';
            map[i][MAP_WIDTH - 1] = '|';
        }
    }
}

void drawMap() // tis contain borders and also draw obstacles with upgraded values 
{
    resetMap();

    // Draw Tank if within bounds
    if (tankY >= 1 && tankY < MAP_HEIGHT - 1 && tankX >= 1 && tankX < MAP_WIDTH - 1) {
        map[tankY][tankX] = 'T';
    }

    // Draw Obstacles
    for (int i = 0; i < 30; ++i) {
        if (obstacles[i][0] != -1) {
            for (int j = obstacles[i][1]; j < obstacles[i][1] + 10; ++j) {
                if (obstacles[i][0] >= 1 && obstacles[i][0] < MAP_HEIGHT - 1 && j >= 1 && j < MAP_WIDTH - 1) {
                    map[obstacles[i][0]][j] = '|';
                }
            }
        }
    }

    // Draw Bullets
    for (int i = 0; i < 10; ++i) {
        if (bullets[i][0] != -1) {
            if (bullets[i][0] >= 1 && bullets[i][0] < MAP_HEIGHT - 1 && bullets[i][1] >= 1 && bullets[i][1] < MAP_WIDTH - 1) {
                map[bullets[i][0]][bullets[i][1]] = '*';
            }
        }
    }

    // Draw Obstacle Bullets
    for (int i = 0; i < 10; ++i) {
        if (obstacleBullets[i][0] != -1) {
            if (obstacleBullets[i][0] >= 1 && obstacleBullets[i][0] < MAP_HEIGHT - 1 && obstacleBullets[i][1] >= 1 && obstacleBullets[i][1] < MAP_WIDTH - 1) {
                map[obstacleBullets[i][0]][obstacleBullets[i][1]] = 'o';
            }
        }
    }

    // Display the map
    system("cls");
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            cout << map[i][j];
        }
        cout << endl;
    }

    // Display Score and Lives
    cout << "Score: " << score << "\tLives: " << lives << endl;
}

void processInput() //this will take input to move our player 
{
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
        case 'q': //For Exit during playing the game
            exit(0);
            break;
        case 'a':  // for left moment
            if (tankX > 1)  // check for border
                tankX -= tankSpeed;
            break;
        case 'd': //For right moment
            if (tankX < MAP_WIDTH - 3) //check for borders
                tankX += tankSpeed;
            break;
        case 's':  //For shooting bullets
            bullets[bulletCount % 10][0] = tankY; // Fire bullet downward
            bullets[bulletCount % 10][1] = tankX;
            bulletCount++;
            break;
        }
    }
}

void savePlayerScore(string playerName, int playerScore) // this is file handling save the score and name of player in txt file
{
    ofstream file(SCORES_FILE, ios::app);   // app is used for saving previous data if we do not use this new data will replace previous data
    if (file.is_open()) {
        file << playerName << " " << playerScore << endl;
        file.close();
    }
}

void generateObstacles() // generate obstacles means enemies
{
    if (rand() % 10 == 0)
    {
        int obstacleX = rand() % (MAP_WIDTH - 10) + 1;  //generating random
        for (int i = 0; i < 30; ++i) {
            if (obstacles[i][0] == -1) {
                obstacles[i][0] = 1;
                obstacles[i][1] = obstacleX;
                break;
            }
        }
    }
}

void moveObstacles()  //this will move enemies automatically in downward direction
{
    for (int i = 0; i < 30; ++i) {
        if (obstacles[i][0] != -1) {
            obstacles[i][0]++;
            if (obstacles[i][0] == MAP_HEIGHT - 1) {
                obstacles[i][0] = -1;
                obstacles[i][1] = -1;
            }
        }
    }

    //obstacle bullets
    for (int i = 0; i < 10; ++i) {
        if (obstacleBullets[i][0] == -1 && rand() % 5 == 0) {
            int randomObstacle = rand() % 30;
            if (obstacles[randomObstacle][0] != -1) {
                obstacleBullets[i][0] = obstacles[randomObstacle][0] + 1;
                obstacleBullets[i][1] = obstacles[randomObstacle][1] + 5;
            }
        }
    }
}

void moveBullets() //move player bulltes
{
    for (int i = 0; i < 10; ++i) {
        if (bullets[i][0] != -1) {
            bullets[i][0]--;
            if (bullets[i][0] == 0) {
                bullets[i][0] = -1;
                bullets[i][1] = -1;
            }
        }
    }
}

void moveObstacleBullets() //move enemies bullets
{
    for (int i = 0; i < 10; ++i) {
        if (obstacleBullets[i][0] != -1) {
            obstacleBullets[i][0]++;
            if (obstacleBullets[i][0] == MAP_HEIGHT - 1) {
                obstacleBullets[i][0] = -1;
                obstacleBullets[i][1] = -1;
            }
        }
    }
}

void checkCollision() //it will check collision 
{
    //this loop check collision with enemy if collison occurs and lives are 0 it will exit
    for (int i = 0; i < 30; ++i) {
        if (obstacles[i][0] != -1 && obstacles[i][0] == tankY && tankX >= obstacles[i][1] && tankX <= obstacles[i][1] + 9) {
            lives--;
            if (lives <= 0) {
                cout << "Game Over! Your final score: " << score << endl;
                savePlayerScore(playerName, score);
                exit(0);
                //game = false;
            }
        }
    }

    // this check collison of player bullets and enemy if occurs score increased by 10
    for (int i = 0; i < 10; ++i) {
        if (bullets[i][0] != -1) {
            for (int j = 0; j < 30; ++j) {
                if (obstacles[j][0] != -1 && obstacles[j][0] == bullets[i][0] && bullets[i][1] >= obstacles[j][1] && bullets[i][1] <= obstacles[j][1] + 9) {
                    obstacles[j][0] = -1;
                    obstacles[j][1] = -1;
                    bullets[i][0] = -1;
                    bullets[i][1] = -1;
                    score += 10;
                }
            }
        }
    }

    //this loop check collision with bullets if collison occurs and lives are 0 it will exit

    for (int i = 0; i < 10; ++i) {
        if (obstacleBullets[i][0] != -1 && obstacleBullets[i][0] == tankY && tankX >= obstacleBullets[i][1] && tankX <= obstacleBullets[i][1]) {
            lives--;
            if (lives <= 0) {
                cout << "Game Over! Your final score: " << score << endl;
                savePlayerScore(playerName, score);
                exit(0);
                //game = false;
            }
        }
    }
}

void update()  //all functions are called in this function
{
    moveObstacles();
    moveBullets();
    moveObstacleBullets();
    checkCollision();
}

void LoadScores() // Load scores of Top 3 players with their names
{
    string name1 = " ", name2 = " ", name3 = " ";
    int score1 = 0, score2 = 0, score3 = 0;
    int Score = 0;
    string Name;
    ifstream read;
    read.open(SCORES_FILE);
    if (read.is_open())
    {
        while (!read.eof())                       // Run loop until we reach end of the text file
        {
            read >> Name >> Score;                          // Inputs Score and Name from file

            if (Score > score1)                           //Next to assign Largest , Second Largest and Third Largest Scores respectively
            {
                score3 = score2;
                score2 = score1;
                score1 = Score;
                name3 = name2;
                name2 = name1;
                name1 = Name;

            }
            else if (Score < score1 && Score > score2)
            {
                score3 = score2;
                score2 = Score;
                name3 = name2;
                name2 = Name;
            }
            else if (Score < score1 && Score < score2 && Score > score3)
            {
                score3 = Score;
                name3 = Name;

            }
        }



    }
    cout << "First Highest Score:    " << name1 << "   " << score1 << endl;
    cout << "Second Highest Score:   " << name2 << "   " << score2 << endl;
    cout << "Third Highest Score:    " << name3 << "   " << score3 << endl << endl << endl;
}

//this is simple title
void Title()
{
    cout << "**********************************" << endl;
    cout << "*         FAST Tank Games        *" << endl;
    cout << "**********************************" << endl;
}

void credits()
{
    system("cls");
    Title();

    cout << "\n<-------------Credits------------->" << endl << endl << endl;;
    cout << "23F-0011 \t\t Kulsoom" << endl; Sleep(750);
    cout << "23F-0013 \t\t Mashal" << endl; Sleep(750);
    cout << "23F-0038 \t\t Arisha" << endl << endl;
    cout << "\n<-------------------------------->" << endl << endl;
}
void instructions()
{
    system("cls");
    Title();
    cout << "\n<-------------Instructions------------->" << endl << endl;
    cout << "1- Player have 3 Lives" << endl; Sleep(250);
    cout << "2- Player will loose the 1 life every time he/she collide with obstacle or bullet" << endl; Sleep(250);
    cout << "3- Press S for shoot" << endl; Sleep(250);
    cout << "4- Press A for left Movement" << endl; Sleep(250);
    cout << "5- Press D for Right Movement" << endl;

    cout << "\n\n<-------------------------------------->" << endl << endl;

}
int main() {
    srand(time(0));
    //by this our player will in the center of map
    tankY = MAP_HEIGHT - 2;
    tankX = MAP_WIDTH / 2;
    char ch;
    
    do
    {
        Title();
        cout << setw(19) << "1. Play" << endl;
        cout << setw(26) << "2. High scores" << endl;
        cout << setw(27) << "3. Instructions" << endl;
        cout << setw(22) << "4. Credits" << endl;
        cout << setw(19) << "5. Exit" << endl;

        cout << "**********************************" << endl;
        int choice;
        cout << "Enter Your Choice : ";  cin >> choice;

        if (choice == 1)
        {
            Sleep(750);
            system("cls");
            Title();
            Sleep(750);
            cout << endl << endl;
            cout << "Enter your name: ";
            cin >> playerName;

            while (true) {
                drawMap();
                processInput();
                generateObstacles();
                update();
                Sleep(100);
            }
        }
        else if (choice == 2)
        {
            Sleep(750);
            system("cls");
            Title();

            cout << endl << endl;
            cout << "Loading . "; Sleep(750); cout << ". "; Sleep(750); cout << "."; Sleep(750);
            system("cls");
            Title();
            cout << endl << endl;
            LoadScores();
        }
        else if (choice == 3)
        {
            cout << "\n\nLoading . "; Sleep(750); cout << ". "; Sleep(750); cout << "."; Sleep(750);
            instructions();
        }
        else if (choice == 4)
        {
            cout << "\n\nLoading . "; Sleep(750); cout << ". "; Sleep(750); cout << "."; Sleep(750);
            credits();
        }
        else if (choice == 5)
        {
            cout << "\n\nExiting . "; Sleep(750); cout << ". "; Sleep(750); cout << "."; Sleep(750);
            cout << "\n\nExit Successfully" << endl;
            exit(0);
        }
        else
            cout << "Invalid Input" << endl;

        cout << "Press Y to Return on Menu : "; cin >> ch;
        Sleep(750);
        system("cls");
    } while (ch == 'y' || ch == 'Y');
    

    system("Pause");
    return 0;
}
