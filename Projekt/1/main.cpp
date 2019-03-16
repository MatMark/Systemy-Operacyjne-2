#include <iostream>
#include <ncurses.h>
#include <thread>
#include <unistd.h>

#define KULKA 'o'

struct point {
    int x;
    int y;
};

struct speed {
    double sX;
    double sY;
};

//zmienne globalne
point max_size = {0, 0};
point start = {0, 0};

class Ball{
public:
    Ball(point start, speed nSpeed) { position = start; act_speed = nSpeed; }

    point getPosition(){ return(position); }
    void setPosition(point nPosition) { position = nPosition; }

    int getX(){ return(position.x); }
    void setX(int nX) { position.x = nX; }

    int getY(){ return(position.y); }
    void setY(int nY) { position.y = nY; }

    speed getSpeed(){ return(act_speed); }
    void setSpeed(speed nSpeed) { act_speed = nSpeed; }

    double getSpeedX(){ return(act_speed.sX); }
    void setSpeedX(double nSpeedX) { act_speed.sX = nSpeedX; }

    double getSpeedY(){ return(act_speed.sY); }
    void setSpeedY(double nSpeedY) { act_speed.sY = nSpeedY; }

    //void setStart(point start) { x = start.x; y = start.y; }

    void runBall()
    {
        while (true)
        {
            //mvprintw(getY(), getX(), "o" );
            clear();
            mvaddch(getY(), getX(), KULKA);
            refresh();
            //std::cout<<getX()<<" :: "<<getY()<<std::endl;
            incPos();
            usleep(1000000);
        }
    }

private:
    point position;
    speed act_speed;
    void incX() { setX(getX() + (int)getSpeedX()); }
    void incY() { setY(getY() + (int)getSpeedY()); }
    void incPos() { incX(); incY(); }
};

int main(int argc, char* argv[])
{
    initscr();
    curs_set(0);
    getmaxyx( stdscr, max_size.y, max_size.x );
    start.x = max_size.x/2;

    Ball dot(start, speed {1.f, 1.f});
	std::thread th(&Ball::runBall, dot);

    //while(true)//wyświetlanie
    //{
     //   clear();
     //   sleep(1);
     //   mvprintw(dot.getY(), dot.getX(), kulka );
    //    refresh();
    //}
    th.join();
    endwin();
    return 0;
}
