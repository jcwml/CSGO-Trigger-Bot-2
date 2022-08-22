clear;clear;
rm train
gcc train.c -lm -Ofast -mavx -mfma -o train
for i in {1..64}
do
   xterm -e './train' &
done
