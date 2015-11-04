#! /bin/sh

echo bye > b.txt && g

cat b.txt

echo no depend > a.txt && d

echo hi > b.txt && c