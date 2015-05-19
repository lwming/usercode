#include <cstdlib>
#include <vector>
#include <iostream> 
#include <string>

#include "TStyle.h"
#include "TH1.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TProfile.h"

//Main differences with respect toe correlation_plots.C
//  -- Hard-coding 2-jet, 2-tag
//  -- Summing over all lepton categories
//  -- Rebinning edges to account for low stats


void correlation_plots2(bool rwt = true, TString catName = "e3je2t"){

  int numhists = 0;
  // jet and tag cut for each category
  TString jettagcut = "";
  if (catName == "ge4je2t"){
    jettagcut = "numJets>3 && numTaggedJets==2";
    numhists = 7;
  }
  else if(catName == "e3je2t"){
    jettagcut = "numJets==3 && numTaggedJets==2";
    numhists = 5;
  }
  else std::cout << "Error, wrong category name" << std::endl;
  //IN GLOBAL SCOPE
//   const int numhists = 5; // 5;
  const int numprofiles = pow(numhists,2);

  TProfile** profiles_Data = new TProfile * [numprofiles];
  TProfile** profiles_MC = new TProfile * [numprofiles];

  //IN MAIN FUNCTION
  char** histnames = new char * [numhists];
  char** histXaxis = new char * [numhists];
  int* histbins = new int[numhists];
  double* histmin = new double[numhists];
  double* histmax = new double[numhists];

  if(catName == "e3je2t"){
    histnames[0] = (char*) "sum_pt";
    histXaxis[0] = (char*) "p_{T}(l, jets)";
    histbins[0] = 13;
    histmin[0] = 150;
    histmax[0] = 930;
    
    histnames[1] = (char*) "min_dr_jets";
    histXaxis[1] = (char*) "minimum #DeltaR(j, j)";
    histbins[1] = 12;
    histmin[1] = 0.5;
    histmax[1] = 3.5;
    
    histnames[2] = (char*) "avg_btag_disc_non_btags";
    histXaxis[2] = (char*) "#mu^{CSV}(non b-tags)";
    histbins[2] = 10;
    histmin[2] = 0.0;
    histmax[2] = 0.68;
    
    histnames[3] = (char*) "avg_btag_disc_btags";
    histXaxis[3] = (char*) "#mu^{CSV}";
    histbins[3] = 10;
    histmin[3] = 0.7;
    histmax[3] = 1;
    
    histnames[4] = (char*) "BDTG_e3je2t";
    histXaxis[4] = (char*) "BDT output";
    histbins[4] = 10;
    histmin[4] = -1.;
    histmax[4] = 1.;
  }
  else if(catName == "ge4je2t"){
    histnames[0] = (char*) "sum_pt";
    histXaxis[0] = (char*) "p_{T}(l, jets)";
    histbins[0] = 15;
    histmin[0] = 200;
    histmax[0] = 1400;
    
    histnames[1] = (char*) "min_dr_jets";
    histXaxis[1] = (char*) "minimum #DeltaR(j, j)";
    histbins[1] = 11;
    histmin[1] = 0.5;
    histmax[1] = 2.7;
    
    histnames[2] = (char*) "avg_btag_disc_non_btags";
    histXaxis[2] = (char*) "#mu^{CSV}(non b-tags)";
    histbins[2] = 11;
    histmin[2] = 0.0;
    histmax[2] = 0.605;
    
    histnames[3] = (char*) "higgsLike_dijet_mass";
    histXaxis[3] = (char*) "higgsLike dijet mass";
    histbins[3] = 17;
    histmin[3] = 34;
    histmax[3] = 255;
    
    histnames[4] = (char*) "higgsLike_dijet_mass2";
    histXaxis[4] = (char*) "higgsLike dijet mass2";
    histbins[4] = 17;
    histmin[4] = 30;
    histmax[4] = 370;

    histnames[5] = (char*) "numJets";
    histXaxis[5] = (char*) "N_{jets}";
    histbins[5] = 5;
    histmin[5] = 4;
    histmax[5] = 9;

    histnames[6] = (char*) "BDTG_ge4je2t";
    histXaxis[6] = (char*) "BDT output";
    histbins[6] = 12;
    histmin[6] = -0.95;
    histmax[6] = 0.85;

  }
  else std::cout << "Error2, wrong category name" << std::endl;
  ////
  TString cuts = "(oppositeLepCharge == 1) && (dR_leplep > 0.2) && (mass_leplep > 12) && isCleanEvent && PassZmask==1 && ";
  cuts += jettagcut;

  //// sample info
  TString htitle[21] = {"_singlet_s","_singlet_tW","_singlet_t","_singletbar_s","_singletbar_tW","_singletbar_t","_ttbarW","_ttbarZ","_ttbar_cc","_ttbar_bb","_ttbar_b","_ttbar","_wjets","_zjets","_zjets_lowmass","_ww","_wz","_zz","_MuEG","_DoubleElectron","_DoubleMu",};
//     Float_t lumi = 19450;

  ////////
  //SKIPPING HISTOGRAM INITIALIZATION FOR OTHER 49 VARIABLES (USE ABOVE THREE EXAMPLES FOR YOUR USE)
  ////////

  TDirectory *currentDir = gDirectory; 

  for (int i = 0; i < numhists; i++){

    std::cout << "-->Figuring out binning for " << histnames[i] << std::endl;

    //Set the binning on the x-axis by looking at the data and combining bins (if necessary) to make sure there are no poorly populated bins
    //Define poorly populated arbitrarily as N < 100
    currentDir->cd(); //This is because the "Draw" command only finds
    TString histName = Form("tempData_%s",histnames[i]);
    TH1 * tempHist = new TH1D(histName,"",histbins[i],histmin[i],histmax[i]);

    //Only look at data
    for (int isam=18; isam<21; isam++){
      TString sample = htitle[isam];
      TString fileName = "/afs/crc.nd.edu/user/w/wluo1/LHCP_2013/CMSSW_5_3_8_patch1/src/BEAN/DrawPlots/bin/treeFiles/dilSummaryTrees" +sample + "_2012_53x_July5th_all.root";
      std::cout << "  -->Including " << fileName << std::endl;

      TFile * tmpfile = new TFile(fileName);
      TTree * tmpTree = (TTree*)tmpfile->Get("summaryTree");

      currentDir->cd(); //This is because the "Draw" command only finds

      
      TString selection =  "(" + cuts + "&&";
      if (isam == 18) selection+= "MuonEle && isMuEGTriggerPass";
      else if (isam == 19) selection += "TwoEle && isDoubleElectronTriggerPass";
      else if (isam == 20) selection += "TwoMuon && isDoubleMuTriggerPass";
      selection += ")";

      TString var = string(histnames[i])+">>+"+histName ;
      tmpTree->Draw(var, selection, "goff");


      tmpfile->Close();
      delete tmpfile;
    }

    std::cout << "tempHist name = " << tempHist->GetName() << std::endl;
    std::cout << "tempHist: Entries = " << tempHist->GetEntries() << ", Integral = " << tempHist->Integral() << std::endl;
    double threshold = (tempHist->Integral())/20;
 
   //Now, look through the bins of the histograms, combining when necessary
    int nBinsUsed = 0;
    double *xBins = new double[histbins[i]+1];
    xBins[0] = histmin[i];  //First bin always starts at minimum
    double binSum = 0;

    for (int iBin = 1; iBin <= histbins[i]; ++iBin) {

      binSum += tempHist->GetBinContent(iBin);
      std::cout << "binSum = " << binSum << ", iBin = " << iBin << std::endl;


      if (binSum >= threshold) {
        std::cout << "Setting bin edge!" << std::endl;
        ++nBinsUsed;
        xBins[nBinsUsed] = tempHist->GetBinLowEdge(iBin+1);
        binSum = 0.;
      }

    }

    //Now check: if binSum > 0, that means the last bin was too small to be a bin
    //by itself.  If so, we need to make sure it's combined with the bin on it's left by setting the bin edge to the histogram max
    if (binSum > 0) {
      xBins[nBinsUsed] = tempHist->GetBinLowEdge(histbins[i]+1);
    }

    std::cout << "  -->Proposed binning: " << histbins[i] << ", " << histmin[i] << ", " << histmax[i] << std::endl;
    std::cout << "  -->Using " << nBinsUsed << " bins: ";
    for (int iBin = 0; iBin < nBinsUsed; ++iBin) std::cout << xBins[iBin] << ", ";
    std::cout << xBins[nBinsUsed] << std::endl;


    for (int j = 0; j < numhists; j++)
    {
      currentDir->cd(); //This is because the "Draw" command only finds
      profiles_Data[numhists*i+j] = new TProfile(Form("profiles_Data_%s_%s",histnames[i],histnames[j]),"",nBinsUsed,xBins);
      profiles_Data[numhists*i+j]->Sumw2();
      profiles_Data[numhists*i+j]->SetLineWidth(2);
      profiles_MC[numhists*i+j] = new TProfile(Form("profiles_MC_%s_%s",histnames[i],histnames[j]),"",nBinsUsed,xBins);
      profiles_MC[numhists*i+j]->Sumw2();
      profiles_MC[numhists*i+j]->SetLineWidth(2);
    }

    delete[] xBins;

  }


  // loop over samples
  for (int isam=0; isam<21; isam++){
    
    /// get the tree file for each sample
    TString sample = htitle[isam];
      TString fileName = "/afs/crc.nd.edu/user/w/wluo1/LHCP_2013/CMSSW_5_3_8_patch1/src/BEAN/DrawPlots/bin/treeFiles/dilSummaryTrees" +sample + "_2012_53x_July5th_all.root";
    std::cout << "-->start processing sample " << fileName << std::endl;
    
    TFile * tmpfile = new TFile(fileName);
    TTree * tmpTree = (TTree*)tmpfile->Get("summaryTree");
    
    TString weight = "(19450*Xsec/nGen)*weight*topPtWgt*lepTotalSF*triggerSF*csvWgtlf*csvWgthf*";   /////  before rwt
    if (rwt) weight = "(19450*Xsec/nGen)*weight*topPtWgt*lepTotalSF*triggerSF*csvWgtlf*csvWgthf*";   ///// after rwt 
    
    TString lepCut = "((MuonEle && isMuEGTriggerPass) || (TwoEle && isDoubleElectronTriggerPass) || (TwoMuon && isDoubleMuTriggerPass))";
    if (isam==18) lepCut = "(MuonEle && isMuEGTriggerPass)";
    if (isam==19) lepCut = "(TwoEle && isDoubleElectronTriggerPass)";
    if (isam==20) lepCut = "(TwoMuon && isDoubleMuTriggerPass)";

	/// selection cut
        TString selection = weight;
//         selection += Form("%f*",lumi); //Adjust for sample normalization;

        selection += ("(" + cuts + "&&" + lepCut + ")");

	if (isam>17) selection = ("(" + cuts + "&&" + lepCut + ")");  // no weights for data
// 	std::cout << " -->selection is: " << selection << std::endl;

      ///// loop through variables
      for (int i = 0; i < numhists; i++){
	for (int j = 0; j < numhists; j++){

        TString proName = profiles_MC[numhists*i+j]->GetName();
        if (isam >= 18) proName = profiles_Data[numhists*i+j]->GetName();

        TString var = string(histnames[j]) + ":" + string(histnames[i])+">>+"+proName ;
        currentDir->cd(); //This is because the "Draw" command only finds
        tmpTree->Draw(var, selection, "goff");
    
      }
    }

    tmpfile->Close(); 
    delete tmpfile;

  }   // end sample loop

  //Format the plots the way Robin does
  for (int i = 0; i < numhists; i++) {
    for (int j = 0; j < numhists; j++) {
      profiles_MC[numhists*i+j]->SetFillColor(2);
      profiles_MC[numhists*i+j]->SetLineColor(0);
      profiles_MC[numhists*i+j]->SetMarkerSize(0);
      profiles_MC[numhists*i+j]->SetFillStyle(3013);
      profiles_MC[numhists*i+j]->GetXaxis()->SetTitle(histXaxis[i]);
      profiles_MC[numhists*i+j]->GetYaxis()->SetTitle(histXaxis[j]);
      profiles_MC[numhists*i+j]->GetYaxis()->SetTitleOffset(1.3);
    
      double minval = 9999999.0;
      double maxval = -9999999.0;
    
      for(int ibin = 0; ibin < profiles_Data[numhists*i+j]->GetNbinsX(); ibin++){
	if(profiles_Data[numhists*i+j]->GetBinContent(ibin+1) != 0.0){
	  if(profiles_Data[numhists*i+j]->GetBinContent(ibin+1)-profiles_Data[numhists*i+j]->GetBinError(ibin+1) < minval)
	    minval = profiles_Data[numhists*i+j]->GetBinContent(ibin+1)-profiles_Data[numhists*i+j]->GetBinError(ibin+1);
	  if(profiles_Data[numhists*i+j]->GetBinContent(ibin+1)+profiles_Data[numhists*i+j]->GetBinError(ibin+1) > maxval)
	    maxval = profiles_Data[numhists*i+j]->GetBinContent(ibin+1)+profiles_Data[numhists*i+j]->GetBinError(ibin+1);
	}
	
	if(profiles_MC[numhists*i+j]->GetBinContent(ibin+1) != 0.0){
	  if(profiles_MC[numhists*i+j]->GetBinContent(ibin+1)-profiles_MC[numhists*i+j]->GetBinError(ibin+1) < minval)
	    minval = profiles_MC[numhists*i+j]->GetBinContent(ibin+1)-profiles_MC[numhists*i+j]->GetBinError(ibin+1);
	  if(profiles_MC[numhists*i+j]->GetBinContent(ibin+1)+profiles_MC[numhists*i+j]->GetBinError(ibin+1) > maxval)
	    maxval = profiles_MC[numhists*i+j]->GetBinContent(ibin+1)+profiles_MC[numhists*i+j]->GetBinError(ibin+1);
	}
      }
      
      if(minval < 0)
        {
          profiles_MC[numhists*i+j]->SetMaximum(maxval + 0.4*fabs(maxval-1.1*minval));
          profiles_MC[numhists*i+j]->SetMinimum(1.1*minval);
        }
      else
        {
          profiles_MC[numhists*i+j]->SetMaximum(maxval + 0.4*fabs(maxval-0.9*minval));
          profiles_MC[numhists*i+j]->SetMinimum(0.9*minval);
        }
    } //end loop j
    
  } //end loop i


  //Save plots in output file
  TString outFileName = "corrPlots";
  if (rwt) outFileName += "_rwt_";
  outFileName += catName;
  outFileName += ".root";

  TFile *outputFile = TFile::Open(outFileName,"RECREATE");
  outputFile->cd();
  for (int i = 0; i < numhists*numhists; ++i) {
    profiles_MC[i]->Write();
    profiles_Data[i]->Write();
  }

  outputFile->Close();


}
