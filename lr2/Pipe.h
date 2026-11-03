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

    friend std::istream& operator>>(std::istream& input, Pipe& pipe);
    friend std::ostream& operator<<(std::ostream& output, const Pipe& pipe);
    friend std::ofstream& operator<<(std::ofstream& file, const Pipe& pipe);
    friend std::ifstream& operator>>(std::ifstream& file, Pipe& pipe);
};

#endif
