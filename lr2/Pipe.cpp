#include "Pipe.h"
#include "Utils.h"

#include <iostream>

using namespace std;

Pipe::Pipe(int id) : id_(id), length_(0), diameter_(0), underRepair_(false) {}

int Pipe::GetId() const { return id_; }
const string& Pipe::GetName() const { return name_; }
bool Pipe::IsUnderRepair() const { return underRepair_; }
void Pipe::SetRepair(bool value) { underRepair_ = value; }

istream& operator>>(istream& input, Pipe& pipe)
{
    cout << "Название трубы: ";
    pipe.name_ = ReadText(input);
    cout << "Длина в километрах: ";
    pipe.length_ = ReadDouble(input);
    while (pipe.length_ <= 0)
    {
        cout << "Длина должна быть больше 0: ";
        pipe.length_ = ReadDouble(input);
    }
    cout << "Диаметр в миллиметрах: ";
    pipe.diameter_ = ReadInt(input);
    while (pipe.diameter_ <= 0)
    {
        cout << "Диаметр должен быть больше 0: ";
        pipe.diameter_ = ReadInt(input);
    }
    cout << "В ремонте? (0 - нет, 1 - да): ";
    int answer = ReadInt(input);
    while (answer != 0 && answer != 1)
    {
        cout << "Введите 0 или 1: ";
        answer = ReadInt(input);
    }
    pipe.underRepair_ = answer == 1;
    return input;
}

ostream& operator<<(ostream& output, const Pipe& pipe)
{
    output << "Труба ID " << pipe.id_ << ": " << pipe.name_ << ", " << pipe.length_ << " км, "
           << pipe.diameter_ << " мм, в ремонте: " << (pipe.underRepair_ ? "да" : "нет");
    return output;
}

ofstream& operator<<(ofstream& file, const Pipe& pipe)
{
    file << pipe.id_ << '\n' << pipe.name_ << '\n' << pipe.length_ << '\n'
         << pipe.diameter_ << '\n' << pipe.underRepair_ << '\n';
    return file;
}

ifstream& operator>>(ifstream& file, Pipe& pipe)
{
    int repair;
    file >> pipe.id_;
    file.ignore(10000, '\n');
    getline(file, pipe.name_);
    file >> pipe.length_ >> pipe.diameter_ >> repair;
    if (file.fail() || pipe.id_ <= 0 || pipe.name_.empty() || pipe.length_ <= 0 ||
        pipe.diameter_ <= 0 || (repair != 0 && repair != 1))
    {
        file.setstate(ios::failbit);
        return file;
    }
    pipe.underRepair_ = repair == 1;
    file.ignore(10000, '\n');
    return file;
}
