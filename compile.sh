clear;clear;
rm train
rm aim
gcc train.c -lm -Ofast -mavx -mfma -o train
./train
clang aim.c -Ofast -mavx -mfma -lX11 -lm -o aim
./aim

