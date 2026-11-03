#include "CompressorStation.h"
#include "Pipe.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int FindPipe(const vector<Pipe>& pipes, int id)
{
    for (int i = 0; i < static_cast<int>(pipes.size()); i++)
    {
        if (pipes[i].GetId() == id) return i;
    }
    return -1;
}

int FindStation(const vector<CompressorStation>& stations, int id)
{
    for (int i = 0; i < static_cast<int>(stations.size()); i++)
    {
        if (stations[i].GetId() == id) return i;
    }
    return -1;
}

void PrintAll(const vector<Pipe>& pipes, const vector<CompressorStation>& stations)
{
    if (pipes.empty()) cout << "Труб нет.\n";
    for (const Pipe& pipe : pipes) pipe.Print();
    if (stations.empty()) cout << "Станций нет.\n";
    for (const CompressorStation& station : stations) station.Print();
}

void EditPipe(vector<Pipe>& pipes)
{
    cout << "ID трубы: ";
    int id = ReadInt();
    int index = FindPipe(pipes, id);
    if (index == -1)
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
    pipes[index].SetRepair(answer == 1);
    LogAction("Изменена труба ID " + to_string(id) + ", ремонт: " + to_string(answer));
}

void EditStation(vector<CompressorStation>& stations)
{
    cout << "ID станции: ";
    int id = ReadInt();
    int index = FindStation(stations, id);
    if (index == -1)
    {
        cout << "Станция не найдена.\n";
        LogAction("Изменение станции ID " + to_string(id) + ": не найдена");
        return;
    }
    stations[index].EditWorkshop();
}

void DeletePipe(vector<Pipe>& pipes)
{
    cout << "ID трубы для удаления: ";
    int id = ReadInt();
    int index = FindPipe(pipes, id);
    if (index == -1)
    {
        cout << "Труба не найдена.\n";
        LogAction("Удаление трубы ID " + to_string(id) + ": не найдена");
        return;
    }
    pipes.erase(pipes.begin() + index);
    cout << "Труба удалена.\n";
    LogAction("Удалена труба ID " + to_string(id));
}

void DeleteStation(vector<CompressorStation>& stations)
{
    cout << "ID станции для удаления: ";
    int id = ReadInt();
    int index = FindStation(stations, id);
    if (index == -1)
    {
        cout << "Станция не найдена.\n";
        LogAction("Удаление станции ID " + to_string(id) + ": не найдена");
        return;
    }
    stations.erase(stations.begin() + index);
    cout << "Станция удалена.\n";
    LogAction("Удалена станция ID " + to_string(id));
}

vector<int> SearchPipes(const vector<Pipe>& pipes)
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
    vector<int> found;
    for (const Pipe& pipe : pipes)
    {
        if ((filter == 1 && pipe.GetName().find(name) != string::npos) ||
            (filter == 2 && pipe.IsUnderRepair() == (repair == 1)))
        {
            pipe.Print();
            found.push_back(pipe.GetId());
        }
    }
    cout << "Найдено труб: " << found.size() << '\n';
    LogAction("Поиск труб, фильтр " + to_string(filter) + ", найдено " + to_string(found.size()));
    return found;
}

void SearchStations(const vector<CompressorStation>& stations)
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
    for (const CompressorStation& station : stations)
    {
        if ((filter == 1 && station.GetName().find(name) != string::npos) ||
            (filter == 2 && station.GetIdlePercent() >= percent))
        {
            station.Print();
            found++;
        }
    }
    cout << "Найдено станций: " << found << '\n';
    LogAction("Поиск станций, фильтр " + to_string(filter) + ", найдено " + to_string(found));
}

void BatchEditPipes(vector<Pipe>& pipes)
{
    vector<int> found = SearchPipes(pipes);
    if (found.empty())
    {
        cout << "Трубы не найдены.\n";
        LogAction("Пакетное действие: трубы не найдены");
        return;
    }
    cout << "1 - все найденные, 0 - выбрать по ID: ";
    int all = ReadInt();
    while (all != 0 && all != 1)
    {
        cout << "Введите 0 или 1: ";
        all = ReadInt();
    }
    vector<int> chosen;
    if (all == 1) chosen = found;
    else
    {
        cout << "Вводите ID нужных труб, 0 - закончить.\n";
        while (true)
        {
            cout << "ID трубы: ";
            int id = ReadInt();
            if (id == 0) break;
            bool inFound = false;
            bool alreadyChosen = false;
            for (int foundId : found) if (foundId == id) inFound = true;
            for (int chosenId : chosen) if (chosenId == id) alreadyChosen = true;
            if (!inFound || alreadyChosen)
            {
                cout << "ID нет среди найденных или он уже выбран.\n";
                LogAction("Пакетное действие: отклонён ID " + to_string(id));
                continue;
            }
            chosen.push_back(id);
        }
    }
    if (chosen.empty())
    {
        cout << "Ничего не выбрано.\n";
        LogAction("Пакетное действие: ничего не выбрано");
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
        int index = FindPipe(pipes, id);
        if (action == 3) pipes.erase(pipes.begin() + index);
        else pipes[index].SetRepair(action == 2);
        LogAction("Пакетное действие " + to_string(action) + " с трубой ID " + to_string(id));
    }
    cout << "Обработано труб: " << chosen.size() << '\n';
}

void SaveData(const vector<Pipe>& pipes, const vector<CompressorStation>& stations,
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
    file << nextPipeId << ' ' << nextStationId << '\n';
    file << pipes.size() << '\n';
    for (const Pipe& pipe : pipes) pipe.Save(file);
    file << stations.size() << '\n';
    for (const CompressorStation& station : stations) station.Save(file);
    file.close();
    if (file.fail()) cout << "Ошибка записи файла.\n";
    else cout << "Данные сохранены.\n";
    LogAction("Сохранение в " + filename + (file.fail() ? ": ошибка записи" : ": успешно"));
}

void LoadData(vector<Pipe>& pipes, vector<CompressorStation>& stations,
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
    vector<Pipe> loadedPipes;
    vector<CompressorStation> loadedStations;
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
        if (!pipe.Load(file) || pipe.GetId() >= pipeId || FindPipe(loadedPipes, pipe.GetId()) != -1)
        {
            cout << "Данные трубы повреждены.\n";
            LogAction("Загрузка " + filename + ": ошибка данных трубы");
            return;
        }
        loadedPipes.push_back(pipe);
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
        if (!station.Load(file) || station.GetId() >= stationId ||
            FindStation(loadedStations, station.GetId()) != -1)
        {
            cout << "Данные станции повреждены.\n";
            LogAction("Загрузка " + filename + ": ошибка данных станции");
            return;
        }
        loadedStations.push_back(station);
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
    vector<Pipe> pipes;
    vector<CompressorStation> stations;
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
                pipe.Input();
                pipes.push_back(pipe);
                LogAction("Добавлена труба ID " + to_string(nextPipeId));
                nextPipeId++;
                break;
            }
            case 2:
            {
                CompressorStation station(nextStationId);
                station.Input();
                stations.push_back(station);
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
