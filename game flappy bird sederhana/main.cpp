#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 10;

class GameObject{
public:
    virtual void update() = 0;
};

class Bird : public GameObject {
public:
    int x,y;

    Bird() : x(5), y(HEIGHT/2){}

    void flap() { if (y > 1) y -= 2; }
    void fall() { if (y < HEIGHT - 2) y++; }
    void update() override { fall(); }

    ~Bird(){
        cout << "Bird destroyed" << endl;
    }
};

class Pipe : public GameObject{
public:
    int x, gapY;
    Pipe(int startX) : x(startX), gapY(rand()%(HEIGHT - 4) + 1){}
    void move () {x--;}
    void update() override { move();}
};

class Game {
private:
    Bird* bird;
    vector<Pipe*> pipes;
    static bool gameOver;
    static int score;

public:
    Game(){
        srand(time(0));
        gameOver = false;
        score = 0;
        bird = new Bird();
        pipes.push_back(new Pipe(WIDTH - 1));
    }

    ~Game(){
        delete bird;
        for (auto pipe : pipes) delete pipe;
    }

void draw() {
    system("cls");
    for (int i=0; i < HEIGHT; i++) {
        for (int j=0; j < WIDTH; j++) {
            if (i==0 || i==HEIGHT-1) cout << "#";
            else if (j==bird->x&&i==bird->y) cout << "B";
            else {
                bool isPipe = false;
                for (auto pipe : pipes){
                    if(j==pipe->x&&(i<pipe->gapY||i>pipe->gapY+2)){
                        cout << "#";
                        isPipe = true;
                        break;
                    }
                }
                if(!isPipe)cout << " ";
            }

        }
        cout << endl;
    }
    cout << "Score: " << score << endl;
}

void input() {
    if (_kbhit()) {
        char key = _getch();
        if (key == ' ')bird->flap();
    }
}

void logic() {
    bird->update();
    for (auto&pipe:pipes){
        pipe->update();
        if (pipe->x==bird->x&&(bird->y<pipe->gapY||bird->y>pipe->gapY+2))
            gameOver = true;
        if (pipe->x==0) {
            delete pipes.front();
            pipes.erase(pipes.begin());
            pipes.push_back(new Pipe(WIDTH-1));
            score++;
        }
    }
    if (bird->y>=HEIGHT-1) gameOver = true;
}

void run() {
    while (!gameOver){
        draw();
        input();
        logic();
        Sleep(200);
    }
    cout << "\nGame Over! Final Score: " << score << endl;
  }
};

bool Game::gameOver = false;
int Game::score = 0;

int main(){
    Game game;
    game.run();
    return 0;
}
