#include <iostream>
#include <iomanip>
#include "md5.hpp"
#include <fstream>

int main(int argc, char *argv[])
{
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

  // *************************************************************************/
  // ****************************    STEP 1    *******************************/
  // *************************************************************************/

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

  // *************************************************************************/
  // ****************************    STEP 2    *******************************/
  // *************************************************************************/

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

  std::ofstream outFile("ThruStep2.txt");

  //for (int i = 0; i < (unsigned long long)(size); i++) {
  for (int i = 0; i < (unsigned long long)(size) + additionalPaddingLength + 8; i++) {
    outFile << paddedBlock[i];
  }

  // *************************************************************************/
  // ****************************    STEP 3    *******************************/
  // *************************************************************************/

  WORD A;
  WORD B;
  WORD C;
  WORD D;

  A.setLowOrderFirst(1, 35, 69, 103);
  B.setLowOrderFirst(137, 171, 205, 239);
  C.setLowOrderFirst(254, 220, 186, 152);
  D.setLowOrderFirst(118, 84, 50, 16);

  // *************************************************************************/
  // ****************************    STEP 4    *******************************/
  // *************************************************************************/

  


  delete[] memoryBlock;
  delete[] paddedBlock;

  return 0;
}
