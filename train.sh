clear;clear;
rm train
gcc train.c -lm -Ofast -mavx -mfma -o train
./train
