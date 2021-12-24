#!/bin/bash

IFS=$'\n'
rm output.txt
./a.out < input.txt >> output.txt
