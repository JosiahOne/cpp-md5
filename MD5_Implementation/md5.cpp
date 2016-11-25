#include <iostream>
#include <iomanip>
#include "md5.hpp"
#include <fstream>

class WORD {
public:

  WORD() : highByte(0), midHighByte(0), midLowByte(0), lowByte(0) {}

  unsigned int asInt() {
    int result = 0;

    if (sizeof(unsigned int) == 4) {
      *((unsigned char *)&result + 0) = lowByte;
      *((unsigned char *)&result + 1) = midLowByte;
      *((unsigned char *)&result + 2) = midHighByte;
      *((unsigned char *)&result + 3) = highByte;

      return result;
    } else {
      std::cerr << "Int not 4 bytes!!!" << std::endl;
      exit(1);
    }
  }

  void print() {
    std::cout << (int)(highByte) << (int)(midHighByte) << (int)(midLowByte) << (int)(lowByte);
  }

public:
  char highByte;
  char midHighByte;
  char midLowByte;
  char lowByte;
};

int main(int argc, char *argv[])
{
  // Implement me.

  if (argc <= 1) {
    exit(1);
  }

  // Read file data

  std::streampos size;
  char *memoryBlock;
  char *paddedBlock;

  std::ifstream inFile(std::string(argv[1]), std::ios::in | std::ios::binary | std::ios::ate);
  if (inFile.is_open()) {
   size = inFile.tellg();
   memoryBlock = new char[size];
   inFile.seekg(0, std::ios::beg);
   inFile.read(memoryBlock, size);
   inFile.close();
  }

  // Step 1. Add Padding to bytelength = 56 mod 64

  size_t additionalPaddingLength = 56 - (size % 64);

  if (additionalPaddingLength == 0) {
   additionalPaddingLength = 64;
  }

  paddedBlock = new char[(unsigned long long)(size) + additionalPaddingLength + 8];

  // Copy original to padded block
  for (unsigned long long i = 0; i < size; i++) {
    paddedBlock[i] = memoryBlock[i];
  }

  // Copy Padding

  for (size_t i = 0; i < additionalPaddingLength; i++) {
    if (i == 0) {
      paddedBlock[(unsigned long long)(size) + i] = 128;
    } else {
      paddedBlock[(unsigned long long)(size) + i] = 0;
    }
  }

  // Append Length
  unsigned long long longSize = (unsigned long long)(size);
  std::cout << longSize << std::endl;

  // Low order word.
  unsigned char lowByte = *((unsigned char *)&longSize + 0);
  unsigned char midLowByte = *((unsigned char *)&longSize + 1);
  unsigned char midHighByte = *((unsigned char *)&longSize + 2);
  unsigned char highByte = *((unsigned char *)&longSize + 3);

  WORD lowWord;
  lowWord.lowByte = lowByte;
  lowWord.midLowByte = midLowByte;
  lowWord.midHighByte = midHighByte;
  lowWord.highByte = highByte;

  // High order word.
  lowByte = *((unsigned char *)&longSize + 4);
  midLowByte = *((unsigned char *)&longSize + 5);
  midHighByte = *((unsigned char *)&longSize + 6);
  highByte = *((unsigned char *)&longSize + 7);

  WORD highWord;
  highWord.lowByte = lowByte;
  highWord.midLowByte = midLowByte;
  highWord.midHighByte = midHighByte;
  highWord.highByte = highByte;

  // Write length
  // Low Byte
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 0] = lowWord.highByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 1] = lowWord.midHighByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 2] = lowWord.midLowByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 3] = lowWord.lowByte;

  // High Byte
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 4] = highWord.highByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 5] = highWord.midHighByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 6] = highWord.midLowByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 7] = highWord.lowByte;


  delete[] memoryBlock;
  delete[] paddedBlock;

  return 0;
}
