clear;clear;
rm train
rm aim
gcc train.c -lm -Ofast -mfma -o train
./train
clang aim.c -Ofast -mfma -lX11 -lm -o aim
./aim

