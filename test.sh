echo echo1
(echo echo2) && false || echo echo3
echo echo4 | cat

echo writing to file > f1.txt
cat f1.txt # filename as argument
echo writing to file again > f1.txt
cat < f1.txt # file from stdin

echo echo5 | cat ; echo echo6; echo echo7
