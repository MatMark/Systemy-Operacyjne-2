#include "Room.h"

Room::Room(int x, int y, int horizontalSize, int verticalSize, std::string name) {
    this->x=x;
    this->y=y;
    this->horizontalSize=horizontalSize;
    this->verticalSize=verticalSize;
    this->name= std::move(name);
}

char const* Room::getName() {

    return this->name.c_str();;
}

void Room::setPlates(int number) {
    this->plates=number;
}