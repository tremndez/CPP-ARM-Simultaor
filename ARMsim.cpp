/*  On  my  honor,  I  have  neither  given  nor  received
unauthorized aid on this assignment */

#include <iostream>
#include <fstream>
#include <string>

struct ARMsim
{
  ARMsim();
  ~ARMsim();

  void Cat1Disassembly(std::string &, int &, unsigned int &, int &);                 // Category 1 Disassembler
  void Cat2Disassembly(std::string &, int &, unsigned int &, unsigned int &, int &); // Category 1 Disassembler
  void Cat3Disassembly(std::string &, int &, unsigned int &, unsigned int &, unsigned int &);
  void Cat4Disassembly(std::string &, int &, unsigned int &, unsigned int &, int &);
  void DummyDisassembly(std::string &, std::ifstream &);
  void twosComplement(std::string &);

  void Cat1(std::string &); // CBZ/CBNZ
  void Cat2(std::string &); // Instance orientated
  void Cat3(std::string &);
  void Cat4(std::string &);

  void UpdateAddress();

private:
  int address = 64;
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

    int instruct = std::stoi(insString);

    if (instruct == 1)
      obj.Cat1(line);
    else if (instruct == 10)
      obj.Cat2(line);
    else if (instruct == 11)
      obj.Cat3(line);
    else if (instruct == 100)
      obj.Cat4(line);
    else
      obj.DummyDisassembly(line, inputFile);
    obj.UpdateAddress();
  }
  return 0;
}

ARMsim::ARMsim()
{
  disassembler.open("disassembly.txt");
}

ARMsim::~ARMsim()
{
  disassembler.close();
}

void ARMsim::Cat1(std::string &cmd)
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
  if (offsetString.at(0) == '1')
  {
    twosComplement(offsetString);

    offset = std::stoi(offsetString, nullptr, 2) * -1;
  }
  else
    offset = std::stoi(offsetString, nullptr, 2);

  Cat1Disassembly(cmd, opcode, src, offset);
}

void ARMsim::Cat1Disassembly(std::string &cmd, int &opcode, unsigned int &src, int &offset)
{
  disassembler << cmd << "\t" << address << "\t";
  if (opcode == 10000)
    disassembler << "CBZ ";
  else
    disassembler << "CBNZ ";
  if (src == 31)
    disassembler << "XZR, #" << offset << "\n";
  else
    disassembler << 'X' << src << ", #" << offset << "\n";
}

void ARMsim::UpdateAddress()
{
  address += 4;
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
    number.pop_back();
  }

  for (int j = i - 1; j >= 0; j--)
  {
    if (number[j] == '1')
      number[j] = '0';
    else
      number[j] = '1';
  }
}

void ARMsim::Cat2(std::string &cmd)
{
  std::string opcodeString = "";
  for (int i = 0; i < 7; i++)
    opcodeString += cmd.at(i + 3);
  int opcode = std::stoi(opcodeString);
  std::string destString = "";
  for (int i = 0; i < 5; i++)
    destString += cmd.at(i + 10);
  unsigned int dest = std::stoi(destString, nullptr, 2);

  std::string srcString = "";
  for (int i = 0; i < 5; i++)
    srcString += cmd.at(i + 15);
  unsigned int src = std::stoi(srcString, nullptr, 2);

  std::string immediateValString = cmd.substr(20, 31);
  int immediateVal;
  if (immediateValString.at(0) == '1')
  {
    twosComplement(immediateValString);

    immediateVal = std::stoi(immediateValString, nullptr, 2) * -1;
  }
  else
    immediateVal = std::stoi(immediateValString, nullptr, 2);

  Cat2Disassembly(cmd, opcode, dest, src, immediateVal);
}

void ARMsim::Cat2Disassembly(std::string &cmd, int &opcode, unsigned int &dest, unsigned int &src, int &immedValue)
{
  disassembler << cmd << "\t" << address << "\t";
  if (opcode == 1000000)
    disassembler << "ORRI";
  else if (opcode == 1000001)
    disassembler << "EORI";
  else if (opcode == 1000010)
    disassembler << "ADDI";
  else if (opcode == 1000011)
    disassembler << "SUBI";
  else
    disassembler << "ANDI";

  if (dest == 31 && src == 31)
    disassembler << " XZR, XZR, #" << immedValue << "\n";
  else if (src == 31)
    disassembler << " X" << dest << ", XZR, #" << immedValue << "\n";
  else if (dest == 31)
    disassembler << " XZR, X" << src << ", #" << immedValue << "\n";
  else
    disassembler << " X" << dest << ", X" << src << ", #" << immedValue << "\n";
}

void ARMsim::Cat3(std::string &cmd)
{
  std::string opcodeString = "";
  for (int i = 0; i < 8; i++)
    opcodeString += cmd.at(i + 3);
  int opcode = std::stoi(opcodeString);

  std::string destString = "";
  for (int i = 0; i < 5; i++)
    destString += cmd.at(i + 11);
  unsigned int dest = std::stoi(destString, nullptr, 2);

  std::string src1String = "";
  for (int i = 0; i < 5; i++)
    src1String += cmd.at(i + 16);
  unsigned int src1 = std::stoi(src1String, nullptr, 2);

  std::string src2String = "";
  for (int i = 0; i < 5; i++)
    src2String += cmd.at(i + 21);
  unsigned int src2 = std::stoi(src2String, nullptr, 2);
  Cat3Disassembly(cmd, opcode, dest, src1, src2);
}

void ARMsim::Cat3Disassembly(std::string &cmd, int &opcode, unsigned int &dest, unsigned int &src1, unsigned int &src2)
{
  disassembler << cmd << "\t" << address << "\t";
  if (opcode == 10100000)
    disassembler << "EOR";
  else if (opcode == 10100010)
    disassembler << "ADD";
  else if (opcode == 10100011)
    disassembler << "SUB";
  else if (opcode == 10100100)
    disassembler << "AND";
  else if (opcode == 10100101)
    disassembler << "ORR";
  else if (opcode == 10100110)
    disassembler << "LSR";
  else
    disassembler << "LSL";

  if (dest == src1 == src2 == 31)
    disassembler << " XZR, XZR, XZR\n";
  else if (dest == src1 == 31)
    disassembler << " XZR, XZR, X" << src2 << "\n";
  else if (dest == src2 == 31)
    disassembler << " XZR, X" << src1 << ", XZR\n";
  else if (dest == 31)
    disassembler << " XZR, X" << src1 << ", X" << src2 << "\n";
  else if (src1 == src2 == 31)
    disassembler << " X" << dest << ", XZR, XZR\n";
  else if (src1 == 31)
    disassembler << " X" << dest << ", XZR, X" << src2 << "\n";
  else if (src2 == 31)
    disassembler << " X" << dest << ", X" << src1 << ", XZR\n";
  else
    disassembler << " X" << dest << ", X" << src1 << ", X" << src2 << "\n";
}

void ARMsim::Cat4(std::string &cmd)
{
  std::string opcodeString = "";
  for (int i = 0; i < 8; i++)
    opcodeString += cmd.at(i + 3);
  int opcode = std::stoi(opcodeString);

  std::string srcDestString = "";
  for (int i = 0; i < 5; i++)
    srcDestString += cmd.at(i + 11);
  unsigned int srcDest = std::stoi(srcDestString, nullptr, 2);

  std::string srcString = "";
  for (int i = 0; i < 5; i++)
    srcString += cmd.at(i + 16);
  unsigned int src = std::stoi(srcString, nullptr, 2);

  std::string immediateValString = cmd.substr(21, 31);
  int immediateVal;
  if (immediateValString.at(0) == '1')
  {
    twosComplement(immediateValString);

    immediateVal = std::stoi(immediateValString, nullptr, 2) * -1;
  }
  else
    immediateVal = std::stoi(immediateValString, nullptr, 2);

  Cat4Disassembly(cmd, opcode, srcDest, src, immediateVal);
}

void ARMsim::Cat4Disassembly(std::string &cmd, int &opcode, unsigned int &srcdest, unsigned int &src, int &immedValue)
{
  disassembler << cmd << "\t" << address << "\t";
  if (opcode == 10101010)
    disassembler << "LDUR";
  else
    disassembler << "STUR";

  if (srcdest == 31 && src == 31)
    disassembler << " XZR, [XZR, #" << immedValue << "]\n";
  else if (src == 31)
    disassembler << " X" << srcdest << ", [XZR, #" << immedValue << "]\n";
  else if (srcdest == 31)
    disassembler << " XZR, [X" << src << ", #" << immedValue << "]\n";
  else
    disassembler << " X" << srcdest << ", [X" << src << ", #" << immedValue << "]\n";
}

void ARMsim::DummyDisassembly(std::string &cmd, std::ifstream &inputFile)
{
  disassembler << cmd << "\t" << address << "\tDUMMY\n";
  UpdateAddress();
  std::string line;
  int num;
  while (std::getline(inputFile, line))
  {
    std::string temp = line;
    if (temp.at(0) == '1')
    {
      twosComplement(temp);

      num = std::stoi(temp, nullptr, 2) * -1;
    }
    else
      num = std::stoi(temp, nullptr, 2);

    disassembler << line << "\t" << address << "\t" << num << "\n";
    UpdateAddress();
  }
}