#include "CompressorStation.h"
#include "Utils.h"

#include <iostream>

using namespace std;

CompressorStation::CompressorStation(int id)
    : id_(id), totalWorkshops_(0), workingWorkshops_(0), stationClass_(0) {}

int CompressorStation::GetId() const { return id_; }
const string& CompressorStation::GetName() const { return name_; }

double CompressorStation::GetIdlePercent() const
{
    return 100.0 * (totalWorkshops_ - workingWorkshops_) / totalWorkshops_;
}

void CompressorStation::Input()
{
    cout << "Название станции: ";
    name_ = ReadText();
    cout << "Всего цехов: ";
    totalWorkshops_ = ReadInt();
    while (totalWorkshops_ <= 0)
    {
        cout << "Количество должно быть больше 0: ";
        totalWorkshops_ = ReadInt();
    }
    cout << "Работающих цехов: ";
    workingWorkshops_ = ReadInt();
    while (workingWorkshops_ < 0 || workingWorkshops_ > totalWorkshops_)
    {
        cout << "Введите число от 0 до " << totalWorkshops_ << ": ";
        workingWorkshops_ = ReadInt();
    }
    cout << "Класс станции: ";
    stationClass_ = ReadInt();
    while (stationClass_ <= 0)
    {
        cout << "Класс должен быть больше 0: ";
        stationClass_ = ReadInt();
    }
}

void CompressorStation::Print() const
{
    cout << "КС ID " << id_ << ": " << name_ << ", цехов " << totalWorkshops_
         << ", работают " << workingWorkshops_ << ", класс " << stationClass_ << '\n';
}

void CompressorStation::EditWorkshop()
{
    cout << "1 - запустить цех, 2 - остановить: ";
    int action = ReadInt();
    while (action != 1 && action != 2)
    {
        cout << "Введите 1 или 2: ";
        action = ReadInt();
    }
    if (action == 1 && workingWorkshops_ < totalWorkshops_)
    {
        workingWorkshops_++;
        cout << "Цех запущен.\n";
        LogAction("Запущен цех станции ID " + to_string(id_));
    }
    else if (action == 2 && workingWorkshops_ > 0)
    {
        workingWorkshops_--;
        cout << "Цех остановлен.\n";
        LogAction("Остановлен цех станции ID " + to_string(id_));
    }
    else
    {
        cout << "Невозможно выполнить действие.\n";
        LogAction("Изменение станции ID " + to_string(id_) + ": невозможно");
    }
}

ofstream& operator<<(ofstream& file, const CompressorStation& station)
{
    file << station.id_ << '\n' << station.name_ << '\n' << station.totalWorkshops_ << '\n'
         << station.workingWorkshops_ << '\n' << station.stationClass_ << '\n';
    return file;
}

ifstream& operator>>(ifstream& file, CompressorStation& station)
{
    file >> station.id_;
    file.ignore(10000, '\n');
    getline(file, station.name_);
    file >> station.totalWorkshops_ >> station.workingWorkshops_ >> station.stationClass_;
    if (file.fail() || station.id_ <= 0 || station.name_.empty() || station.totalWorkshops_ <= 0 ||
        station.workingWorkshops_ < 0 || station.workingWorkshops_ > station.totalWorkshops_ ||
        station.stationClass_ <= 0)
    {
        file.setstate(ios::failbit);
        return file;
    }
    file.ignore(10000, '\n');
    return file;
}
