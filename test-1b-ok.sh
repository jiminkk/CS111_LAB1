#! /bin/sh

# UCLA CS 111 Lab 1b - Same script file as test-p-ok.sh except the commands

tmp=$0-$$.tmp
mkdir "$tmp" || exit

(
cd "$tmp" || exit

cat >test.sh <<'EOF'
echo a
(echo b) && false || echo c
echo d | cat

echo odysseus won the war > f1.txt
cat f1.txt # filename is argument
echo odysseus is coming home > f1.txt
cat < f1.txt #file is from stdin

echo thislab | cat ; echo isalmost; echo done
EOF

cat >test.exp <<'EOF'
a
b
c
d
odysseus won the war
odysseus is coming home
thislab
isalmost
done
EOF

../timetrash test.sh >test.out 2>test.err || exit

diff -u test.exp test.out || exit
test ! -s test.err || {
  cat test.err
  exit 1
}

) || exit

rm -fr "$tmp"