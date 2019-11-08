#!/bin/bash

g++ main.cpp
./a.out < $1 > results.txt && diff results.txt $2
