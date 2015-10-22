#! /bin/sh
# UCLA CS 111 Lab 1b - Same script file as test-p-ok.sh except the commands

tmp=$0-$$.tmp
mkdir "$tmp" || exit

(
cd "$tmp" || exit

echo "jasmine
kim
jason
jiang" > team

cat >test.sh <<'EOF'
(cat team | head -n 3 | sort -r)

echo parallel

EOF

cat >test.exp <<'EOF'
jiang
kim
this
does
not
work!!!
EOF

../timetrash -t test.sh >test.out 2>test.err || exit

diff -u test.exp test.out || exit
test ! -s test.err || {
  cat test.err
  exit 1
}

) || exit

rm -fr "$tmp"
