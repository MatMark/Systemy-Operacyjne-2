#include <ncurses.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <random>
#include "Resources.h"
#include "Client.h"
#include "Room.h"

#define OSOBA 'o'   //znak symbolizujący klienta
#define MAX_T 4     //maksymalny czas do pojawienia się nowego klienta
#define MIN_T 2     //minimalny czas do pojawienia się nowego klienta

unsigned long seed = (unsigned long)(time(nullptr));    //ziarno do losowania liczb
std::mt19937_64 rng(seed);                              //generator liczb losowych
std::uniform_int_distribution<int> randInt(MIN_T, MAX_T);

std::vector < Client* > clients;                        //wektor klientów
std::vector < std::thread* > thClients;                 //wektor wątków klientów

point max_size;                                         //wielkość okna
Room *kitchen = new Room(0,0,21,7, "Kuchnia");          //reprezentacja pokoi
Room *dinning = new Room(0,0,35,7, "Jadalnia");
Room *dishwasher = new Room(0,0,15,7, "Zmywak");
Room *corridor;
bool end = false;                                       //flaga zakończenia programu
bool closing = true;                                   //flaga kończenia wątków

void init();                                    //funkcja inicjalizująca
void print();                                   //funkcja wyświetlająca
void exiting();                                 //funkcja sprawdzająca ESC
void newClient();                               //funkcja dodająca nowego klienta
void exitedClients();                           //funkcja usuwająca klientów wychodzących
void cleaningPlate(Room * dish, Room * kit);    //funkcja myjąca talerze
void cook(Room * kit);                          //funkcja nakładająca jedzenie
void eating(Room * din);                        //funkcja jedząca


//funkcja inicjalizująca
void init()
{
    initscr();
    noecho();
    curs_set(0);

    getmaxyx( stdscr, max_size.y, max_size.x );

    kitchen->x = max_size.x/2;
    dinning->x = max_size.x/2;
    dinning->y = max_size.y-dinning->verticalSize;
    dishwasher->y = max_size.y-dishwasher->verticalSize;
    kitchen->setPlates(PLATES);

    int corh = max_size.y - kitchen->verticalSize - dinning->verticalSize;
    int corw = max_size.x;
    corridor = new Room(0,kitchen->verticalSize,corw,corh, "Korytarz");
}

//funkcja wyświetlająca
void print()
{
    while (!end) {
        WINDOW *kit = newwin(kitchen->verticalSize, kitchen->horizontalSize, kitchen->y,
                             kitchen->x - (kitchen->horizontalSize / 2));
        box(kit, 0, 0);
        touchwin(kit);
        mvwprintw(kit, 1, 1, kitchen->getName());
        mvwprintw(kit, 2, 1, "W kolejce: %d", kitchen->clients);
        mvwprintw(kit, 3, 1, "Talerzy: %d", kitchen->plates);
        wrefresh(kit);

        WINDOW *din = newwin(dinning->verticalSize, dinning->horizontalSize, dinning->y,
                             dinning->x - (dinning->horizontalSize / 2));
        box(din, 0, 0);
        touchwin(din);
        mvwprintw(din, 1, 1, dinning->getName());
        mvwprintw(din, 2, 1, "Jedzacych: %d", dinning->clients);
        mvwprintw(din, 3, 1, "Czekajacych: %d", dinning->inQueue);
        wrefresh(din);

        WINDOW *dish = newwin(dishwasher->verticalSize, dishwasher->horizontalSize, dishwasher->y, dishwasher->x);
        box(dish, 0, 0);
        touchwin(dish);
        mvwprintw(dish, 1, 1, dishwasher->getName());
        mvwprintw(dish, 2, 1, "Talerzy: %d", dishwasher->plates);
        wrefresh(dish);

        WINDOW *cor = newwin(corridor->verticalSize, corridor->horizontalSize, corridor->y, corridor->x);
        touchwin(cor);
        mvwprintw(cor, 2, 1, "Wszystkich: %d", clients.size());
        for (auto &client : clients) { mvwaddch(cor, client->pos.y, client->pos.x, OSOBA); }
        wrefresh(cor);
        exitedClients();
        usleep(100000);
    }
    while (closing) {
        clear();
        mvprintw(max_size.y/2, max_size.x/2-7, "Zamykanie .  ");
        refresh();
        usleep(500000);
        clear();
        mvprintw(max_size.y/2, max_size.x/2-7, "Zamykanie .. ");
        refresh();
        usleep(500000);
        clear();
        mvprintw(max_size.y/2, max_size.x/2-7, "Zamykanie ...");
        refresh();
        usleep(500000);
    }
}

//funkcja sprawdzająca ESC
void exiting() {
    while(!end) {
        if (getch() == 27) end = true;
    }
}

//funkcja dodająca nowego klienta
void newClient()
{
    Client *client = new Client(corridor->horizontalSize-2, 0, kitchen, dinning, dishwasher);
    clients.push_back(client);
    std::thread *th = new std::thread (Client::run, client);
    thClients.push_back(th);
    sleep(randInt(rng));
}

//funkcja usuwająca klientów wychodzących
void exitedClients(){
    for(unsigned int i = 0; i < clients.size(); i++) {
        if(clients.at(i)->exited) {
            thClients.erase(thClients.begin()+i);
            clients.erase(clients.begin()+i);
        }
    }
}

//funkcja myjąca talerze
void cleaningPlate(Room * dish, Room * kit){
    while(!end){
        if(dish->plates>0){
          sleep(C_TIME);
          dish->plates--;
          kit->plates++;
        }else sleep(1);
    }
}

//funkcja nakładająca jedzenie
void cook(Room * kit){
    while(!end){
        if(kit->clients > 0 && kit->plates > 0){
            sleep(2);
            kit->plates--;
            kit->clients--;
            Resources::cvCook.notify_one();
        }
        else sleep(1);
    }
}

//funkcja jedząca
void eating(Room * din){
    while(!end){
        if(din->inQueue > 0 && !din->isFull)
        {
            din->inQueue--;
            din->clients++;
            if(din->clients >= TABLES){
                din->isFull = true;
            }
            Resources::cvDinning.notify_one();
        }
        else sleep(1);
    }
}

int main(int argc, char **argv) {
    init();

    std::thread ending(exiting);
    std::thread screen(print);
    std::thread cleaner(cleaningPlate, dishwasher, kitchen);
    std::thread foodGiver(cook, kitchen);
    std::thread dinQueue(eating, dinning);

    while(!end) newClient();

    //po naciśnięciu ESC
    ending.join();

    cleaner.join();
    foodGiver.join();
    dinQueue.join();

    for (auto &client : clients) {
        client->exited = true;
    }
    Resources::cvCook.notify_all();
    Resources::cvDinning.notify_all();

    exitedClients();

//    for (auto &client : thClients) {
//        client->join();
//    }
//    clients.clear();
//    thClients.clear();
    closing=false;
    screen.join();

    endwin();
    return 0;
}