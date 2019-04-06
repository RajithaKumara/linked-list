#!/bin/bash 

cd ./test.out/

for((j = 1; j <= 8; j=j*2)); do
    for((i = 0; i < $LL_NUMBER; i++)); do
        ./linked-list-rwlock 1000 10000 $j 0.99 0.005 0.005 >> ./thread-$j-rwlock-case-1.txt
    done
    python ../test/read.py $PWD/thread-$j-rwlock-case-1.txt > thread-$j-rwlock-case-1-python.txt

    for((i = 0; i < $LL_NUMBER; i++)); do
        ./linked-list-rwlock 1000 10000 $j 0.90 0.05 0.05 >> ./thread-$j-rwlock-case-2.txt
    done
    python ../test/read.py $PWD/thread-$j-rwlock-case-2.txt > thread-$j-rwlock-case-2-python.txt

    for((i = 0; i < $LL_NUMBER; i++)); do
        ./linked-list-rwlock 1000 10000 $j 0.50 0.25 0.25 >> ./thread-$j-rwlock-case-3.txt
    done
    python ../test/read.py $PWD/thread-$j-rwlock-case-3.txt > thread-$j-rwlock-case-3-python.txt
done

timestamp=$(date +%s)
mkdir -p ./$LL_NUMBER-rwlock-$timestamp
mv ./*.txt ./$LL_NUMBER-rwlock-$timestamp

echo "rwlock-OK $LL_NUMBER times"
