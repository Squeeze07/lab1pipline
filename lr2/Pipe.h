#ifndef PIPE_H
#define PIPE_H

#include <fstream>
#include <string>

class Pipe
{
private:
    int id_;
    std::string name_;
    double length_;
    int diameter_;
    bool underRepair_;

public:
    Pipe(int id = 0);

    int GetId() const;
    const std::string& GetName() const;
    bool IsUnderRepair() const;
    void SetRepair(bool value);

    void Input();
    void Print() const;
    void Save(std::ofstream& file) const;
    bool Load(std::ifstream& file);
};

#endif
