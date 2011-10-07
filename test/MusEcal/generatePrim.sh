#!/bin/bash

# argument is config file
source $1
source ${MON_CMSSW_INSTALL_DIR}/scripts/setup.sh
cd ${MON_CMSSW_REL_DIR}
eval `scramv1 runtime -sh`
source ${MON_MUSECAL_DIR}/setup $1
echo $2
#pwd=`pwd`
echo ${MON_MUSECAL_DIR}
#ls $2

#fix this:
source ${MON_MUSECAL_DIR}/createListsOfRuns.sh $2

if [ $2 = 0 ]; then   
    ${MON_MUSECAL_DIR}/bin/writePrim -t 0 -c 0 -e 0  # LASER BLUE
    ${MON_MUSECAL_DIR}/bin/writePrim -t 0 -c 3 -e 0  # LASER RED
    ${MON_MUSECAL_DIR}/bin/writePrim -t 1 -c 0 -e 0  # TP
fi
if [ $2 = 1 ]; then   
    ${MON_MUSECAL_DIR}/bin/writePrim -t 0 -c 0 -e 1  # LASER BLUE
    ${MON_MUSECAL_DIR}/bin/writePrim -t 0 -c 3 -e 1  # LASER RED
    ${MON_MUSECAL_DIR}/bin/writePrim -t 1 -c 0 -e 1  # TP
fi
if [ $2 = 2 ]; then 
    ${MON_MUSECAL_DIR}/bin/writePrim -t 0 -c 0 -e 2  # LASER BLUE
    ${MON_MUSECAL_DIR}/bin/writePrim -t 1 -c 0 -e 2  # TP
    ${MON_MUSECAL_DIR}/bin/writePrim -t 2 -c 0 -e 2  # LED BLUE
    ${MON_MUSECAL_DIR}/bin/writePrim -t 2 -c 2 -e 2  # LED RED
fi