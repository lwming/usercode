#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH3.h"
#include "TH2F.h"
#include "TF1.h"
#include "TF2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMath.h"
#include "TRandom3.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TGraphAsymmErrors.h"
#include "TLorentzVector.h"

#include "LumiReweightingStandAlone.h"
#include "PUConstants.h"

#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

//Headers for the data items
#include "ProductArea/BNcollections/interface/BNelectron.h"
#include "ProductArea/BNcollections/interface/BNevent.h"
#include "ProductArea/BNcollections/interface/BNjet.h"
#include "ProductArea/BNcollections/interface/BNmcparticle.h"
#include "ProductArea/BNcollections/interface/BNmet.h"
#include "ProductArea/BNcollections/interface/BNmuon.h"
#include "ProductArea/BNcollections/interface/BNphoton.h"
#include "ProductArea/BNcollections/interface/BNsupercluster.h"
#include "ProductArea/BNcollections/interface/BNtrack.h"
#include "ProductArea/BNcollections/interface/BNtrigger.h"
#include "ProductArea/BNcollections/interface/BNskimbits.h"
#include "ProductArea/BNcollections/interface/BNtrigobj.h"
#include "ProductArea/BNcollections/interface/BNprimaryvertex.h"

#include "AnglesUtil.h"

#endif

using std::string;
//******************************************************************************
typedef std::vector<std::vector<double> > vvdouble;
typedef std::vector<std::vector<std::string> > vvstring;
typedef std::vector<string> vstring;
typedef std::vector<bool> vbool;
typedef std::vector<int> vint;

//******************************************************************************



void test_beans_v1( int maxNentries, string sampleName, int iJob ) {

  typedef BNeventCollection::const_iterator         EventIter;
  typedef BNelectronCollection::const_iterator      EleIter;
  typedef BNjetCollection::const_iterator           JetIter;
  typedef BNmcparticleCollection::const_iterator    MCparIter;
  typedef BNmetCollection::const_iterator           MetIter;
  typedef BNmuonCollection::const_iterator          MuonIter;
  typedef BNphotonCollection::const_iterator        PhotonIter;
  typedef BNprimaryvertexCollection::const_iterator PVIter;
  typedef BNskimbitsCollection::const_iterator      SkimBitIter;
  typedef BNsuperclusterCollection::const_iterator  SCIter;
  typedef BNtrackCollection::const_iterator         TrackIter;
  typedef BNtriggerCollection::const_iterator       TrigIter;
  typedef BNtrigobjCollection::const_iterator       TrigObjIter;


  // Used for PU re-weighting
  std::vector< float > DataDist2011_it;
  std::vector< float > MCDist2011_it;
  for( int i=0; i<35; i++ ){
    DataDist2011_it.push_back(pu::ObsDist2011_f[i]);
    MCDist2011_it.push_back(pu::PoissonOneXDist_f[i]);
  }
  reweight::LumiReWeighting lumiWeights_it = reweight::LumiReWeighting( MCDist2011_it, DataDist2011_it );


  // Load the files
  vstring fileNames;

  //fileNames.push_back("file:/store/user/slaunwhj/TTJets_TuneZ2_7TeV-madgraph-tauola/ttbar_BEAN_Fall11_atND_v1/bf3f7e6eadefdfce276392fc368ccaeb/pat_42x_fall11_withANDfilter_ttbar_366_1_sH5.root");



  //====================== TTH samples   ==============================

  string listFileName(sampleName + ".list");
  ifstream listOfFiles(listFileName.c_str());
  string iFileName;
  int nLines = 0;

  cout << "iJob = " << iJob << "  searching for input file in " << listFileName << "..." << endl;
  
  if (listOfFiles.is_open())
    {
      while ( listOfFiles.good() )
        {
          getline(listOfFiles, iFileName);
          
          if (iFileName.find(".root") != string::npos){
            if ( nLines == iJob ){
              cout << "Line = " << nLines << ", Name = " <<iFileName << endl;
              fileNames.push_back(iFileName);
            }
          } else {
            cout << "Line " << nLines << " doesn't have .root file in it " << endl;
          }
          nLines++;
        }
      listOfFiles.close();
    }

  if (fileNames.size() < 1 ){
    cout << "You don't have any files in your list... this is is probably bad... exiting" << endl;
    exit(22);
  }

  
  // Name of file containing histograms
  std::stringstream outFileName;
  outFileName <<"batchBEAN/" << sampleName << "/test_beans_v1_" << sampleName << "_job_" << iJob << ".root";
  std::string histofilename = outFileName.str();
  std::cout << "Writing out to file " << histofilename << endl;


  // Creates a ChainEvent, allowing files to be linked   
  fwlite::ChainEvent ev(fileNames);   

  TFile histofile(histofilename.c_str(),"recreate");

  histofile.cd();


  //////////////////////////////////////////////////////////////////////////
  ///  Booking Histograms
  //////////////////////////////////////////////////////////////////////////


  // Variables used for histogram ranges and binning
  double metmax   = 500.;
  double muptmax  = 350.;
  double jetptmax = 500.;
  double massmax  = 200.;

  int NmetBins   = int( metmax/10. + 0.0001 );
  int NmuptBins  = int( muptmax/10. + 0.0001 );
  int NjetptBins = int( jetptmax/10. + 0.0001 );
  int NmassBins  = int( massmax/1. + 0.0001 );


  // Book the histograms

  TH1D* h_mu_pt = new TH1D("h_mu_pt",";#mu p_{T}", NmuptBins, 0, muptmax );
  TH1D* h_mu_phi = new TH1D("h_mu_phi",";#mu #phi", 16, -3.2, 3.2 );
  TH1D* h_mu_eta = new TH1D("h_mu_eta",";#mu #eta", 25, -2.5, 2.5 );
  TH1D* h_dR_mu_hlt = new TH1D("h_dR_mu_hlt",";#DeltaR(#mu,hlt #mu)", 120, 0., 6. );

  TH1D* h_jet_pt = new TH1D("h_jet_pt",";jet p_{T}", NjetptBins, 0, jetptmax );
  TH1D* h_jet_phi = new TH1D("h_jet_phi",";jet #phi", 16, -3.2, 3.2 );
  TH1D* h_jet_eta = new TH1D("h_jet_eta",";jet #eta", 25, -2.5, 2.5 );

  TH1D* h_jet_pt_b = new TH1D("h_jet_pt_b",";b jet p_{T}", NjetptBins, 0, jetptmax );
  TH1D* h_jet_pt_c = new TH1D("h_jet_pt_c",";c jet p_{T}", NjetptBins, 0, jetptmax );
  TH1D* h_jet_pt_l = new TH1D("h_jet_pt_l",";light jet p_{T}", NjetptBins, 0, jetptmax );

  TH1D* h_jet_disc_b = new TH1D("h_jet_disc_b",";b jet SSVHE discriminant", 70, -1.0, 6.0 );
  TH1D* h_jet_disc_c = new TH1D("h_jet_disc_c",";c jet SSVHE discriminant", 70, -1.0, 6.0 );
  TH1D* h_jet_disc_l = new TH1D("h_jet_disc_l",";light jet SSHVE discriminant", 70, -1.0, 6.0 );

  TH1D* h_met_pt = new TH1D("h_met_pt",";MET p_{T}", NmetBins, 0, metmax );
  TH1D* h_met_phi = new TH1D("h_met_phi",";MET #phi", 16, -3.2, 3.2 );
  TH1D* h_met_Upt = new TH1D("h_met_Upt",";MET raw p_{T}", NmetBins, 0, metmax );
  TH1D* h_met_Uphi = new TH1D("h_met_Uphi",";MET raw #phi", 16, -3.2, 3.2 );

  TH2D* h_W0_decay_W1_decay = new TH2D("h_W0_decay_W1_decay",";W0 decay;W1 decay", 13, 0, 13, 13, 0, 13 );
  TH1D* h_H_decay = new TH1D("h_H_decay",";H decay", 9, 0, 9 );
  TH1D* h_H_mass = new TH1D("h_H_mass",";H mass", NmassBins, 0, massmax );


  std::vector<std::string> Wdecay_names;
  Wdecay_names.push_back("ud");
  Wdecay_names.push_back("us");
  Wdecay_names.push_back("ub");
  Wdecay_names.push_back("cd");
  Wdecay_names.push_back("cs");
  Wdecay_names.push_back("cb");
  Wdecay_names.push_back("td");
  Wdecay_names.push_back("ts");
  Wdecay_names.push_back("tb");
  Wdecay_names.push_back("e#nu");
  Wdecay_names.push_back("#mu#nu");
  Wdecay_names.push_back("#tau#nu");
  Wdecay_names.push_back("Other");

  int numWdecays = int(Wdecay_names.size());

  for( int i=0; i<numWdecays; i++ ){
    h_W0_decay_W1_decay->GetXaxis()->SetBinLabel(i+1,Wdecay_names[i].c_str());
    h_W0_decay_W1_decay->GetYaxis()->SetBinLabel(i+1,Wdecay_names[i].c_str());
  }

  std::vector<std::string> Hdecay_names;
  Hdecay_names.push_back("b#bar{b}");
  Hdecay_names.push_back("WW");
  Hdecay_names.push_back("#tau#tau");
  Hdecay_names.push_back("gg");
  Hdecay_names.push_back("#gamma#gamma");
  Hdecay_names.push_back("ZZ");
  Hdecay_names.push_back("c#bar{c}");
  Hdecay_names.push_back("Z#gamma");
  Hdecay_names.push_back("Other");

  int numHdecays = int(Hdecay_names.size());

  for( int i=0; i<numHdecays; i++ ) h_H_decay->GetXaxis()->SetBinLabel(i+1,Hdecay_names[i].c_str());


  // Used to determine number of events that pass each cut

  std::vector<std::string> cutflow_name;
  cutflow_name.push_back("All Events");
  cutflow_name.push_back("Cleaning");
  cutflow_name.push_back("Trigger");
  cutflow_name.push_back("==1 tight muon");
  cutflow_name.push_back("1 tight mu & >=3 jets");
  cutflow_name.push_back("No loose muons");
  cutflow_name.push_back("No loose electrons");
  cutflow_name.push_back(">=4 jets");
  cutflow_name.push_back(">=2 b-tagged jets");
  cutflow_name.push_back(">=5 jets");
  cutflow_name.push_back(">=3 b-tagged jets");

  int Ncuts = int(cutflow_name.size());

  // cutflow
  TH1D* h_cutflow_unwgt = new TH1D("h_cutflow_unwgt","",Ncuts, 0, Ncuts);
  TH1D* h_cutflow = new TH1D("h_cutflow","",Ncuts, 0, Ncuts);
  for( int i=0; i<Ncuts; i++ ){
    h_cutflow->GetXaxis()->SetBinLabel(i+1,cutflow_name[i].c_str());
    h_cutflow_unwgt->GetXaxis()->SetBinLabel(i+1,cutflow_name[i].c_str());
  }


  std::vector<std::string> selector_name;

  for( int i=0; i<int(cutflow_name.size()); i++ ) selector_name.push_back(cutflow_name[i]);

  int numSelectors = selector_name.size();
  std::vector<std::vector<double> > selected(numSelectors, std::vector<double>(3) );
  std::vector<std::vector<double> > selected_wgt(numSelectors, std::vector<double>(3) );

  TH2D* h_cutflow_expand = new TH2D("h_cutflow_expand","", 3, 0, 3, numSelectors, 0, numSelectors );
  TH2D* h_cutflow_expand_wgt = new TH2D("h_cutflow_expand_wgt","", 3, 0, 3, numSelectors, 0, numSelectors );

  for( int i=0; i<numSelectors; i++ ){
    h_cutflow_expand->GetYaxis()->SetBinLabel(numSelectors-i,selector_name[i].c_str());
    h_cutflow_expand_wgt->GetYaxis()->SetBinLabel(numSelectors-i,selector_name[i].c_str());
    for( int j=0; j<3; j++ ){
      selected[i][j]=0.;
      selected_wgt[i][j]=0.;
      if( i==0 ){
	if( j==0 ){
	  h_cutflow_expand->GetXaxis()->SetBinLabel(j+1,"Selected");
	  h_cutflow_expand_wgt->GetXaxis()->SetBinLabel(j+1,"Selected");
	}
	else if( j==1 ){
	  h_cutflow_expand->GetXaxis()->SetBinLabel(j+1,"N - 1");
	  h_cutflow_expand_wgt->GetXaxis()->SetBinLabel(j+1,"N - 1");
	}
	else if( j==2 ){
	  h_cutflow_expand->GetXaxis()->SetBinLabel(j+1,"Cumulative");
	  h_cutflow_expand_wgt->GetXaxis()->SetBinLabel(j+1,"Cumulative");
	}
      }
    }
  }


 

  //////////////////////////////////////////////////////////////////////////
  ///  Event variables
  //////////////////////////////////////////////////////////////////////////

  // Triggers to be used
  vstring mc_hlt_trigger_collection;
  mc_hlt_trigger_collection.push_back("HLT_IsoMu24_v");
  //mc_hlt_trigger_collection.push_back("HLT_IsoMu24_eta2p1_v");

  // Trigger filter name associated with trigger above
  vstring mc_hlt_filter_names;
  mc_hlt_filter_names.push_back("hltSingleMuIsoL3IsoFiltered24::HLT");


  double minNDOF = 4;
  double maxAbsZ = 24;
  double maxd0   = 2.;

  int nevents=0;
  int nevents_pass_trigger=0;
  int nevents_pass_cleaning=0;

  double nevents_wgt=0;
  double nevents_pass_trigger_wgt=0;
  double nevents_pass_cleaning_wgt=0;

  int nentries = ev.size();
  std::cout << "\n\t Number of entries = " << nentries << std::endl;
  std::cout << "\t Max number of entries = " << maxNentries << std::endl;
  std::cout << "\n" << std::endl;


  int cnt = 0;

  //
  // Loop over events
  //
  std::cout << "========  Starting Event Loop  ========" << std::endl;
  try {
    for( ev.toBegin(); !ev.atEnd(); ++ev) {

      cnt++;

      if( cnt==1 )        std::cout << "     Event " << cnt << std::endl;
      if( cnt%100000==0 && cnt!=1 ) std::cout << "           " << cnt << "\t" 
					      << int(double(cnt)/double(nentries)*100) << "% done" << std::endl;

      if( cnt==(maxNentries+1) ) break;

      //std::cout << "========  Event! ========" << std::endl;
      // Get Handle for each collection used
      fwlite::Handle<BNeventCollection> h_event;
      h_event.getByLabel(ev,"BNproducer");
      BNeventCollection const &events = *h_event;

      fwlite::Handle<BNmuonCollection> h_muons;
      h_muons.getByLabel(ev,"BNproducer","selectedPatMuonsPFlow");
      BNmuonCollection const &muons = *h_muons;

      fwlite::Handle<BNmetCollection> h_pfmet;
      h_pfmet.getByLabel(ev,"BNproducer","patMETsTypeIPFlow");
      BNmetCollection const &pfmets = *h_pfmet;

      fwlite::Handle<BNtriggerCollection> h_hlt;
      h_hlt.getByLabel(ev,"BNproducer","HLT");
      BNtriggerCollection const &hlt = *h_hlt;

      fwlite::Handle<BNprimaryvertexCollection> h_pvs;
      h_pvs.getByLabel(ev,"BNproducer","offlinePrimaryVertices");
      BNprimaryvertexCollection const &pvs = *h_pvs;

      fwlite::Handle<BNjetCollection> h_pfjets;
      h_pfjets.getByLabel(ev,"BNproducer","selectedPatJetsPFlow");
      BNjetCollection const &pfjets = *h_pfjets;

      fwlite::Handle<BNelectronCollection> h_electrons;
      h_electrons.getByLabel(ev,"BNproducer","selectedPatElectronsPFlow");
      BNelectronCollection const &electrons = *h_electrons;

      fwlite::Handle<BNmcparticleCollection> h_mcparticles;
      h_mcparticles.getByLabel(ev,"BNproducer","MCstatus3");
      BNmcparticleCollection const &mcparticles = *h_mcparticles;

      fwlite::Handle<BNtrigobjCollection> h_hltobj;
      h_hltobj.getByLabel(ev,"BNproducer","HLT");
      BNtrigobjCollection const &hltobjs = *h_hltobj;


      // Some of the other collections in the tree that are not used here

      // fwlite::Handle<BNmuonCollection> h_muons;
      // h_muons.getByLabel(ev,"BNproducer","selectedPatMuons");
      // BNmuonCollection const &muons = *h_muons;

      // fwlite::Handle<BNmetCollection> h_calomet;
      // h_calomet.getByLabel(ev,"BNproducer","patMETsAK5Calo");
      // BNmetCollection const &calomets = *h_calomet;

      // fwlite::Handle<BNmetCollection> h_tcmet;
      // h_tcmet.getByLabel(ev,"BNproducer","patMETsTC");
      // BNmetCollection const &tcmets = *h_tcmet;

      // fwlite::Handle<BNmcparticleCollection> h_mcparticles;
      // h_mcparticles.getByLabel(ev,"BNproducer","MCstatus3");
      // BNmcparticleCollection const &mcparticles = *h_mcparticles;

      // fwlite::Handle<BNtriggerCollection> h_l1talgo;
      // h_l1talgo.getByLabel(ev,"BNproducer","L1Talgo");
      // BNtriggerCollection const &l1talgo = *h_l1talgo;

      // fwlite::Handle<BNtrigobjCollection> h_l1tmuonobj;
      // h_l1tmuonobj.getByLabel(ev,"BNproducer","L1MuonParticles");
      // BNtrigobjCollection const &l1tmuonobjs = *h_l1tmuonobj;

      // fwlite::Handle<BNtrackCollection> h_tracks;
      // h_tracks.getByLabel(ev,"BNproducer","generalTracks");
      // BNtrackCollection const &tracks = *h_tracks;

      // fwlite::Handle<BNjetCollection> h_calojets;
      // h_calojets.getByLabel(ev,"BNproducer","selectedPatJetsAK5Calo");
      // BNjetCollection const &calojets = *h_calojets;

      // fwlite::Handle<BNmcparticleCollection> h_mcelectrons;
      // h_mcelectrons.getByLabel(ev,"BNproducer","MCeleStatus1");
      // BNmcparticleCollection const &mcelectrons = *h_mcelectrons;

      // fwlite::Handle<BNphotonCollection> h_photons;
      // h_photons.getByLabel(ev,"BNproducer","selectedPatPhotons");
      // BNphotonCollection const &photons = *h_photons;



      /////////
      ///
      /// Event / MET
      ///
      ////////

      EventIter event = events.begin();

      int sample = event->sample;
      double weight = event->weight;

      int n0  = event->n0;

      double prescale = 1.;

      // Luminosity reweighting for the MC samples
      // NOTE: if running over only a fraction of the sample, that must be included in the weight

      if( sample==3410 ) prescale *= 1.26;

      double intLumi = 4470.;

      if( sample>=100 && sample<=140 )  prescale *= intLumi * 1./4653;
      if( sample==120 )                 prescale *= 1000000 * 1./999900;

      double wgt = prescale * weight;

      if( sample==2500 )      wgt = 157.7 * intLumi * 1./3627909;     //NNLL
      else if( sample==2300 ) wgt = 3048. * intLumi * 1./35504033;    // NNLO
      else if( sample==2400 ) wgt = 31314. * intLumi * 1./70681213;  //NNLO
      else if( sample==2455 ) wgt = 35.3 * intLumi * 1./21792790;    //LO
      else if( sample==2600 ) wgt = 3.19 * intLumi * 1./259971; //NNLO
      else if( sample==2601 ) wgt = 1.44 * intLumi * 1./137980; //NNLO
      else if( sample==2602 ) wgt = 41.92 * intLumi * 1./3861170; //NNLO
      else if( sample==2603 ) wgt = 22.65 * intLumi * 1./1944826; //NNLO
      else if( sample==2604 ) wgt = 7.87 * intLumi * 1./814390; //NNLO
      else if( sample==2605 ) wgt = 7.87 * intLumi * 1./809984; //NNLO


      // int run  = event->run;
      // int lumi = event->lumi;
      // long evt = event->evt;


      MetIter pfmet = pfmets.begin();
      // MetIter calomet = calomets.begin();
      // MetIter tcmet = tcmets.begin();

      // Type I corrected MET
      h_met_pt->Fill(pfmet->pt,wgt);
      h_met_phi->Fill(pfmet->phi,wgt);
      // Uncorrected / raw MET
      h_met_Upt->Fill(pfmet->Upt,wgt);
      h_met_Uphi->Fill(pfmet->Uphi,wgt);

      
      // Loop over and count the number of primary vertices
      int numpv = 0;
      for( int i=0; i<int(pvs.size()); i++ ){
	bool isGood = ( (pvs.at(i).ndof>=minNDOF) &&
			(fabs(pvs.at(i).z)<maxAbsZ) &&
			(fabs(pvs.at(i).rho)<maxd0) );
	bool isFake = ( pvs.at(i).isFake==1 );
	if( isGood && !isFake ) numpv++;
      }

      // Pile-up reweighting
      double PUwgt = 1;
      if( sample>=0 ) PUwgt = lumiWeights_it.ITweight( n0 );

      wgt *= PUwgt;



      // Trigger Requirement
      bool triggerFound = false;
      bool triggerPass  = false;
      if( sample>=0 ){
      	for( TrigIter hltbit = hlt.begin(); hltbit != hlt.end(); ++hltbit ){
      	  for( int t=0; t<int(mc_hlt_trigger_collection.size()); t++ ){
	    std::string hlt_name = hltbit->name;
	    if( (hlt_name.find(mc_hlt_trigger_collection[t])!=std::string::npos) ){
	      if( hltbit->prescale!=1 ) continue;
	      triggerFound = true;
	      triggerPass = ( hltbit->pass==1 );
	    }
	    if( triggerPass ) break;
	  }
	  if( triggerPass ) break;
	}
      }

      /// make sure trigger used to tag exists in the event
      if( !triggerFound ){
	std::cout << "  error ===> Trigger not found!!!  Here are the available triggers: " << std::endl;
	for( TrigIter hltbit = hlt.begin(); hltbit != hlt.end(); ++hltbit ){
	  std::string hlt_name = hltbit->name;
	  std::cout << "\t\t " << hlt_name << "\t\t prescale = " << hltbit->prescale << "\t\t pass = " << hltbit->pass << std::endl;
	}
	break;
      }



      nevents++;
      nevents_wgt+=wgt;

      int mcut = 0;
      h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // preselection



      // Event cleaning requirements
      bool passGoodVertex = ( numpv>0 ) ? true : false;
      bool passFilterOutScraping = ( event->FilterOutScraping==1 ) ? true : false;
      bool passHBHENoiseFilter = ( event->HBHENoiseFilter==1 ) ? true : false;

      bool cleanEvent = ( passGoodVertex && passFilterOutScraping && passHBHENoiseFilter );
      cleanEvent = ( cleanEvent || (sample>=0) );




      if( sample>=0 ){

	double binW0decay = -99;
	double binW1decay = -99;
	double binHdecay  = -99;

	// Get the decays product of the W
	int W0decay = event->W0decay;
	int W1decay = event->W1decay;

	// loop over status==3 mc particles
	std::vector<int> higgs_daughter_id;
	for( int i=0; i<int(mcparticles.size()); i++ ){
	  int id = mcparticles.at(i).id;
	  int motherID = mcparticles.at(i).motherId;
	  int motherAbsID = fabs(motherID);

	  if( motherAbsID==25 ) higgs_daughter_id.push_back(id);
	  if( abs(id)==25 ) h_H_mass->Fill(mcparticles.at(i).mass);
	}

	// Convert from decay to bin
	if( (W0decay==201 || W0decay==102) )      binW0decay=0.5;
	else if( (W0decay==203 || W0decay==302) ) binW0decay=1.5;
	else if( (W0decay==205 || W0decay==502) ) binW0decay=2.5;
	else if( (W0decay==401 || W0decay==104) ) binW0decay=3.5;
	else if( (W0decay==403 || W0decay==304) ) binW0decay=4.5;
	else if( (W0decay==405 || W0decay==504) ) binW0decay=5.5;
	else if( (W0decay==601 || W0decay==106) ) binW0decay=6.5;
	else if( (W0decay==603 || W0decay==306) ) binW0decay=7.5;
	else if( (W0decay==605 || W0decay==506) ) binW0decay=8.5;
	else if( (W0decay==1112 || W0decay==1211) ) binW0decay=9.5;
	else if( (W0decay==1314 || W0decay==1413) ) binW0decay=10.5;
	else if( (W0decay==1516 || W0decay==1615) ) binW0decay=11.5;
	else                                        binW0decay=12.5;

	if( (W1decay==201 || W1decay==102) )      binW1decay=0.5;
	else if( (W1decay==203 || W1decay==302) ) binW1decay=1.5;
	else if( (W1decay==205 || W1decay==502) ) binW1decay=2.5;
	else if( (W1decay==401 || W1decay==104) ) binW1decay=3.5;
	else if( (W1decay==403 || W1decay==304) ) binW1decay=4.5;
	else if( (W1decay==405 || W1decay==504) ) binW1decay=5.5;
	else if( (W1decay==601 || W1decay==106) ) binW1decay=6.5;
	else if( (W1decay==603 || W1decay==306) ) binW1decay=7.5;
	else if( (W1decay==605 || W1decay==506) ) binW1decay=8.5;
	else if( (W1decay==1112 || W1decay==1211) ) binW1decay=9.5;
	else if( (W1decay==1314 || W1decay==1413) ) binW1decay=10.5;
	else if( (W1decay==1516 || W1decay==1615) ) binW1decay=11.5;
	else                                        binW1decay=12.5;

	h_W0_decay_W1_decay->Fill(binW0decay,binW1decay,wgt);


	// Loop over Higgs daughters
	int d0 = -99, d1=-99;
	for( int d=0; d<int(higgs_daughter_id.size()); d++ ){
	  int id = higgs_daughter_id[d];
	  double absId = fabs(id);

	  if( d0==-99 ) d0 = absId;
	  else if( d1==-99 ) d1 = absId;
	}

	if( d0==5 && d1==5 )        binHdecay=0.5;
	else if( d0==24 && d1==24 ) binHdecay=1.5;
	else if( d0==15 && d1==15 ) binHdecay=2.5;
	else if( d0==21 && d1==21 ) binHdecay=3.5;
	else if( d0==22 && d1==22 ) binHdecay=4.5;
	else if( d0==23 && d1==23 ) binHdecay=5.5;
	else if( d0==4  && d1==4  ) binHdecay=6.5;
	else if( (d0==23 && d1==22) || (d1==23 && d0==22) ) binHdecay=7.5;
	else                        binHdecay=8.5;

	h_H_decay->Fill(binHdecay,wgt);
      }

      /////////
      ///
      /// Electrons
      ///
      ////////
      std::vector<int> tight_ele_index;
      std::vector<int> loose_ele_index;
      for( int i=0; i<int(electrons.size()); i++ ){

        double eleSCEta = electrons.at(i).scEta;
        double absSCeta = fabs(eleSCEta);
        double eleEta = electrons.at(i).eta;
        double elePt = electrons.at(i).pt;

	bool isCrack = ( (absSCeta>1.4442) && (absSCeta<1.5660) );

      	bool kin = ( (elePt>15.) && !isCrack && fabs(eleEta)<2.5 );

      	if( !kin ) continue;

	double chargedHadronIso = electrons.at(i).chargedHadronIso;
	double neutralHadronIso = electrons.at(i).neutralHadronIso;
	double photonIso = electrons.at(i).photonIso;

	double relIso = ( chargedHadronIso + neutralHadronIso + photonIso ) * 1./elePt;

      	bool looseIso = ( relIso < 0.2 );

	if( !looseIso ) continue;

      	loose_ele_index.push_back(i);

	int eidBitTight = electrons.at(i).eidTight;
	if( eidBitTight<0 ){
	  std::cout << "   ===>>>>  BREAK!!  eidBitTight = " << eidBitTight << std::endl;
	  break;
	}
	bool eidTight = ( (eidBitTight & 1)==1 );

	bool d0 = ( fabs(electrons.at(i).correctedD0) < 0.02 );
	bool dZ = ( fabs(electrons.at(i).correctedDZ) < 1. );

	bool dist  = ( fabs(electrons.at(i).dist)<0.02 );
	bool dcot  = ( fabs(electrons.at(i).dcot)<0.02 );
	bool nlost = ( electrons.at(i).numberOfLostHits<1 );
        bool notConv = ( !(dist && dcot) && nlost );

	bool tightIso = ( relIso < 0.1 );

      	bool id = ( eidTight && d0 && dZ && notConv );

      	if( ((elePt>30.) && id && tightIso) ) tight_ele_index.push_back(i);

      }// end electron loop

      int numTightElectrons = int(tight_ele_index.size());
      int numLooseElectrons = int(loose_ele_index.size()) - numTightElectrons;


      /////////
      ///
      /// Muons
      ///
      ////////
      std::vector<int> tight_mu_index;
      std::vector<int> loose_mu_index;
      for( int i=0; i<int(muons.size()); i++ ){

	double muPt  = muons.at(i).pt;
	double muEta = muons.at(i).eta;
	double muPhi = muons.at(i).phi;
	double muAbsEta = fabs(muEta);

	bool kin = ( (muPt>10.) && (muAbsEta<2.4) );

	if( !kin ) continue;

	double chargedHadronIso = muons.at(i).chargedHadronIso;
	double neutralHadronIso = muons.at(i).neutralHadronIso;
	double photonIso = muons.at(i).photonIso;

	double relIso = ( chargedHadronIso + neutralHadronIso + photonIso ) * 1./muPt;

	bool looseIso = ( relIso<0.2 );

	bool isGlobalMuon = ( muons.at(i).isGlobalMuon==1 );

	if( !(isGlobalMuon && looseIso) ) continue;

	loose_mu_index.push_back(i);

	bool isTrackerMuon = ( muons.at(i).isTrackerMuon==1 );
	bool isGlobalMuonPromptTight = ( muons.at(i).isGlobalMuonPromptTight==1 );

	bool numTrackHits = ( muons.at(i).numberOfValidTrackerHitsInnerTrack > 10 );
	bool numPixelHits = ( muons.at(i).pixelLayersWithMeasurement>0 );
	bool numberOfMatches = ( muons.at(i).numberOfMatches>1 );

	bool passd0 = ( fabs(muons.at(i).correctedD0)<0.02 );
	bool passdz = ( fabs(muons.at(i).correctedDZ)<1. );

	bool id = ( isTrackerMuon && isGlobalMuonPromptTight && numTrackHits && numPixelHits && numberOfMatches && passd0 && passdz );

	bool tightIso = ( relIso<0.125 );

	if( ((muPt>30.) && (muAbsEta<2.1) && id && tightIso) ){
	  tight_mu_index.push_back(i);
	  h_mu_pt->Fill(muPt,wgt);
	  h_mu_eta->Fill(muEta,wgt);
	  h_mu_phi->Fill(muPhi,wgt);

	  bool mu_fired_trigger = false;
	  for( TrigObjIter hltobj = hltobjs.begin(); hltobj != hltobjs.end(); ++hltobj ){
	    //// Dump out filter name, pt, eta, phi for each HLT object
	    //std::cout << "\t" << hltobj->filter << "\t pt, eta, phi = " << hltobj->pt << ",\t " << hltobj->eta << ",\t " << hltobj->phi << std::endl;
	    if( hltobj->filter==mc_hlt_filter_names[0] ){
	      double trigEta = hltobj->eta;
	      double trigPhi = hltobj->phi;

	      double dR_mu_hlt = kinem::delta_R(trigEta,trigPhi,muEta,muPhi);
	      h_dR_mu_hlt->Fill(dR_mu_hlt,wgt);
	      if( dR_mu_hlt<0.3 ){
		mu_fired_trigger = true;
		break;
	      }
	    }
	  }
	}


      }// end muon loop


      int numTightMuons = int(tight_mu_index.size());
      int numLooseMuons = int(loose_mu_index.size()) - numTightMuons;


    
      /////////
      ///
      /// Pfjets
      ///
      ////////
      std::vector<int> tight_pfjet_index;
      std::vector<int> tag_pfjet_index;
      std::vector<int> untag_pfjet_index;
      for( int i=0; i<int(pfjets.size()); i++ ){

	double jetPt = pfjets.at(i).pt;
	double jetEta = pfjets.at(i).eta;
	double jetPhi = pfjets.at(i).phi;
	double jetAbsEta = fabs(jetEta);

	bool kin = ( jetPt>30. );
	bool eta = ( jetAbsEta<2.4 );
	bool id  = ( pfjets.at(i).jetIDLoose==1 );

	if( !(kin && eta && id) ) continue;

	bool hasCloseLepton = false;
	for( int m=0; m<int(tight_mu_index.size()); m++ ){
	  int mu_index = tight_mu_index[m];
	  double dR_mu_jet = kinem::delta_R(muons.at(mu_index).eta,muons.at(mu_index).phi,jetEta,jetPhi);
	  if( dR_mu_jet<0.3 ){
	    hasCloseLepton = true;
	    break;
	  }
	}
	for( int m=0; m<int(tight_ele_index.size()); m++ ){
	  int ele_index = tight_ele_index[m];
	  double dR_ele_jet = kinem::delta_R(electrons.at(ele_index).eta,electrons.at(ele_index).phi,jetEta,jetPhi);
	  if( dR_ele_jet<0.3 ){
	    hasCloseLepton = true;
	    break;
	  }
	}
	if( hasCloseLepton ) continue;

	tight_pfjet_index.push_back(i);

	h_jet_pt->Fill(jetPt,wgt);
	h_jet_eta->Fill(jetEta,wgt);
	h_jet_phi->Fill(jetPhi,wgt);


	double svhe = pfjets.at(i).btagSecVertexHighEff;
	bool svheL = ( svhe>1.74 );

	if( svheL ) tag_pfjet_index.push_back(i);
	else        untag_pfjet_index.push_back(i);


	int parId =  abs(pfjets.at(i).genPartonId);

	if( parId==5  ){
	  h_jet_pt_b->Fill(jetPt,wgt);
	  h_jet_disc_b->Fill(svhe,wgt);
	}
	else if( parId==4  ){
	  h_jet_pt_c->Fill(jetPt,wgt);
	  h_jet_disc_c->Fill(svhe,wgt);
	}
	else if( parId==1 || parId==2 || parId==3 || parId==21 ){
	  h_jet_pt_l->Fill(jetPt,wgt);
	  h_jet_disc_l->Fill(svhe,wgt);
	}

      }


      int numJet = int(tight_pfjet_index.size());
      int numTag = int(tag_pfjet_index.size());


      bool oneTightLepton = (numTightMuons==1);
      bool oneLooseLepton = (numLooseMuons==0);
      bool oneTightSecondLepton = (numTightElectrons==0);
      bool oneLooseSecondLepton = (numLooseElectrons==0);


      if( cleanEvent ){
	h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // cleaning
	nevents_pass_cleaning++;
	nevents_pass_cleaning_wgt += wgt;

	if( triggerPass ){
	  h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // trigger
	  nevents_pass_trigger++;
	  nevents_pass_trigger_wgt += wgt;

	  if( oneTightLepton ){
	    h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // 1 good muon

	    if( numJet>=3 ){
	      h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // 3 good jets

	      if( oneLooseLepton ){
		h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // 0 loose muons

		if( oneTightSecondLepton && oneLooseSecondLepton ){
		  h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // 0 loose electrons

		  if( numJet>=4 ){
		    h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // 4 good jets

		    if( numTag>=2 ){
		      h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // 3 bjets

		      if( numJet>=5 ){
			h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // 5 good jets

			if( numTag>=3 ){
			  h_cutflow_unwgt->Fill(0.5 + mcut); h_cutflow->Fill(0.5 + (mcut++),wgt);  // 3 bjets
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }

      bool passCleaning      = cleanEvent;
      bool passTrigger       = ( triggerPass );
      bool pass1TightLepton  = ( oneTightLepton );
      bool passMin3jets      = ( numJet>=3 );
      bool pass0LooseLeptons = ( oneLooseLepton );
      bool pass0OtherLeptons = ( oneTightSecondLepton && oneLooseSecondLepton );
      bool passMin4jets      = ( numJet>=4 );
      bool passMin2tags      = ( numTag>=2 );
      bool passMin5jets      = ( numJet>=5 );
      bool passMin3tags      = ( numTag>=3 );

      std::vector<bool> dec;
      dec.push_back(true); // All Events
      dec.push_back(passCleaning);
      dec.push_back(passTrigger);
      dec.push_back(pass1TightLepton);
      dec.push_back(passMin3jets);
      dec.push_back(pass0LooseLeptons);
      dec.push_back(pass0OtherLeptons);
      dec.push_back(passMin4jets);
      dec.push_back(passMin2tags);
      dec.push_back(passMin5jets);
      dec.push_back(passMin3tags);


      if( dec.size()!=selector_name.size() ){
        std::cout << " ERROR!! dec.size()!=selector_name.size()" << std::endl;
        break;
      }
      for( int j=0; j<numSelectors; j++ ){
        bool Complementary = true, Cumulative = true;
        if( dec[j] ){
          selected_wgt[j][0]+=wgt;
          selected[j][0]+=1;
        }
        for( int k=0; k<numSelectors; k++ ){
          if( k==j ) continue;
          if( !dec[k] ){
            Complementary=false;
            break;
          }
        }
        if( Complementary ){
	  selected_wgt[j][1]+=wgt;
	  selected[j][1]+=1;
	}
        for( int k=0; k<=j; k++ ){
          if( !dec[k] ){
            Cumulative=false;
            break;
          }
        }
        if( Cumulative ){
	  selected_wgt[j][2]+=wgt;
	  selected[j][2]+=1;
	}
      }
      dec.clear();


    } // end loop over events


  }// end try
  catch(std::exception& e) {
    std::cerr << " ==> caught exception " << e.what() << std::endl;
    //continue;
  }





  std::cout << " *********************************************************** " << std::endl;
  std::cout << "   Number of Events Processed (wgt)     = " << nevents << " (" << nevents_wgt << ")" << std::endl;
  std::cout << "   Number of Events Pass cleaning (wgt) = "   << nevents_pass_cleaning << " (" << nevents_pass_cleaning_wgt << ")" << std::endl;
  std::cout << "   Number of Events Passed HLT (wgt)    = " << nevents_pass_trigger << " (" << nevents_pass_trigger_wgt << ")" << std::endl;
  std::cout << " *********************************************************** " << std::endl;
  std::cout << "   Number of MC events " << std::endl;
  std::cout << std::setw(25) << " "
	    << std::setw(27) << "Selected"
	    << std::setw(27) << "N - 1"
	    << std::setw(27) << "Cumulative" << std::endl;
  for( int j=0; j<numSelectors; j++ ){
    std::cout << std::setw(25) << selector_name[j]
              << std::setw(15) << selected[j][0]
              << " [" << std::setw(12) << (selected[j][0]/nevents)*100.0 << "%], "
              << std::setw(15) << selected[j][1]
              << " [" << std::setw(12) << (selected[j][1]/nevents)*100.0 << "%], "
              << std::setw(15) << selected[j][2]
             << " [" << std::setw(12) << (selected[j][2]/nevents)*100.0 << "%] " << std::endl;
  }
  std::cout << " *********************************************************** " << std::endl;
  std::cout << "   Weighted number of MC events " << std::endl;
  std::cout << std::setw(25) << " "
	    << std::setw(27) << "Selected"
	    << std::setw(27) << "N - 1"
	    << std::setw(27) << "Cumulative" << std::endl;
  for( int j=0; j<numSelectors; j++ ){
    std::cout << std::setw(25) << selector_name[j]
              << std::setw(15) << selected_wgt[j][0]
              << " [" << std::setw(12) << (selected_wgt[j][0]/nevents_wgt)*100.0 << "%], "
              << std::setw(15) << selected_wgt[j][1]
              << " [" << std::setw(12) << (selected_wgt[j][1]/nevents_wgt)*100.0 << "%], "
              << std::setw(15) << selected_wgt[j][2]
             << " [" << std::setw(12) << (selected_wgt[j][2]/nevents_wgt)*100.0 << "%] " << std::endl;
  }
  std::cout << " *********************************************************** " << std::endl;

  for( int i=0; i<numSelectors; i++ ){
    for( int j=0; j<3; j++ ){
      h_cutflow_expand->Fill(j+0.5,numSelectors-i-0.5,selected[i][j]);
      h_cutflow_expand_wgt->Fill(j+0.5,numSelectors-i-0.5,selected_wgt[i][j]);
    }
  }

  histofile.Write();
  histofile.Close();

  std::cout << " Done! " << std::endl;

}
