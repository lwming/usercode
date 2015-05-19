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
	Zmask = "Zmask"
	charge = "OS"
	try:
		if (str(sys.argv[index+1]) == "2011" or str(sys.argv[index+1]) == "2012_52x" or str(sys.argv[index+1]) == "2012_53x" or str(sys.argv[index+1])=="2012"):
			year = str(sys.argv[index+1])
			index += 1
	except:
		index +=0
	try:
		if (str(sys.argv[index+1]) == "Zmask" or str(sys.argv[index+1]) == "noZmask" or str(sys.argv[index+1]) == "Zpeak"):
# 		if (str(sys.argv[index+1]) == "noZmask" or str(sys.argv[index+1]) == "noZmask_lowPV" or str(sys.argv[index+1]) == "noZmask_medPV" or str(sys.argv[index+1]) == "noZmask_highPV"):
			Zmask = str(sys.argv[index+1])
			index += 1
	except:
		index +=0
	try:
		if (str(sys.argv[index+1]) == "SS" or str(sys.argv[index+1]) == "OS"):
			charge = str(sys.argv[index+1])
			index += 1
	except:
		index +=0
	jetselection = str(sys.argv[index+1])
	
	try:
		numBins = int(sys.argv[index+2])
	except:
		if "3t" in jetselection:
			numBins = 10
		elif "2t" in jetselection:
			numBins = 25
		else:
			numBins = 50
		
	parser = OptionParser()
	parser.add_option('-L', '--Lumi', dest='lumi', default=19.443, help='integrated lumi')
	parser.add_option('-b', dest="batch", action='store_true', default=false)
	
	(options, args) = parser.parse_args()
	
	# This function is defined in
	# myPlots.py
	# it returns the plots you want
##	  defaultPlotGroup = getMyPlotsDefaultNorm()
	AndrewPlotGroup = getMyPlotsAndrewNorm(lepselection,year,Zmask,charge,jetselection)
	#AndrewPlotGroup = getMyPlotsAndrewNorm(lepselection,jetselection)

##	  defaultPlotGroup.lumi = myLumi*1e6
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
	#	1. What distribution?
	#	2. Which plots?
	#	3. Title for the plots
	#	4. Title for the x-axis
	#	5,6,7. New binning, just like TH1F arguments (bins, xMin, xMax)
	#	9. Jet treatment: which jet bins to plot this for (0,1,2,>=0,>=1 etc)

		# Echo the current config

	pg.show()

	pival = 3.14

#	if (jetselection != "ge1t"):
#		drawStackPlot("min_dr_tagged_jets", pg, "minimum #DeltaR(b-tags)", 10, 0.5, 4.5, lepselection, year, "draw")
#		drawStackPlot("mindr_lep1_jet", pg, "minimum #DeltaR(leading lepton,jet)", 10, 0, 4, lepselection, year, "draw")
#		drawStackPlot("avg_btag_disc_btags", pg, "average CSV output (b-tags)", 10, 0.7, 1, lepselection,year, "draw")
#		drawStackPlot("second_jet_pt", pg, "2nd jet p_{T}", 20, 30, 230, lepselection,year, "draw")
#		drawStackPlot("pt_of_ttbar", pg, "p_{T} (lep,met,2bjets)", 20, 0, 240, lepselection,year, "draw")
#		drawStackPlot("pt_of_everything", pg, "p_{T} vector sum", 20, 0, 140, lepselection,year, "draw")		
		
	#if (jetselection == "ge2t"):
#		drawStackPlot("Ht", pg, "sum p_{T} (leptons,jets,MHT)",20, 200, 1100, lepselection,year, "draw")
#		drawStackPlot("first_jet_pt", pg, "highest jet p_{T}", 20, 30, 330, lepselection,year, "draw")
		#drawStackPlot("sum_jet_pt", pg, "sum jet p_{T}", 20, 60, 660, lepselection,year, "draw")
	#if (jetselection == "eq2jeq2t" or jetselection == "eq2jge0t" or jetselection == "eq2jeq0t" or jetselection == "eq2jeq1t"):
		#drawStackPlot("min_dr_tagged_jets", pg, "min. #DeltaR(j_{m}^{tag}, j_{n}^{tag})", 10, 0.5, 4.5, lepselection, year, "draw")
		#drawStackPlot("mindr_lep1_jet", pg, "#DeltaR(l, j_{closest})", 10, 0, 4, lepselection, year, "draw")
		#drawStackPlot("avg_btag_disc_btags", pg, "#mu^{CSV}", 10, 0.7, 1, lepselection,year, "draw")		
##		drawStackPlot("CFMlpANN_e2je2t", pg,  "ANN output", 9, 0.4981, 0.5035, lepselection,year, "draw")
#		drawStackPlot("CFMlpANN_e2je2t", pg,  "ANN output", 10, 0.4975, 0.5035, lepselection,year, "draw")
		#drawStackPlot("Ht", pg, "p_{T}(l, #slash{E}_{T}, jets)",10, 200, 1100, lepselection,year, "draw")
		#drawStackPlot("first_jet_pt", pg, "jet 1 p_{T}", 10, 30, 330, lepselection,year, "draw")
#		drawStackPlot("sum_jet_pt", pg, "sum jet p_{T}", 20, 60, 460, lepselection,year, "draw")
#		drawStackPlot("pt_of_everything", pg, "p_{T} vector sum", 20, 0, 140, lepselection,year, "draw")
#		drawStackPlot("pt_of_ttbar", pg, "p_{T} (lep,met,2bjets)", 20, 0, 140, lepselection,year, "draw")		
	#if (jetselection == "eq3jeq2t"):
		#drawStackPlot("CFMlpANN_e3je2t", pg,  "ANN output", 10, 0.33, 0.73, lepselection, year, "draw")    
	#if (jetselection == "ge4jeq2t"):
#		drawStackPlot("CFMlpANN_ge4je2t", pg,  "ANN output", 10, 0.26, 0.66, lepselection, year, "draw")
		#drawStackPlot("first_jet_pt", pg, "highest jet p_{T}", 20, 30, 350, lepselection,year, "draw")
		#drawStackPlot("Ht", pg, "sum p_{T} (leptons,jets,MHT)",20, 300, 1900, lepselection,year, "draw")
		#drawStackPlot("third_jet_pt", pg, "3rd jet p_{T}", 20, 30, 210, lepselection,year, "draw")
		#drawStackPlot("fourth_jet_pt", pg, "4th jet p_{T}", 20, 30, 130, lepselection,year, "draw")
		#drawStackPlot("sum_jet_pt", pg, "sum jet p_{T}", 20, 120, 840, lepselection,year, "draw")
		#drawStackPlot("second_jet_pt", pg, "2nd jet p_{T}", 20, 30, 230, lepselection,year, "draw")
	#if (jetselection == "ge3t"):
		#drawStackPlot("min_dr_tagged_jets", pg, "min. #DeltaR(j_{m}^{tag}, j_{n}^{tag})", 10, 0.5, 3.5, lepselection, year, "draw")
		#drawStackPlot("mindr_lep1_jet", pg, "#DeltaR(l, j_{closest})", 10, 0.2, 3.2, lepselection, year, "draw")
		#drawStackPlot("avg_btag_disc_btags", pg, "#mu^{CSV}", 10, 0.7, 1, lepselection,year, "draw")		
##		drawStackPlot("CFMlpANN_ge3t", pg, "ANN output", 9, 0.12, 0.84, lepselection,year, "draw")
#		drawStackPlot("CFMlpANN_ge3t", pg, "ANN output", 10, 0.04, 0.84, lepselection,year, "draw")
		#drawStackPlot("numJets", pg, "N_{jets}", 5, 3, 8, lepselection,year, "draw")
		#drawStackPlot("Ht", pg, "p_{T}(l, #slash{E}_{T}, jets)", 10, 200, 1600, lepselection,year, "draw")
		#drawStackPlot("first_jet_pt", pg, "jet 1 p_{T}", 10, 30, 400, lepselection,year, "draw")
	#if (jetselection == "ge1t"):
		#drawStackPlot("numJets", pg,  "number of jets", 5, 2, 7, lepselection, year, "draw")
		#drawStackPlot("numTaggedJets", pg,  "number of b-tagged jets", 4, 1, 5, lepselection, year, "draw")
		
	#drawStackPlot("numTaggedJets", pg, "numTaggedJets", 3, 0, 3, lepselection, year, "draw")
	drawStackPlot("numPV", pg, "numPV", 30, 0, 30, lepselection, year, "draw")
	drawStackPlot("met", pg, "met", numBins, 0, 300, lepselection, year, "draw")
	drawStackPlot("MHT", pg, "MHT", numBins, 0, 300, lepselection, year, "draw")
	drawStackPlot("lep1Pt", pg, "lep1Pt", numBins, 10, 210, lepselection, year, "draw")
	drawStackPlot("lep2Pt", pg, "lep2Pt", numBins, 10, 210, lepselection, year, "draw")
	drawStackPlot("lep1Eta", pg, "lep1Eta", numBins, -2.5, 2.5, lepselection, year, "draw")
	drawStackPlot("lep2Eta", pg, "lep2Eta", numBins, -2.5, 2.5, lepselection, year, "draw")
	#drawStackPlot("lep1Iso", pg, "lep1Iso", numBins, 0, 0.2, lepselection, year, "draw")
	#drawStackPlot("lep2Iso", pg, "lep2Iso", numBins, 0, 0.2, lepselection, year, "draw")
	drawStackPlot("first_jet_pt", pg, "highest jet p_{T}", 20, 30, 330, lepselection,year, "draw")
	drawStackPlot("second_jet_pt", pg, "second_jet_pt", numBins, 20, 220, lepselection, year, "draw")
	drawStackPlot("first_jet_eta", pg, "first_jet_eta", numBins, -2.5, 2.5, lepselection, year, "draw")
	drawStackPlot("second_jet_eta", pg, "second_jet_eta", numBins, -2.5, 2.5, lepselection, year, "draw")

	if jetselection == "ge3t" or jetselection == "eq3jeq2t" or jetselection == "ge4je2t":
		drawStackPlot("third_jet_pt", pg, "third_jet_pt", numBins, 0, 200, lepselection, year, "draw")
		drawStackPlot("third_jet_eta", pg, "third_jet_eta", numBins, -2.5, 2.5, lepselection, year, "draw")
	if jetselection == "ge4je2t":
		drawStackPlot("fourth_jet_pt", pg, "fourth_jet_pt", numBins, 0, 150, lepselection, year, "draw")
		drawStackPlot("fourth_jet_eta", pg, "fourth_jet_eta", numBins, -2.5, 2.5, lepselection, year, "draw")
	
	#drawStackPlot("mass_of_everything", pg, "mass_of_everything", numBins, 0, 1000, lepselection, year, "draw")
	#drawStackPlot("mass_leplep", pg, "mass_leplep", numBins, 0, 400, lepselection, year, "draw")
	#drawStackPlot("sum_pt", pg, "sum_pt", numBins*6/5, 100, 700, lepselection, year, "draw")
	#drawStackPlot("sum_jet_pt", pg, "sum_jet_pt", numBins*4/5, 60, 460, lepselection, year, "draw")

	##Same-sign variables
# 	drawStackPlot("numTightMuons", pg, "numTightMuons", 3, 0, 3, lepselection, year, "draw")
# 	drawStackPlot("numTightElectrons", pg, "numElectrons", 3, 0, 3, lepselection, year, "draw")
# 	drawStackPlot("lep1TkCharge", pg, "lep1TkCharge", 2, -1, 1, lepselection, year, "draw")
# 	drawStackPlot("lep1NeutralIso", pg, "lep1NeutralIso", numBins, 0, 0.2, lepselection, year, "draw")
# 	drawStackPlot("lep2NeutralIso", pg, "lep2NeutralIso", numBins, 0, 0.2, lepselection, year, "draw")
# 	drawStackPlot("lep1IP", pg, "lep1IP", numBins, 0, 0.025, lepselection, year, "draw")
# 	drawStackPlot("lep2IP", pg, "lep2IP", numBins, 0, 0.025, lepselection, year, "draw")
# 	drawStackPlot("lep1IPError", pg, "lep1IPError", numBins, 0, 0.05, lepselection, year, "draw")
# 	drawStackPlot("lep2IPError", pg, "lep2IPError", numBins, 0, 0.05, lepselection, year, "draw")
# 	drawStackPlot("mindr_lep1_allJet", pg, "mindr_lep1_allJet", numBins, 0, 5, lepselection, year, "draw")
# 	drawStackPlot("mindr_lep2_allJet", pg, "mindr_lep2_allJet", numBins, 0, 5, lepselection, year, "draw")
# 	drawStackPlot("mindr_lep1_jet", pg, "mindr_lep1_jet", numBins, 0, 5, lepselection, year, "draw")
# 	drawStackPlot("mindr_lep2_jet", pg, "mindr_lep2_jet", numBins, 0, 5, lepselection, year, "draw")
# 	drawStackPlot("ptRel_lep1_allJet", pg, "ptRel_lep1_allJet", numBins, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("ptRel_lep2_allJet", pg, "ptRel_lep2_allJet", numBins, 0, 1, lepselection, year, "draw")

    ##CSV study variables
# 	drawStackPlot("first_jet_CSV", pg, "first_jet_CSV", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("second_jet_CSV", pg, "second_jet_CSV", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("first_jet_CSV_unc", pg, "first_jet_CSV_unc", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("second_jet_CSV_unc", pg, "second_jet_CSV_unc", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_30to45_0p0to1p2", pg, "CSV_30to45_0p0to1p2", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_45to75_0p0to1p2", pg, "CSV_45to75_0p0to1p2", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_75to150_0p0to1p2", pg, "CSV_75to150_0p0to1p2", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_150toInf_0p0to1p2", pg, "CSV_150toInf_0p0to1p2", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_30to45_1p2to2p1", pg, "CSV_30to45_1p2to2p1", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_45to75_1p2to2p1", pg, "CSV_45to75_1p2to2p1", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_75to150_1p2to2p1", pg, "CSV_75to150_1p2to2p1", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_150toInf_1p2to2p1", pg, "CSV_150toInf_1p2to2p1", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_30to45_2p1to2p4", pg, "CSV_30to45_2p1to2p4", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_45to75_2p1to2p4", pg, "CSV_45to75_2p1to2p4", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_75to150_2p1to2p4", pg, "CSV_75to150_2p1to2p4", 50, 0, 1, lepselection, year, "draw")
# 	drawStackPlot("CSV_150toInf_2p1to2p4", pg, "CSV_150toInf_2p1to2p4", 50, 0, 1, lepselection, year, "draw")


#	drawStackPlot("all_sum_pt", pg, "p_{T} (leptons, jets, MHT)", numBins*6/5, 100, 1300, lepselection, year, "draw")
#	drawStackPlot("avg_btag_disc_btags", pg, "average CSV output (b-tags)", numBins*40/50, 0.6, 1, lepselection, year, "draw")
#	drawStackPlot("avg_tagged_dijet_mass", pg, "avg_tagged_dijet_mass", numBins*40/50, 0, 400, lepselection, year, "draw")
#	drawStackPlot("closest_tagged_dijet_mass", pg, "closest_tagged_dijet_mass", numBins*40/50, 0, 400, lepselection, year, "draw")
#	#drawStackPlot("correctedDZ_leplep", pg, "correctedDZ_leplep", numBins, -0.1, 0.1, lepselection, year, "draw")
#	drawStackPlot("dEta_leplep", pg, "dEta_leplep", numBins, -5, 5, lepselection, year, "draw")
#	drawStackPlot("dPhi_leplep", pg, "dPhi_leplep", numBins, -1*pival, pival, lepselection, year, "draw")
#	drawStackPlot("dR_leplep", pg, "dR_leplep", numBins, 0, 5, lepselection, year, "draw")
#	drawStackPlot("first_jet_pt", pg, "highest jet p_{T}", numBins, 20, 420, lepselection, year, "draw")
#	#	 drawStackPlot("first_allJet_pt", pg, "highest allJet p_{T}", numBins, 0, 300, lepselection, year, "draw")
#	#drawStackPlot("fourth_jet_pt", pg, "fourth jet p_{T}", numBins, 0, 100, lepselection, year, "draw")
#	#drawStackPlot("fourth_jet_eta", pg, "fourth jet eta", numBins, -5, 5, lepselection, year, "draw")
#	#drawStackPlot("fourth_allJet_pt", pg, "fourth allJet p_{T}", numBins, 0, 100, lepselection, year, "draw")
#	drawStackPlot("higgsLike_dijet_mass", pg, "higgsLike_dijet_mass", numBins, 0, 400, lepselection, year, "draw")
#	drawStackPlot("Ht", pg, "Ht (E_{T} leptons + E jets + MHT)", numBins, 100, 2100, lepselection, year, "draw")
#	#	 drawStackPlot("isCleanEvent", pg, "isCleanEvent", 2, 0, 2, lepselection, year, "draw")
#	#	 drawStackPlot("isTriggerPass", pg, "isTriggerPass", 2, 0, 2, lepselection, year, "draw")
#	#	 drawStackPlot("lep1Phi", pg, "lep1Phi", 62, -pival, pival, lepselection, year, "draw")
#	#	 drawStackPlot("lep2Phi", pg, "lep2Phi", numBins, -pival, pival, lepselection, year, "draw")
#	drawStackPlot("mass_MHT", pg, "mass_MHT", numBins*16/10, 100, 1700, lepselection, year, "draw")
#	#drawStackPlot("mass_of_leps_and_allJets", pg, "mass_of_leps_and_allJets", numBins, 0, 4000, lepselection, year, "draw")
#	drawStackPlot("min_dr_tagged_jets", pg, "minimum #DeltaR(b-tags)", numBins, 0, 5, lepselection, year, "draw")
#	drawStackPlot("mindr_lep1_jet", pg, "minimum #DeltaR(leading lepton,jet)", numBins, 0, 5, lepselection, year, "draw")
#	drawStackPlot("numJets", pg, "Number of jets", 7, 2, 9, lepselection, year, "draw")
#	drawStackPlot("numAllJets", pg, "Good + bad jets", 19, 1.5, 20.5, lepselection, year, "draw")
#	#drawStackPlot("numBadJets", pg, "Number of bad jets", 16, 0, 16, lepselection, year, "draw")
#	drawStackPlot("numTaggedJets", pg, "numTaggedJets", 6, 0, 6, lepselection, year, "draw")
#	#	 drawStackPlot("prob", pg, "prob", numBins, 0, 2, lepselection, year, "draw")
#	drawStackPlot("pt_leplep", pg, "pt_leplep", numBins, 0, 200, lepselection, year, "draw")
#	#	 drawStackPlot("pt_of_leps_and_allJets", pg, "pt_of_leps_and_allJets", numBins, 0, 300, lepselection, year, "draw")
#	#	 drawStackPlot("tkDZ_leplep", pg, "tkDZ_leplep", numBins, -2.5, 2.5, lepselection, year, "draw")
# 	drawStackPlot("weight", pg, "weight", numBins, 0, 2.5, lepselection, year, "draw")
														
	print "Done"

	return
	
# This allows you to run at the command line	
# tells you to call the main function defined above
if __name__ == '__main__':
	main()


