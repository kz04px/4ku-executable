#!/bin/sh
T=`mktemp`
tail -c +75 "$0"|xz -d>$T
chmod 755 $T
$T
rm $T
exit
