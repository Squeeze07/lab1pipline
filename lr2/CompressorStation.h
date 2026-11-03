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

    friend std::istream& operator>>(std::istream& input, CompressorStation& station);
    friend std::ostream& operator<<(std::ostream& output, const CompressorStation& station);
    void EditWorkshop();
    friend std::ofstream& operator<<(std::ofstream& file, const CompressorStation& station);
    friend std::ifstream& operator>>(std::ifstream& file, CompressorStation& station);
};

#endif
