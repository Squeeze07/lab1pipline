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
    explicit Pipe(int id = 0);

    int GetId() const;
    const std::string& GetName() const;
    bool IsUnderRepair() const;
    void SetRepair(bool value);

    void Input();
    void Print() const;
    friend std::ofstream& operator<<(std::ofstream& file, const Pipe& pipe);
    friend std::ifstream& operator>>(std::ifstream& file, Pipe& pipe);
};

#endif
