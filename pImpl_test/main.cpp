#include <iostream>
#include "widget.h"
// user (main.cpp)
int main()
{
    widget w(35);
    w.set_id(10);
    std::cout << w.get_id() << std::endl;
    return 0;
}
