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
};

// Данные компрессорной станции
struct CompressorStation
{
    string name;
    int totalWorkshops = 0;
    int workingWorkshops = 0;
    int stationClass = 0;
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

    cout << "Введите класс станции (1, 2 или 3): ";
    station.stationClass = ReadInt();
    while (station.stationClass < 1 || station.stationClass > 3)
    {
        cout << "Ошибка. Введите число от 1 до 3: ";
        station.stationClass = ReadInt();
    }
}

void PrintPipe(const Pipe& pipe)
{
    cout << "\nИнформация о трубе\n";
    cout << "Название: " << pipe.name << '\n';
    cout << "Длина: " << pipe.length << " км\n";
    cout << "Диаметр: " << pipe.diameter << " мм\n";
    cout << "Находится в ремонте: " << (pipe.isUnderRepair ? "да" : "нет") << '\n';
}

void PrintStation(const CompressorStation& station)
{
    cout << "\nИнформация о компрессорной станции\n";
    cout << "Название: " << station.name << '\n';
    cout << "Общее количество цехов: " << station.totalWorkshops << '\n';
    cout << "Работающих цехов: " << station.workingWorkshops << '\n';
    cout << "Класс станции: " << station.stationClass << '\n';
}

void EditPipe(Pipe& pipe)
{
    cout << "\n0. Убрать трубу из ремонта\n";
    cout << "1. Отправить трубу в ремонт\n";
    cout << "Выберите действие: ";
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

// Труба и станция записываются в один общий файл
void SaveData(const Pipe& pipe, bool pipeExists, const CompressorStation& station, bool stationExists)
{
    if (!pipeExists && !stationExists)
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

    file << pipeExists << '\n';
    if (pipeExists)
    {
        file << pipe.name << '\n';
        file << pipe.length << '\n';
        file << pipe.diameter << '\n';
        file << pipe.isUnderRepair << '\n';
    }

    file << stationExists << '\n';
    if (stationExists)
    {
        file << station.name << '\n';
        file << station.totalWorkshops << '\n';
        file << station.workingWorkshops << '\n';
        file << station.stationClass << '\n';
    }

    file.close();
    cout << "Данные сохранены в data.txt.\n";
}

// Все данные читаются из одного общего файла
void LoadData(Pipe& pipe, bool& pipeExists, CompressorStation& station, bool& stationExists)
{
    ifstream file("data.txt");
    if (!file.is_open())
    {
        cout << "Файл data.txt не найден.\n";
        return;
    }

    Pipe loadedPipe;
    CompressorStation loadedStation;
    int pipeFlag;
    int stationFlag;
    int repairValue;

    file >> pipeFlag;
    if (file.fail() || (pipeFlag != 0 && pipeFlag != 1))
    {
        cout << "Ошибка чтения data.txt.\n";
        return;
    }
    file.ignore(10000, '\n');

    if (pipeFlag == 1)
    {
        getline(file, loadedPipe.name);
        file >> loadedPipe.length;
        file >> loadedPipe.diameter;
        file >> repairValue;

        if (file.fail() || loadedPipe.name.empty() || loadedPipe.length <= 0 || loadedPipe.diameter <= 0 || (repairValue != 0 && repairValue != 1))
        {
            cout << "Данные трубы в файле повреждены.\n";
            return;
        }

        loadedPipe.isUnderRepair = repairValue == 1;
    }

    file >> stationFlag;
    if (file.fail() || (stationFlag != 0 && stationFlag != 1))
    {
        cout << "Ошибка чтения data.txt.\n";
        return;
    }
    file.ignore(10000, '\n');

    if (stationFlag == 1)
    {
        getline(file, loadedStation.name);
        file >> loadedStation.totalWorkshops;
        file >> loadedStation.workingWorkshops;
        file >> loadedStation.stationClass;

        if (file.fail() || loadedStation.name.empty() || loadedStation.totalWorkshops <= 0 || loadedStation.workingWorkshops < 0 || loadedStation.workingWorkshops > loadedStation.totalWorkshops || loadedStation.stationClass < 1 || loadedStation.stationClass > 3)
        {
            cout << "Данные станции в файле повреждены.\n";
            return;
        }
    }

    file.close();

    pipeExists = pipeFlag == 1;
    stationExists = stationFlag == 1;

    if (pipeExists)
    {
        pipe = loadedPipe;
    }

    if (stationExists)
    {
        station = loadedStation;
    }

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
    bool pipeExists = false;
    bool stationExists = false;

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
                pipeExists = true;
                break;

            case 2:
                InputStation(station);
                stationExists = true;
                break;

            case 3:
                if (pipeExists)
                {
                    PrintPipe(pipe);
                }
                else
                {
                    cout << "Труба не добавлена.\n";
                }

                if (stationExists)
                {
                    PrintStation(station);
                }
                else
                {
                    cout << "Компрессорная станция не добавлена.\n";
                }
                break;

            case 4:
                if (pipeExists)
                {
                    EditPipe(pipe);
                }
                else
                {
                    cout << "Сначала добавьте трубу.\n";
                }
                break;

            case 5:
                if (stationExists)
                {
                    EditStation(station);
                }
                else
                {
                    cout << "Сначала добавьте компрессорную станцию.\n";
                }
                break;

            case 6:
                SaveData(pipe, pipeExists, station, stationExists);
                break;

            case 7:
                LoadData(pipe, pipeExists, station, stationExists);
                break;

            case 0:
                cout << "Программа завершена.\n";
                return 0;
        }
    }
}
