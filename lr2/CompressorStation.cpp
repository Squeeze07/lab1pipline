#include "CompressorStation.h"
#include "Utils.h"

#include <iostream>

using namespace std;

CompressorStation::CompressorStation(int stationId) : id(stationId), totalWorkshops(0), workingWorkshops(0), stationClass(0) {}

int CompressorStation::GetId() const { return id; }
const string& CompressorStation::GetName() const { return name; }

double CompressorStation::GetIdlePercent() const
{
    return 100.0 * (totalWorkshops - workingWorkshops) / totalWorkshops;
}

istream& operator>>(istream& input, CompressorStation& station)
{
    cout << "Название станции: ";
    station.name = ReadText(input);
    cout << "Всего цехов: ";
    station.totalWorkshops = ReadInt(input);
    while (station.totalWorkshops <= 0)
    {
        cout << "Количество должно быть больше 0: ";
        station.totalWorkshops = ReadInt(input);
    }
    cout << "Работающих цехов: ";
    station.workingWorkshops = ReadInt(input);
    while (station.workingWorkshops < 0 || station.workingWorkshops > station.totalWorkshops)
    {
        cout << "Введите число от 0 до " << station.totalWorkshops << ": ";
        station.workingWorkshops = ReadInt(input);
    }
    cout << "Класс станции: ";
    station.stationClass = ReadInt(input);
    while (station.stationClass <= 0)
    {
        cout << "Класс должен быть больше 0: ";
        station.stationClass = ReadInt(input);
    }
    return input;
}

ostream& operator<<(ostream& output, const CompressorStation& station)
{
    output << "КС ID " << station.id << ": " << station.name << ", цехов " << station.totalWorkshops << ", работают " << station.workingWorkshops << ", класс " << station.stationClass;
    return output;
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
    if (action == 1 && workingWorkshops < totalWorkshops)
    {
        workingWorkshops++;
        cout << "Цех запущен.\n";
        LogAction("Запущен цех станции ID " + to_string(id));
    }
    else if (action == 2 && workingWorkshops > 0)
    {
        workingWorkshops--;
        cout << "Цех остановлен.\n";
        LogAction("Остановлен цех станции ID " + to_string(id));
    }
    else
    {
        cout << "Невозможно выполнить действие.\n";
        LogAction("Изменение станции ID " + to_string(id) + ": невозможно");
    }
}

ofstream& operator<<(ofstream& file, const CompressorStation& station)
{
    file << station.id << '\n' << station.name << '\n' << station.totalWorkshops << '\n' << station.workingWorkshops << '\n' << station.stationClass << '\n';
    return file;
}

ifstream& operator>>(ifstream& file, CompressorStation& station)
{
    file >> station.id;
    file.ignore(10000, '\n');
    getline(file, station.name);
    file >> station.totalWorkshops >> station.workingWorkshops >> station.stationClass;
    if (file.fail() || station.id <= 0 || station.name.empty() || station.totalWorkshops <= 0 || station.workingWorkshops < 0 || station.workingWorkshops > station.totalWorkshops || station.stationClass <= 0)
    {
        file.setstate(ios::failbit);
        return file;
    }
    file.ignore(10000, '\n');
    return file;
}
