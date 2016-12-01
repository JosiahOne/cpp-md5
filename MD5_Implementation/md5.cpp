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
  unsigned long long size;
  char *memoryBlock;
  char *paddedBlock;

  memoryBlock = argv[1];
  size = strlen(memoryBlock);

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
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 0] = lowWord.lowByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 1] = lowWord.midLowByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 2] = lowWord.midHighByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 3] = lowWord.highByte;

  // High Byte
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 4] = highWord.lowByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 5] = highWord.midLowByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 6] = highWord.midHighByte;
  paddedBlock[(unsigned long long)(size) + additionalPaddingLength + 7] = highWord.highByte;

  std::ofstream outFile("ThruStep2.txt");

  //for (int i = 0; i < (unsigned long long)(size); i++) {
  for (int i = 0; i < (unsigned long long)(size) + additionalPaddingLength + 8; i++) {
    outFile << paddedBlock[i];
  }

  // *************************************************************************/
  // ****************************    STEP 3    *******************************/
  // *************************************************************************/

  WORD initialA;
  WORD initialB;
  WORD initialC;
  WORD initialD;

  initialA.setLowOrderFirst(1, 35, 69, 103);
  initialB.setLowOrderFirst(137, 171, 205, 239);
  initialC.setLowOrderFirst(254, 220, 186, 152);
  initialD.setLowOrderFirst(118, 84, 50, 16);

  unsigned int A = initialA.asInt();
  unsigned int B = initialB.asInt();
  unsigned int C = initialC.asInt();
  unsigned int D = initialD.asInt();

  // *************************************************************************/
  // ****************************    STEP 4    *******************************/
  // *************************************************************************/

  for (int i = 0; i < (unsigned long long)(size) + additionalPaddingLength + 8; i++) {
    BLOCK X;

    for (int j = 0; j < 16; j++) {
      // Get WORD
      WORD word;
      word.lowByte = paddedBlock[i++];
      word.midLowByte = paddedBlock[i++];
      word.midHighByte = paddedBlock[i++];
      word.highByte = paddedBlock[i++];

      X.insert(word);
    }

    unsigned int AA = A;
    unsigned int BB = B;
    unsigned int CC = C;
    unsigned int DD = D;

    // Round 1

    MD5::Round1(A, B, C, D, 0, 7, 1, X);
    MD5::Round1(D, A, B, C, 1, 12, 2, X);
    MD5::Round1(C, D, A, B, 2, 17, 3, X);
    MD5::Round1(B, C, D, A, 3, 22, 4, X);

    MD5::Round1(A, B, C, D, 4, 7, 5, X);
    MD5::Round1(D, A, B, C, 5, 12, 6, X);
    MD5::Round1(C, D, A, B, 6, 17, 7, X);
    MD5::Round1(B, C, D, A, 7, 22, 8, X);

    MD5::Round1(A, B, C, D, 8, 7, 9, X);
    MD5::Round1(D, A, B, C, 9, 12, 10, X);
    MD5::Round1(C, D, A, B, 10, 17, 11, X);
    MD5::Round1(B, C, D, A, 11, 22, 12, X);

    MD5::Round1(A, B, C, D, 12, 7, 13, X);
    MD5::Round1(D, A, B, C, 13, 12, 14, X);
    MD5::Round1(C, D, A, B, 14, 17, 15, X);
    MD5::Round1(B, C, D, A, 15, 22, 16, X);

    // Round 2

    MD5::Round2(A, B, C, D, 1, 5, 17, X);
    MD5::Round2(D, A, B, C, 6, 9, 18, X);
    MD5::Round2(C, D, A, B, 11, 14, 19, X);
    MD5::Round2(B, C, D, A, 0, 20, 20, X);

    MD5::Round2(A, B, C, D, 5, 5, 21, X);
    MD5::Round2(D, A, B, C, 10, 9, 22, X);
    MD5::Round2(C, D, A, B, 15, 14, 23, X);
    MD5::Round2(B, C, D, A, 4, 20, 24, X);

    MD5::Round2(A, B, C, D, 9, 5, 25, X);
    MD5::Round2(D, A, B, C, 14, 9, 26, X);
    MD5::Round2(C, D, A, B, 3, 14, 27, X);
    MD5::Round2(B, C, D, A, 8, 20, 28, X);

    MD5::Round2(A, B, C, D, 13, 5, 29, X);
    MD5::Round2(D, A, B, C, 2, 9, 30, X);
    MD5::Round2(C, D, A, B, 7, 14, 31, X);
    MD5::Round2(B, C, D, A, 12, 20, 32, X);

    // Round 3

    MD5::Round3(A, B, C, D, 5, 4, 33, X);
    MD5::Round3(D, A, B, C, 8, 11, 34, X);
    MD5::Round3(C, D, A, B, 11, 16, 35, X);
    MD5::Round3(B, C, D, A, 14, 23, 36, X);

    MD5::Round3(A, B, C, D, 1, 4, 37, X);
    MD5::Round3(D, A, B, C, 4, 11, 38, X);
    MD5::Round3(C, D, A, B, 7, 16, 39, X);
    MD5::Round3(B, C, D, A, 10, 23, 40, X);

    MD5::Round3(A, B, C, D, 13, 4, 41, X);
    MD5::Round3(D, A, B, C, 0, 11, 42, X);
    MD5::Round3(C, D, A, B, 3, 16, 43, X);
    MD5::Round3(B, C, D, A, 6, 23, 44, X);

    MD5::Round3(A, B, C, D, 9, 4, 45, X);
    MD5::Round3(D, A, B, C, 12, 11, 46, X);
    MD5::Round3(C, D, A, B, 15, 16, 47, X);
    MD5::Round3(B, C, D, A, 2, 23, 48, X);

    // Round 4

    MD5::Round4(A, B, C, D, 0, 6, 49, X);
    MD5::Round4(D, A, B, C, 7, 10, 50, X);
    MD5::Round4(C, D, A, B, 14, 15, 51, X);
    MD5::Round4(B, C, D, A, 5, 21, 52, X);

    MD5::Round4(A, B, C, D, 12, 6, 53, X);
    MD5::Round4(D, A, B, C, 3, 10, 54, X);
    MD5::Round4(C, D, A, B, 10, 15, 55, X);
    MD5::Round4(B, C, D, A, 1, 21, 56, X);

    MD5::Round4(A, B, C, D, 8, 6, 57, X);
    MD5::Round4(D, A, B, C, 15, 10, 58, X);
    MD5::Round4(C, D, A, B, 6, 15, 59, X);
    MD5::Round4(B, C, D, A, 13, 21, 60, X);

    MD5::Round4(A, B, C, D, 4, 6, 61, X);
    MD5::Round4(D, A, B, C, 11, 10, 62, X);
    MD5::Round4(C, D, A, B, 2, 15, 63, X);
    MD5::Round4(B, C, D, A, 9, 21, 64, X);

    A = A + AA;
    B = B + BB;
    C = C + CC;
    D = D + DD;
  }

  // *************************************************************************/
  // ****************************    STEP 5    *******************************/
  // *************************************************************************/

  char lowAByte = *((unsigned char *)&A + 0);
  char midLowAByte = *((unsigned char *)&A + 1);
  char midHighAByte = *((unsigned char *)&A + 2);
  char highAByte = *((unsigned char *)&A + 3);

  std::cout << (unsigned int)(lowAByte) << (unsigned int)(midLowAByte) << (unsigned int)(midHighAByte) << (unsigned int)(highAByte);

  char lowBByte = *((unsigned char *)&B + 0);
  char midLowBByte = *((unsigned char *)&B + 1);
  char midHighBByte = *((unsigned char *)&B + 2);
  char highBByte = *((unsigned char *)&B + 3);

  std::cout << (unsigned int)(lowBByte) << (unsigned int)(midLowBByte) << (unsigned int)(midHighBByte) << (unsigned int)(highBByte);

  char lowCByte = *((unsigned char *)&C + 0);
  char midLowCByte = *((unsigned char *)&C + 1);
  char midHighCByte = *((unsigned char *)&C + 2);
  char highCByte = *((unsigned char *)&C + 3);

  std::cout << (unsigned int)(lowCByte) << (unsigned int)(midLowCByte) << (unsigned int)(midHighCByte) << (unsigned int)(highCByte);

  char lowDByte = *((unsigned char *)&D + 0);
  char midLowDByte = *((unsigned char *)&D + 1);
  char midHighDByte = *((unsigned char *)&D + 2);
  char highDByte = *((unsigned char *)&D + 3);

  std::cout << (unsigned int)(lowDByte) << (unsigned int)(midLowDByte) << (unsigned int)(midHighDByte) << (unsigned int)(highDByte);


  delete[] memoryBlock;
  delete[] paddedBlock;

  return 0;
}
