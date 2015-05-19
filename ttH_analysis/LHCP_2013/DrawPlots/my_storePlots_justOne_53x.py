#!/usr/bin/env python

# import the environment 
from PlotEnv import *

# import the plots you want
from myPlots2012_53x import *

# import the root libraries
# with this kind of import
# you won't need to prefix your root objects with ROOT

from ROOT import *

# this is the function that is called
# when you type ./drawPlots.py
# there are some optional arguments

def main ():
	
	# Parse the options
    index = 1
    if "-b" in str(sys.argv[1]) or "-L" in str(sys.argv[1]):
        index+=1
    if "-b" in str(sys.argv[2]) or "-L" in str(sys.argv[2]):
        index+=1

    lepselection = str(sys.argv[index])
    year = "2012_53x"
#    Zmask = "noZmask"
    Zmask = "Zmask"
    try:
        if (str(sys.argv[index+1]) == "2011" or str(sys.argv[index+1]) == "2012_52x" or str(sys.argv[index+1]) == "2012_53x"):
            year = str(sys.argv[index+1])
            index += 1
    except:
        index +=0
    try:
        if (str(sys.argv[index+1]) == "Zmask" or str(sys.argv[index+1]) == "noZmask" or str(sys.argv[index+1]) == "Zpeak"):
            Zmask = str(sys.argv[index+1])
            index += 1
    except:
        index +=0

    charge = "OS" 
    jetselection = str(sys.argv[index+1])
    
    try:
        numBins = int(sys.argv[index+2])
    except:
        if "3t" in jetselection:
            numBins = 25
        elif "2t" in jetselection:
            numBins = 50
        else:
            numBins = 100
        
    parser = OptionParser()
    parser.add_option('-L', '--Lumi', dest='lumi', default=5.0, help='integrated lumi')
    parser.add_option('-b', dest="batch", action='store_true', default=false)
    
    (options, args) = parser.parse_args()
    
	# This function is defined in
	# myPlots.py
	# it returns the plots you want
##    defaultPlotGroup = getMyPlotsDefaultNorm()
    AndrewPlotGroup = getMyPlotsAndrewNorm(lepselection,year,Zmask,charge,jetselection)
    #AndrewPlotGroup = getMyPlotsAndrewNorm(lepselection,jetselection)

##    defaultPlotGroup.lumi = myLumi*1e6
    myLumi = AndrewPlotGroup.lumi
    AndrewPlotGroup.lumi = myLumi*1e3

    print "Using lumi %f" % myLumi

    pg = AndrewPlotGroup

    # Set the style for your plots

    ROOT.gROOT.SetStyle("Plain")
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetPadLeftMargin(0.15)
    ROOT.gStyle.SetPadRightMargin(0.07)
    ROOT.gStyle.SetPadTopMargin(0.07)
    ROOT.gStyle.SetPadBottomMargin(0.1)
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetTitleOffset(1.4, "y")

	# This is the good stuff
	# draw the plots
	# Arguments are
	#   1. What distribution?
	#   2. Which plots?
	#   3. Title for the plots
	#   4. Title for the x-axis
    #   5,6,7. New binning, just like TH1F arguments (bins, xMin, xMax)
    #   9. Jet treatment: which jet bins to plot this for (0,1,2,>=0,>=1 etc)

    	# Echo the current config

    pg.show()

    pival = 3.14

    if ( jetselection == "eq2jeq2t" ):
###        drawStackPlot("CFMlpANN_e2je2t", pg,  "ANN output", 10, 0.4975, 0.5035, lepselection, year, "root")
#        drawStackPlot("CFMlpANN_e2je2t_oldvar", pg,  "ANN output", 10, 0.5, 0.502, lepselection, year, "root")
        drawStackPlot("CFMlpANN_e2je2t_v1", pg,  "ANN output", 10, 0.498, 0.503, lepselection, year, "root")        
    elif ( jetselection == "eq3jeq2t" ):
##        drawStackPlot("BDTG_e3je2t", pg,  "BDTG output", 10, -0.9, 0.8, lepselection, year, "root")
##        drawStackPlot("CFMlpANN_e3je2t", pg,  "ANN output", 10, 0.2, 0.9, lepselection, year, "root")
##        drawStackPlot("CFMlpANN_e3je2t_v1", pg,  "ANN output", 10, 0.1, 0.9, lepselection, year, "root")

        drawStackPlot("BDTG_e3je2t_4var", pg,  "BDTG output", 10, -0.9, 0.8, lepselection, year, "root")
##        drawStackPlot("CFMlpANN_e3je2t_4var", pg,  "ANN output", 10, 0.2, 0.9, lepselection, year, "root")

    elif ( jetselection == "ge4jeq2t" ):
        drawStackPlot("BDTG_ge4je2t", pg,  "BDTG output", 10, -0.8, 0.7, lepselection, year, "root")
#        drawStackPlot("CFMlpANN_ge4je2t", pg,  "ANN output", 10, 0.2, 0.9, lepselection, year, "root")
#        drawStackPlot("CFMlpANN_ge4je2t_v1", pg,  "ANN output", 10, 0.04, 0.94, lepselection, year, "root")
    elif ( jetselection == "ge3t" ):
        drawStackPlot("BDTG_ge3t", pg,  "BDTG output", 10, -0.9, 0.8, lepselection,year, "root")
#        drawStackPlot("CFMlpANN_ge3t", pg,  "ANN output", 10, 0.1, 0.8, lepselection, year, "root")
#        drawStackPlot("CFMlpANN_ge3t_v1", pg, "ANN output", 10, 0.04, 0.84, lepselection,year, "root")

    print "Done"

    return
	
# This allows you to run at the command line	
# tells you to call the main function defined above
if __name__ == '__main__':
	main()


