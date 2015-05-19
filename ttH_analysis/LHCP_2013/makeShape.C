#include <cmath>      //necessary for absolute function fabs()     
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include "math.h"
#include <fstream>
#include <stdio.h>

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

//Utility class for managing binning information
class sampleInfo {
  
public:
  TString name;
  double  xs;
  double  nGen;
   
  sampleInfo() : name(""), xs(0), nGen(0) {}
  
  sampleInfo(TString sampleName,  double xsMC, double num_gen) :
    name (sampleName),  xs(xsMC), nGen(num_gen) {}
};

class categoryInfo {

public:
  TString label;
  TString cut;
  TString BTwgt;
  int     nBins;
  double  Xmin;
  double  Xmax;
  
  categoryInfo(): label(""), cut(""), BTwgt(""), nBins(0), Xmin(0), Xmax(0) {}

  categoryInfo(TString categoryLabel, TString cutString, TString BTString, int numBins, double binMin, double binMax) : 
  label(categoryLabel), cut(cutString), BTwgt(BTString), nBins(numBins), Xmin(binMin), Xmax(binMax){}

};



///////////////////////////
////////
//////////////////////////

void makeShape(double lumi = 5000, bool fakeData = false ){
  TH1::SetDefaultSumw2() ;
  //output file
  //  TString ofName = "mvaShapeAll_fixttbarV_lowZ.root" ;
  TString ofName = "mvaShapeAll";
  if(fakeData) ofName += "_fake" ;
  ofName +=".root";
  TFile * outputFile = new TFile(ofName, "RECREATE");

  /////// sample list
  std::vector<sampleInfo *> sampleList;
  sampleInfo * ttH120 = new sampleInfo("ttH120", 0.09756, 998833);
  sampleList.push_back(ttH120);


  sampleInfo * ttH100 = new sampleInfo("ttH100", 0.1638, 99885);
  sampleList.push_back(ttH100);

  sampleInfo * ttH105 = new sampleInfo("ttH105", 0.1433, 99898);
  sampleList.push_back(ttH105);

  sampleInfo * ttH110 = new sampleInfo("ttH110", 0.1257, 99880);
  sampleList.push_back(ttH110);

  sampleInfo * ttH115 = new sampleInfo("ttH115", 0.1106, 99915);
  sampleList.push_back(ttH115);

  sampleInfo * ttH125 = new sampleInfo("ttH125", 0.08634, 99896);
  sampleList.push_back(ttH125);

  sampleInfo * ttH130 = new sampleInfo("ttH130", 0.07658, 99915);
  sampleList.push_back(ttH130);

  sampleInfo * ttH135 = new sampleInfo("ttH135", 0.06810, 99898);
  sampleList.push_back(ttH135);

  sampleInfo * ttH140 = new sampleInfo("ttH140", 0.06072, 99909);
  sampleList.push_back(ttH140);


  /////////////////// bkg 
//   sampleInfo * ttbar = new sampleInfo("ttbar", 157.7, 52135272);
//   sampleList.push_back(ttbar);

  sampleInfo * tt = new sampleInfo("ttbar", 157.7, 52135272);
  sampleList.push_back(tt);

  sampleInfo * ttbb = new sampleInfo("ttbarPlusBBbar", 157.7, 52135272);
  sampleList.push_back(ttbb);

  sampleInfo * ttcc = new sampleInfo("ttbarPlusCCbar", 157.7, 52135272);
  sampleList.push_back(ttcc);

  sampleInfo * zjets_h = new sampleInfo("zjets_h", 3048, 35891264);
  sampleList.push_back(zjets_h);

  sampleInfo * zjets_low = new sampleInfo("zjets_lowmass", 12782.6, 31480628);
  sampleList.push_back(zjets_low);

  sampleInfo * WW = new sampleInfo("WW", 43, 4225916);
  sampleList.push_back(WW);

  sampleInfo * WZ = new sampleInfo("WZ", 18.2, 4265241);
  sampleList.push_back(WZ);

  sampleInfo * ZZ = new sampleInfo("ZZ", 5.9, 4191045);
  sampleList.push_back(ZZ);

  //////
  sampleInfo * st_s = new sampleInfo("singlet_s", 3.17, 259595);
  sampleList.push_back(st_s);

  sampleInfo * st_t = new sampleInfo("singlet_t", 41.92, 3891841);
  sampleList.push_back(st_t);

  sampleInfo * st_tW = new sampleInfo("singlet_tW", 7.87, 812600);
  sampleList.push_back(st_tW);

  sampleInfo * stbar_s = new sampleInfo("singletbar_s", 1.44, 137662);
  sampleList.push_back(stbar_s);

  sampleInfo * stbar_t = new sampleInfo("singletbar_t", 22.65, 1939703);
  sampleList.push_back(stbar_t);

  sampleInfo * stbar_tW = new sampleInfo("singletbar_tW", 7.87, 808200);
  sampleList.push_back(stbar_tW);
  ///
  sampleInfo * ttW = new sampleInfo("ttbarW", 0.163, 1085456);
  sampleList.push_back(ttW);

  sampleInfo * ttZ = new sampleInfo("ttbarZ", 0.136, 1458573);
  sampleList.push_back(ttZ);

  sampleInfo * wjets = new sampleInfo("wjets", 31314, 81011945);
  sampleList.push_back(wjets);

  /////// category list
  std::vector<categoryInfo *> catList;
  categoryInfo * ge3t = new categoryInfo("ge3t","numTaggedJets>=3", "probge3", 75, 0.05, 0.8);
  catList.push_back(ge3t);

  categoryInfo * e2je2t = new categoryInfo("e2je2t","numJets==2 && numTaggedJets==2", "prob2", 40, 0.0072, 0.0074);
  catList.push_back(e2je2t);

  const unsigned int NumCat = catList.size();

//   /////
//   std::vector< TString > btagList ;
//   btagList.push_back("probge3");
//   btagList.push_back("prob2");

  ///// sub category                                                                                                      
  std::vector< TString > lepList ;                                                                                        
  lepList.push_back("TwoMuon");                                                                                           
  lepList.push_back("TwoEle");                                                                                            
  lepList.push_back("MuonEle"); 

  ///////////////////////////////////////
  std::vector< TString > sysList ;
  sysList.push_back("CMS_scale_jUp");
  sysList.push_back("CMS_scale_jDown");
  sysList.push_back("");
  sysList.push_back("PUUp");
  sysList.push_back("PUDown");
  sysList.push_back("CMS_eff_bUp");
  sysList.push_back("CMS_eff_bDown");
  sysList.push_back("CMS_fake_bUp");
  sysList.push_back("CMS_fake_bDown");
  const unsigned int NumSys = sysList.size();

  ////////// prepare the weight
  TString weight[NumCat][NumSys];
  for ( unsigned int jcat = 0 ; jcat < NumCat ; ++jcat ) {
    weight[jcat][0] = "weight*prob"  ;
    weight[jcat][1] = "weight*prob"  ;
    weight[jcat][2] = "weight*prob"  ;

    weight[jcat][3] = "weight_PUup*prob"  ;
    weight[jcat][4] = "weight_PUdown*prob"  ;

    weight[jcat][5] = "weight*prob_hfSFup";
    weight[jcat][6] = "weight*prob_hfSFdown";
    weight[jcat][7] = "weight*prob_lfSFup";
    weight[jcat][8] = "weight*prob_lfSFdown";
  }

  TH1F * datahist[NumCat] ;
  TH1F * diBosonhist[NumCat][NumSys] ;
  TH1F * sthist[NumCat][NumSys] ;
  TH1F * zhist[NumCat][NumSys] ;
  //////////// start sample loop
  for (std::vector<sampleInfo *>::const_iterator tIter = sampleList.begin();
       tIter != sampleList.end(); ++tIter) {
    
    const sampleInfo *samInfo = *tIter; 
    TString nameTemp =    samInfo->name;
    double xsTemp =   samInfo->xs;
    double nGenTemp = samInfo->nGen;

//     TString samfileName = "output/mva_" + name ;
//     samfileName += ".root" ;
//     TFile * inFile = new TFile (samfileName);
 
    std::cout << "======starting processing sample " << nameTemp << std::endl;

    //////loop over systematics
    for (unsigned int ksys = 0 ; ksys < NumSys ; ++ksys ){
      TString sys = sysList[ksys];

      TString  sigFName = "output/mva_" + nameTemp ;      
      if (ksys == 0) {
	sigFName += "_JESUp.root" ;
      }
      else if (ksys == 1) {
	sigFName += "_JESDown.root" ;
      }
      else  sigFName+= ".root" ;
      
      std::cout << "  ====starting processing systematic " << sys << std::endl;
      std::cout << "input File is " << sigFName << std::endl;    
            
      TFile * sigFile = new TFile (sigFName);
      TTree * sigTree = (TTree *) sigFile->Get("summaryTree");
            
      
      /////loop over categories
      int jcat = 0 ;
      //      for( unsigned int jcat = 0 ; jcat < NumCat ; ++jcat ){
      for (std::vector<categoryInfo *>::const_iterator cIter = catList.begin(); cIter != catList.end(); ++cIter) {

	const categoryInfo *catInfo = *cIter;

	TString label = catInfo->label ; 
	TString cut = catInfo->cut ; 
	int nBins = catInfo->nBins ;
	double Xmin = catInfo->Xmin ;
	double Xmax = catInfo->Xmax ;
	
	TString sigName =  nameTemp +"_CFMlpANN_"  + label ;
	
	if (sys != ""){
	  sigName += "_" ;
	  sigName += sys ;
	}

        TH1F *sigHistSub[3];                                                                                               
        //      TH1F *sigHist = new TH1F(sigName, "mva output" , nBins, Xmin, Xmax);                                       
                                                                                                                           
        for (unsigned int nlep = 0 ; nlep < 3 ; ++nlep ){                                                                  
          TString lepcut = lepList[nlep];                                                                                  
          TString sigNameSub = sigName + lepList[nlep] ;                                                                   
                                                                                                                           
          sigHistSub[nlep] = new TH1F(sigNameSub, "mva output" , nBins, Xmin, Xmax);                                       
                                                                                                                           
          //cut                                                                                                            
          TString selection = "PassTwoLepton && isTriggerPass && isCleanEvent && dR_leplep>0.2 && mass_leplep>12 && " + cut;          
          selection += " && " ;                                                                                            
          selection += lepcut ;                                                                                            
	  
          TString allcuts = weight[jcat][ksys] + "*(" ;                                                                    
          allcuts += selection ;                                                                                           
          allcuts += ")";                                                                                                  
	  //// trig and Lep Eff
	  if ( nlep == 0 ) allcuts += "* 0.9885 * 0.9885 * 0.987 * 0.987" ;                                                               
	  else if ( nlep == 1 ) allcuts += "* 1.004 * 1.004" ;                                                                
	  else allcuts += "* 0.9885 * 0.987 * 1.004" ;                                                                           
                                                                                                                           
          std::cout << " --> cut is " << allcuts << std::endl;                                                             
                                                                                                                           
          TString varSig = "CFMlpANN_" + label + ">>" + sigNameSub;                                                        
          sigTree->Draw(varSig, allcuts, "goff");                                                                          
          sigHistSub[nlep]->Scale(lumi*xsTemp/nGenTemp);                                                                   

        }                                                                                                                  
        TH1F *sigHist = (TH1F*)sigHistSub[0]->Clone(sigName);                                                              
        sigHist->Add(sigHistSub[1]);                                                                                       
        sigHist->Add(sigHistSub[2]);

	sigHist->SetDirectory(outputFile);
	
	///////// fake data
	if(fakeData){
	  if (sys == ""){
	    if (nameTemp == "ttH120") {
	      datahist[jcat] = (TH1F *)sigHist->Clone("data_obs_CFMlpANN_" + label);
	      datahist[jcat]->SetDirectory(outputFile);
	    }
	    else if ( !nameTemp.Contains("ttH1") )  datahist[jcat]->Add(sigHist);
	  }
	}

	//// combine Di-boson
	TString diBosonName = "diboson_CFMlpANN_" + label ;
	if (sys != ""){
	  diBosonName += "_" ;
	  diBosonName += sys ;
	}
	if (nameTemp == "WW"){
	  diBosonhist[jcat][ksys] = (TH1F *)sigHist->Clone(diBosonName);
	  diBosonhist[jcat][ksys]->SetDirectory(outputFile);
	}
	else if (nameTemp == "WZ" || nameTemp == "ZZ") diBosonhist[jcat][ksys]->Add(sigHist);

	//// combine single top
	TString stName = "singlet_CFMlpANN_" + label ;
	if (sys != ""){
	  stName += "_" ;
	  stName += sys ;
	}
	if (nameTemp == "singlet_s"){
	  sthist[jcat][ksys] = (TH1F *)sigHist->Clone(stName);
	  sthist[jcat][ksys]->SetDirectory(outputFile);
	}
	else if (nameTemp.Contains("singlet")) sthist[jcat][ksys]->Add(sigHist);

	//// combine zjets
	TString zName = "zjets_CFMlpANN_" + label ;
	if (sys != ""){
	  zName += "_" ;
	  zName += sys ;
	}
	if (nameTemp == "zjets_h"){
	  zhist[jcat][ksys] = (TH1F *)sigHist->Clone(zName);
	  zhist[jcat][ksys]->SetDirectory(outputFile);
	}
	else if (nameTemp.Contains("zjets")) zhist[jcat][ksys]->Add(sigHist);


	jcat++ ;

      } // end category loop
      
      sigFile->Close();
      
      std::cout << "Done with systematic " << sys << std::endl; 
      std::cout << '\n' ;
      std::cout << '\n' ;
    }  // end sys loop
    
  } // end sample loop
  
  ////----------------- handle real data
  if ( !fakeData ){
    std::vector<sampleInfo *> dataList;  
    sampleInfo * DiMuon = new sampleInfo("data_DiMuon", 1, 1);
    dataList.push_back(DiMuon);

    sampleInfo * DiEle = new sampleInfo("data_DiEle", 1, 1);
    dataList.push_back(DiEle);
    
    sampleInfo * MuEG = new sampleInfo("data_MuEG", 1, 1);
    dataList.push_back(MuEG);

    int iData = 0 ;
    for (std::vector<sampleInfo *>::const_iterator tIter = dataList.begin();
       tIter != dataList.end(); ++tIter) {
      
      const sampleInfo *samInfo = *tIter; 
      TString nameTemp =    samInfo->name;
//       double xsTemp =   samInfo->xs;
//       double nGenTemp = samInfo->nGen;
      
      std::cout << "======starting processing " << nameTemp << std::endl;

      TString  sigFName = "output/mva_" + nameTemp + ".root";      
      TFile * sigFile = new TFile (sigFName);
      TTree * sigTree = (TTree *) sigFile->Get("summaryTree");
      
      //     TString  FName2 = "output/mva_data_DiEle.root" ;
      //     TFile * File2 = new TFile (FName2);
      //     TTree * Tree2 = (TTree *) File2->Get("summaryTree");
      
      //     TString  FName3 = "output/mva_data_MuEG.root" ;
      //     TFile * File3 = new TFile (FName3);
      //     TTree * Tree3 = (TTree *) File3->Get("summaryTree");
      
      /////loop over categories
      int jcat = 0 ;
      for (std::vector<categoryInfo *>::const_iterator cIter = catList.begin(); cIter != catList.end(); ++cIter) {
	
	const categoryInfo *catInfo = *cIter;
	
	TString label = catInfo->label ; 
	TString cut = catInfo->cut ; 
	int nBins = catInfo->nBins ;
	double Xmin = catInfo->Xmin ;
	double Xmax = catInfo->Xmax ;
	
	std::cout << " ====starting processing category " << label << std::endl;
	
	TString sigName =  nameTemp +"_CFMlpANN_"  + label ;
	TH1F *sigHist = new TH1F(sigName, "mva output" , nBins, Xmin, Xmax);
	
	TString selection = "PassTwoLepton && isTriggerPass && isCleanEvent && dR_leplep>0.2 && mass_leplep>12 && " + cut;  
	if (iData == 0) selection += " && TwoMuon";  
	if (iData == 1) selection += " && TwoEle";  
      	if (iData == 2) selection += " && MuonEle";  

	std::cout << " --> cut is " << selection << std::endl;
	
	TString varSig = "CFMlpANN_" + label + ">>" + sigName;
	sigTree->Draw(varSig, selection, "goff");

	if(iData == 0){
	  datahist[jcat] = (TH1F *)sigHist->Clone("data_obs_CFMlpANN_" + label);
	  datahist[jcat]->SetDirectory(outputFile);
	}
	else datahist[jcat]->Add(sigHist);

	jcat++ ;

      }

      sigFile->Close();
      iData++;
    }
  }    

  ////// add in ttbar scale sample
//   std::vector<sampleInfo *> ttbarscaleList;  
//   sampleInfo * tt_scaleup = new sampleInfo("ttbar", 157.7, 3691845);
//   ttbarscaleList.push_back(tt_scaleup);

//   sampleInfo * ttbb_scaleup = new sampleInfo("ttbar_bb", 157.7, 3691845);
//   ttbarscaleList.push_back(ttbb_scaleup);

//   sampleInfo * ttcc_scaleup = new sampleInfo("ttbar_cc", 157.7, 3691845);
//   ttbarscaleList.push_back(ttcc_scaleup);

//   sampleInfo * tt_scaledown = new sampleInfo("ttbar", 157.7, 4000585);
//   ttbarscaleList.push_back(tt_scaledown);

//   sampleInfo * ttbb_scaledown = new sampleInfo("ttbar_bb", 157.7, 4000585);
//   ttbarscaleList.push_back(ttbb_scaledown);

//   sampleInfo * ttcc_scaledown = new sampleInfo("ttbar_cc", 157.7, 4000585);
//   ttbarscaleList.push_back(ttcc_scaledown);

//   /////
//   std::vector< TString > scalelabelList ;
//   scalelabelList.push_back("ttbarScaleUp");
//   scalelabelList.push_back("ttbar_bbScaleUp");
//   scalelabelList.push_back("ttbar_ccScaleUp");
//   scalelabelList.push_back("ttbarScaleDown");
//   scalelabelList.push_back("ttbar_bbScaleDown");
//   scalelabelList.push_back("ttbar_ccScaleDown");

  ///////////
//   int iscale = 0 ;
//   for (std::vector<sampleInfo *>::const_iterator tIter = ttbarscaleList.begin();
//        tIter != ttbarscaleList.end(); ++tIter) {

//     const sampleInfo *samInfo = *tIter; 
//     TString nameTemp =    samInfo->name;
//     double xsTemp =   samInfo->xs;
//     double nGenTemp = samInfo->nGen;
 
//     TString  sigFName = "output/mva_" + nameTemp ;      
//     if (iscale < 3) sigFName += "_scaleup";
//     else  sigFName += "_scaledown";
//     sigFName+= ".root" ;
//     TFile * sigFile = new TFile (sigFName);
//     TTree * sigTree = (TTree *) sigFile->Get("summaryTree");

//     std::cout << "======starting processing sample " << nameTemp << std::endl;
//     std::cout << "input File is " << sigFName << std::endl;    

//     /////
//     /////loop over categories
//     int jcat = 0 ;

//     for (std::vector<categoryInfo *>::const_iterator cIter = catList.begin(); cIter != catList.end(); ++cIter) {
      
//       const categoryInfo *catInfo = *cIter;
      
//       TString label = catInfo->label ; 
//       TString cut = catInfo->cut ; 
//       int nBins = catInfo->nBins ;
//       double Xmin = catInfo->Xmin ;
//       double Xmax = catInfo->Xmax ;
      
//       TString sigName =  nameTemp +"_CFMlpANN_"  + label ;
//       sigName += "_";
//       sigName += scalelabelList[iscale] ;    
      
//       TH1F *sigHist = new TH1F(sigName, "mva output" , nBins, Xmin, Xmax);
      
//       //cut
//       TString selection = "PassTwoLepton && isTriggerPass && isCleanEvent && dR_leplep>0.2 && mass_leplep>12 && " + cut;      
//       //      TString selection = "PassTwoLepton && isTriggerPass && isCleanEvent && " + cut;
//       TString allcuts = weight[jcat][2] + "*(" ;
//       allcuts += selection ;
//       allcuts += ")";

//       std::cout << " --> cut is " << allcuts << std::endl;
      
//       TString varSig = "CFMlpANN_" + label + ">>" + sigName;
//       sigTree->Draw(varSig, allcuts, "goff");
//       sigHist->Scale(lumi*xsTemp/nGenTemp);
      
//       //  sigHist->Sumw2(); 
//       //  bkgHist->Sumw2(); 
//       sigHist->SetDirectory(outputFile);
	
//       jcat++ ;
//     }

//     iscale++ ;
//     sigFile->Close();
//   }
  
  //-----------------------
  outputFile->Write();    
  outputFile->Close();
  
  std::cout << "Done! shapes saved to " << ofName << endl;
  
}


    //   // Add overflow to last bin   
    //   int numBins = newHist->GetNbinsX();
    //   double overFlow = newHist->GetBinContent(numBins+1);
    //   newHist->AddBinContent(numBins, overFlow);


