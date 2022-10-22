clear;clear;
rm train
gcc train.c -lm -Ofast -mfma -o train
./train
