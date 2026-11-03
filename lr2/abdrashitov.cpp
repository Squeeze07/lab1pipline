#include "CompressorStation.h"
#include "Pipe.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

// Отдельные фильтры: каждый отвечает только за одно условие поиска.
bool PipeByName(const Pipe& pipe, const string& text)
{
    return pipe.GetName().find(text) != string::npos;
}

bool PipeByRepair(const Pipe& pipe, bool repair)
{
    return pipe.IsUnderRepair() == repair;
}

bool StationByName(const CompressorStation& station, const string& text)
{
    return station.GetName().find(text) != string::npos;
}

bool StationByIdlePercent(const CompressorStation& station, double percent)
{
    return station.GetIdlePercent() >= percent;
}

void PrintAll(const map<int, Pipe>& pipes, const map<int, CompressorStation>& stations)
{
    if (pipes.empty()) cout << "Труб нет.\n";
    for (const auto& item : pipes) cout << item.second << '\n';
    if (stations.empty()) cout << "Станций нет.\n";
    for (const auto& item : stations) cout << item.second << '\n';
}

void EditPipe(map<int, Pipe>& pipes)
{
    cout << "ID трубы: ";
    int id = ReadInt();
    auto it = pipes.find(id);
    if (it == pipes.end())
    {
        cout << "Труба не найдена.\n";
        LogAction("Изменение трубы ID " + to_string(id) + ": не найдена");
        return;
    }
    cout << "В ремонте? (0 - нет, 1 - да): ";
    int answer = ReadInt();
    while (answer != 0 && answer != 1)
    {
        cout << "Введите 0 или 1: ";
        answer = ReadInt();
    }
    it->second.SetRepair(answer == 1);
    LogAction("Изменена труба ID " + to_string(id) + ", ремонт: " + to_string(answer));
}

void EditStation(map<int, CompressorStation>& stations)
{
    cout << "ID станции: ";
    int id = ReadInt();
    auto it = stations.find(id);
    if (it == stations.end())
    {
        cout << "Станция не найдена.\n";
        LogAction("Изменение станции ID " + to_string(id) + ": не найдена");
        return;
    }
    it->second.EditWorkshop();
}

void DeletePipe(map<int, Pipe>& pipes)
{
    cout << "ID трубы для удаления: ";
    int id = ReadInt();
    if (pipes.erase(id) == 0)
    {
        cout << "Труба не найдена.\n";
        LogAction("Удаление трубы ID " + to_string(id) + ": не найдена");
        return;
    }
    cout << "Труба удалена.\n";
    LogAction("Удалена труба ID " + to_string(id));
}

void DeleteStation(map<int, CompressorStation>& stations)
{
    cout << "ID станции для удаления: ";
    int id = ReadInt();
    if (stations.erase(id) == 0)
    {
        cout << "Станция не найдена.\n";
        LogAction("Удаление станции ID " + to_string(id) + ": не найдена");
        return;
    }
    cout << "Станция удалена.\n";
    LogAction("Удалена станция ID " + to_string(id));
}

set<int> SearchPipes(const map<int, Pipe>& pipes)
{
    cout << "1 - по названию, 2 - по ремонту: ";
    int filter = ReadInt();
    string name;
    int repair = 0;
    if (filter == 1)
    {
        cout << "Название или его часть: ";
        name = ReadText();
    }
    else if (filter == 2)
    {
        cout << "В ремонте? (0 или 1): ";
        repair = ReadInt();
        while (repair != 0 && repair != 1)
        {
            cout << "Введите 0 или 1: ";
            repair = ReadInt();
        }
    }
    else
    {
        cout << "Неизвестный фильтр.\n";
        LogAction("Поиск труб: неверный фильтр");
        return {};
    }
    set<int> found;
    for (const auto& item : pipes)
    {
        const Pipe& pipe = item.second;
        if ((filter == 1 && PipeByName(pipe, name)) ||
            (filter == 2 && PipeByRepair(pipe, repair == 1)))
        {
            cout << pipe << '\n';
            found.insert(item.first);
        }
    }
    cout << "Найдено труб: " << found.size() << '\n';
    LogAction("Поиск труб, фильтр " + to_string(filter) + ", найдено " + to_string(found.size()));
    return found;
}

void SearchStations(const map<int, CompressorStation>& stations)
{
    cout << "1 - по названию, 2 - по проценту незадействованных цехов: ";
    int filter = ReadInt();
    string name;
    double percent = 0;
    if (filter == 1)
    {
        cout << "Название или его часть: ";
        name = ReadText();
    }
    else if (filter == 2)
    {
        cout << "Минимальный процент незадействованных цехов (0-100): ";
        percent = ReadDouble();
        while (percent < 0 || percent > 100)
        {
            cout << "Введите число от 0 до 100: ";
            percent = ReadDouble();
        }
    }
    else
    {
        cout << "Неизвестный фильтр.\n";
        LogAction("Поиск станций: неверный фильтр");
        return;
    }
    int found = 0;
    for (const auto& item : stations)
    {
        const CompressorStation& station = item.second;
        if ((filter == 1 && StationByName(station, name)) ||
            (filter == 2 && StationByIdlePercent(station, percent)))
        {
            cout << station << '\n';
            found++;
        }
    }
    cout << "Найдено станций: " << found << '\n';
    LogAction("Поиск станций, фильтр " + to_string(filter) + ", найдено " + to_string(found));
}

void BatchEditPipes(map<int, Pipe>& pipes)
{
    set<int> found = SearchPipes(pipes);
    if (found.empty())
    {
        cout << "Трубы не найдены.\n";
        return;
    }
    cout << "1 - все найденные, 0 - выбрать по ID: ";
    int all = ReadInt();
    while (all != 0 && all != 1)
    {
        cout << "Введите 0 или 1: ";
        all = ReadInt();
    }
    set<int> chosen;
    if (all == 1) chosen = found;
    else
    {
        cout << "Вводите ID нужных труб, 0 - закончить.\n";
        while (true)
        {
            cout << "ID трубы: ";
            int id = ReadInt();
            if (id == 0) break;
            if (found.count(id) == 0 || chosen.count(id) != 0)
            {
                cout << "ID нет среди найденных или он уже выбран.\n";
                LogAction("Пакетное действие: отклонён ID " + to_string(id));
                continue;
            }
            chosen.insert(id);
        }
    }
    if (chosen.empty())
    {
        cout << "Ничего не выбрано.\n";
        return;
    }
    cout << "1 - убрать из ремонта, 2 - отправить в ремонт, 3 - удалить: ";
    int action = ReadInt();
    while (action < 1 || action > 3)
    {
        cout << "Введите 1, 2 или 3: ";
        action = ReadInt();
    }
    for (int id : chosen)
    {
        if (action == 3) pipes.erase(id);
        else pipes.at(id).SetRepair(action == 2);
        LogAction("Пакетное действие " + to_string(action) + " с трубой ID " + to_string(id));
    }
    cout << "Обработано труб: " << chosen.size() << '\n';
}

void SaveData(const map<int, Pipe>& pipes, const map<int, CompressorStation>& stations,
              int nextPipeId, int nextStationId)
{
    cout << "Имя файла для сохранения: ";
    string filename = ReadText();
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Не удалось открыть файл.\n";
        LogAction("Сохранение " + filename + ": ошибка открытия");
        return;
    }
    file << nextPipeId << ' ' << nextStationId << '\n' << pipes.size() << '\n';
    for (const auto& item : pipes) file << item.second;
    file << stations.size() << '\n';
    for (const auto& item : stations) file << item.second;
    file.close();
    if (file.fail()) cout << "Ошибка записи файла.\n";
    else cout << "Данные сохранены.\n";
    LogAction("Сохранение в " + filename + (file.fail() ? ": ошибка записи" : ": успешно"));
}

void LoadData(map<int, Pipe>& pipes, map<int, CompressorStation>& stations,
              int& nextPipeId, int& nextStationId)
{
    cout << "Имя файла для загрузки: ";
    string filename = ReadText();
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Файл не найден.\n";
        LogAction("Загрузка " + filename + ": файл не найден");
        return;
    }
    map<int, Pipe> loadedPipes;
    map<int, CompressorStation> loadedStations;
    int pipeId, stationId, pipeCount, stationCount;
    file >> pipeId >> stationId >> pipeCount;
    if (file.fail() || pipeId <= 0 || stationId <= 0 || pipeCount < 0 || pipeCount > 100000)
    {
        cout << "Ошибка формата файла.\n";
        LogAction("Загрузка " + filename + ": ошибка формата");
        return;
    }
    file.ignore(10000, '\n');
    for (int i = 0; i < pipeCount; i++)
    {
        Pipe pipe;
        file >> pipe;
        if (file.fail() || pipe.GetId() >= pipeId || loadedPipes.count(pipe.GetId()) != 0)
        {
            cout << "Данные трубы повреждены.\n";
            LogAction("Загрузка " + filename + ": ошибка данных трубы");
            return;
        }
        loadedPipes.emplace(pipe.GetId(), pipe);
    }
    file >> stationCount;
    if (file.fail() || stationCount < 0 || stationCount > 100000)
    {
        cout << "Ошибка формата файла.\n";
        LogAction("Загрузка " + filename + ": ошибка формата");
        return;
    }
    file.ignore(10000, '\n');
    for (int i = 0; i < stationCount; i++)
    {
        CompressorStation station;
        file >> station;
        if (file.fail() || station.GetId() >= stationId ||
            loadedStations.count(station.GetId()) != 0)
        {
            cout << "Данные станции повреждены.\n";
            LogAction("Загрузка " + filename + ": ошибка данных станции");
            return;
        }
        loadedStations.emplace(station.GetId(), station);
    }
    pipes = loadedPipes;
    stations = loadedStations;
    nextPipeId = pipeId;
    nextStationId = stationId;
    cout << "Данные загружены.\n";
    LogAction("Загрузка из " + filename + ": успешно");
}

void PrintMenu()
{
    cout << "\n1. Добавить трубу\n2. Добавить КС\n3. Показать все объекты\n";
    cout << "4. Изменить трубу\n5. Изменить КС\n6. Удалить трубу\n7. Удалить КС\n";
    cout << "8. Найти трубы\n9. Найти КС\n10. Пакетное действие с трубами\n";
    cout << "11. Сохранить\n12. Загрузить\n0. Выход\nВыберите действие: ";
}

int main()
{
    map<int, Pipe> pipes;
    map<int, CompressorStation> stations;
    int nextPipeId = 1;
    int nextStationId = 1;
    while (true)
    {
        PrintMenu();
        int action = ReadInt();
        LogAction("Выбран пункт меню " + to_string(action));
        switch (action)
        {
            case 1:
            {
                Pipe pipe(nextPipeId);
                cin >> pipe;
                pipes.emplace(nextPipeId, pipe);
                LogAction("Добавлена труба ID " + to_string(nextPipeId));
                nextPipeId++;
                break;
            }
            case 2:
            {
                CompressorStation station(nextStationId);
                cin >> station;
                stations.emplace(nextStationId, station);
                LogAction("Добавлена станция ID " + to_string(nextStationId));
                nextStationId++;
                break;
            }
            case 3: PrintAll(pipes, stations); break;
            case 4: EditPipe(pipes); break;
            case 5: EditStation(stations); break;
            case 6: DeletePipe(pipes); break;
            case 7: DeleteStation(stations); break;
            case 8: SearchPipes(pipes); break;
            case 9: SearchStations(stations); break;
            case 10: BatchEditPipes(pipes); break;
            case 11: SaveData(pipes, stations, nextPipeId, nextStationId); break;
            case 12: LoadData(pipes, stations, nextPipeId, nextStationId); break;
            case 0: return 0;
            default: cout << "Неизвестный пункт меню.\n";
        }
    }
}
