#!/bin/sh

# ****************************************************************************
# Visual Studio manages to mungle up the dsp->vcproj upgrade when it comes
# to custom build rules (Qt moc-file generation in our case).  This small
# script is for fixing the problems VC7.1 introduces when doing the upgrade.
# 20050105 larsa

file=sowin1.vcproj
mv $file $file.new

# remove the unwanted end-of-string newline and remove the &quot; parts
grep -v "^\"" $file.new \
  | sed -e 's,&quot;$(QTDIR)&quot;,$(QTDIR),g' \
        -e 's/\.h$/.h"/' \
        -e 's/\.c$/.c"/' \
        -e 's/\.cpp$/.cpp"/' \
  | sed -e 's/$/\r/g' >$file
rm $file.new

