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
    CompressorStation(int id = 0);

    int GetId() const;
    const std::string& GetName() const;
    double GetIdlePercent() const;

    void Input();
    void Print() const;
    void EditWorkshop();
    void Save(std::ofstream& file) const;
    bool Load(std::ifstream& file);
};

#endif
