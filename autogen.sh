#!/bin/bash

#cleanup cache for correct versioning when run multiple times
rm -rf autom4te.cache

gprefix=`which glibtoolize 2>&1 >/dev/null`
if [ $? -eq 0 ]; then
  glibtoolize --force
else
  libtoolize --force
fi

aclocal -I m4
autoconf
autoheader
automake --add-missing

if [ -z $NOCONFIGURE ]; then
  ./configure "$@"
fi
