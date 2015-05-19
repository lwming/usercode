{
#include <cmath>
#include <algorithm>

  TString ofName = "ptbin_zjets_1b_check.root";
  //  TString ofName = "csv_rwt_50Bins_topPtrwt.root";
  TFile * outputFile = new TFile(ofName, "RECREATE");

  TString WP = "-100"; //100x probe jet working point - loose (244), medium (679), tight (898)
  TString lfSF = "10"; //10x Scale factor for light-flavor backgrounds (single top, Z+jets). 10 is nominal, 15 is 50% higher, 5 is 50% lower.
  bool EfficiencySF = false; //Set to true if you are measuring the efficiency for different CSV working points
  TString pt_lower = "0";
  TString pt_upper = "10000";
  TString eta_lower = "0.0";
  TString eta_upper = "5.0";
  
  vector<TString> ptBins; 
  ptBins.push_back("30");
  ptBins.push_back("40");
  ptBins.push_back("60");
  ptBins.push_back("100");
//   ptBins.push_back("160");
  ptBins.push_back("10000");
  unsigned int nPt = ptBins.size();
  vector<TString> etaBins;
  etaBins.push_back("0.0");
//   etaBins.push_back("0.8");
//   etaBins.push_back("1.6");
  etaBins.push_back("2.4");
  unsigned int nEta = etaBins.size();

  ///////
  for (unsigned int iPt = 0; iPt < nPt-1; iPt++){
    pt_lower = ptBins[iPt];
    pt_upper = ptBins[iPt+1];
    cout << "pt range is " << pt_lower << " to " << pt_upper << endl;
    for (unsigned int iEta = 0; iEta < nEta-1; iEta++){
      eta_lower = etaBins[iEta];
      eta_upper = etaBins[iEta+1];
      cout << "eta range is " << eta_lower << " to " << eta_upper << endl;

  TFile * sourceFile1a = new TFile ("../csvReWeighting_DIR/TwoMuon.root");
  TFile * sourceFile1b = new TFile ("../csvReWeighting_DIR/TwoEle.root");
//   TFile * sourceFile1c = new TFile ("../csvReWeighting_DIR/MuonEle.root");
  TFile * sourceFile2 = new TFile  ("../csvReWeighting_DIR/allMC.root");

  /////////
  TString lepselection1a = "(TwoMuon && isDoubleMuTriggerPass && PassZmask2==0 && met<30 && abs(mass_leplep-91)<10)"; //Selection for TwoMuon data events
  TString lepselection1b = "(TwoEle && isDoubleElectronTriggerPass && PassZmask2==0 && met<30 && abs(mass_leplep-91)<10)"; //Selection for TwoEle data events
//   TString lepselection1c = "(MuonEle && isMuEGTriggerPass && TwoMuon && TwoEle)"; //Selection for MuonEle data events
  //Selection for MC events
  TString lepselection2 = "( ((TwoMuon && isDoubleMuTriggerPass ) || (TwoEle && isDoubleElectronTriggerPass )) && PassZmask2==0 && met<30 && abs(mass_leplep-91)<10) ";
//   TString jetselection1a = "(numJets == 2 && second_jet_CSV_unc < 0.244)"; //Tag jet requirement for probing first_jet
//   TString jetselection1b = "(numJets == 2 && first_jet_CSV_unc < 0.244)"; //Tag jet requirement for probing second_jet
  TString jetselection2a = "(numJets == 2 && second_jet_CSV_unc > 0.898 && first_jet_CSV_unc < 0.898"; //Probe jet requirement for first_jet
  if (WP != "-100") jetselection2a += " && first_jet_CSV_unc > 0."+WP;
  TString jetselection2b = "(numJets == 2 && first_jet_CSV_unc > 0.898 && second_jet_CSV_unc < 0.898"; //Probe jet requirement for second_jet
  if (WP != "-100") jetselection2b += " && second_jet_CSV_unc > 0."+WP;

  jetselection2a += " && first_jet_pt >= "+pt_lower+" && first_jet_pt < "+pt_upper;
  jetselection2a += " && abs(first_jet_eta) >= "+eta_lower+" && abs(first_jet_eta) < "+eta_upper+")";
  jetselection2b += " && second_jet_pt >= "+pt_lower+" && second_jet_pt < "+pt_upper;
  jetselection2b += " && abs(second_jet_eta) >= "+eta_lower+" && abs(second_jet_eta) < "+eta_upper+")";
//   TString jetselection = "(numJets == 2 && (second_jet_CSV_unc > 0.898 || first_jet_CSV_unc > 0.898))";
  TString jetselection = "(("+jetselection2a +") || ("+jetselection2b +"))";
  TString firstjetb = "( abs(first_jet_flavor)==5 || abs(first_jet_flavor)==4 )"; 
  TString secondjetb = "( abs(second_jet_flavor)==5 || abs(second_jet_flavor)==4 )";
  TString jetbc = "(( abs(first_jet_flavor)==5 || abs(first_jet_flavor)==4 ) && ( abs(second_jet_flavor)==5 || abs(second_jet_flavor)==4 ))";
  TString zjetsSample = "((sampleNumber>=2800 && sampleNumber<=2804) || sampleNumber==2850)";
  TString trigselection = "((dR_leplep > 0.2) && (mass_leplep > 12) && (isCleanEvent == 1) && (oppositeLepCharge == 1))"; //General dilepton selection
  TString nonttbar = "( (Xsec>24.56 && Xsec<=24.57)*1 + (!(Xsec>24.56 && Xsec<=24.57))*("+lfSF+")*0.1)*"; //Scaling of the non-ttbar_ll background component
  TString lumi = "19450.0*"; //Nominal luminosity
  
  if (EfficiencySF) {
    TString varName = "jet_pt"; //Variable which the scale factor is a function of
    TString varName1 = "first_jet_pt"; //Variable which the scale factor is a function of
    TString varName2 = "second_jet_pt"; //Variable which the scale factor is a function of

//     TString varName = "jet_eta"; //Variable which the scale factor is a function of
//     TString varName1 = "first_jet_eta"; //Variable which the scale factor is a function of
//     TString varName2 = "second_jet_eta"; //Variable which the scale factor is a function of
  }
  else {
  TString varName =  "numTaggedJets"; //Variable which the scale factor is a function of
  TString varName1 = "numTaggedJets"; //Variable which the scale factor is a function of
  TString varName2 = "numTaggedJets"; //Variable which the scale factor is a function of
  }

  if (EfficiencySF) {
  double Xmin = 30; //Lower bound for x-axis
  double Xmax = 260; //Upper bound for x-axis
  int nBins = 10; //Number of bins
  Double_t xBins[11] = {30,40,50,60,70,80,100,120,160,210,260}; //Binning
  //  Double_t xBins[13] = {30,40,50,60,70,80,100,120,160,210,260,320,800};

//   double Xmin = -2.5; //Lower bound for x-axis
//   double Xmax = 2.5; //Upper bound for x-axis
//   int nBins = 12; //Number of bins
//   Double_t xBins[13] = {-2.5,-2.1,-1.6,-1.2,-0.8,-0.4,0.0,0.4,0.8,1.2,1.6,2.1,2.5}; //Binning
//   //  Double_t xBins[13] = {30,40,50,60,70,80,100,120,160,210,260,320,800};
  }
  else {
  double Xmin = 1; //Lower bound for x-axis
  double Xmax = 3; //Upper bound for x-axis
  int nBins = 2; //Number of bins
//   Double_t xBins[16] = {0,0.061,0.122,0.183,0.244,0.389,0.534,0.679,0.752,0.825,0.898,0.932,0.966,0.99,0.999,1.0}; //Binning
//  double xBins[18] = {0.0, 0.0488, 0.0976, 0.1464, 0.1952, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.949, 1};
//  double xBins[17] = {0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1};
  double xBins[16] = {0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 1};
  }
  
  TString canvName = Form("canvas_Pt%i_Eta%i",iPt,iEta);  
  TCanvas * can1a = new TCanvas (canvName, canvName);


  //DoubleMu
  sourceFile1a->cd();
  TH1F *histTemp3 = new TH1F("histTemp3", "histTemp3", nBins, Xmin, Xmax); //varName1, jetselection2a
//   TH1F *histTemp4 = new TH1F("histTemp4", "histTemp4", nBins, Xmin, Xmax); //varName2, jetselection2b
  histTemp3->Sumw2();
//   histTemp4->Sumw2();
  TH1 * sourceHisto3 = (TH1*) summaryTree->Draw(varName1+" >> histTemp3","weight*(Xsec/nGen)*("+lepselection1a+ " && "+trigselection+" && "+jetselection+")","");
//   TH1 * sourceHisto4 = (TH1*) summaryTree->Draw(varName2+" >> histTemp4","weight*(Xsec/nGen)*("+lepselection1a+ " && "+trigselection+" && "+jetselection2b+")","");

  //DoubleElectron
  sourceFile1b->cd();
  TH1F *histTemp7 = new TH1F("histTemp7", "histTemp7", nBins, Xmin, Xmax); //varName1, jetselection
//   TH1F *histTemp8 = new TH1F("histTemp8", "histTemp8", nBins, Xmin, Xmax); //varName2, jetselection2b
  histTemp7->Sumw2();
//   histTemp8->Sumw2();
  TH1 * sourceHisto7 = (TH1*) summaryTree->Draw(varName1+" >> histTemp7","weight*(Xsec/nGen)*("+lepselection1b+ " && "+trigselection+" && "+jetselection+")","");
//   TH1 * sourceHisto8 = (TH1*) summaryTree->Draw(varName2+" >> histTemp8","weight*(Xsec/nGen)*("+lepselection1b+ " && "+trigselection+" && "+jetselection2b+")","");

  //MuEG
 //  sourceFile1c->cd();
//   TH1F *histTemp11 = new TH1F("histTemp11", "histTemp11", nBins, Xmin, Xmax); //varName1, jetselection
//   TH1F *histTemp12 = new TH1F("histTemp12", "histTemp12", nBins, Xmin, Xmax); //varName2, jetselection2b
//   histTemp11->Sumw2();
//   histTemp12->Sumw2();
//   TH1 * sourceHisto11 = (TH1*) summaryTree->Draw(varName1+" >> histTemp11","weight*(Xsec/nGen)*("+lepselection1c+ " && "+trigselection+" && "+jetselection+")","");
//   TH1 * sourceHisto12 = (TH1*) summaryTree->Draw(varName2+" >> histTemp12","weight*(Xsec/nGen)*("+lepselection1c+ " && "+trigselection+" && "+jetselection2b+")","");

  
//  histTemp3->Add(histTemp4); //Add first and second jet distributions in DoubleMu
  histTemp3->Add(histTemp7); //Add first jet DoubleEle 
  //  histTemp3->Add(histTemp8); //Add second jet DoubleEle
//   histTemp3->Add(histTemp11); //Add first jet MuEG
//   histTemp3->Add(histTemp12); //Add second jet MuEG
  
  float Data_tagged = histTemp3->Integral(); //Numerator
//   float Data_untagged = histTemp1->Integral(); //Denominator
  if (EfficiencySF) {
    std::cout << "Data tagged: " << Data_tagged << endl;
//     std::cout << "Data untagged: " << Data_untagged << endl;
//     std::cout << "Data tag eff: " << Data_tagged/Data_untagged << endl;
  }
  else std::cout << "Data: " << Data_tagged << endl;
//   if (EfficiencySF) histTemp3->Divide(histTemp3,histTemp1);
  histTemp3->SetMarkerStyle(8);
  histTemp3->SetLineColor(kBlack);
  histTemp3->SetTitle("Two leptons, Z-masked, ==2 jet");
  TString proName3 = Form("numTags_Data_Pt%i_Eta%i",iPt,iEta);
  histTemp3->SetName(proName3);
  histTemp3->GetXaxis()->SetTitle("N_{b-tags}");
  //MC
  sourceFile2->cd();
  TH1F *histTemp3a = new TH1F("histTemp3a", "histTemp3a", nBins, Xmin, Xmax);
  TH1F *histTemp4a = new TH1F("histTemp4a", "histTemp4a", nBins, Xmin, Xmax);
  TH1F *histTemp5a = new TH1F("histTemp5a", "histTemp5a", nBins, Xmin, Xmax);
//   TH1F *histTemp6a = new TH1F("histTemp6a", "histTemp6a", nBins, Xmin, Xmax);
  histTemp3a->Sumw2();
  histTemp4a->Sumw2();
  histTemp5a->Sumw2();
//   histTemp6a->Sumw2();
  TH1 * sourceHisto3a = (TH1*) summaryTree->Draw(varName1+" >> histTemp3a",nonttbar+lumi+"weight*topPtWgt*csvWgthf*csvWgtlf*(Xsec/nGen)*("+lepselection2+ " && "+trigselection+" && "+jetselection+ " && " +zjetsSample +")","");
  TH1 * sourceHisto4a = (TH1*) summaryTree->Draw(varName2+" >> histTemp4a",nonttbar+lumi+"weight*topPtWgt*csvWgthf*csvWgtlf*(Xsec/nGen)*("+lepselection2+ " && "+trigselection+" && "+jetselection+ " && !" +zjetsSample +")","");
  TH1 * sourceHisto5a = (TH1*) summaryTree->Draw(varName1+" >> histTemp5a",nonttbar+lumi+"weight*topPtWgt*csvWgthf*csvWgtlf*(Xsec/nGen)*("+lepselection2+ " && "+trigselection+" && "+jetselection+ " && " +zjetsSample + " && " +jetbc +")","");
//   TH1 * sourceHisto6a = (TH1*) summaryTree->Draw(varName2+" >> histTemp6a",nonttbar+lumi+"weight*topPtWgt*csvWgthf*(Xsec/nGen)*("+lepselection2+ " && !"+secondjetb+" && "+trigselection+" && "+jetselection2b+")","");


  double MC_tagged = histTemp3a->Integral();
  double MC_nonzjets = histTemp4a->Integral();
  double MC_zjetsbc = histTemp5a->Integral();
  double MC_total = MC_tagged + MC_nonzjets;
  if (EfficiencySF) {
    std::cout << "MC tagged: " << MC_tagged << endl;
//     std::cout << "MC untagged: " << MC_untagged << endl;
//     std::cout << "MC tag eff: " << MC_tagged/MC_untagged << endl;
//     std::cout << "" << endl;
//     std::cout << "Data/MC SF: " << (Data_tagged/Data_untagged)/(MC_tagged/MC_untagged) << endl;
  }
  else {
    //    std::cout << "MC: bjets: nonbjets: " << MC_tagged << ", " << MC_tagged_bjets << ", " << MC_tagged_nonbjets << endl;
    std::cout << "MC zjets: " << MC_tagged << endl;
    std::cout << "MC nonzjets: " << MC_nonzjets << endl; ////
    std::cout << "MC zjetsbc: " << MC_zjetsbc << endl; ////
    std::cout << "" << endl;
    std::cout << "Data/MC : " << Data_tagged/MC_total << endl;
    std::cout << "Rescaling MC to match data yields ..." << endl;
  }
//   histTemp3a->Scale(Data_tagged/MC_tagged);
  histTemp3a->SetMarkerStyle(8);
  histTemp3a->SetMarkerColor(kRed);
  histTemp3a->SetLineColor(kRed);
  histTemp3a->SetName(Form("MC_zjets_Pt%i_Eta%i",iPt,iEta));

//   histTemp5a->Scale(Data_tagged/MC_tagged);
  histTemp4a->SetMarkerStyle(8);
  histTemp4a->SetMarkerColor(kGreen);
  histTemp4a->SetLineColor(kGreen);
  histTemp4a->SetName(Form("MC_nonzjets_Pt%i_Eta%i",iPt,iEta));

  histTemp5a->SetMarkerStyle(8);
  histTemp5a->SetMarkerColor(kBlue);
  histTemp5a->SetLineColor(kBlue);
  histTemp5a->SetName(Form("MC_zjetsbc_Pt%i_Eta%i",iPt,iEta));

  TH1F *hClone = (TH1F*)histTemp3->Clone(Form("numTags_ratio_Pt%i_Eta%i",iPt,iEta));
  TH1F *hmc = (TH1F*)histTemp3a->Clone("total_MC");
  hmc->Add(histTemp4a);

//   TH1F *hsub = (TH1F*)hClone->Clone(Form("csv_sub_Pt%i_Eta%i",iPt,iEta));
  hClone->Divide(hClone,hmc);
  hClone->SetMarkerStyle(8);
  hClone->SetMarkerColor(kBlue);
  hClone->SetLineColor(kBlue);

  hClone->SetMaximum(1.5);
  hClone->SetMinimum(0.5);

  TH1F *hratio = (TH1F*)hClone->Clone();
  histTemp3->SetDirectory(outputFile);
  histTemp3a->SetDirectory(outputFile);
  histTemp4a->SetDirectory(outputFile);
  histTemp5a->SetDirectory(outputFile);
  hratio->SetDirectory(outputFile);
  hmc->SetDirectory(outputFile);

//   if (EfficiencySF) {
//     histTemp3->SetMaximum(1.5);
//     histTemp3->SetMinimum(0.1);
//   }
//   else {
//     histTemp3->SetMaximum(1.3);
//     histTemp3->SetMinimum(0.7);
//   }
  
  can1a->cd();
  gStyle->SetOptStat(0);
  //Overlay shapes
  if (EfficiencySF) {
    histTemp3->Draw("histE1");
    histTemp3a->Draw("histsameE1");
    hClone->Draw("histsameE1");
  }
  else hClone->Draw("histE1");

  TString plotName = Form("_Pt%i_Eta%i",iPt,iEta);
  can1a->Print(varName+plotName +"_z1b_pt.png");
//   can1a->Print(varName+plotName +"_z1b.pdf");


    } //end iEta bin 
  } // end iPt bin

  /////
  outputFile->Write();    
  outputFile->Close();  
  
}  
  
