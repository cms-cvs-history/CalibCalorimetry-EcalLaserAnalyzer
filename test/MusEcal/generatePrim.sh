#!/bin/bash

# argument is config file
source $1
source ~cmssw2/scripts/setup.sh
cd ${MON_CMSSW_REL_DIR}
eval `scramv1 runtime -sh`
source ${MON_MUSECAL_DIR}/setup $1
source ${MON_MUSECAL_DIR}/createListsOfRuns.sh 
${MON_MUSECAL_DIR}/bin/writePrim
${MON_MUSECAL_DIR}/bin/writePrim -t 0 -c 3
${MON_MUSECAL_DIR}/bin/writePrim -t 1 -c 0
