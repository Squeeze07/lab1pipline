#include "Pipe.h"
#include "Utils.h"

#include <iostream>

using namespace std;

Pipe::Pipe(int id) : id_(id), length_(0), diameter_(0), underRepair_(false) {}

int Pipe::GetId() const { return id_; }
const string& Pipe::GetName() const { return name_; }
bool Pipe::IsUnderRepair() const { return underRepair_; }
void Pipe::SetRepair(bool value) { underRepair_ = value; }

void Pipe::Input()
{
    cout << "Название трубы: ";
    name_ = ReadText();
    cout << "Длина в километрах: ";
    length_ = ReadDouble();
    while (length_ <= 0)
    {
        cout << "Длина должна быть больше 0: ";
        length_ = ReadDouble();
    }
    cout << "Диаметр в миллиметрах: ";
    diameter_ = ReadInt();
    while (diameter_ <= 0)
    {
        cout << "Диаметр должен быть больше 0: ";
        diameter_ = ReadInt();
    }
    cout << "В ремонте? (0 - нет, 1 - да): ";
    int answer = ReadInt();
    while (answer != 0 && answer != 1)
    {
        cout << "Введите 0 или 1: ";
        answer = ReadInt();
    }
    underRepair_ = answer == 1;
}

void Pipe::Print() const
{
    cout << "Труба ID " << id_ << ": " << name_ << ", " << length_ << " км, "
         << diameter_ << " мм, в ремонте: " << (underRepair_ ? "да" : "нет") << '\n';
}

void Pipe::Save(ofstream& file) const
{
    file << id_ << '\n' << name_ << '\n' << length_ << '\n'
         << diameter_ << '\n' << underRepair_ << '\n';
}

bool Pipe::Load(ifstream& file)
{
    int repair;
    file >> id_;
    file.ignore(10000, '\n');
    getline(file, name_);
    file >> length_ >> diameter_ >> repair;
    if (file.fail() || id_ <= 0 || name_.empty() || length_ <= 0 ||
        diameter_ <= 0 || (repair != 0 && repair != 1)) return false;
    underRepair_ = repair == 1;
    file.ignore(10000, '\n');
    return true;
}
