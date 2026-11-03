#ifndef COMPRESSOR_STATION_H
#define COMPRESSOR_STATION_H

#include <fstream>
#include <string>

class CompressorStation
{
private:
    int id_;
    std::string name_;
    int totalWorkshops_;
    int workingWorkshops_;
    int stationClass_;

public:
    explicit CompressorStation(int id = 0);

    int GetId() const;
    const std::string& GetName() const;
    double GetIdlePercent() const;

    void Input();
    void Print() const;
    void EditWorkshop();
    friend std::ofstream& operator<<(std::ofstream& file, const CompressorStation& station);
    friend std::ifstream& operator>>(std::ifstream& file, CompressorStation& station);
};

#endif
