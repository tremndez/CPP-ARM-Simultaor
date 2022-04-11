#include <iostream>
#include <fstream>
#include <string>

int main()
{
  std::ifstream inputFile("sample.txt");
  std::string line;
  std::getline(inputFile, line);

  std::cout << line << std::endl;
}