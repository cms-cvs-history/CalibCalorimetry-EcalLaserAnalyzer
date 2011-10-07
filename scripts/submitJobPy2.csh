#!/bin/tcsh

eval `scramv1 runtime -csh`
#set here = `pwd`
cd $2
rm -f $1.log

set datestart1=`date +%F`
set datestart2=`date +%R:%S`

# send job:
#============
cmsRun $1.py >& $1.log 
mv $1.log $1bis.log
cat $1bis.log | grep -v LumiSection | grep -v 'Begin processing' | grep -v 'No matacq file found' | grep -v '\%MSG' > $1.log
rm $1bis.log

set date1=`date +%F`
set date2=`date +%R:%S`

# write sendfile:
#=================
echo "$4 START=${datestart1}_${datestart2} STOP=${date1}_${date2} NPROC=${5}" >> $3


# eventually remove unzipped file:
#=================================

if( $6 == 1) then
  rm input.lmf
endif
