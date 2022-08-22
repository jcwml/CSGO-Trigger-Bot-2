clear;clear;
for i in {1..64}
do
   xterm -e './train; read -p "Press [Enter] key to quit..."' &
done
