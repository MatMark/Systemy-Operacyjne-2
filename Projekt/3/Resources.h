#ifndef PROJEKT3_RESOURCES_H
#define PROJEKT3_RESOURCES_H
#include <condition_variable>

class Resources {
public:
    static std::mutex mtxCook;
    static std::condition_variable cvCook;
    static std::mutex mtxDinning;
    static std::condition_variable cvDinning;
};

#endif //PROJEKT3_RESOURCES_H
