#include "Resources.h"

std::mutex Resources::mtxCook;
std::condition_variable Resources::cvCook;
std::mutex Resources::mtxDinning;
std::condition_variable Resources::cvDinning;