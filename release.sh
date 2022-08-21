clear;clear;
rm aim
clang aim.c -Ofast -mavx -mfma -lX11 -lm -o aim
./aim

