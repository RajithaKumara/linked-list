#!/bin/bash

rm -f ./build/linked-list-serial
gcc -g -Wall -o build/linked-list-serial linked-list-serial.c
./build/linked-list-serial
