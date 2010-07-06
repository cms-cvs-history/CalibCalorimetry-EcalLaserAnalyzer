#!/bin/bash

init() {

usage="Usage: ./prim [OPTION] <iz=0,1,2,3> <iSM=1,...18>"
[ -z $1 ] && [ -z $2 ] && echo $usage && return

dir="EB"
[ $1 -eq 1 ] || [ $1 -eq 2 ] || dir="EE"

iz=$1
ism=$2
firstrun=$3
lastrun=$4

topdir=${MESTORE}
echo '> Top directory : ' $topdir
if [ $iz -lt 2 ]
then
    dir=${dir}-${ism}
else
    dir=${dir}+${ism}
fi


if [ -d ${topdir} ] 
then
    echo '> Store directory exists: ' $dir
else
    echo '> Check Top Directory first. Exit.'
    return
fi

dir=${topdir}/${dir}
if [ -d ${dir} ] 
then
    echo '> Store directory exists: ' $dir
else
    echo '> Creating directory ' $dir
    mkdir $dir
fi

echo '> Make the initial list of runs (Blue Laser, Red Laser, Test Pulse) for ' $dir

$MUSECAL/makeRunListInitFromFiles.pl $dir $firstrun $lastrun

echo '> Done.'

return

}

firstrun=$1
lastrun=$2

echo 'Preparing initial list of runs from files with runs: ' 
echo $firstrun 
echo 'to: '
echo $lastrun

for i in 1 2
do
  for j in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18
  do
    init $i $j $firstrun $lastrun
  done
done
for j in 1 2 3 4 5 6 7 8 9
do
  init 0 $j $firstrun $lastrun
done
for j in 1 2 3 4 5 6 7 8 9
do
  init 3 $j $firstrun $lastrun
done
