#!/bin/tcsh
if (! -d $1) then
	mkdir $1
endif

foreach i ( 170 180 190 200 250 300 350 400 450 500 550 600 )
    echo "Generating limits for mass ${i}"
    combine -M HybridNew --freq -m ${i} -d datacard_5p0invfb-M=${i}.txt --grid=${i}GeVFrequentist/res/${i}HNGrid.root  > ${1}/${i}GeVObs.log
    combine -M HybridNew --freq -m ${i} -d datacard_5p0invfb-M=${i}.txt --grid=${i}GeVFrequentist/res/${i}HNGrid.root --expectedFromGrid 0.025 > ${1}/${i}GeV2.5.log
    combine -M HybridNew --freq -m ${i} -d datacard_5p0invfb-M=${i}.txt --grid=${i}GeVFrequentist/res/${i}HNGrid.root --expectedFromGrid 0.16 >  ${1}/${i}GeV16.log
    combine -M HybridNew --freq -m ${i} -d datacard_5p0invfb-M=${i}.txt --grid=${i}GeVFrequentist/res/${i}HNGrid.root --expectedFromGrid 0.5 >   ${1}/${i}GeVMedian.log
    combine -M HybridNew --freq -m ${i} -d datacard_5p0invfb-M=${i}.txt --grid=${i}GeVFrequentist/res/${i}HNGrid.root --expectedFromGrid 0.84 >  ${1}/${i}GeV84.log
    combine -M HybridNew --freq -m ${i} -d datacard_5p0invfb-M=${i}.txt --grid=${i}GeVFrequentist/res/${i}HNGrid.root --expectedFromGrid 0.975 > ${1}/${i}GeV97.5.log
end

cat ${1}/*Obs.log | grep '95%' | awk '{print $4}' >&! resultsData.txt
cat ${1}/*2.5.log | grep '95%' | awk '{print $4}' >&! results2.5.txt
cat ${1}/*16.log | grep '95%' | awk '{print $4}' >&! results16.0.txt
cat ${1}/*Median.log | grep '95%' | awk '{print $4}' >&! resultsMedian.txt
cat ${1}/*84.log | grep '95%' | awk '{print $4}' >&! results84.0.txt
cat ${1}/*97.5.log | grep '95%' | awk '{print $4}' >&! results97.5.txt
ls -d *GeVFrequentist | sed 's|\([0-9]*\).*|\1|' >&! resultsMass.txt



