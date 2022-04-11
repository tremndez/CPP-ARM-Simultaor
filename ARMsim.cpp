/*  On  my  honor,  I  have  neither  given  nor  received
unauthorized aid on this assignment */

#include <iostream>
#include <fstream>
#include <string>

struct ARMsim
{
  ARMsim();
  ~ARMsim();
  void Disassembly(std::string &, int &, unsigned int &, int &); // CBZ / CBNZ Disassembler
  void CBZ(std::string &);
  void twosComplement(std::string &);

  void UpdateAddress();

private:
  int address = 0;
  std::ofstream disassembler;
};

int main(int argc, char *argv[])
{
  ARMsim obj;
  std::string line;
  std::ifstream inputFile(argv[1]);
  while (std::getline(inputFile, line))
  {
    std::string insString = "";
    for (int i = 0; i < 3; i++)
      insString += line.at(i);

    int instruc = std::stoi(insString);

    switch (instruc)
    {
    case 1:
      obj.CBZ(line);
      break;
    default:
      break;
    }
    obj.UpdateAddress();
  }
  return 0;
}

ARMsim::ARMsim()
{
  disassembler.open("disassembler.txt");
}

ARMsim::~ARMsim()
{
  disassembler.close();
}

void ARMsim::CBZ(std::string &cmd)
{
  std::string opcodeString = "";
  for (int i = 0; i < 5; i++)
    opcodeString += cmd.at(i + 3);
  int opcode = std::stoi(opcodeString);

  std::string srcString = "";
  for (int i = 0; i < 5; i++)
    srcString += cmd.at(i + 8);

  unsigned int src = std::stoi(srcString, nullptr, 2);

  std::string offsetString = cmd.substr(13, 19);
  int offset;
  if (offsetString.at(0) == 1)
  {
    twosComplement(offsetString);
    offset = std::stoi(offsetString, nullptr, 2);
    offset *= -1;
  }
  else
    offset = std::stoi(offsetString, nullptr, 2);

  Disassembly(cmd, opcode, src, offset);
}

void ARMsim::Disassembly(std::string &cmd, int &opcode, unsigned int &src, int &offset)
{
  disassembler << cmd << "\t" << 64 + address * 4 << "\t";
  if (opcode == 10000)
    disassembler << "CBZ ";
  else
    disassembler << "CBNZ ";
  disassembler << 'X' << src << ", #" << offset << "\n";
}

void ARMsim::UpdateAddress()
{
  address++;
}

void ARMsim::twosComplement(std::string &number)
{
  int i;
  for (i = number.length() - 1; i >= 0; i--)
    if (number[i] == '1')
      break;

  if (i == 0)
  {
    number += '1';
    twosComplement(number);
  }

  // Continue traversal after the position of
  // first '1'
  for (int j = i - 1; j >= 0; j--)
  {
    if (number[j] == '1')
      number[j] = '0';
    else
      number[j] = '1';
  }
  number.pop_back();
}