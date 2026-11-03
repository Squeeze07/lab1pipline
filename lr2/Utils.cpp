#include "Utils.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

int ReadInt()
{
    int value;
    cin >> value;
    if (cin.eof()) exit(0);
    while (cin.fail() || cin.peek() != '\n')
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка. Введите целое число: ";
        cin >> value;
        if (cin.eof()) exit(0);
    }
    cin.ignore(10000, '\n');
    return value;
}

double ReadDouble()
{
    double value;
    cin >> value;
    if (cin.eof()) exit(0);
    while (cin.fail() || cin.peek() != '\n')
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка. Введите число: ";
        cin >> value;
        if (cin.eof()) exit(0);
    }
    cin.ignore(10000, '\n');
    return value;
}

string ReadText()
{
    string text;
    getline(cin, text);
    if (cin.eof()) exit(0);
    while (text.empty())
    {
        cout << "Ошибка. Строка не должна быть пустой: ";
        getline(cin, text);
        if (cin.eof()) exit(0);
    }
    return text;
}

void LogAction(const string& message)
{
    ofstream file("actions.log", ios::app);
    if (file.is_open()) file << message << '\n';
}
