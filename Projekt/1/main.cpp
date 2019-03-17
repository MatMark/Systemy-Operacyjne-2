#include <ncurses.h>
#include <thread>
#include <unistd.h>
#include <cmath>
#include <vector>

#define KULKA 'o'
#define refresh_kulki 100000
#define refresh_ekranu 10000
#define min_speed 0.2f
#define liczba_kulek 8

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

//deklaracje funkcji
speed randDirection();
void init();
void print();
void addBall();
void addThread();

//zmienne globalne
point max_size = {0, 0};
point start = {0, 1};
std::vector < Ball > balls;
std::vector < std::thread > ballsThreads;
bool end = false;

class Ball{
public:
    //---KONSTRUKTORY---
    Ball() = default;
    Ball(speed nSpeed) { act_position.x = start.x; act_position.y = start.y; act_speed = nSpeed; }

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
        if (getX() >= (max_size.x)) //czy nie wypadła po prawej stronie okna
        {
            setX(max_size.x-1);
            setSpeedX(-(getSpeedX()/2));
            setSpeedY(getSpeedY()/2);
        }
        else if (getX() <= 0) //czy nie wypadła po lewej stronie okna
        {
            setX(1);
            setSpeedX(-(getSpeedX()/2));
            setSpeedY(getSpeedY()/2);
        }

        if (getY() >= (max_size.y)) //czy nie wypadła z dołu okna
        {
            setY(max_size.y-1);
            setSpeedX(getSpeedX()/2);
            setSpeedY(-getSpeedY()/2);
        }
        else if (getY() <= 0) //czy nie wypadła u góry okna
        {
            setY(1);
            setSpeedX(getSpeedX()/2);
            setSpeedY(-getSpeedY()/2);
        }
    }

    //metoda sprawdzająca czy prędkość x i y jest większa niż min_speed
    //bool checkSpeed(){ return std::fabs(getSpeedX()) >= min_speed && std::fabs(getSpeedY()) >= min_speed; } //nie będzie pionowych
    bool checkSpeed(){ return std::fabs(getSpeedY()) >= min_speed; }

    //metoda ruchu kulki
    void runBall()
    {
        while (checkSpeed())
        {
            checkBoudary();
            incPos();
            usleep(refresh_kulki);
        }
        restart();
    }

    //metoda dodająca do wyświetlenia kulkę (zaokrągląnie i zrzutowane do int'a pozycji kulki)
    void show(){ mvaddch((int) std::round(getY()), (int) std::round(getX()), KULKA); }

private:
    fpoint act_position{};
    speed act_speed{};
    void incX() { setX(getX() + getSpeedX()); } //zwiększenie x w zależności od prędkości horyzontalnej
    void incY() { setY(getY() + getSpeedY()); } //zwiększenie y w zależności od prędkości wertykalnej
    void incPos() { incX(); incY(); } //zwiększenie pozycji
    void setStart(){ act_position.x = start.x; act_position.y = start.y; }
    void restart(){ setStart(); setSpeed(randDirection()); runBall(); }
};

//funkcja inicjalizująca
void init()
{
    srand (time(NULL));
    initscr();
    curs_set(0);
    getmaxyx( stdscr, max_size.y, max_size.x );
    start.x = max_size.x/2;
}

//funkcja wyświetlająca
void print()
{
    while (!end)
    {
        refresh();
        usleep(refresh_ekranu);
        clear();

        for(int i = 0; i < ballsThreads.size(); i++) balls.at(i).show();
    }
}

//funkcja losująca kierunek (prędkość)
speed randDirection()
{
    speed dir;
    dir.sX = (float) (rand() % 7 - 3)*2;
    //dir.sY = (float) (rand() % 7 - 3); //więcej niż 7 kierunków
    dir.sY = (float) (2);
    //if (dir.sX == 0) dir.sX = (float) (rand() % 7 - 3); //żeby nie leciała w poziomie
    //if (dir.sY == 0) dir.sY = (float) (rand() % 7 - 3); //żeby nie leciała w pionie
    return dir;
}

//funkcja dodająca kulki
void addBall() { balls.emplace_back(Ball(randDirection())); }

//funkcja dodająca wątki
void addThread() { ballsThreads.emplace_back(&Ball::runBall, &(balls.at(ballsThreads.size()))); }

int main(int argc, char* argv[])
{
    init();

    //dodawanie kulek do wektora
    for(int i = 0; i < liczba_kulek; i ++) addBall();

    //tworzenie wątków
    std::thread thScreen(print);
    for(int i = 0; i < liczba_kulek; i ++) { addThread(); sleep(1); }

    //łączenie wątków
    thScreen.join();
    for(int i = 0; i < liczba_kulek; i ++) ballsThreads.at(i).join();

    endwin();
    return 0;
}