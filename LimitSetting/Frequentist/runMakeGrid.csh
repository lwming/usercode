#!/bin/tcsh

foreach i ( 170 180 190 200 250 300 350 400 450 500 550 600 )
    echo "Making crab cfg and sh files for mass ${i}"
    ./makeGridUsingCrab.py ${i}GeVmodel.root 0 5 -n 51 -m ${i} -o job${i}HN
    ./makeGridUsingCrab.py ${i}GeVmodel.root 6 20 -n 15 -m ${i} -o job${i}HN_p2
end
