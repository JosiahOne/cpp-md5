echo "MD5 test suite:";

echo "MD5('') = ";

./a.out;

echo "MD5('a') = ";

./a.out a;

echo "MD5('abc') = ";

./a.out abc;

echo "MD5('message digest') = ";

./a.out "message digest";

echo "MD5('abcdefghijklmnopqrstuvwxyz') = ";

./a.out "abcdefghijklmnopqrstuvwxyz";

echo "MD5('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789') = ";

./a.out "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

echo "MD5(12345678901234567890123456789012345678901234567890123456789012345678901234567890) = ";

./a.out "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
