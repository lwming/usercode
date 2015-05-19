#!/bin/csh -f

#set listFileName = $1
#set iLabel = $2
#set iJob = $2
#set nJobs = $3


set iLabel = "localJob_JesUp_v1"

#set sample = ${listFileName:r}

#shyft_pretag shyftPretag_main_condor.py $listFileName $iJob $nJobs \
#	>&! $outDirName/shyftPretag_${sample}_${iJob}.log
#

#mttbarTree mttbarTree_batch.py $listFileName $iJob $nJobs \
#   >&! $outDirName/shyftPretag_${sample}_${iJob}.log
#
foreach listFileName (ttH_m100 ttH_m105 ttH_m110 ttH_m115 ttH_m120  ttH_m125 ttH_m130 ttH_m135 ttH_m140)
#foreach listFileName (ttH_m100 )
  set sample = ${listFileName}
  set outDirName = batchBEAN/${sample}_${iLabel}/log

  if (! -e $outDirName) then
	  mkdir -p $outDirName
  endif
  echo "Running sample $sample"
  root -b -q head.C 'test_beans_dileptonStudies_ttH.C+(-1,"'${sample}'", "'$iLabel'")' >&! $outDirName/test_beans_dileptonStudies_${sample}_${iLabel}_ALL.log
  
end
