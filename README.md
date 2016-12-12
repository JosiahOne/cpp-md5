Specification at: https://www.ietf.org/rfc/rfc1321.txt

To Compile (On OS X):
cd into the MD5_Implementation directory.
Run: c++ md5.cpp -std=c++11

To Run:
Command: ./a.out "Some Data String Here"
Output: 52eb45b467f45522bd3f291b0fdf9b61

Running Tests:
1. Build as a.out
2. ./tests.sh >> output.txt
3. diff output.txt testCompare.txt

If nothing returned, pass, else there is a problem.
