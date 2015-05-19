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
#include "TH2D.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TProfile.h"
#include "TPaveStats.h"
void check_correlation_plots(TString catName = "e3je2t"){
  //IN GLOBAL SCOPE
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

//   const int numhists = 5;
  const int numprofiles = pow(numhists,2);
  const int saveCorrPlots = 1;
  //  const int lepType = 1;
  TH2D** profiles_Data = new TH2D * [numprofiles];
  TH2D** profiles_MC = new TH2D * [numprofiles];


  //IN MAIN FUNCTION
  char** histnames = new char * [numhists];
  char** histXaxis = new char * [numhists];
//   char** histYaxis = new char * [numhists];
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

//   Float_t lumi = 5111;

  ////////
  TString ofName = "var_correlation_";
  ofName += catName;
  ofName += ".root";
  TFile * outputFile = new TFile(ofName, "RECREATE");
  ////////

  TDirectory *currentDir = gDirectory; 

  int* nBinList = new int[numhists];
  std::vector< double > xBinList[numhists];
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
      TString fileName = "/afs/crc.nd.edu/user/w/wluo1/LHCP_2013/CMSSW_5_3_8_patch1/src/BEAN/DrawPlots/bin/treeFiles/dilSummaryTrees" +sample + "_2012_53x_latest_all.root";
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
//     vector<double> xBins; //&xBins[0]
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
    for (int iBin = 0; iBin < nBinsUsed; ++iBin){
      std::cout << xBins[iBin] << ", ";
      xBinList[i].push_back(xBins[iBin]);
    }
    std::cout << xBins[nBinsUsed] << std::endl;
    xBinList[i].push_back(xBins[nBinsUsed]);

    nBinList[i] = nBinsUsed;

    delete[] xBins;

  }

  ////////
  for (int i = 0; i < numhists; i++){

    int nBini = nBinList[i]+1;
    double * xBini = new double[nBini];

    std::cout << "  -->Using " << nBinList[i] << " bins: ";
    for (int iBin = 0; iBin < nBini; ++iBin) {
      std::cout << xBinList[i][iBin] << ", " ;
      xBini[iBin] = xBinList[i][iBin];
    }
    std::cout << std::endl;

    for (int j = 0; j < numhists; j++){

      int nBinj = nBinList[j]+1;
      double * xBinj = new double[nBinj];

      for (int iBin = 0; iBin < nBinj; ++iBin) {
	xBinj[iBin] = xBinList[j][iBin];
      }

      currentDir->cd(); //This is because the "Draw" command only finds

      profiles_Data[numhists*i+j] = new TH2D(Form("TH2D_Data_X_%s_Y_%s",histnames[i],histnames[j]),"",nBinList[i],xBini,nBinList[j],xBinj);
      profiles_Data[numhists*i+j]->GetXaxis()->SetTitle(histXaxis[i]);
      profiles_Data[numhists*i+j]->GetYaxis()->SetTitle(histXaxis[j]);
      profiles_Data[numhists*i+j]->Sumw2();
      profiles_Data[numhists*i+j]->SetLineWidth(2);
      profiles_Data[numhists*i+j]->SetDirectory(outputFile);
      profiles_MC[numhists*i+j] = new TH2D(Form("TH2D_MC_X_%s_Y_%s",histnames[i],histnames[j]),"",nBinList[i],xBini,nBinList[j],xBinj);
      profiles_MC[numhists*i+j]->GetXaxis()->SetTitle(histXaxis[i]);
      profiles_MC[numhists*i+j]->GetYaxis()->SetTitle(histXaxis[j]);
      profiles_MC[numhists*i+j]->Sumw2();
      profiles_MC[numhists*i+j]->SetLineWidth(2);
      profiles_MC[numhists*i+j]->SetDirectory(outputFile);

      delete[] xBinj;
    }
    delete[] xBini;

  }


  // loop over samples
  for (int isam=0; isam<21; isam++){
    
    /// get the tree file for each sample
    TString sample = htitle[isam];
    TString fileName = "/afs/crc.nd.edu/user/w/wluo1/LHCP_2013/CMSSW_5_3_8_patch1/src/BEAN/DrawPlots/bin/treeFiles/dilSummaryTrees" +sample + "_2012_53x_latest_all.root";
    std::cout << "-->start processing sample " << fileName << std::endl;
    
    TFile * tmpfile = new TFile(fileName);
    TTree * tmpTree = (TTree*)tmpfile->Get("summaryTree");
    
    TString weight = "(19450*Xsec/nGen)*weight*topPtWgt*lepTotalSF*triggerSF*csvWgtlf*csvWgthf*";   /////  before rwt
    
    TString lepCut = "((MuonEle && isMuEGTriggerPass) || (TwoEle && isDoubleElectronTriggerPass) || (TwoMuon && isDoubleMuTriggerPass))";
    if (isam==18) lepCut = "(MuonEle && isMuEGTriggerPass)";
    if (isam==19) lepCut = "(TwoEle && isDoubleElectronTriggerPass)";
    if (isam==20) lepCut = "(TwoMuon && isDoubleMuTriggerPass)";

	/// selection cut
        TString selection = weight;
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


  ////-----------------
//   int bad_bins[5][5];
//   double bad_bins_sig[5][5];
  std::vector<int> bad_bins[numhists][numhists];
  std::vector<double> bad_bins_sig[numhists][numhists];


  for (int i = 0; i < numhists; i++){
    for (int j = 0; j < numhists; j++){
      double previous_sig = 0 ;
      if (j==i) continue;
      TProfile * pro_MC_tmp = (TProfile*)profiles_MC[numhists*i+j]->ProfileX(Form("profile_MC_%s_VS_%s",histnames[j],histnames[i]));
      TProfile * pro_data_tmp = (TProfile*)profiles_Data[numhists*i+j]->ProfileX(Form("Profile_Data_%s_VS_%s",histnames[j],histnames[i]));
      pro_MC_tmp->GetXaxis()->SetTitle(histXaxis[i]);
      pro_MC_tmp->GetYaxis()->SetTitle(histXaxis[j]);
      pro_data_tmp->GetXaxis()->SetTitle(histXaxis[i]);
      pro_data_tmp->GetYaxis()->SetTitle(histXaxis[j]);

      pro_MC_tmp->SetDirectory(outputFile);
      pro_data_tmp->SetDirectory(outputFile);

      for(int ibin = 0; ibin < pro_data_tmp->GetNbinsX(); ibin++){
	double data = pro_data_tmp->GetBinContent(ibin+1);
	double data_err = pro_data_tmp->GetBinError(ibin+1);
	double bkg = pro_MC_tmp->GetBinContent(ibin+1);
	double bkg_err = pro_MC_tmp->GetBinError(ibin+1);
	
	double err2 = data_err*data_err + bkg_err*bkg_err;
	double diff = fabs( data - bkg );
	
	double sig = ( err2>0. ) ? diff/sqrt(err2) : 0.;

	if( sig>2.5 ){
	  if (ibin !=0){
	    bad_bins[i][j].push_back(ibin);
	    bad_bins_sig[i][j].push_back(previous_sig);
	  }
	  bad_bins[i][j].push_back(ibin+1);
	  bad_bins_sig[i][j].push_back(sig);
	}

	else if( previous_sig>2.5 ){
	  bad_bins[i][j].push_back(ibin+1);
	  bad_bins_sig[i][j].push_back(sig);
	}

	previous_sig = sig;
	
      } //end loop over bins
      std::cout << " name = " << Form("%s_VS_%s",histnames[j],histnames[i]) << ", number of bad bins = " << bad_bins[i][j].size() << std::endl;
      for( int b=0; b<int(bad_bins[i][j].size()); b++ ) std::cout << "\t bad bin = " << bad_bins[i][j][b] << ", sig = " << bad_bins_sig[i][j][b] << std::endl;
    }
  }


  ////-----------------

  if(saveCorrPlots == 1){
    //    TCanvas* c_pr = new TCanvas ("c_pr","canvas",150,10,990,990);
    TCanvas** c_pr = new TCanvas * [numhists*numhists];

    for (int i = 0; i < numhists; i++){
      //      if (i!=3) continue;
      for (int j = 0; j < numhists; j++){
	if (j == i) continue;
	int numBadbin = bad_bins[i][j].size() ;
	if (numBadbin == 0) continue;
	int nPad = int(numBadbin/2)+1;
	c_pr[numhists*i+j] = new TCanvas(Form("%s_VS_%s_badBinCheck_",histnames[j],histnames[i]) + catName,"",150,10,330*nPad,660);

	c_pr[numhists*i+j]->Divide(nPad,2);
	int numPad = 0 ;

	for( int b=0; b<numBadbin; b++ ){
	  //	  std::cout << "\t bad bin = " << bad_bins[i][j][b] << ", sig = " << bad_bins_sig[i][j][b] << std::endl;
	  int badBin = bad_bins[i][j][b];
	  numPad++;
	  c_pr[numhists*i+j]->cd(numPad);
 

	//       makeCorrPlot(numhists*i+j,histXaxis[i],histXaxis[j]);
	////--------
	  //	gStyle->SetErrorX(0.5);
// 	  gStyle->SetOptStat(2211);
	  gStyle->SetOptStat(0);

	TLegend* leg = new TLegend(0.75,0.78,0.90,0.90);
	leg->AddEntry(profiles_Data[numhists*i+j],"Data","le");
	leg->AddEntry(profiles_MC[numhists*i+j],"MC","f");
	leg->SetFillColor(0);
	leg->SetLineColor(0);
	leg->SetShadowColor(0);
	leg->SetTextFont(62);
	leg->SetTextSize(0.03);
	//    profiles_MC[numhists*i+j]->Draw("e2");
	
	profiles_MC[numhists*i+j]->SetFillColor(2);
	profiles_MC[numhists*i+j]->SetLineColor(0);
	profiles_MC[numhists*i+j]->SetMarkerSize(0);
	profiles_MC[numhists*i+j]->SetFillStyle(3013);
// 	profiles_MC[numhists*i+j]->GetXaxis()->SetTitle(histXaxis[i]);
// 	profiles_MC[numhists*i+j]->GetYaxis()->SetTitle(histXaxis[j]);
	profiles_MC[numhists*i+j]->GetYaxis()->SetTitleOffset(1.3);
	TH1D* tmpMC = (TH1D*)profiles_MC[numhists*i+j]->ProjectionY(Form("%s_projection_MC_%s_bin%i",histnames[j],histnames[i],badBin),badBin,badBin,"e");
	//	gDirectory->GetObject(Form("%s_projection_MC",histnames[j]), tmpMC);
	tmpMC->SetLineColor(2);
	tmpMC->SetDirectory(outputFile);
	///---
	//    leg->Draw();
	TLatex l;
	l.SetNDC();
	l.SetTextAlign(12);
	l.SetTextSize(0.052);
// 	l.DrawLatex(0.17,0.89,"CMS Preliminary");
	l.SetTextSize(0.04);
// 	l.DrawLatex(0.17,0.84,"#sqrt{s} = 8 TeV, L = 5.0 fb^{-1}");

	TH1D* tmpData = profiles_Data[numhists*i+j]->ProjectionY(Form("%s_projection_Data_%s_bin%i",histnames[j],histnames[i],badBin),badBin,badBin,"e");
	//	gDirectory->GetObject(Form("%s_projection_Data",histnames[j]), tmpData);
	tmpData->SetLineColor(1);
	tmpData->SetDirectory(outputFile);
	//     double maxxx = (tmpMC->GetMaximum() > tmpData->GetMaximum())? tmpMC->GetMaximum():tmpData->GetMaximum(); 
	//     tmpMC->SetMaximum(maxxx);
	tmpData->Draw();
	//	tmpData->GetXaxis()->SetTitle(Form("%s_VS_%s",histnames[j],histnames[i]));

 	tmpMC->Draw("same");

	leg->Draw();
	l.DrawLatex(0.17,0.95,Form("bin %i, sigma is %f",badBin,bad_bins_sig[i][j][b]));
    
	double minval = 9999999.0;
	double maxval = -9999999.0;
    
    for(int ibin = 0; ibin < tmpData->GetNbinsX(); ibin++)
    {
      if(tmpData->GetBinContent(ibin+1) != 0.0)
      {
        if(tmpData->GetBinContent(ibin+1)-tmpData->GetBinError(ibin+1) < minval)
          minval = tmpData->GetBinContent(ibin+1)-tmpData->GetBinError(ibin+1);
        if(tmpData->GetBinContent(ibin+1)+tmpData->GetBinError(ibin+1) > maxval)
          maxval = tmpData->GetBinContent(ibin+1)+tmpData->GetBinError(ibin+1);
      }
  
      if(tmpMC->GetBinContent(ibin+1) != 0.0)
      {
        if(tmpMC->GetBinContent(ibin+1)-tmpMC->GetBinError(ibin+1) < minval)
          minval = tmpMC->GetBinContent(ibin+1)-tmpMC->GetBinError(ibin+1);
        if(tmpMC->GetBinContent(ibin+1)+tmpMC->GetBinError(ibin+1) > maxval)
          maxval = tmpMC->GetBinContent(ibin+1)+tmpMC->GetBinError(ibin+1);
      }
    }
    
    if(minval < 0)
    {
      tmpData->SetMaximum(maxval + 0.4*fabs(maxval-1.1*minval));
      tmpData->SetMinimum(1.1*minval);
    }
    else
    {
      tmpData->SetMaximum(maxval + 0.4*fabs(maxval-0.9*minval));
      tmpData->SetMinimum(0.9*minval);
    }

	////--------
     
        //c_pr->SaveAs(Form("corr-%s-%s-%s-%s.eps",modetext,CStext,histnames[i],histnames[j])); //FOR INDIVIDUAL PLOTS
       
	}
 
	c_pr[numhists*i+j]->Print(".png","png");
// 	c_pr[numhists*i+j]->Print(".pdf","pdf");
      } //end loop j
      
      //      c_pr[i]->Print(".png","png");

    } //end loop i

  }

  outputFile->Write();    
  outputFile->Close();  

}
