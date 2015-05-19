#!/usr/local/bin/perl

$script = "numPV_test_v1";       # Name of job

$sample    = 120;
$Njobs     = 15;
$useSkim   = 2;

$num = @ARGV;

if( $num >= 1 ){
    $sample = $ARGV[0];

    if( $num>=2 ){
        $Njobs = $ARGV[1];

	if( $num>=3 ){
	    $useSkim = $ARGV[2];
	}
    }
}


open SHFILE, "> script/condor\_submit\_$script.sh";
print SHFILE "#!/bin/sh\n";
print SHFILE "\n";
print SHFILE "echo $1\n";
print SHFILE "\n";
print SHFILE "echo \"\"\n";
print SHFILE "echo \"Using ROOT on Condor\"\n";
print SHFILE "echo \"\"\n";
print SHFILE "\n";
print SHFILE "sample=\$1\n";
print SHFILE "NumEvents=\$2\n";
print SHFILE "NumJobs=\$3\n";
print SHFILE "jobN=\$4+1\n";
print SHFILE "\n";
print SHFILE "root -b -q head.C \"$script.C+(\$sample,\$NumEvents,$useSkim,\$NumJobs,\$jobN)\"\n";
print SHFILE "\n";
close SHFILE;


open CONDORFILE, "> condor/condor\_jobs\_$script.jdl";
print CONDORFILE "# A Condor submission file\n";
print CONDORFILE "Executable              = script/condor\_submit\_$script.sh\n";
print CONDORFILE "Universe                = vanilla\n";
print CONDORFILE "Getenv                  = true\n";
print CONDORFILE "\n";
print CONDORFILE "Arguments               = $sample -1 $Njobs \$(Process)\n";
print CONDORFILE "Output                  = Output/condor\_submit\_$script\_$sample\_\$(Process).out\n";
print CONDORFILE "Error                   = Error/condor\_submit\_$script\_$sample\_\$(Process).err\n";
print CONDORFILE "Log                     = Log/condor\_submit\_$script\_$sample\_\$(Process).log\n";
print CONDORFILE "\n";
print CONDORFILE "Queue $Njobs\n";
print CONDORFILE "\n";
close CONDORFILE;

system("chmod a+x script/condor\_submit\_$script.sh");
print "submitting: condor_submit condor/condor\_jobs\_$script.jdl\n";
system("condor_submit condor/condor\_jobs\_$script.jdl");

