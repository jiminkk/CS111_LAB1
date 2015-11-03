#! /bin/sh

# UCLA CS 111 Lab 1 - Test that valid syntax is processed correctly.

tmp=$0-$$.tmp
mkdir "$tmp" || exit

(
cd "$tmp" || exit

cat >graph.sh <<'EOF'
a b<c > d

a&&b||
 c &&
  d | e && f|

g<h
EOF


../timetrash -t graph.sh



) || exit

rm -fr "$tmp"