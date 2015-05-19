#!/bin/tcsh
foreach i (`/bin/ls -d $1` )
    cd ${i}
    set sample = `echo ${i} | sed 's|_2012_newPU||' `
    echo "    ===> start merging files for sample ${sample} <==="
    hadd -f ../../../../DrawPlots/bin/treeFiles/dilSummaryTrees_${sample}_all.root *.root | grep -v 'Cannot'
    cd ../
end


#cd ../../../DrawPlots/bin/treeFiles

#hadd -f dilSummaryTrees_DoubleMu_2011_btagShapeStudy_all.root dilSummaryTrees_DoubleMu_Run2011*_2011_btagShapeStudy_all.root
#hadd -f dilSummaryTrees_DoubleElectron_2011_btagShapeStudy_all.root dilSummaryTrees_DoubleElectron_Run2011*_2011_btagShapeStudy_all.root
#hadd -f dilSummaryTrees_MuEG_2011_btagShapeStudy_all.root dilSummaryTrees_MuEG_Run2011*_2011_btagShapeStudy_all.root

