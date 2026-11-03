#include "Utils.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

int ReadInt(istream& input)
{
    int value;
    input >> value;
    if (input.eof()) exit(0);
    while (input.fail() || input.peek() != '\n')
    {
        input.clear();
        input.ignore(10000, '\n');
        cout << "Ошибка. Введите целое число: ";
        input >> value;
        if (input.eof()) exit(0);
    }
    input.ignore(10000, '\n');
    return value;
}

double ReadDouble(istream& input)
{
    double value;
    input >> value;
    if (input.eof()) exit(0);
    while (input.fail() || input.peek() != '\n')
    {
        input.clear();
        input.ignore(10000, '\n');
        cout << "Ошибка. Введите число: ";
        input >> value;
        if (input.eof()) exit(0);
    }
    input.ignore(10000, '\n');
    return value;
}

string ReadText(istream& input)
{
    string text;
    getline(input, text);
    if (input.eof()) exit(0);
    while (text.empty())
    {
        cout << "Ошибка. Строка не должна быть пустой: ";
        getline(input, text);
        if (input.eof()) exit(0);
    }
    return text;
}

void LogAction(const string& message)
{
    ofstream file("actions.log", ios::app);
    if (file.is_open()) file << message << '\n';
}
