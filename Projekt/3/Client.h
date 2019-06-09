#ifndef PROJEKT3_CLIENT_H
#define PROJEKT3_CLIENT_H

#include "Room.h"

#define E_TIME 15   //czas potrzebny na zjedzenie posiłku
#define TABLES 4    //liczba dostępnych stolików

struct point {
    int x;
    int y;
};

class Client {
public:
    static void run(Client* client);        //ruch
    static void checkRoom(Client *client);  //zmiana celu
    static void getDish(Client *client);    //branie posiłku
    static void eat(Client *client);        //jedzenie
    Room *kitchen;                          //kuchnia
    Room *dinning;                          //jadalnia
    Room *dishwasher;                       //zmywak
    int target = 0;                         //pokój docelowy
    bool exited = false;                    //czy klient już wychodzi
    point pos = point{0,0};                 //pozycja klienta

    Client(int x, int y, Room *kit, Room *din, Room *dish);
};

#endif //PROJEKT3_CLIENT_H