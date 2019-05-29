@echo off

REM **********************************************************************
REM * Create all the directories for the installed files

pushd %COINDIR%

if exist bin\*.* goto binexists
echo mkdir %COINDIR%\bin
mkdir bin
:binexists
if exist data\*.* goto dataexists
echo mkdir %COINDIR%\data
mkdir data
:dataexists
chdir data
if exist draggerDefaults\*.* goto draggerdefaultsexists
echo mkdir %COINDIR%\data\draggerDefaults
mkdir draggerDefaults
:draggerdefaultsexists
chdir ..
if exist lib\*.* goto libexists
echo mkdir %COINDIR%\lib
mkdir lib
:libexists
if exist include\*.* goto includeexists
echo mkdir %COINDIR%\include
mkdir include
:includeexists
chdir include
if exist Inventor\*.* goto inventorexists
echo mkdir %COINDIR%\include\Inventor
mkdir Inventor
:inventorexists
chdir Inventor
if exist Win\*.* goto winexists
echo mkdir %COINDIR%\include\Inventor\Win
mkdir Win
:winexists
chdir Win
if exist common\*.* goto wincommonexists
echo mkdir %COINDIR%\include\Inventor\Win\common
mkdir common
:wincommonexists
if exist devices\*.* goto windevicesexists
echo mkdir %COINDIR%\include\Inventor\Win\devices
mkdir devices
:windevicesexists
if exist editors\*.* goto wineditorsexists
echo mkdir %COINDIR%\include\Inventor\Win\editors
mkdir editors
:wineditorsexists
if exist engines\*.* goto winenginesexists
echo mkdir %COINDIR%\include\Inventor\Win\engines
mkdir engines
:winenginesexists
if exist nodes\*.* goto winnodesexists
echo mkdir %COINDIR%\include\Inventor\Win\nodes
mkdir nodes
:winnodesexists
if exist viewers\*.* goto winviewersexists
echo mkdir %COINDIR%\include\Inventor\Win\viewers
mkdir viewers
:winviewersexists
if exist widgets\*.* goto winwidgetsexists
echo mkdir %COINDIR%\include\Inventor\Win\widgets
mkdir widgets
:winwidgetsexists

popd
