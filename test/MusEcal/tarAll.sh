#!/bin/bash



tarall() {

lastrun=$1

datapath=$MELMDAT
datadir=${datapath}/${dir}
if [ -d $datadir ] 
then
    echo '> Data directory exists: ' $datadir
else
    echo '> Data directory does not exist: ' $datadir '. Return'
    return
fi

cd $datapath
for j in E*/Runs; do
    cd $j; echo $j;
    for i in Run*_LB????; do

	f=${i:3:6} 

	if [[ $f > $lastrun ]]; then
	    continue; 
	fi;
	# echo '> Taring and deleting: ' $i
	tar -cvzf $i.tar.gz $i;
	rm -rf $i;

	link=../Laser/Analyzed/$i
	if [ -h $link ]
	    then
	    # echo '> Deleting laser analyzed link:  ' $link
	    rm  $link
	fi

	link=../Laser/Analyzed/Failed/$i
	if [ -h $link ]
	    then
	    # echo '> Deleting laser failed link:  ' $link
	    rm  $link
	fi
	link=../TestPulse/Analyzed/$i
	if [ -h $link ]
	    then
	    # echo '> Deleting TP analyzed link:  ' $link
	    rm  $link
	fi
	link=../TestPulse/Analyzed/Failed/$i
	if [ -h $link ]
	    then
	    # echo '> Deleting TP failed link:  ' $link
	    rm  $link
	fi

	link=../LED/Analyzed/$i
	if [ -h $link ]
	    then
	    # echo '> Deleting LED analyzed link:  ' $link
	    rm  $link
	fi
	link=../LED/Analyzed/Failed/$i
	if [ -h $link ]
	    then
	    # echo '> Deleting LED failed link:  ' $link
	    rm  $link
	fi

	
    done;
    cd -;
done;

}

lastrun=$1

if [[ $lastrun < 138000 ]]; then
    echo 'WARNING: argument too small lastrun has to be > 138000 ';
else
    tarall $lastrun
fi