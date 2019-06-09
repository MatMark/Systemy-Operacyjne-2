#include <unistd.h>
#include "Client.h"
#include "Resources.h"

Client::Client(int x, int y, Room *kit, Room *din, Room *dish) {
    this->pos.x = x;
    this->pos.y = y;
    this->kitchen = kit;
    this->dinning = din;
    this->dishwasher = dish;
}


void Client::run(Client *client) {
    while (!client->exited) {
        if((client->pos.x > 40 && client->pos.y == 0) || (client->pos.x >= 7 && client->pos.y >= 9)){
            client->pos.x--;
        }
        else if(client->pos.x <= 40 && client->pos.y < 9){
            client->pos.y++;
        }
        else{
            client->exited = true;
        }
        usleep(100000);
        checkRoom(client);
    }
}

void Client::checkRoom(Client *client) {
    if(client->target != 1 && client->pos.x <= 40 && client->pos.y < 9){
        Client::getDish(client);
        client->target=1;
    }
    else if(client->target != 2 && client->pos.x > 7 && client->pos.y >= 9){
        Client::eat(client);
        client->target=2;
    }
    else if(client->target != 3 && client->pos.x == 7 && client->pos.y == 9){
        client->dishwasher->plates++;
        client->target=3;
    }
}

void Client::getDish(Client *client) {
    client->kitchen->clients++;
    std::unique_lock<std::mutex> lck(Resources::mtxCook);
    Resources::cvCook.wait(lck);
}

void Client::eat(Client *client) {
    if(!client->dinning->isFull)
    {
        client->dinning->clients++;
        if (client->dinning->clients >= TABLES) client->dinning->isFull = true;
        sleep(E_TIME);
        client->dinning->clients--;
        if (client->dinning->clients < TABLES) client->dinning->isFull = false;
    }else{
        client->dinning->inQueue++;
        std::unique_lock<std::mutex> lck(Resources::mtxDinning);
        Resources::cvDinning.wait(lck);
        sleep(E_TIME);
        client->dinning->clients--;
        if (client->dinning->clients < TABLES) client->dinning->isFull = false;
    }

}