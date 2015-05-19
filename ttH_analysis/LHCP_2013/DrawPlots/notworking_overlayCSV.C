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

#include <cmath>
#include <algorithm>

void overlayCSV () {
  ////////
//   TString ofName = "csv_rwt.root";
//   TFile * outputFile = new TFile(ofName, "RECREATE");
  ////////


  TString varName = "jet_CSV_unc"; //Variable which the scale factor is a function of
  TString varName1 = "first_jet_CSV_unc"; //Variable which the scale factor is a function of
  TString varName2 = "second_jet_CSV_unc"; //Variable which the scale factor is a function of

  double Xmin = 0; //Lower bound for x-axis
  double Xmax = 1.0; //Upper bound for x-axis
  int nBins = 100; //15; //Number of bins
  //  Double_t xBins[16] = {0,0.061,0.122,0.183,0.244,0.389,0.534,0.679,0.752,0.825,0.898,0.932,0.966,0.99,0.999,1.0}; //Binning

  ///////////// selection
  TString WP = "000"; //100x probe jet working point - loose (224), medium (679), tight (898)
  TString lfSF = "10"; //10x Scale factor for light-flavor backgrounds (single top, Z+jets). 10 is nominal, 15 is 50% higher, 5 is 50% lower.
  bool EfficiencySF = false; //Set to true if you are measuring the efficiency for different CSV working points
  TString pt_lower = "0";
  TString pt_upper = "10000";
  TString eta_lower = "0.0";
  TString eta_upper = "2.4";
  vector<TString> ptBins; 
  ptBins.push_back("30");
  ptBins.push_back("40");
//   ptBins.push_back("60");
//   ptBins.push_back("100");
//   ptBins.push_back("160");
//   ptBins.push_back("10000");
  unsigned int nPt = ptBins.size();
  vector<TString> etaBins;
  etaBins.push_back("0.0");
  etaBins.push_back("0.8");
  etaBins.push_back("1.6");
  etaBins.push_back("2.4");
  unsigned int nEta = etaBins.size();


  /////////// book the histograms 
  int nDiv = (nPt-1)*(nEta-1);
  TH1F** csv_Data = new TH1F * [nDiv];
  TH1F** csv_MC = new TH1F * [nDiv];
  TH1F** csv_ratio = new TH1F * [nDiv];

//   for (unsigned int iPt = 0; iPt < nPt-1; iPt++){
//     for (unsigned int iEta = 0; iEta < nEta-1; iEta++){
//       unsigned int j = iPt*(nEta-1) + iEta;
//       csv_Data[j] = new TH1F(Form("csv_Data_Pt%i_Eta%i",iPt,iEta),"",nBins,Xmin,Xmax);
//      csv_Data[j]->Sumw2();
//      csv_Data[j]->SetDirectory(outputFile);

//      csv_MC[j] = new TH1F(Form("csv_MC_Pt%i_Eta%i",iPt,iEta),"",nBins,Xmin,Xmax);
//      csv_MC[j]->Sumw2();
//      csv_MC[j]->SetDirectory(outputFile);

//      csv_ratio[j] = new TH1F(Form("csv_ratio_Pt%i_Eta%i",iPt,iEta),"",nBins,Xmin,Xmax);
//      csv_ratio[j]->Sumw2();
//      csv_ratio[j]->SetDirectory(outputFile);
//     }
//   }

  TDirectory *currentDir = gDirectory;


  int jDiv = 0;
  //////// loop over pt and eta bins
  for (unsigned int iPt = 0; iPt < nPt-1; iPt++){
    pt_lower = ptBins[iPt];
    pt_upper = ptBins[iPt+1];
    cout << "pt range is " << pt_lower << " to " << pt_upper << endl;
    for (unsigned int iEta = 0; iEta < nEta-1; iEta++){
      eta_lower = etaBins[iEta];
      eta_upper = etaBins[iEta+1];    
      cout << "eta range is " << eta_lower << " to " << eta_upper << endl;

  TFile * sourceFile1a = new TFile ("/afs/crc.nd.edu/user/a/abrinke1/BEANsHelper/CMSSW_5_3_2_patch5/src/BEAN/DrawPlots/bin/TwoMuon.root");
  TFile * sourceFile1b = new TFile ("/afs/crc.nd.edu/user/a/abrinke1/BEANsHelper/CMSSW_5_3_2_patch5/src/BEAN/DrawPlots/bin/TwoEle.root");
  TFile * sourceFile1c = new TFile ("/afs/crc.nd.edu/user/a/abrinke1/BEANsHelper/CMSSW_5_3_2_patch5/src/BEAN/DrawPlots/bin/MuonEle.root");
  TFile * sourceFile2 = new TFile  ("/afs/crc.nd.edu/user/a/abrinke1/BEANsHelper/CMSSW_5_3_2_patch5/src/BEAN/DrawPlots/bin/allMC.root");
  TTree * Tree1a = (TTree*) sourceFile1a->Get("summaryTree");
  TTree * Tree1b = (TTree*) sourceFile1b->Get("summaryTree");
  TTree * Tree1c = (TTree*) sourceFile1c->Get("summaryTree");
  TTree * Tree2 = (TTree*) sourceFile2->Get("summaryTree");



      currentDir->cd();
      csv_Data[jDiv] = new TH1F(Form("csv_Data_Pt%i_Eta%i",iPt,iEta),"",nBins,Xmin,Xmax);
     csv_Data[jDiv]->Sumw2();
     csv_MC[jDiv] = new TH1F(Form("csv_MC_Pt%i_Eta%i",iPt,iEta),"",nBins,Xmin,Xmax);
     csv_MC[jDiv]->Sumw2();
     csv_ratio[jDiv] = new TH1F(Form("csv_ratio_Pt%i_Eta%i",iPt,iEta),"",nBins,Xmin,Xmax);
     csv_ratio[jDiv]->Sumw2();

  TString lepselection1a = "(TwoMuon && isDoubleMuTriggerPass && PassZmask2==1 && met>50)"; //Selection for TwoMuon data events
  TString lepselection1b = "(TwoEle && isDoubleElectronTriggerPass && PassZmask2==1 && met>50)"; //Selection for TwoEle data events
  TString lepselection1c = "(MuonEle && isMuEGTriggerPass)"; //Selection for MuonEle data events
  //Selection for MC events
  TString lepselection2 = "( (TwoMuon && isDoubleMuTriggerPass && PassZmask2==1 && met>50) || (TwoEle && isDoubleElectronTriggerPass && PassZmask2==1 && met>50) || (MuonEle && isMuEGTriggerPass) ) ";
  TString jetselection1a = "(numJets == 2 && second_jet_CSV_unc > 0.679)"; //Tag jet requirement for probing first_jet
  TString jetselection1b = "(numJets == 2 && first_jet_CSV_unc > 0.679)"; //Tag jet requirement for probing second_jet
  TString jetselection2a = "(numJets == 2 && second_jet_CSV_unc > 0.679 "; //Probe jet requirement for first_jet
  jetselection2a += " && first_jet_CSV_unc > 0."+WP;
  jetselection2a += " && first_jet_pt >= "+pt_lower+" && first_jet_pt < "+pt_upper;
  jetselection2a += " && abs(first_jet_eta) >= "+eta_lower+" && abs(first_jet_eta) < "+eta_upper+")";
  TString jetselection2b = "(numJets == 2 && first_jet_CSV_unc > 0.679 "; //Probe jet requirement for second_jet
  jetselection2b += " && second_jet_CSV_unc > 0."+WP;
  jetselection2b += " && second_jet_pt >= "+pt_lower+" && second_jet_pt < "+pt_upper;
  jetselection2b += " && abs(second_jet_eta) >= "+eta_lower+" && abs(second_jet_eta) < "+eta_upper+")";
  TString trigselection = "((dR_leplep > 0.2) && (mass_leplep > 12) && (isCleanEvent == 1) && (oppositeLepCharge == 1))"; //General dilepton selection
  TString nonttbar = "( (Xsec>24.56 && Xsec<=24.57)*1 + (!(Xsec>24.56 && Xsec<=24.57))*("+lfSF+")*0.1)*"; //Scaling of the non-ttbar_ll background component
  TString lumi = "19450.0*"; //Nominal luminosity
  
  ///////////  

  TCanvas * can1a = new TCanvas ("can1a", "can1a");

  //DoubleMu
//   TH1F *histTemp3 = new TH1F("histTemp3", "histTemp3", nBins, Xmin, Xmax); //varName1, jetselection2a
//   TH1F *histTemp4 = new TH1F("histTemp4", "histTemp4", nBins, Xmin, Xmax); //varName2, jetselection2b
//   histTemp3->Sumw2();
//   histTemp4->Sumw2();
  currentDir->cd();
  TString proName =  Form("tmp_csv_Data_Pt%i_Eta%i",iPt,iEta);
  cout << "nPV" << jDiv << ", iPt:iEta " << iPt << ":" << iEta << endl;
  cout << "proName " << proName << endl;

  TH1 *tmpHisto = new TH1F(proName,"",nBins,Xmin,Xmax);
  //  tmpHisto->Sumw2();
  //  tmpHisto->SetDirectory(outputFile);
  cout << "tree 1a entries " << Tree1a->GetEntries() << endl;
  currentDir->cd();
Tree1a->Draw(varName1+" >> proName","weight*(Xsec/nGen)*("+lepselection1a+ " && "+trigselection+" && "+jetselection2a+")","");
currentDir->cd();
Tree1a->Draw(varName2+" >>+ proName","weight*(Xsec/nGen)*("+lepselection1a+ " && "+trigselection+" && "+jetselection2b+")","");
 cout << "cut is"  << lepselection1a << " && "<< trigselection <<" && " << jetselection2a << endl;
 // TH1F* tmpHisto = (TH1F*)gDirectory->GetObject(*tmpHisto1); 
  cout << "DoubleMu cnt is " << tmpHisto->Integral() << endl;
  //DoubleElectron
//   TH1F *histTemp7 = new TH1F("histTemp7", "histTemp7", nBins, Xmin, Xmax); //varName1, jetselection2a
//   TH1F *histTemp8 = new TH1F("histTemp8", "histTemp8", nBins, Xmin, Xmax); //varName2, jetselection2b
//   histTemp7->Sumw2();
//   histTemp8->Sumw2();
  TH1 * sourceHisto7 = (TH1*) Tree1b->Draw(varName1+" >>+ proName","weight*(Xsec/nGen)*("+lepselection1b+ " && "+trigselection+" && "+jetselection2a+")","");
  TH1 * sourceHisto8 = (TH1*) Tree1b->Draw(varName2+" >>+ proName","weight*(Xsec/nGen)*("+lepselection1b+ " && "+trigselection+" && "+jetselection2b+")","");
  //   cout << "DoubleEle cnt is " << tmpHisto->Integral() << endl;
  //MuEG
//   TH1F *histTemp11 = new TH1F("histTemp11", "histTemp11", nBins, Xmin, Xmax); //varName1, jetselection2a
//   TH1F *histTemp12 = new TH1F("histTemp12", "histTemp12", nBins, Xmin, Xmax); //varName2, jetselection2b
//   histTemp11->Sumw2();
//   histTemp12->Sumw2();
  TH1 * sourceHisto11 = (TH1*) Tree1c->Draw(varName1+" >>+ proName","weight*(Xsec/nGen)*("+lepselection1c+ " && "+trigselection+" && "+jetselection2a+")","");
  TH1 * sourceHisto12 = (TH1*) Tree1c->Draw(varName2+" >>+ proName","weight*(Xsec/nGen)*("+lepselection1c+ " && "+trigselection+" && "+jetselection2b+")","");
  //   cout << "MuEG cnt is " << tmpHisto->Integral() << endl;
//   histTemp3->Add(histTemp4); //Add first and second jet distributions in DoubleMu
//   histTemp3->Add(histTemp7); //Add first jet DoubleEle 
//   histTemp3->Add(histTemp8); //Add second jet DoubleEle
//   histTemp3->Add(histTemp11); //Add first jet MuEG
//   histTemp3->Add(histTemp12); //Add second jet MuEG
  
// csv_Data[jDiv] = (TH1F*)tmpHisto->Clone();
// float Data_tagged = tmpHisto->Integral(); //Numerator
   float Data_tagged = csv_Data[jDiv]->Integral(); //Numerator
  std::cout << "Data: " << Data_tagged << endl;

  csv_Data[jDiv]->SetMarkerStyle(8);
  csv_Data[jDiv]->SetLineColor(kBlack);
  csv_Data[jDiv]->SetTitle("Two leptons, Z-masked, ==2 jet");


  //MC
//   sourceFile2->cd();
//   TH1F *histTemp1a = new TH1F("histTemp1a", "histTemp1a", nBins, Xmin, Xmax);
//   TH1F *histTemp2a = new TH1F("histTemp2a", "histTemp2a", nBins, Xmin, Xmax);
//   TH1F *histTemp3a = new TH1F("histTemp3a", "histTemp3a", nBins, Xmin, Xmax);
//   TH1F *histTemp4a = new TH1F("histTemp4a", "histTemp4a", nBins, Xmin, Xmax);
//   histTemp1a->Sumw2();
//   histTemp2a->Sumw2();
//   histTemp3a->Sumw2();
//   histTemp4a->Sumw2();

  TString proNameMC = Form("csv_MC_Pt%i_Eta%i",iPt,iEta);
  TH1 * sourceHisto3a = (TH1*) Tree2->Draw(varName1+" >> proNameMC",nonttbar+lumi+"weight*(Xsec/nGen)*("+lepselection2+ " && "+trigselection+" && "+jetselection2a+")","");
  TH1 * sourceHisto4a = (TH1*) Tree2->Draw(varName2+" >>+ proNameMC",nonttbar+lumi+"weight*(Xsec/nGen)*("+lepselection2+ " && "+trigselection+" && "+jetselection2b+")","");

  float MC_tagged = csv_MC[jDiv]->Integral();
    std::cout << "MC: " << MC_tagged << endl;
    std::cout << "" << endl;
    std::cout << "Data/MC : " << Data_tagged/MC_tagged << endl;
    std::cout << "Rescaling MC to match data yields ..." << endl;


  csv_MC[jDiv]->Scale(Data_tagged/MC_tagged);
  csv_MC[jDiv]->SetMarkerStyle(8);
  csv_MC[jDiv]->SetMarkerColor(kRed);
  csv_MC[jDiv]->SetLineColor(kRed);

  csv_ratio[jDiv] = (TH1F*)csv_Data[jDiv]->Clone();
  csv_ratio[jDiv]->Divide(csv_ratio[jDiv],csv_MC[jDiv]);
  csv_ratio[jDiv]->SetMarkerStyle(8);
  csv_ratio[jDiv]->SetMarkerColor(kBlue);
  csv_ratio[jDiv]->SetLineColor(kBlue);
  
  can1a->cd();

  //Overlay shapes
  csv_ratio[jDiv]->Rebin(2); csv_ratio[jDiv]->Draw("histE1");

  TString plotName = "_iPt";
  plotName += iPt;
  plotName += "_iEta";
  plotName += iEta;
  can1a->Print("BtagReshaping/csvRWT_"+varName+"_"+WP+plotName +".png");


//   delete gDirectory->FindObject("hitsTemp3");
//   delete gDirectory->FindObject("hitsTemp4");
//   delete gDirectory->FindObject("hitsTemp7");
//   delete gDirectory->FindObject("hitsTemp8");
//   delete gDirectory->FindObject("hitsTemp11");
//   delete gDirectory->FindObject("hitsTemp12");
//   delete gDirectory->FindObject("hitsTemp3a");
//   delete gDirectory->FindObject("hitsTemp4a");

// //Best fit for loose and medium CSV
//   TF1 *f1 = new TF1("f1","(x<-1*[4])*([0]+[1]*pow([2]*(x+[4]),3)) + (x>=-1*[4])*([0]+[3]*pow([2]*(x+[4]),2))");
//   f1->SetParLimits(0,0.9,1.1);
//   f1->SetParLimits(1,-0.5,-0.0001);
//   f1->SetParLimits(3,-0.5,-0.0001);  
//   f1->SetParLimits(2,0.0001,0.5);
//   f1->SetParLimits(4,-200,-80);
//   csv_ratio[jDiv]->Draw("histE1");
//   csv_ratio[jDiv]->Fit("f1");
//   can1a->Print("BtagReshaping/SF_"+varName+"_"+WP+"_"+extraLabel+lfSF+".png");

// //Best fit for tight CSV
//   TF1 *f2 = new TF1("f2","([0]+[1]*pow(fabs(x+[2]),2))");
//   f2->SetParLimits(0,0.5,1.5);
//   f2->SetParLimits(1,-0.001,0.001);
//   f2->SetParLimits(2,-100,100);
// //   f2->SetParLimits(3,2.0,4.0);
//   csv_ratio[jDiv]->Draw("histE1");
//   csv_ratio[jDiv]->Fit("f2");
//   can1a->Print("BtagReshaping/SF_"+varName+"_"+WP+"_"+extraLabel+lfSF+".png");

  jDiv++;
    } //end iEta bin 
  } // end iPt bin

  /////
//   outputFile->Write();    
//   outputFile->Close();  
}
