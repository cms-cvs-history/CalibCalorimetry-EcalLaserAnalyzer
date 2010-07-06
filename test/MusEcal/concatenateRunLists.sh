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
#echo '> Top directory : ' $topdir
if [ $iz -lt 2 ]
then
    dir=${dir}-${ism}
else
    dir=${dir}+${ism}
fi


#if [ -d ${topdir} ] 
#then
#    echo '> Store directory exists: ' $dir
#else
#    echo '> Check Top Directory first. Exit.'
#    return
#fi

dir=${topdir}/${dir}
#if [ -d ${dir} ] 
#then
#    echo '> Store directory exists: ' $dir
#else
#    echo '> Creating directory ' $dir
#    mkdir $dir
#fi

#echo '> Concatenate TP runlists for dir:' $dir
command='cat '$dir'/runlist_Test_Pulse_init_fromdirs '$dir'/runlist_Test_Pulse_init_fromfiles > '$dir'/runlist_Test_Pulse_init'
echo $command
#$command
#echo '> Concatenate Blue runlists for dir:' $dir
command2='cat '$dir'/runlist_Blue_Laser_init_fromdirs '$dir'/runlist_Blue_Laser_init_fromfiles > '$dir'/runlist_Blue_Laser_init'
echo $command2
#$command2
#echo '> Concatenate Red runlists for dir:' $dir
command3='cat '$dir'/runlist_IRed_Laser_init_fromdirs '$dir'/runlist_IRed_Laser_init_fromfiles > '$dir'/runlist_IRed_Laser_init'
echo $command3
#$command3
#echo '> Done.'

return

}


echo 'Preparing initial list of runs from files with runs: ' 
echo $firstrun 
echo 'to: '
echo $lastrun

for i in 1 2
do
  for j in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18
  do
    init $i $j 
  done
done
for j in 1 2 3 4 5 6 7 8 9
do
  init 0 $j 
done
for j in 1 2 3 4 5 6 7 8 9
do
  init 3 $j 
done
