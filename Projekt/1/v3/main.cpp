#include <iostream>
#include <ncurses.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <random>

#define KULKA 'o'
#define refresh_ekranu 100000

class Ball;
struct point;
void init();                                    //funkcja inicjalizująca ncurses
void removeOld();                               //funkcja usuwająca stare kulki
void runBall(Ball* ball, point direction);      //funkcja poruszająca kulką
void print();                                   //funkcja wyświetlająca
void quit();                                    //funkcja kończąca program
void exiting();                                 //funkcja sprawdzająca wciśnięcie ESC
point randDirection();                          //funkcja losująca kierunek poruszania się kulki
void addBall();                                 //funkcja dodająca nową kulkę
int main();

struct point {
    int x;
    int y;
};

unsigned long seed = (unsigned long)(time(nullptr));    //ziarno do losowania liczb
std::mt19937_64 rng(seed);                              //generator liczb losowych
std::uniform_int_distribution<int> randInt(-3, 3);
point max_size = {0, 0};                                //rozmiar okeinka
point start = {0, 1};                                   //punkt startowy dla kulek
bool end = false;
std::vector < Ball* > balls;                            //wektor kulek
std::vector < std::thread* > thBalls;                   //wektor wątków połączonych z kulkami


class Ball {
    public:
        Ball() { position = start; }

        point getPosition(){ return(position); }
        void setPosition(point nPosition) { position = nPosition; }
        bool isDeleteFlag() const { return deleteFlag; }
        void setDeleteFlag(bool deleteFlag) { Ball::deleteFlag = deleteFlag; }
        void show(){ mvaddch(position.y, position.x, KULKA); }

    private:
        point position;
        bool deleteFlag;
};

//funkcja inicjalizująca ncurses
void init()
{
    initscr();
    noecho();
    curs_set(0);
    getmaxyx( stdscr, max_size.y, max_size.x );
    start.x = max_size.x/2;
}

//funkcja usuwająca stare kulki
void removeOld()
{
    for(unsigned int i = 0; i < balls.size(); i++) {
        if(balls.at(i)->isDeleteFlag()) {
            thBalls.erase(thBalls.begin()+i);
            balls.erase(balls.begin()+i);
        }
    }
}

//funkcja poruszająca kulką
void runBall(Ball* ball, point direction)
{
    unsigned int sleepTime = 100000;
    while (sleepTime <= 800000 && !end)
    {
        ball->setPosition(point{ ball->getPosition().x+direction.x, ball->getPosition().y+direction.y });
        usleep(sleepTime);

        if (ball->getPosition().x >= (max_size.x-1) || ball->getPosition().x <= 1) //czy nie wypadła po bokach okna
        {
            direction.x = -direction.x;
            sleepTime *= 2;
        }
        if (ball->getPosition().y >= (max_size.y-1) || ball->getPosition().y <= 1) //czy nie wypadła u góry albo u dołu okna
        {
            direction.y = -direction.y;
            sleepTime *= 2;
        }
    }
    ball->setDeleteFlag(true);
    removeOld();
}

//funkcja wyświetlająca
void print()
{
    while (!end)
    {
        refresh();
        usleep(refresh_ekranu);
        clear();
        for (auto &ball : balls) { ball->show(); }
        std::string str = ("Kulki: " + std::to_string(thBalls.size()));
        char const *pchar = str.c_str();
        mvprintw(0,0, pchar);
    }

    quit();
    clear();
    mvprintw(start.y, start.x - 3, "THE END");
    refresh();
    sleep(3);
}

//funkcja kończąca program
void quit()
{
    for (auto &ball : thBalls) {
        try {
            ball->join();
        }
        catch (std::system_error &err) {}
    }
    balls.clear();
    thBalls.clear();
}

//funkcja sprawdzająca wciśnięcie ESC
void exiting() { if(getch() == 27) end = true; }

//funkcja losująca kierunek poruszania się kulki
point randDirection()
{
    point dir = {0,0};
    dir.x = randInt(rng);
    dir.y = 1;
    return dir;
}

//funkcja dodająca nową kulkę
void addBall()
{
    Ball *ball = new Ball();
    balls.push_back(ball);
    std::thread *th = new std::thread (runBall, ball, randDirection());
    thBalls.push_back(th);
    sleep(2);
}

int main() {
    init();

    std::thread ending(exiting);
    std::thread screen(print);

    while(!end) addBall();

    ending.join();
    screen.join();
    sleep(2);

    endwin();
    return 0;
}