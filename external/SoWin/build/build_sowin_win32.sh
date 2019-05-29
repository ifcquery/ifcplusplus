#!/bin/bash

# Before running this script, please
# - install cygwin, including wget
# - make sure you've done cvs login to cvs.coin3d.org

SOWINBUILD_PARAMS_OK="no"

if test $# -eq 1; then
  SOWINBUILD_PARAMS_OK="yes"
fi

if test $# -eq 3; then
  SOWINBUILD_PARAMS_OK="yes"
fi

if test "X$SOWINBUILD_PARAMS_OK" = "Xno"; then
  echo "Usage: build_sowin_win32.sh <target dir>"
  echo ""
  echo "Ex:    build_sowin_win32.sh /cygdrive/c/Coin3D"
  echo ""
  echo "       The script will download and build SoWin and all dependent"
  echo "       libraries. If something fails during this process, try fixing"
  echo "       the problem based on the error messages, then _delete_ the"
  echo "       directories affected by the error. This is important because"
  echo "       the script has not been built to be able to continue after"
  echo "       any error."
  echo ""
  echo "       After the script has completed, the \"install\" directory will"
  echo "       contain a binary release of SoWin, including all dependant"
  echo "       libraries. This release consists of 8 different configurations"
  echo "       using four different CRTs (MT MTD MD MDD), with and without"
  echo "       sound support."
  echo ""
  exit
fi

if test $# -eq 1; then
  if ! test -d $1; then
    echo "[SOWIN]      Creating directory $1"
    mkdir $1
  fi
  SOWINBUILD_CHECKOUT=$1/checkout
  SOWINBUILD_CONFIGURE=$1/configure
  SOWINBUILD_INSTALL=$1/install
fi

#if test $# -eq 3; then
#  SOWINBUILD_CHECKOUT=$2
#  SOWINBUILD_CONFIGURE=$3
#  SOWINBUILD_INSTALL=$4
#fi

if ! test -d $SOWINBUILD_CHECKOUT; then
  echo "[SOWIN]   Creating directory $SOWINBUILD_CHECKOUT"
  mkdir $SOWINBUILD_CHECKOUT
fi

if ! test -d $SOWINBUILD_CHECKOUT; then
  echo "[SOWIN]   Couldn\'t create directory $SOWINBUILD_CHECKOUT. Aborting."
  exit
fi

if ! test -d $SOWINBUILD_CONFIGURE; then
  echo "[SOWIN]   Creating directory $SOWINBUILD_CONFIGURE"
  mkdir $SOWINBUILD_CONFIGURE
fi

if ! test -d $SOWINBUILD_CONFIGURE; then
  echo "[SOWIN]   Couldn\'t create directory $SOWINBUILD_CONFIGURE. Aborting."
  exit
fi

if ! test -d $SOWINBUILD_INSTALL; then
  echo "[SOWIN]   Creating directory $SOWINBUILD_INSTALL"
  mkdir $SOWINBUILD_INSTALL
fi

if ! test -d $SOWINBUILD_INSTALL; then
  echo "[SOWIN]   Couldn\'t create directory $SOWINBUILD_INSTALL. Aborting."
  exit
fi

# cvs update simage

if ! test -d $SOWINBUILD_CHECKOUT/simage; then
  echo "[SOWIN]      Checking out simage from scratch"
  cd $SOWINBUILD_CHECKOUT
  cvs -z3 -d:pserver:cvs@cvs.coin3d.org:/export/cvsroot co simage
else
  echo "[SOWIN]      Updating simage"
  cd $SOWINBUILD_CHECKOUT/simage
  cvs -z3 update -dP
fi

# build simage

# $SOWINBUILD_CHECKOUT/simage/build/build_simage_win32.sh $1

# cvs update Coin-2

if ! test -d $SOWINBUILD_CHECKOUT/Coin-2; then
  echo "[SOWIN]      Checking out Coin-2 from scratch"
  cd $SOWINBUILD_CHECKOUT
  cvs -z3 -d:pserver:cvs@cvs.coin3d.org:/export/cvsroot co Coin-2
else
  echo "[SOWIN]      Updating Coin-2"
  cd $SOWINBUILD_CHECKOUT/Coin-2
  cvs -z3 update -dP
fi

# build Coin-2

# $SOWINBUILD_CHECKOUT/Coin-2/build/build_coin_win32.sh $1

# cvs update SoWin

if ! test -d $SOWINBUILD_CHECKOUT/SoWin; then
  echo "[SOWIN]      Checking out SoWin from scratch"
  cd $SOWINBUILD_CHECKOUT
  cvs -z3 -d:pserver:cvs@cvs.coin3d.org:/export/cvsroot co SoWin
else
  echo "[SOWIN]      Updating SoWin"
  cd $SOWINBUILD_CHECKOUT/SoWin
  cvs -z3 update -dP
fi

# configure and make coinbuild

echo "[SOWIN]      Configuring SoWin"

for SOWINBUILD_SOUND in yes no; do

  if test "X$SOWINBUILD_SOUND" = "Xyes"; then
    SOWINBUILD_SOUND_DIRNAME="with_sound"
    SOWINBUILD_SOUND_NAME="s_"
  else
    SOWINBUILD_SOUND_DIRNAME="without_sound"
    SOWINBUILD_SOUND_NAME=""
  fi

  if ! test -d $SOWINBUILD_CONFIGURE/SoWin; then
    echo "[SOWIN]         Creating directory $SOWINBUILD_CONFIGURE/SoWin"
    mkdir $SOWINBUILD_CONFIGURE/SoWin
  fi

  if ! test -d $SOWINBUILD_CONFIGURE/SoWin/$SOWINBUILD_SOUND_DIRNAME; then
    echo "[SOWIN]         Creating directory $SOWINBUILD_CONFIGURE/SoWin/$SOWINBUILD_SOUND_DIRNAME"
    mkdir $SOWINBUILD_CONFIGURE/SoWin/$SOWINBUILD_SOUND_DIRNAME
  fi

  for SOWINBUILD_CRT in md mdd mt mtd; do

    if ! test -d $SOWINBUILD_CONFIGURE/SoWin/$SOWINBUILD_SOUND_DIRNAME/$SOWINBUILD_CRT; then
      echo "[SOWIN]         Creating directory $SOWINBUILD_CONFIGURE/SoWin/$SOWINBUILD_SOUND_DIRNAME/$SOWINBUILD_CRT"
      mkdir $SOWINBUILD_CONFIGURE/SoWin/$SOWINBUILD_SOUND_DIRNAME/$SOWINBUILD_CRT
    fi

    cd $SOWINBUILD_CONFIGURE/SoWin/$SOWINBUILD_SOUND_DIRNAME/$SOWINBUILD_CRT

    if test -e "config.status"; then
      echo "[SOWIN]         Rechecking existing configuration in SoWin/$SOWINBUILD_SOUND_DIRNAME/$SOWINBUILD_CRT"
      ./config.status --recheck && ./config.status
    else
      echo "[SOWIN]         Configuring from scratch in SoWin/$SOWINBUILD_SOUND_DIRNAME/$SOWINBUILD_CRT"

      SOWINBUILD_EXTRAFLAGS1=""
      if test "X$SOWINBUILD_CRT" = "Xmd"; then
        SOWINBUILD_EXTRAFLAGS1="--disable-debug --disable-symbols"
      elif test "X$SOWINBUILD_CRT" = "Xmt"; then
        SOWINBUILD_EXTRAFLAGS1="--disable-debug --disable-symbols"
      fi

      $SOWINBUILD_CHECKOUT/SoWin/configure --with-msvcrt=$SOWINBUILD_CRT --with-suffix=_$SOWINBUILD_SOUND_NAME$SOWINBUILD_CRT --with-alternate=$SOWINBUILD_SOUND_NAME$SOWINBUILD_CRT --prefix=$SOWINBUILD_INSTALL $SOWINBUILD_EXTRAFLAGS1

    fi

    echo "[SOWIN]         Making install in SoWin/$SOWINBUILD_CRT"
    make install

  done # for SOWINBUILD_CRT

done # for SOWINBUILD_SOUND

echo "[SOWIN]   All systems ready to go"


