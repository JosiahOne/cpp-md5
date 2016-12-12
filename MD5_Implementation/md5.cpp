#include <iostream>
#include <iomanip>
#include "md5.hpp"
#include <fstream>

int main(int argc, char *argv[])
{
  bool noInput = false;

  if (argc <= 1) {
    noInput = true;
  }

  // Read file data
  unsigned long long size;
  const char *memoryBlock;
  char *paddedBlock;

  if (!noInput) {
    memoryBlock = argv[1];
  } else {
    memoryBlock = std::string("").c_str();
  }
  size = strlen(memoryBlock);

  // *************************************************************************/
  // ****************************    STEP 1    *******************************/
  // *************************************************************************/

  // Step 1. Add Padding so bytelength = 56 mod 64

  unsigned int additionalPaddingLength = 0;

  while (((additionalPaddingLength + size) % 64) != 56) {
    additionalPaddingLength++;
  }
  if (additionalPaddingLength == 0) {
    additionalPaddingLength = 64;
  }

  if (((additionalPaddingLength + size) % 64) != 56) {
    std::cerr << "INVALID";
    exit(1);
  }

  paddedBlock = new char[(unsigned long long)(size) + additionalPaddingLength + 8];

  // Copy original to padded block
  for (unsigned long long i = 0; i < size; i++) {
    paddedBlock[i] = memoryBlock[i];
  }

  // Copy Padding
  for (unsigned int i = 0; i < additionalPaddingLength; i++) {
    if (i == 0) {
      paddedBlock[(unsigned long long)(size) + (unsigned long long)(i)] = (unsigned char)(0x80);
    } else {
      paddedBlock[(unsigned long long)(size) + (unsigned long long)(i)] = 0;
    }
  }

  // *************************************************************************/
  // ****************************    STEP 2    *******************************/
  // *************************************************************************/

  // Append Length
  unsigned long long longSize = 8 * (unsigned long long)(size);

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

  unsigned int A = initialA.asInt(); //VERIFIED
  unsigned int B = initialB.asInt(); //VERIFIED
  unsigned int C = initialC.asInt(); //VERIFIED
  unsigned int D = initialD.asInt(); //VERIFIED

  // *************************************************************************/
  // ****************************    STEP 4    *******************************/
  // *************************************************************************/

  for (int i = 0; i < (unsigned long long)(size) + additionalPaddingLength + 8;) {
    BLOCK X;

    for (int j = 0; j < 16; j++) {
      // Get WORD
      WORD word;
      word.lowByte = (unsigned char)(paddedBlock[i++]);
      word.midLowByte = (unsigned char)(paddedBlock[i++]);
      word.midHighByte = (unsigned char)(paddedBlock[i++]);
      word.highByte = (unsigned char)(paddedBlock[i++]);

      X.insert(word);
    }

    unsigned int AA = A;
    unsigned int BB = B;
    unsigned int CC = C;
    unsigned int DD = D;

    // Round 1
    MD5::Round1(A, B, C, D, 0, 7, 0, X);
    MD5::Round1(D, A, B, C, 1, 12, 1, X);
    MD5::Round1(C, D, A, B, 2, 17, 2, X);
    MD5::Round1(B, C, D, A, 3, 22, 3, X);

    MD5::Round1(A, B, C, D, 4, 7, 4, X);
    MD5::Round1(D, A, B, C, 5, 12, 5, X);
    MD5::Round1(C, D, A, B, 6, 17, 6, X);
    MD5::Round1(B, C, D, A, 7, 22, 7, X);

    MD5::Round1(A, B, C, D, 8, 7, 8, X);
    MD5::Round1(D, A, B, C, 9, 12, 9, X);
    MD5::Round1(C, D, A, B, 10, 17, 10, X);
    MD5::Round1(B, C, D, A, 11, 22, 11, X);

    MD5::Round1(A, B, C, D, 12, 7, 12, X);
    MD5::Round1(D, A, B, C, 13, 12, 13, X);
    MD5::Round1(C, D, A, B, 14, 17, 14, X);
    MD5::Round1(B, C, D, A, 15, 22, 15, X);

    // Round 2

    MD5::Round2(A, B, C, D, 1, 5, 16, X);
    MD5::Round2(D, A, B, C, 6, 9, 17, X);
    MD5::Round2(C, D, A, B, 11, 14, 18, X);
    MD5::Round2(B, C, D, A, 0, 20, 19, X);

    MD5::Round2(A, B, C, D, 5, 5, 20, X);
    MD5::Round2(D, A, B, C, 10, 9, 21, X);
    MD5::Round2(C, D, A, B, 15, 14, 22, X);
    MD5::Round2(B, C, D, A, 4, 20, 23, X);

    MD5::Round2(A, B, C, D, 9, 5, 24, X);
    MD5::Round2(D, A, B, C, 14, 9, 25, X);
    MD5::Round2(C, D, A, B, 3, 14, 26, X);
    MD5::Round2(B, C, D, A, 8, 20, 27, X);

    MD5::Round2(A, B, C, D, 13, 5, 28, X);
    MD5::Round2(D, A, B, C, 2, 9, 29, X);
    MD5::Round2(C, D, A, B, 7, 14, 30, X);
    MD5::Round2(B, C, D, A, 12, 20, 31, X);

    // Round 3

    MD5::Round3(A, B, C, D, 5, 4, 32, X);
    MD5::Round3(D, A, B, C, 8, 11, 33, X);
    MD5::Round3(C, D, A, B, 11, 16, 34, X);
    MD5::Round3(B, C, D, A, 14, 23, 35, X);

    MD5::Round3(A, B, C, D, 1, 4, 36, X);
    MD5::Round3(D, A, B, C, 4, 11, 37, X);
    MD5::Round3(C, D, A, B, 7, 16, 38, X);
    MD5::Round3(B, C, D, A, 10, 23, 39, X);

    MD5::Round3(A, B, C, D, 13, 4, 40, X);
    MD5::Round3(D, A, B, C, 0, 11, 41, X);
    MD5::Round3(C, D, A, B, 3, 16, 42, X);
    MD5::Round3(B, C, D, A, 6, 23, 43, X);

    MD5::Round3(A, B, C, D, 9, 4, 44, X);
    MD5::Round3(D, A, B, C, 12, 11, 45, X);
    MD5::Round3(C, D, A, B, 15, 16, 46, X);
    MD5::Round3(B, C, D, A, 2, 23, 47, X);

    // Round 4

    MD5::Round4(A, B, C, D, 0, 6, 48, X);
    MD5::Round4(D, A, B, C, 7, 10, 49, X);
    MD5::Round4(C, D, A, B, 14, 15, 50, X);
    MD5::Round4(B, C, D, A, 5, 21, 51, X);

    MD5::Round4(A, B, C, D, 12, 6, 52, X);
    MD5::Round4(D, A, B, C, 3, 10, 53, X);
    MD5::Round4(C, D, A, B, 10, 15, 54, X);
    MD5::Round4(B, C, D, A, 1, 21, 55, X);

    MD5::Round4(A, B, C, D, 8, 6, 56, X);
    MD5::Round4(D, A, B, C, 15, 10, 57, X);
    MD5::Round4(C, D, A, B, 6, 15, 58, X);
    MD5::Round4(B, C, D, A, 13, 21, 59, X);

    MD5::Round4(A, B, C, D, 4, 6, 60, X);
    MD5::Round4(D, A, B, C, 11, 10, 61, X);
    MD5::Round4(C, D, A, B, 2, 15, 62, X);
    MD5::Round4(B, C, D, A, 9, 21, 63, X);

    A = A + AA;
    B = B + BB;
    C = C + CC;
    D = D + DD;
  }

  // *************************************************************************/
  // ****************************    STEP 5    *******************************/
  // *************************************************************************/

  unsigned char lowAByte = *((unsigned char *)&A + 0);
  unsigned char midLowAByte = *((unsigned char *)&A + 1);
  unsigned char midHighAByte = *((unsigned char *)&A + 2);
  unsigned char highAByte = *((unsigned char *)&A + 3);

  printf("%2.2x%2.2x%2.2x%2.2x", lowAByte, midLowAByte, midHighAByte, highAByte);

  unsigned char lowBByte = *((unsigned char *)&B + 0);
  unsigned char midLowBByte = *((unsigned char *)&B + 1);
  unsigned char midHighBByte = *((unsigned char *)&B + 2);
  unsigned char highBByte = *((unsigned char *)&B + 3);

  printf("%2.2x%2.2x%2.2x%2.2x", lowBByte, midLowBByte, midHighBByte, highBByte);

  unsigned char lowCByte = *((unsigned char *)&C + 0);
  unsigned char midLowCByte = *((unsigned char *)&C + 1);
  unsigned char midHighCByte = *((unsigned char *)&C + 2);
  unsigned char highCByte = *((unsigned char *)&C + 3);

  printf("%2.2x%2.2x%2.2x%2.2x", lowCByte, midLowCByte, midHighCByte, highCByte);

  unsigned char lowDByte = *((unsigned char *)&D + 0);
  unsigned char midLowDByte = *((unsigned char *)&D + 1);
  unsigned char midHighDByte = *((unsigned char *)&D + 2);
  unsigned char highDByte = *((unsigned char *)&D + 3);

  printf("%2.2x%2.2x%2.2x%2.2x", lowDByte, midLowDByte, midHighDByte, highDByte);

  std::cout << std::endl;

  delete[] paddedBlock;

  return 0;
}
