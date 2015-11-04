#! /bin/sh

# UCLA CS 111 Lab 1 - Test that valid syntax is processed correctly.

tmp=$0-$$.tmp
mkdir "$tmp" || exit

(
cd "$tmp" || exit

cat >graph.sh <<'EOF'
ls > jas.txt

cat jas.txt

echo abcd > jas.txt
EOF


../timetrash -t graph.sh



) || exit

rm -fr "$tmp"
