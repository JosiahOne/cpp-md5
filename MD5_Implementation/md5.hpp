#include <vector>

//#define DEBUG true

// MD5 Main Header
class WORD {
public:

  WORD() : highByte(0), midHighByte(0), midLowByte(0), lowByte(0) {}

  unsigned int asInt() {
    unsigned int result = 0;

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

  void setLowOrderFirst(unsigned char aLowByte, unsigned char aMidLowByte, unsigned char aMidHighByte, unsigned char aHighByte) {
    lowByte = aLowByte;
    midLowByte = aMidLowByte;
    midHighByte = aMidHighByte;
    highByte = aHighByte;
  }

  void print() {
    std::cout << (unsigned int)(highByte) << (unsigned int)(midHighByte) << (unsigned int)(midLowByte) << (unsigned int)(lowByte);
  }

public:
  unsigned char highByte;
  unsigned char midHighByte;
  unsigned char midLowByte;
  unsigned char lowByte;
};

class BLOCK {
public:
  void insert(WORD word) {
    words.push_back(word);
  }

  unsigned int size() {
    return words.size();
  }

  unsigned int operator[](unsigned int i) {
    return words[i].asInt();
  }

private:
  std::vector<WORD> words;
};

namespace MD5 {
  std::vector<unsigned int> T = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,  // VERIFIED
                                 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                                 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                                 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                                 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                                 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                                 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                                 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                                 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                                 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                                 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                                 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                                 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                                 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                                 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                                 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
                                };


  unsigned int F(unsigned int x, unsigned int y, unsigned int z) {
    return ((x & y) | ((~x) & z));
  }

  unsigned int G(unsigned int x, unsigned int y, unsigned int z) {
    return ((x & z) | (y & (~z)));
  }

  unsigned int H(unsigned int x, unsigned int y, unsigned int z) {
    return x ^ y ^ z;
  }

  unsigned int I(unsigned int x, unsigned int y, unsigned int z) {
    return y ^ (x | (~z));
  }

  unsigned int RotateLeft(unsigned int x, unsigned int n) {
    return (((x) << (n)) | ((x) >> (32 - (n))));
  }

  void Round1(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int k, unsigned int s, unsigned int i, BLOCK X) {
    #ifdef DEBUG
    printf("Rotate Left(%x, %x, %x, %x, %d)\n", a, MD5::F(b, c, d), X[k], MD5::T[i], s);
    #endif
    a = b + MD5::RotateLeft((a + MD5::F(b, c, d) + X[k] + MD5::T[i]), s);
  }

  void Round2(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int k, unsigned int s, unsigned int i, BLOCK X) {
    a = b + MD5::RotateLeft((a + MD5::G(b, c, d) + X[k] + MD5::T[i]), s);
  }

  void Round3(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int k, unsigned int s, unsigned int i, BLOCK X) {
    a = b + MD5::RotateLeft((a + MD5::H(b, c, d) + X[k] + MD5::T[i]), s);
  }

  void Round4(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int k, unsigned int s, unsigned int i, BLOCK X) {
    a = b + MD5::RotateLeft((a + MD5::I(b, c, d) + X[k] + MD5::T[i]), s);
  }

}
