#! /bin/sh

# UCLA CS 111 Lab 1 - Same script file as test-p-ok.sh except the commands

tmp=$0-$$.tmp
mkdir "$tmp" || exit

(
cd "$tmp" || exit

cat >test.sh <<'EOF'
echo a
echo b | cat

(echo c) && false || echo d
echo odysseus won the war > ody.txt
echo odysseus is coming home > ody.txt
cat ody.txt #arg = file
cat < ody.txt #file is from stdin

echo thislab ; echo isalmost | cat ; echo done
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

../timetrash -p test.sh >test.out 2>test.err || exit

diff -u test.exp test.out || exit
test ! -s test.err || {
  cat test.err
  exit 1
}

) || exit

rm -fr "$tmp"