#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

int ReadInt(std::istream& input = std::cin);
double ReadDouble(std::istream& input = std::cin);
std::string ReadText(std::istream& input = std::cin);
void LogAction(const std::string& message);

#endif
