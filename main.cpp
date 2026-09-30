#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Данные трубы
struct Pipe
{
    string name;
    double length = 0;
    int diameter = 0;
    bool isUnderRepair = false;
    bool pipeExists = false;
};

// Данные компрессорной станции
struct CompressorStation
{
    string name;
    int totalWorkshops = 0;
    int workingWorkshops = 0;
    int stationClass = 0;
    bool stationExists = false;
};

// Чтение целого числа с проверкой
int ReadInt()
{
    int value;
    cin >> value;

    while (cin.fail() || cin.peek() != '\n')
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка. Введите целое число: ";
        cin >> value;
    }

    cin.ignore(10000, '\n');
    return value;
}

// Чтение дробного числа с проверкой
double ReadDouble()
{
    double value;
    cin >> value;

    while (cin.fail() || cin.peek() != '\n')
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка. Введите число: ";
        cin >> value;
    }

    cin.ignore(10000, '\n');
    return value;
}

// Чтение непустой строки
string ReadText()
{
    string text;
    getline(cin, text);

    while (text.empty())
    {
        cout << "Ошибка. Строка не должна быть пустой: ";
        getline(cin, text);
    }

    return text;
}

void InputPipe(Pipe& pipe)
{
    cout << "\nВвод трубы\n";
    cout << "Введите название: ";
    pipe.name = ReadText();

    cout << "Введите длину в километрах: ";
    pipe.length = ReadDouble();
    while (pipe.length <= 0)
    {
        cout << "Ошибка. Длина должна быть больше 0: ";
        pipe.length = ReadDouble();
    }

    cout << "Введите диаметр в миллиметрах: ";
    pipe.diameter = ReadInt();
    while (pipe.diameter <= 0)
    {
        cout << "Ошибка. Диаметр должен быть больше 0: ";
        pipe.diameter = ReadInt();
    }

    cout << "Труба находится в ремонте? (0 - нет, 1 - да): ";
    int repairAnswer = ReadInt();
    while (repairAnswer != 0 && repairAnswer != 1)
    {
        cout << "Ошибка. Введите 0 или 1: ";
        repairAnswer = ReadInt();
    }

    pipe.isUnderRepair = repairAnswer == 1;
    pipe.pipeExists = true;
}

void InputStation(CompressorStation& station)
{
    cout << "\nВвод компрессорной станции\n";
    cout << "Введите название: ";
    station.name = ReadText();

    cout << "Введите общее количество цехов: ";
    station.totalWorkshops = ReadInt();
    while (station.totalWorkshops <= 0)
    {
        cout << "Ошибка. Количество цехов должно быть больше 0: ";
        station.totalWorkshops = ReadInt();
    }

    cout << "Введите количество работающих цехов: ";
    station.workingWorkshops = ReadInt();
    while (station.workingWorkshops < 0 || station.workingWorkshops > station.totalWorkshops)
    {
        cout << "Ошибка. Введите число от 0 до " << station.totalWorkshops << ": ";
        station.workingWorkshops = ReadInt();
    }

    cout << "Введите класс станции: ";
    station.stationClass = ReadInt();

    while (station.stationClass <= 0)
    {
        cout << "Ошибка. Класс станции должен быть больше 0: ";
        station.stationClass = ReadInt();
    }

    station.stationExists = true;
}

void PrintPipe(const Pipe& pipe)
{
    if (!pipe.pipeExists)
    {
        cout << "Труба не добавлена.\n";
        return;
    }

    cout << "\nИнформация о трубе\n";
    cout << "Название: " << pipe.name << '\n';
    cout << "Длина: " << pipe.length << " км\n";
    cout << "Диаметр: " << pipe.diameter << " мм\n";
    cout << "Находится в ремонте: " << (pipe.isUnderRepair ? "да" : "нет") << '\n';
}

void PrintStation(const CompressorStation& station)
{
    if (!station.stationExists)
    {
        cout << "Компрессорная станция не добавлена.\n";
        return;
    }

    cout << "\nИнформация о компрессорной станции\n";
    cout << "Название: " << station.name << '\n';
    cout << "Общее количество цехов: " << station.totalWorkshops << '\n';
    cout << "Работающих цехов: " << station.workingWorkshops << '\n';
    cout << "Класс станции: " << station.stationClass << '\n';
}

void EditPipe(Pipe& pipe)
{
    if (!pipe.pipeExists)
    {
        cout << "Сначала добавьте трубу.\n";
        return;
    }

    cout << "Выберите действие: ";
    cout << "\n0. Убрать трубу из ремонта\n";
    cout << "1. Отправить трубу в ремонт\n";

    int action = ReadInt();

    while (action != 0 && action != 1)
    {
        cout << "Ошибка. Введите 0 или 1: ";
        action = ReadInt();
    }

    pipe.isUnderRepair = action == 1;
    cout << "Состояние трубы изменено.\n";
}

void EditStation(CompressorStation& station)
{
    if (!station.stationExists)
    {
        cout << "Сначала добавьте компрессорную станцию.\n";
        return;
    }

    cout << "\n1. Запустить один цех\n";
    cout << "2. Остановить один цех\n";
    cout << "Выберите действие: ";
    int action = ReadInt();

    while (action < 1 || action > 2)
    {
        cout << "Ошибка. Введите 1 или 2: ";
        action = ReadInt();
    }

    if (action == 1)
    {
        if (station.workingWorkshops < station.totalWorkshops)
        {
            station.workingWorkshops++;
            cout << "Цех запущен.\n";
        }
        else
        {
            cout << "Все цеха уже работают.\n";
        }
    }
    else
    {
        if (station.workingWorkshops > 0)
        {
            station.workingWorkshops--;
            cout << "Цех остановлен.\n";
        }
        else
        {
            cout << "Все цеха уже остановлены.\n";
        }
    }
}

void SavePipe(ofstream& file, const Pipe& pipe)
{
    file << pipe.pipeExists << '\n';

    if (pipe.pipeExists)
    {
        file << pipe.name << '\n';
        file << pipe.length << '\n';
        file << pipe.diameter << '\n';
        file << pipe.isUnderRepair << '\n';
    }
}

void SaveStation(ofstream& file, const CompressorStation& station)
{
    file << station.stationExists << '\n';

    if (station.stationExists)
    {
        file << station.name << '\n';
        file << station.totalWorkshops << '\n';
        file << station.workingWorkshops << '\n';
        file << station.stationClass << '\n';
    }
}

// Труба и станция записываются в один общий файл
void SaveData(const Pipe& pipe, const CompressorStation& station)
{
    if (!pipe.pipeExists && !station.stationExists)
    {
        cout << "Нет данных для сохранения.\n";
        return;
    }

    ofstream file("data.txt");
    if (!file.is_open())
    {
        cout << "Не удалось открыть data.txt.\n";
        return;
    }

    SavePipe(file, pipe);
    SaveStation(file, station);

    file.close();
    cout << "Данные сохранены в data.txt.\n";
}

bool LoadPipe(ifstream& file, Pipe& pipe)
{
    int pipeFlag;

    file >> pipeFlag;
    if (file.fail() || (pipeFlag != 0 && pipeFlag != 1))
    {
        cout << "Ошибка чтения data.txt.\n";
        return false;
    }
    file.ignore(10000, '\n');

    if (pipeFlag == 1)
    {
        getline(file, pipe.name);
        file >> pipe.length;
        file >> pipe.diameter;
        file >> pipe.isUnderRepair;

        if (file.fail() || pipe.name.empty() || pipe.length <= 0 || pipe.diameter <= 0)
        {
            cout << "Данные трубы в файле повреждены.\n";
            return false;
        }
    }

    pipe.pipeExists = pipeFlag == 1;
    return true;
}

bool LoadStation(ifstream& file, CompressorStation& station)
{
    int stationFlag;

    file >> stationFlag;
    if (file.fail() || (stationFlag != 0 && stationFlag != 1))
    {
        cout << "Ошибка чтения data.txt.\n";
        return false;
    }
    file.ignore(10000, '\n');

    if (stationFlag == 1)
    {
        getline(file, station.name);
        file >> station.totalWorkshops;
        file >> station.workingWorkshops;
        file >> station.stationClass;

        if (file.fail() || station.name.empty() || station.totalWorkshops <= 0 || station.workingWorkshops < 0 || station.workingWorkshops > station.totalWorkshops || station.stationClass <= 0)
        {
            cout << "Данные станции в файле повреждены.\n";
            return false;
        }
    }

    station.stationExists = stationFlag == 1;
    return true;
}

// Все данные читаются из одного общего файла
void LoadData(Pipe& pipe, CompressorStation& station)
{
    ifstream file("data.txt");
    if (!file.is_open())
    {
        cout << "Файл data.txt не найден.\n";
        return;
    }

    Pipe loadedPipe;
    CompressorStation loadedStation;

    if (!LoadPipe(file, loadedPipe) || !LoadStation(file, loadedStation))
    {
        return;
    }

    file.close();
    pipe = loadedPipe;
    station = loadedStation;

    cout << "Данные загружены из data.txt.\n";
}

void PrintMenu()
{
    cout << "\n1. Добавить трубу\n";
    cout << "2. Добавить компрессорную станцию\n";
    cout << "3. Посмотреть объекты\n";
    cout << "4. Изменить состояние трубы\n";
    cout << "5. Запустить или остановить цех\n";
    cout << "6. Сохранить данные\n";
    cout << "7. Загрузить данные\n";
    cout << "0. Выход\n";
}

int main()
{
    Pipe pipe;
    CompressorStation station;

    while (true)
    {
        PrintMenu();
        cout << "Выберите действие: ";
        int action = ReadInt();

        while (action < 0 || action > 7)
        {
            cout << "Ошибка. Введите число от 0 до 7: ";
            action = ReadInt();
        }

        switch (action)
        {
            case 1:
                InputPipe(pipe);
                break;

            case 2:
                InputStation(station);
                break;

            case 3:
                PrintPipe(pipe);
                PrintStation(station);
                break;

            case 4:
                EditPipe(pipe);
                break;

            case 5:
                EditStation(station);
                break;

            case 6:
                SaveData(pipe, station);
                break;

            case 7:
                LoadData(pipe, station);
                break;

            case 0:
                cout << "Программа завершена.\n";
                return 0;
        }
    }
}
