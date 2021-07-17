#!/bin/sh
T=`mktemp`
tail -c +86 "$0"|lzma -d>$T
chmod +x $T
(sleep 3;rm $T)&exec $T
