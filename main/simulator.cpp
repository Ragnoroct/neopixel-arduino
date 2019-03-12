#include <iostream>
#include <unistd.h>

int main() 
{
    while (true) {
        std::cout << "\033[38;5;208mpeach\033[0;00m\n";
        usleep(150 * 1000);
    }
    return 0;
}