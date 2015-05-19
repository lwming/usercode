#!/bin/tcsh
foreach i (`/bin/ls -d $1` )
    cd ${i}/res/
    set mass = `echo ${i} | sed 's|\([0-9.]*\).*|\1|' `
    echo "start merging toys for mass ${mass}"
    hadd -f ${mass}HNGrid.root job${mass}HN*.root | grep -v 'Cannot'
    cd ../../
end
