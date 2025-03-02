#!/bin/bash

echo -e "\E[H\E[J"

echo -e "\E(0"
echo -n -e "\E[10;5H"

echo "lqqqqqqk"

str=10
for i in {1..6}; do
((str = str + 1));
echo -n -e "\E[$str;5H"
echo "x      x"
done
((str = str + 1))
echo -n -e "\E[$str;5H"
echo "mqqqqqqj"


echo -n -e "\E[5;10Haa"
echo -n -e "\E[6;10Haa"
echo -e "\E[7;10Haa"
echo -e "\E[8;10Haa"
echo -e "\E[9;10Haa"
echo -e "\E[10;10Haa"
echo -e "\E[5;6Haa"
echo -e "\E[6;6Haa"
echo -e "\E[7;6Haa"
echo -e "\E[8;6Haa"
echo -e "\E[8;7Haaa"

echo -n -e "\E[20;1H"

printf "\E(B"