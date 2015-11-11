echo 1 > d
echo bye > b.txt
(echo 123 | echo 0) || echo yo > c
cat b.txt

echo no depend > a
cat d
echo hi > b; echo hellooo