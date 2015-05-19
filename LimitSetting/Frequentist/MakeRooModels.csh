#!/bin/tcsh
eval 'scramv1 runtime -csh'
if (! -d $1) then
	mkdir $1
endif

foreach i ( 170 180 190 200 250 300 350 400 450 500 550 600 )
    echo "Making ${i}GeVmodel.root"
    text2workspace.py -m ${i} datacard_5p0invfb-M=${i}.txt -b -o ${1}/${i}GeVmodel.root
end

