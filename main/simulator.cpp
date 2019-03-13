#include <iostream>
#include <windows.h>
#include <string>
// #include <unistd.h>

std::string getColor(int);

int main() 
{
    while (true) {
        std::cout << getColor(0xe67e22) << "\n";
        Sleep(150);
    }
    return 0;
}

std::string getColor(int hexValue)
{
    int red = ((hexValue >> 16) & 0xFF);    // Extract the RR byte
    int green = ((hexValue >> 8) & 0xFF);   // Extract the GG byte
    int blue = ((hexValue) & 0xFF);         // Extract the BB byte
    char buffer[25];

    int n = sprintf(buffer, "\x1b[48;2;%d;%d;%dm \x1b[0m", red, green, blue);
    return std::string(&buffer[0], n);
} 