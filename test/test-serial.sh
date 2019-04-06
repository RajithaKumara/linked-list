#!/bin/bash 

cd ./test.out/

for((i = 0; i < $LL_NUMBER; i++)); do
    ./linked-list-serial 1000 10000 0.99 0.005 0.005 >> ./serial-case-1.txt
done
python ../test/read.py $PWD/serial-case-1.txt > serial-case-1-python.txt

for((i = 0; i < $LL_NUMBER; i++)); do
    ./linked-list-serial 1000 10000 0.90 0.05 0.05 >> ./serial-case-2.txt
done
python ../test/read.py $PWD/serial-case-2.txt > serial-case-2-python.txt

for((i = 0; i < $LL_NUMBER; i++)); do
    ./linked-list-serial 1000 10000 0.50 0.25 0.25 >> ./serial-case-3.txt
done
python ../test/read.py $PWD/serial-case-3.txt > serial-case-3-python.txt

timestamp=$(date +%s)
mkdir -p ./$LL_NUMBER-serial-$timestamp
mv ./*.txt ./$LL_NUMBER-serial-$timestamp

echo "serial-OK $LL_NUMBER times"
