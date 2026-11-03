#include "Pipe.h"
#include "Utils.h"

#include <iostream>

using namespace std;

Pipe::Pipe(int pipeId) : id(pipeId), length(0), diameter(0), underRepair(false) {}

int Pipe::GetId() const { return id; }
const string& Pipe::GetName() const { return name; }
bool Pipe::IsUnderRepair() const { return underRepair; }
void Pipe::SetRepair(bool value) { underRepair = value; }

istream& operator>>(istream& input, Pipe& pipe)
{
    cout << "Название трубы: ";
    pipe.name = ReadText(input);
    cout << "Длина в километрах: ";
    pipe.length = ReadDouble(input);
    while (pipe.length <= 0)
    {
        cout << "Длина должна быть больше 0: ";
        pipe.length = ReadDouble(input);
    }
    cout << "Диаметр в миллиметрах: ";
    pipe.diameter = ReadInt(input);
    while (pipe.diameter <= 0)
    {
        cout << "Диаметр должен быть больше 0: ";
        pipe.diameter = ReadInt(input);
    }
    cout << "В ремонте? (0 - нет, 1 - да): ";
    int answer = ReadInt(input);
    while (answer != 0 && answer != 1)
    {
        cout << "Введите 0 или 1: ";
        answer = ReadInt(input);
    }
    pipe.underRepair = answer == 1;
    return input;
}

ostream& operator<<(ostream& output, const Pipe& pipe)
{
    output << "Труба ID " << pipe.id << ": " << pipe.name << ", " << pipe.length << " км, " << pipe.diameter << " мм, в ремонте: " << (pipe.underRepair ? "да" : "нет");
    return output;
}

ofstream& operator<<(ofstream& file, const Pipe& pipe)
{
    file << pipe.id << '\n' << pipe.name << '\n' << pipe.length << '\n' << pipe.diameter << '\n' << pipe.underRepair << '\n';
    return file;
}

ifstream& operator>>(ifstream& file, Pipe& pipe)
{
    int repair;
    file >> pipe.id;
    file.ignore(10000, '\n');
    getline(file, pipe.name);
    file >> pipe.length >> pipe.diameter >> repair;
    if (file.fail() || pipe.id <= 0 || pipe.name.empty() || pipe.length <= 0 || pipe.diameter <= 0 || (repair != 0 && repair != 1))
    {
        file.setstate(ios::failbit);
        return file;
    }
    pipe.underRepair = repair == 1;
    file.ignore(10000, '\n');
    return file;
}
