#!/bin/csh -f

set listFileName = $1
set iYear = $2
set iJob = $3
#set nJobs = $3
set iLabel = $4
set iJes = $5
set iJer = $6
set iBtag = $7
set iPUPeriod = $8
set NJOBS = $9
set DENSE = $10
#echo "listFileName, iYear, iJob, iLabel, iJes, iJer = "${listFileName}" , "${iYear}" , "${iJob}" , "${iLabel}" , "${iJes}" , "${iJer}

#set sample = ${listFileName:r}
set sample = ${listFileName}

set outDirName = ""

if ($iJes == "0") then
    if ($iJer == "0") then
        set outDirName = batchBEAN/${sample}_${iYear}_${iLabel}/log
    endif
    if ($iJer == "-1") then
        set outDirName = batchBEAN/${sample}_${iYear}_${iLabel}_JERDown/log
    endif
    if ($iJer == "1") then
        set outDirName = batchBEAN/${sample}_${iYear}_${iLabel}_JERUp/log
    endif
endif
if ($iJes == "-1") then
    set outDirName = batchBEAN/${sample}_${iYear}_${iLabel}_JESDown/log
endif
if ($iJes == "1") then
    set outDirName = batchBEAN/${sample}_${iYear}_${iLabel}_JESUp/log
endif

if ($iBtag == "1") then
    set outDirName = batchBEAN/${sample}_${iYear}_${iLabel}_btagHFUp/log
endif

if ($iBtag == "-1") then
    set outDirName = batchBEAN/${sample}_${iYear}_${iLabel}_btagHFDown/log
endif

if ($iBtag == "2") then
    set outDirName = batchBEAN/${sample}_${iYear}_${iLabel}_btagLFUp/log
endif

if ($iBtag == "-2") then
    set outDirName = batchBEAN/${sample}_${iYear}_${iLabel}_btagLFDown/log
endif


if ($outDirName == "") then

  echo "Script called with bad arguments, can't make the output dir"
  echo "Failed trying to create $outDirName"
  exit (5)

endif 


if (! -e $outDirName) then
	mkdir -p $outDirName
endif


#shyft_pretag shyftPretag_main_condor.py $listFileName $iJob $nJobs \
#	>&! $outDirName/shyftPretag_${sample}_${iJob}.log
#

#mttbarTree mttbarTree_batch.py $listFileName $iJob $nJobs \
#   >&! $outDirName/shyftPretag_${sample}_${iJob}.log


#root -b -q head.C 'test_beans_dileptonStudies.C+(-1,"'${sample}'", '$iJob', "'$iLabel'")' >&! $outDirName/test_beans_dileptonStudies_${sample}_${iLabel}_${iJob}.log

#eventDisplay dilAnalysis_condor_cfg.py ${sample} $iYear $iJob $iLabel $iJes $iJer $iBtag $iPUPeriod $NJOBS $DENSE > & ! $outDirName/dilAnalysis_${sample}_${iYear}_${iLabel}_${iJob}.log
helper_dilepton dilAnalysis_condor_cfg.py ${sample} $iYear $iJob $iLabel $iJes $iJer $iBtag $iPUPeriod $NJOBS $DENSE > & ! $outDirName/dilAnalysis_${sample}_${iYear}_${iLabel}_${iJob}.log
# helper_trilepton dilAnalysis_condor_cfg.py ${sample} $iYear $iJob $iLabel $iJes $iJer $iBtag $iPUPeriod > & ! $outDirName/dilAnalysis_${sample}_${iYear}_${iLabel}_${iJob}.log
