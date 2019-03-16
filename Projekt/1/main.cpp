#include <iostream>
#include <ncurses.h>
#include <thread>
#include <unistd.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>

#define KULKA 'o'
#define min_speed 0.2f

class Ball;

struct point {
    int x;
    int y;
};

struct fpoint {
    float x;
    float y;
};

struct speed {
    float sX;
    float sY;
};

//zmienne globalne
point max_size = {0, 0};
point start = {0, 1};
std::vector < Ball > balls;

class Ball{
public:
    //---KONSTRUKTORY---
    Ball() = default;
    Ball(point start, speed nSpeed) { act_position.x = start.x; act_position.y = start.y; act_speed = nSpeed; }

    //---GET/SET---

    fpoint getPosition(){ return(act_position); }
    void setPosition(fpoint nPosition) { act_position = nPosition; }

    float getX(){ return(act_position.x); }
    void setX(float nX) { act_position.x = nX; }

    float getY(){ return(act_position.y); }
    void setY(float nY) { act_position.y = nY; }

    speed getSpeed(){ return(act_speed); }
    void setSpeed(speed nSpeed) { act_speed = nSpeed; }

    float getSpeedX(){ return(act_speed.sX); }
    void setSpeedX(float nSpeedX) { act_speed.sX = nSpeedX; }

    float getSpeedY(){ return(act_speed.sY); }
    void setSpeedY(float nSpeedY) { act_speed.sY = nSpeedY; }

    //---METODY---

    //metoda sprawdzająca czy nie zaszła kolizja z brzegami okna
    void checkBoudary()
    {
        if (getX() > (max_size.x-1)) //czy nie wypadła po prawej stronie okna
        {
            setX(max_size.x-2);
            setSpeedX(-(getSpeedX()/2));
            setSpeedY(getSpeedY()/2);
        }
        else if (getX() < 0) //czy nie wypadła po lewej stronie okna
        {
            setX(1);
            setSpeedX(-(getSpeedX()/2));
            setSpeedY(getSpeedY()/2);
        }

        if (getY() > (max_size.y-1)) //czy nie wypadła z dołu okna
        {
            setY(max_size.y-2);
            setSpeedX(getSpeedX()/2);
            setSpeedY(-getSpeedY()/2);
        }
        else if (getY() < 0) //czy nie wypadła u góry okna
        {
            setY(1);
            setSpeedX(getSpeedX()/2);
            setSpeedY(-getSpeedY()/2);
        }
    }

    //metoda sprawdzająca czy prędkość x i y jest większa niż min_speed
    bool checkSpeed(){ return std::fabs(getSpeedX()) >= min_speed && std::fabs(getSpeedY()) >= min_speed; }

    //metoda ruchu kulki
    void runBall()
    {
        while (checkSpeed())
        {
            checkBoudary();

            incPos();
            usleep(100000);
        }
        setPosition(fpoint{0,0});
    }

    //metoda dodająca do wyświetlenia kulkę (zaokrągląnie i zrzutowane do int'a pozycji kulki)
    void show(){ mvaddch((int) std::round(getY()), (int) std::round(getX()), KULKA); }

private:
    fpoint act_position{};
    speed act_speed{};
    void incX() { setX(getX() + getSpeedX()); } //zwiększenie x w zależności od prędkości horyzontalnej
    void incY() { setY(getY() + getSpeedY()); } //zwiększenie y w zależności od prędkości wertykalnej
    void incPos() { incX(); incY(); } //zwiększenie pozycji
};

//funkcja inicjalizująca
void init()
{
    initscr();
    curs_set(0);
    getmaxyx( stdscr, max_size.y, max_size.x );
    start.x = max_size.x/2;
}

//funkcja wyświetlająca
void print()
{
    while (true)
    {
        refresh();
        usleep(100000);
        clear();
        balls.at(0).show();
        balls.at(1).show();
        balls.at(2).show();
    }
}

int main(int argc, char* argv[])
{
    init();

    //dodawanie kulek do wektora
    balls.emplace_back(Ball(start, speed {4.f, 2.f}));
    balls.emplace_back(Ball(start, speed {-4.f, 3.f}));
    balls.emplace_back(Ball(start, speed {1.f, 5.f}));

    //tworzenie wątków
    std::thread thScreen(print);
    std::thread th0(&Ball::runBall, std::ref(balls.at(0)));
    std::thread th1(&Ball::runBall, std::ref(balls.at(1)));
    std::thread th2(&Ball::runBall, std::ref(balls.at(2)));

    //łączenie wątków
    thScreen.join();
    th0.join();
    th1.join();
    th2.join();

    endwin();
    return 0;
}