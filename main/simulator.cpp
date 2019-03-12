#include <iostream>
#include <windows.h>
// #include <unistd.h>

int main() 
{
    while (true) {
        std::cout << "\033[38;5;208mpeach\033[0;00m\n";
        Sleep(150);
    }
    return 0;
}