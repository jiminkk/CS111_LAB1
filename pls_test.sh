#! /bin/sh

echo bye > b

cat b.txt

echo no depend > a && d

echo hi > b && c