#include <cmath>      //necessary for absolute function fabs()     
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include "math.h"
#include <fstream>
#include <stdio.h>
#include <algorithm>

//Root includes                                   
#include "TROOT.h"
#include "Riostream.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TTree.h"
#include "TString.h"
#include "TMath.h"
#include "TAxis.h"


class varInfo {

public:
  TString var;
  TString hName;
  TString hTitle;
  int nBins;
  int min;
  int max;

  TH1F * myHist;
  Float_t * myValuePointer;

  varInfo(): var(""), hName(""), hTitle(""), nBins(0), min(0), max(0), myHist(0), myValuePointer(0) {}

  varInfo(TString varName, TString histName, TString histTitle, int numBins, int binMin, int binMax) : 
    var(varName), hName(histName), hTitle(histTitle), nBins(numBins), min(binMin), max(binMax),
    myHist(0), myValuePointer(0)
  {}

  // no setters/getters, all values are public

  void bookHist(TFile * outputFile) {
    myHist = new TH1F ( hName, hTitle, nBins, min, max);
    myHist->SetDirectory(outputFile);
  }
  void fillHist() {
    myHist->Fill(*myValuePointer);
  }

};

void printAccTable_Andrew_Dilepton_v1 () {

  //----------------------------------
  //
  //  Global job params
  //  File names, etc
  //---------------------------------
  
  std::vector<string> InputFileNames;
  std::string InputDirName = "./";
  //InputFileName format: "batchBEAN/"+InputFileName+"/test_beans_v1_"+InputFileName+"_ALL.root"

  InputFileNames.push_back("test_beans_v1_TTH_HtoAll_M_120_7TeV_pythia6__ALL.root");
//   InputFileNames.push_back("BEAN_V05_CV02_toND_v4_DoubleMu_Run2011A-05Aug2011-v1_");
//   InputFileNames.push_back("BEAN_V05_CV02_toND_v4_DoubleMu_Run2011A-PromptReco-v6_");
//   InputFileNames.push_back("BEAN_V05_CV02_toND_v4_DoubleMu_Run2011B-PromptReco-v1_");
//   InputFileNames.push_back("DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_");
//   InputFileNames.push_back("TTH_HtoAll_M_120_7TeV_pythia6_");
//   InputFileNames.push_back("TTJets_TuneZ2_7TeV-madgraph-tauola_");
//   InputFileNames.push_back("WW_TuneZ2_7TeV_pythia6_tauola_");
//   InputFileNames.push_back("WZ_TuneZ2_7TeV_pythia6_tauola_");
//   InputFileNames.push_back("ZZ_TuneZ2_7TeV_pythia6_tauola_");

  std::vector<varInfo*> varList;
  
  varInfo *Ht = new varInfo("Ht", "Ht", "Ht", 1000, 0, 1000);
  varList.push_back(Ht);
  
  varInfo *M2_of_closest_tagged_jets = new varInfo("M2_of_closest_tagged_jets", "M2_of_closest_tagged_jets", "M2_of_closest_tagged_jets", 1000, 0, 1000);
  varList.push_back(M2_of_closest_tagged_jets);
  
//   varInfo *all_sum_pt = new varInfo("all_sum_pt", "all_sum_pt", "all_sum_pt", 10000, 0, 10000);
//   varList.push_back(all_sum_pt);
//   varInfo *avg_tagged_dijet_mass = new varInfo("avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 1000, 0, 1000);
//   varList.push_back(avg_tagged_dijet_mass);
//   varInfo *avg_untagged_dijet_mass = new varInfo("avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 1000, 0, 1000);
//   varList.push_back(avg_untagged_dijet_mass);
//   varInfo *closest_tagged_dijet_mass = new varInfo("closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 1000, 0, 1000);
//   varList.push_back(closest_tagged_dijet_mass);
//   varInfo *first_jet_pt = new varInfo("first_jet_pt", "first_jet_pt", "first_jet_pt", 1000, 0, 1000);
//   varList.push_back(first_jet_pt);
//   varInfo *fourth_jet_pt = new varInfo("fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 1000, 0, 1000);
//   varList.push_back(fourth_jet_pt);
//   varInfo *fstElePt = new varInfo("fstElePt", "fstElePt", "fstElePt", 1000, 0, 1000);
//   varList.push_back(fstElePt);
//   varInfo *fstMuPt = new varInfo("fstMuPt", "fstMuPt", "fstMuPt", 1000, 0, 1000);
//   varList.push_back(fstMuPt);
//   varInfo *lep1Pt = new varInfo("lep1Pt", "lep1Pt", "lep1Pt", 1000, 0, 1000);
//   varList.push_back(lep1Pt);
//   varInfo *lep2Pt = new varInfo("lep2Pt", "lep2Pt", "lep2Pt", 1000, 0, 1000);
//   varList.push_back(lep2Pt);
//   varInfo *mass_leplep = new varInfo("mass_leplep", "mass_leplep", "mass_leplep", 1000, 0, 1000);
//   varList.push_back(mass_leplep);
//   varInfo *mass_of_everything = new varInfo("mass_of_everything", "mass_of_everything", "mass_of_everything", 10000, 0, 10000);
//   varList.push_back(mass_of_everything);
//   varInfo *met = new varInfo("met", "met", "met", 1000, 0, 1000);
//   varList.push_back(met);
//   varInfo *pt_leplep = new varInfo("pt_leplep", "pt_leplep", "pt_leplep", 1000, 0, 1000);
//   varList.push_back(pt_leplep);
//   varInfo *pt_of_everything = new varInfo("pt_of_everything", "pt_of_everything", "pt_of_everything", 10000, 0, 10000);
//   varList.push_back(pt_of_everything);
//   varInfo *second_jet_pt = new varInfo("second_jet_pt", "second_jet_pt", "second_jet_pt", 1000, 0, 1000);
//   varList.push_back(second_jet_pt);
//   varInfo *sndElePt = new varInfo("sndElePt", "sndElePt", "sndElePt", 1000, 0, 1000);
//   varList.push_back(sndElePt);
//   varInfo *sndMuPt = new varInfo("sndMuPt", "sndMuPt", "sndMuPt", 1000, 0, 1000);
//   varList.push_back(sndMuPt);
//   varInfo *sum_pt = new varInfo("sum_pt", "sum_pt", "sum_pt", 10000, 0, 10000);
//   varList.push_back(sum_pt);
//   varInfo *third_jet_pt = new varInfo("third_jet_pt", "third_jet_pt", "third_jet_pt", 1000, 0, 1000);
//   varList.push_back(third_jet_pt);
//   varInfo *unc_met = new varInfo("unc_met", "unc_met", "unc_met", 1000, 0, 1000);
//   varList.push_back(unc_met);

  const unsigned int nInputFiles = InputFileNames.size();
  for( unsigned int i = 0; i < nInputFiles; i++) {

    std::string InputFileLabel = InputFileNames[i];

    //TString InputFileName = InputDirName+InputFileLabel+"/test_beans_v1_"+InputFileLabel+"_ALL.root";
    TString InputFileName = InputFileNames[i];

    std::cout << "==== Processing File " << InputFileName << " ==== " << std::endl;
    TFile * DileptonFile = new TFile(InputFileName);
    TTree * DileptonSummaryTree = (TTree *) DileptonFile->Get("summaryTree");

    //TString OutputFileName = InputFileLabel+".root";
    TString OutputFileName = "test_VariableLoop.root";
    TFile * OutputFile = new TFile(OutputFileName, "RECREATE");

    //double Dilepton_numEvents = DileptonSummaryTree->GetEntries();

    OutputFile->cd();

    //std::vector<Float_t*> FillValues;
    //std::vector<TH1F*> Histograms;
    unsigned int j = 0;

    //Float_t * testValue = new Float_t(0.0);
    //std::cout <<"Test value has value " << *testValue << std::endl;
    //DileptonSummaryTree->SetBranchAddress( "Ht", testValue );


    for(std::vector<varInfo*>::iterator tIter1 = varList.begin(); tIter1 != varList.end(); tIter1++) {
     
      // Pass a pointer to variable info
      varInfo *u = *tIter1;

      // Declare histogram for that variable
      u->bookHist(OutputFile);       
      u->myValuePointer = new Float_t(0.0);
      

      std::cout <<"Setting Branch Address for " << u->hName << std::endl;

      DileptonSummaryTree->SetBranchAddress( u->hName, u->myValuePointer );

      j++;
    }

    //const unsigned int nVariables = varList.size();

    bool debugPrint = false;
    //for (Long64_t ievt=0; ievt < 10; ievt++) {
    for (Long64_t ievt=0; ievt < DileptonSummaryTree->GetEntries(); ievt++) {

      if (ievt%10000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

      if (debugPrint) std::cout <<"Trying to get entry" << std::endl;
      DileptonSummaryTree->GetEntry(ievt);
      if (debugPrint) std::cout <<"Got it OK" << std::endl;

      

      for(std::vector<varInfo*>::iterator tIter1 = varList.begin(); tIter1 != varList.end(); tIter1++) {

        varInfo * iVar = *tIter1;
        if (debugPrint) std::cout << "Value is for iVar=" << iVar->hName <<" is " << (*(iVar->myValuePointer)) << std::endl;
        if (debugPrint) std::cout << "Trying to fill histogram iVar=" << iVar->hName << " title " << iVar->myHist->GetTitle() << std::endl;
        iVar->fillHist();
      }

      

    }

  OutputFile->Write();
  std::cout << "Wrote out " << OutputFileName << std::endl;
  OutputFile->Close();
  } // end files loop
  // Write and close output file


}
