
#!/bin/bash
#############################################################
#
# Driver script for creating Hybrid or Frequentist grids
#
# author: Giovanni Petrucciani, UCSD                       
#         from a similar script by Luca Lista, INFN        
#
##############################################################

i="$1"
if [ "$i" = "" ]; then
  echo "Error: missing job index"
  exit 1;
fi
echo "max events from CRAB: $MaxEvents"
n="$MaxEvents"
if [ "$n" = "" ]; then
  n="$2"
fi
if [ "$n" = "" ]; then
  echo "Error: missing number of experiments"
  exit 2;
fi

echo "## Starting at $(date)"
(( ($i + 0) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0 
(( ($i + 1) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0.1 
(( ($i + 2) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0.2 
(( ($i + 3) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0.3 
(( ($i + 4) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0.4 
(( ($i + 5) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0.5 
(( ($i + 6) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0.6 
(( ($i + 7) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0.7 
(( ($i + 8) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0.8 
(( ($i + 9) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 0.9 
(( ($i + 10) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1 
(( ($i + 11) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1.1 
(( ($i + 12) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1.2 
(( ($i + 13) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1.3 
(( ($i + 14) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1.4 
(( ($i + 15) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1.5 
(( ($i + 16) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1.6 
(( ($i + 17) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1.7 
(( ($i + 18) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1.8 
(( ($i + 19) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 1.9 
(( ($i + 20) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2 
(( ($i + 21) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2.1 
(( ($i + 22) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2.2 
(( ($i + 23) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2.3 
(( ($i + 24) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2.4 
(( ($i + 25) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2.5 
(( ($i + 26) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2.6 
(( ($i + 27) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2.7 
(( ($i + 28) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2.8 
(( ($i + 29) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 2.9 
(( ($i + 30) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3 
(( ($i + 31) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3.1 
(( ($i + 32) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3.2 
(( ($i + 33) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3.3 
(( ($i + 34) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3.4 
(( ($i + 35) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3.5 
(( ($i + 36) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3.6 
(( ($i + 37) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3.7 
(( ($i + 38) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3.8 
(( ($i + 39) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 3.9 
(( ($i + 40) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4 
(( ($i + 41) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4.1 
(( ($i + 42) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4.2 
(( ($i + 43) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4.3 
(( ($i + 44) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4.4 
(( ($i + 45) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4.5 
(( ($i + 46) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4.6 
(( ($i + 47) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4.7 
(( ($i + 48) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4.8 
(( ($i + 49) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 4.9 
(( ($i + 50) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 5 
(( ($i + 0) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 6 
(( ($i + 1) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 7 
(( ($i + 2) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 8 
(( ($i + 3) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 9 
(( ($i + 4) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 10 
(( ($i + 5) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 11 
(( ($i + 6) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 12 
(( ($i + 7) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 13 
(( ($i + 8) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 14 
(( ($i + 9) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 15 
(( ($i + 10) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 16 
(( ($i + 11) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 17 
(( ($i + 12) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 18 
(( ($i + 13) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 19 
(( ($i + 14) % 1 == 0 )) &&  ./combine 600GeVmodel.root -M HybridNew -m 600 --optimizeSim=1 --newToyMC=1 --freq --fork 1 -T 300   --clsAcc 0 -v 0 -n job600HN --saveHybridResult --saveToys -s -1 -i 1 --singlePoint 20 

hadd job600HN.root higgsCombine*.root
echo "## Done at $(date)"
