#ifndef PROJEKT3_ROOM_H
#define PROJEKT3_ROOM_H
#define PLATES 10   //liczba dostępnych talerzy
#define C_TIME 4    //czas potrzebny na umycie talerza

#include <string>

class Room {
public:
    Room(int x, int y, int horizontalSize, int verticalSize, std::string name);
    int x;                  //pozycja box'a
    int y;
    int horizontalSize;     //wielkość box'a
    int verticalSize;
    int clients = 0;        //liczba klientów w pokoju
    int plates = 0;         //liczba talerzy w pokoju
    bool isFull = false;    //czy nadmiar klientów
    int inQueue = 0;        //liczba klientów w kolejce
    std::string name;       //nazwa pokoju

    char const* getName();
    void setPlates(int number);
};

#endif //PROJEKT3_ROOM_H