#!/bin/tcsh -f



set datasetsToCopy = `cat copyFromOSU.list`

foreach thing ($datasetsToCopy)

	echo "Starting to copy $thing"

	python dbs_transferRegister.py --dbs=ph02 --to-site T3_US_NotreDame $thing
	
end
