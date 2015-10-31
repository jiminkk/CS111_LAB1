#! /bin/sh


(
cat >test.sh <<'EOF'
echo echo1
(echo echo2) && false || echo echo3
echo echo4 | cat

echo writing to file > f1.txt
cat f1.txt # filename as argument
echo writing to file again > f1.txt
cat < f1.txt # file from stdin

echo echo5 | cat ; echo echo6; echo echo7
EOF


cat >test.exp <<'EOF'
echo1
echo2
echo3
echo4
writing to file
writing to file again
echo5
echo6
echo7
EOF

./timetrash test.sh >test.out

diff -u test.exp test.out || exit 
test ! -s test.err || {   
	cat test.err   
	exit 1 
}  

) || exit

