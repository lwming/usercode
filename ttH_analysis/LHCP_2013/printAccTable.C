

void printAccTable () {

  //----------------------------------
  //
  //  Global job params
  //  File names, etc
  //---------------------------------
  TString ttHFileName = "batchBEAN/merged/ttH_m120_dilepStudy_withSummaryTree_merged.root";
  TString ttBarFileName = "batchBEAN/merged/ttbar_dilepStudy_withSummaryTree_merged.root";


  // units are fb and inverse fb
  double xsec_ttH = 198;
  double xsec_ttBar = 157500;

  double lumi = 15;

  

  TFile * ttHFile = new TFile(ttHFileName);

  TTree * ttHSummaryTree = (TTree *) ttHFile->Get("summaryTree");



  //  this is your baseline
  double ttH_numEvents = ttHSummaryTree->GetEntries();

  double ttH_numClean = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) ","goff");
  

  double ttH_numCleanAndTrig = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1)","goff");


  cout << "Done with ttH first selections, moving on to details for ttH  " << endl;
  
  //  two tight muons

  double ttH_numTwoTightMuon = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 )","goff");
  double ttH_numTwoTightMuonGe2j = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets >=2)","goff");
  double ttH_numTwoTightMuonGe2t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets >=2) && (numTaggedJets>=2)","goff");
  
  double ttH_numTwoTightMuon2jEq2t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==2) && (numTaggedJets==2)","goff");
  double ttH_numTwoTightMuon3jEq2t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==3) && (numTaggedJets==2)","goff");
  double ttH_numTwoTightMuon4jEq2t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==4) && (numTaggedJets==2)","goff");
  
  double ttH_numTwoTightMuon3jEq3t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==3) && (numTaggedJets==3)","goff");
  double ttH_numTwoTightMuon4jEq3t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==4) && (numTaggedJets==3)","goff");
  
  double ttH_numTwoTightMuonGe3t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets >=3) && (numTaggedJets>=3)","goff");
  double ttH_numTwoTightMuonEq4t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==4) && (numTaggedJets==4)","goff");

  double ttH_numTwoTightMuonGe3jEq2t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets >=3) && (numTaggedJets==2)","goff");

  cout << "Starting ttBar selections " << endl;
  
  ttHFile->Close();

  //  ttbar two tight
  TFile * ttBarFile = new TFile (ttBarFileName);
  TTree * ttBarSummaryTree = (TTree *) ttBarFile->Get("summaryTree");    
  
  double ttBar_numEvents = ttBarSummaryTree->GetEntries();
  double ttBar_numClean = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) ","goff");
  double ttBar_numCleanAndTrig = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1)","goff");
  
  double ttBar_numTwoTightMuon = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 )","goff");
  cout << "One" << endl;
  double ttBar_numTwoTightMuonGe2j = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets >=2)","goff");
  cout << "Two" << endl;
  double ttBar_numTwoTightMuonGe2t = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets >=2) && (numTaggedJets>=2)","goff");
  cout << "Three" << endl;

    
  double ttBar_numTwoTightMuon2jEq2t = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==2) && (numTaggedJets==2)","goff");
  double ttBar_numTwoTightMuon3jEq2t = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==3) && (numTaggedJets==2)","goff");
  double ttBar_numTwoTightMuon4jEq2t = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==4) && (numTaggedJets==2)","goff");
  
  double ttBar_numTwoTightMuon3jEq3t = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==3) && (numTaggedJets==3)","goff");
  double ttBar_numTwoTightMuon4jEq3t = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==4) && (numTaggedJets==3)","goff");
  
  double ttBar_numTwoTightMuonGe3t = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets >=3) && (numTaggedJets>=3)","goff");
  double ttBar_numTwoTightMuonEq4t = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets ==4) && (numTaggedJets==4)","goff");

  double ttBar_numTwoTightMuonGe3jEq2t = ttBarSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==2) && (numTightElectrons ==0) && (numLooseMuons == 0) && (numLooseElectrons == 0 ) && (numJets >=3) && (numTaggedJets==2)","goff");

  
  // one tight one loose muon

//   double numTightLooseMuon = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==1) && (numTightElectrons ==0) && (numLooseMuons == 1) && (numLooseElectrons == 0 )","goff");
//   double numTightLooseMuonGe2j = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==1) && (numTightElectrons ==0) && (numLooseMuons == 1) && (numLooseElectrons == 0 ) && (numJets >=2)","goff");
//   double numTightLooseMuonGe2t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==1) && (numTightElectrons ==0) && (numLooseMuons == 1) && (numLooseElectrons == 0 ) && (numJets >=2) && (numTaggedJets>=2)","goff");
//   double numTightLooseMuon2t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==1) && (numTightElectrons ==0) && (numLooseMuons == 1) && (numLooseElectrons == 0 ) && (numJets ==2) && (numTaggedJets==2)","goff");
//   double numTightLooseMuonEq3t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==1) && (numTightElectrons ==0) && (numLooseMuons == 1) && (numLooseElectrons == 0 ) && (numJets ==3) && (numTaggedJets==3)","goff");
//   double numTightLooseMuonGe3t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==1) && (numTightElectrons ==0) && (numLooseMuons == 1) && (numLooseElectrons == 0 ) && (numJets >=3) && (numTaggedJets>=3)","goff");
//   double numTightLooseMuonEq4t = ttHSummaryTree->Draw("numJets","(isCleanEvent==1) && (isTriggerPass==1) && (numTightMuons ==1) && (numTightElectrons ==0) && (numLooseMuons == 1) && (numLooseElectrons == 0 ) && (numJets ==4) && (numTaggedJets==4)","goff");



  cout << "##############    " << ttHFileName << "   ######################" << endl;

  double ttH_num_ge3j2t = (ttH_numTwoTightMuonGe3jEq2t/ttH_numEvents) * lumi * xsec_ttH;
  double ttH_num_ge3t = (ttH_numTwoTightMuonGe3t/ttH_numEvents) * lumi * xsec_ttH;
  
  cout << "Original = " << ttH_numEvents << endl
       << "Clean = " << ttH_numClean << endl
       << "Clean and trig = " << ttH_numCleanAndTrig << endl
       << "----------------------------------------" << endl
       << "Two Tight Muons = " << ttH_numTwoTightMuon << " Expected: "  << (ttH_numTwoTightMuon/ttH_numEvents) * lumi * xsec_ttH <<  endl
       << " && >=2 jets = " << ttH_numTwoTightMuonGe2j << " Expected: "  << (ttH_numTwoTightMuonGe2j/ttH_numEvents) * lumi * xsec_ttH << endl
       << " && >=2 tags = " << ttH_numTwoTightMuonGe2t << " Expected: "  << (ttH_numTwoTightMuonGe2t/ttH_numEvents) * lumi * xsec_ttH << endl
       << " && ==2 jets 2 tags = " << ttH_numTwoTightMuon2jEq2t << " Expected: "  << (ttH_numTwoTightMuon2jEq2t/ttH_numEvents) * lumi * xsec_ttH << endl
       << " && ==3 jets 2 tags = " << ttH_numTwoTightMuon3jEq2t << " Expected: "  << (ttH_numTwoTightMuon3jEq2t/ttH_numEvents) * lumi * xsec_ttH << endl
       << " && ==4 jets 2 tags = " << ttH_numTwoTightMuon4jEq2t << " Expected: "  << (ttH_numTwoTightMuon4jEq2t/ttH_numEvents) * lumi * xsec_ttH << endl    
       << " && ==3 jets 3 tags = " << ttH_numTwoTightMuon3jEq3t << " Expected: "  << (ttH_numTwoTightMuon3jEq3t/ttH_numEvents) * lumi * xsec_ttH << endl
       << " && ==4 jets 3 tags = " << ttH_numTwoTightMuon4jEq3t << " Expected: "  << (ttH_numTwoTightMuon4jEq3t/ttH_numEvents) * lumi * xsec_ttH << endl
       << " && ==4 jets 4 tags = " << ttH_numTwoTightMuonEq4t << " Expected: "  << (ttH_numTwoTightMuonEq4t/ttH_numEvents) * lumi * xsec_ttH << endl
       << " && >=3 jets 2 tags = " <<  ttH_numTwoTightMuonGe3jEq2t << " Expected: "  << ttH_num_ge3j2t  << endl
       << " && >=3  tags       = " <<  ttH_numTwoTightMuonGe3t << " Expected: "  << ttH_num_ge3t  << endl
       << endl;

  
  
  cout << "##############    " << ttBarFileName << "   ######################" << endl;


  double ttBar_num_ge3j2t = (ttBar_numTwoTightMuonGe3jEq2t/ttBar_numEvents) * lumi * xsec_ttBar;
  double ttBar_num_ge3t = (ttBar_numTwoTightMuonGe3t/ttBar_numEvents) * lumi * xsec_ttBar;

    
  cout << "Original = " << ttBar_numEvents << endl
       << "Clean = " << ttBar_numClean << endl
       << "Clean and trig = " << ttBar_numCleanAndTrig << endl
       << "----------------------------------------" << endl
       << "Two Tight Muons = " << ttBar_numTwoTightMuon << " Expected: "  << (ttBar_numTwoTightMuon/ttBar_numEvents) * lumi * xsec_ttBar <<  endl
       << " && >=2 jets = " << ttBar_numTwoTightMuonGe2j << " Expected: "  << (ttBar_numTwoTightMuonGe2j/ttBar_numEvents) * lumi * xsec_ttBar << endl
       << " && >=2 tags = " << ttBar_numTwoTightMuonGe2t << " Expected: "  << (ttBar_numTwoTightMuonGe2t/ttBar_numEvents) * lumi * xsec_ttBar << endl
       << " && ==2 jets 2 tags = " << ttBar_numTwoTightMuon2jEq2t << " Expected: "  << (ttBar_numTwoTightMuon2jEq2t/ttBar_numEvents) * lumi * xsec_ttBar << endl
       << " && ==3 jets 2 tags = " << ttBar_numTwoTightMuon3jEq2t << " Expected: "  << (ttBar_numTwoTightMuon3jEq2t/ttBar_numEvents) * lumi * xsec_ttBar << endl
       << " && ==4 jets 2 tags = " << ttBar_numTwoTightMuon4jEq2t << " Expected: "  << (ttBar_numTwoTightMuon4jEq2t/ttBar_numEvents) * lumi * xsec_ttBar << endl    
       << " && ==3 jets 3 tags = " << ttBar_numTwoTightMuon3jEq3t << " Expected: "  << (ttBar_numTwoTightMuon3jEq3t/ttBar_numEvents) * lumi * xsec_ttBar << endl
       << " && ==4 jets 3 tags = " << ttBar_numTwoTightMuon4jEq3t << " Expected: "  << (ttBar_numTwoTightMuon4jEq3t/ttBar_numEvents) * lumi * xsec_ttBar << endl
       << " && ==4 jets 4 tags = " << ttBar_numTwoTightMuonEq4t << " Expected: "  << (ttBar_numTwoTightMuonEq4t/ttBar_numEvents) * lumi * xsec_ttBar << endl
       << " && >=3 jets 2 tags = " <<  ttBar_numTwoTightMuonGe3jEq2t << " Expected: "  << ttBar_num_ge3j2t  << endl
       << " && >=3  tags       = " <<  ttBar_numTwoTightMuonGe3t << " Expected: "  << ttBar_num_ge3t  << endl

       << endl;

  
  
  cout << "------------- Done ------------------" << endl << endl;


}
