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
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TGraphAsymmErrors.h"
#include "TLorentzVector.h"
//Root includes
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

//#include "LumiReweightingStandAlone.h"
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


// headers for python config processing

#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "NtupleMaker/BEANmaker/interface/BtagWeight.h"
#include "NtupleMaker/BEANmaker/interface/BEANhelper.h"

#include "AnglesUtil.h"
#include "PhysicsTools/Utilities/interface/LumiReweightingStandAlone.h"
// For MVA reprocessing
//#include "TMVAGui.C"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

#endif

using std::string;
using std::cout;
using std::endl;

//******************************************************************************
typedef std::vector<float> vfloat;
typedef std::vector<std::vector<float> > vvfloat;
typedef std::vector<std::vector<std::string> > vvstring;
typedef std::vector<string> vstring;
typedef std::vector<bool> vbool;
typedef std::vector<int> vint;
typedef std::vector<TLorentzVector> vLorentz;


// here is where you can change the btag threshold
// Medium combined tag threshold
float btagThres = 0.679;
float btagThresLoose = 0.244;

// super loose combined tag threshold
//float btagThres = 0.244;

using namespace std;

int main ( int argc, char ** argv )
{
   // load framework libraries
   gSystem->Load( "libFWCoreFWLite" );
   gSystem->Load("libNtupleMakerBEANmaker.so");
   AutoLibraryLoader::enable();

   //adding in python config parsing

   if ( argc < 2 ) {
        std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
        return 2;
   }

   //////////////////////////////////////////////////////////////////
   //
   // Get the python configuration
   //
   // Configuration will have
   //   1. a process object that everyhting is attached to
   //   2. various parameter sets, including input and output
   //  
   /////////////////////////////////////////////////////////////////////
   
   cout << "Unpacking configuation parameters." << endl;
   PythonProcessDesc builder(argv[1],argc,argv);

   edm::ParameterSet const& inputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("inputs");
   edm::ParameterSet const& outputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("outputs");
   edm::ParameterSet const& anaParams = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("dilAnalysis");

   std::vector<std::string> inputFileNames = inputs.getParameter< std::vector<std::string> >("fileNames");
   std::string outputFileName = outputs.getParameter<std::string >("outputName");
   //// switch between 2011 and 2012
   std::string selectionYearStr = anaParams.getParameter<std::string>("selectionYear");
   std::string tempPUPeriodStr = anaParams.getParameter<std::string>("PUPeriod");
   std::cout << "CONFIG: selectionYear = " << selectionYearStr << std::endl;
   std::cout << "CONFIG: 2012PUPeriod input from command line = " << tempPUPeriodStr << std::endl;

   // Translate PUPeriod into something more relevant
   std::string PUPeriodStr = "";
   if (tempPUPeriodStr == "2012AB"){
     PUPeriodStr = "2012A_13July, 2012A_06Aug, 2012B_13July";
   } else if (tempPUPeriodStr == "2012ABC") {
     PUPeriodStr = "2012A_13July, 2012A_06Aug, 2012B_13July, 2012C_PR, 2012C_24Aug";
   } else if (tempPUPeriodStr == "2012ABCD") {
     PUPeriodStr = "2012A_13July, 2012A_06Aug, 2012B_13July, 2012C_PR, 2012C_24Aug, 2012D_PR";
   } else {
     std::cout << "Couldn't understand PUPeriodStr = " << tempPUPeriodStr << endl;
     exit (12);
   }

   std::cout << "CONFIG: 2012PUPeriod string to be used at setup = " << PUPeriodStr << std::endl;
   
   //JES
   int jes = anaParams.getParameter<int> ("jes");
   int jer = anaParams.getParameter<int> ("jer");
   int btagCSVShape = anaParams.getParameter<int> ("btagCSVShape");
   std::string sampleName = anaParams.getParameter<string>("sampleName");
   TString tmpName = sampleName;
   std::cout <<"CONFIG: using jes = " << jes << " jer = " << jer << " btagShape = " << btagCSVShape << std::endl; 
   if (jes != 0 && jer != 0) {
     cout << "JES and JER are both non-0 ... exiting" << endl;
     exit(22);
   }

   bool applySelectionJets = anaParams.getParameter<bool>("applySelectionJets");
   bool applySelectionSameSign = anaParams.getParameter<bool>("applySelectionSameSign");
   bool useSideLeptons = false;
   
   bool debug_ = false;

   int maxNentries = inputs.getParameter<int> ("maxEvents");

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


  // data detection
  //
  bool isData = false;
  std::string sysType_lep = "MC";
  if (TString(sampleName).Contains("DoubleElectron")
      || TString(sampleName).Contains("DoubleMu")
      || TString(sampleName).Contains("MuEG")
      || TString(sampleName).Contains("MET")) {

    std::cout << "CONFIG: DATA detected for sample " << sampleName << std::endl;

    isData = true;
    sysType_lep = "data";

  }

  sysType::sysType iSysTypeJE = sysType::NA;

  sysType::sysType iSysTypeCSV = sysType::NA;

  sysType::sysType iSysType = sysType::NA;
  if (btagCSVShape == 0) iSysType = iSysTypeJE;
  else iSysType = iSysTypeCSV;
  

  //Sample numbers may not match event->sample; used for setMCsample() only
  int dInt = -9*pow(10,20);
  float dFloat = -9.0*pow(10,20);

  std::string dset = "SingleMu" ;
  int sampleNumber = 999999;

  if (selectionYearStr == "2012_52x" || selectionYearStr == "2012_53x") {
    if (isData) sampleNumber = -1;
  }
  else {
    assert (selectionYearStr == "either 2012_52x, 2012_53x, or 2011");
  }


  std::cout << "CONFIG: Sample Name = " << sampleName
            << ", sample Number = " << sampleNumber << endl;

  
  BEANhelper beanHelper;
  //BEANhelper::SetUp(string iEra, int iSampleNumber, analysisType::analysisType, bool iIsData, string iDataset, bool iReshapeCSV, bool iPfLeptons = true)
  if (debug_) std::cout << "BEANhelper::SetUp(string iEra, int iSampleNumber, analysisType::analysisType, bool iIsData, string iDataset, bool iReshapeCSV, bool iPfLeptons = true)" << std::endl;
  if (debug_) std::cout << "beanHelper.SetUp(" << selectionYearStr << " , "
                        << sampleNumber << " , analysisType::DIL  " << isData << " , "
                        << dset << " , true, true );" << std::endl;
  beanHelper.SetUp(selectionYearStr,sampleNumber,analysisType::DIL,isData,dset,false,true,PUPeriodStr);
  if (debug_) std::cout << "beanHelper.SetUp successful" << std::endl;
  
  // Load the files
  vstring fileNames = inputFileNames;

  if (fileNames.size() < 1 ){
    cout << "You don't have any files in your list... this is is probably bad... exiting" << endl;
    exit(22);
  }

  
  // Name of file containing histograms
  std::cout << "Writing out to file " << outputFileName << endl;


  // ================= TMVA STUFF  =====================

  // One NN for each category (just tag categories for now)
  // One NN for each category
  // 
  
  std::vector< TString >catList ;

  catList.push_back("e2je2t");   /////
  catList.push_back("e3je2t");   /////
  catList.push_back("ge4je2t");   /////
  catList.push_back("ge3t");   ///// 

  //// BDT
  catList.push_back("e3je2t");   /////
  catList.push_back("ge4je2t");   /////
  catList.push_back("ge3t");   /////
  
  const unsigned int nCat = catList.size();

  //Float_t fCFMlpANN[nCat];
  //TBranch* branchCFMlpANN[nCat] ;

  //Robin's variables

  //Opposite sign variables
  Float_t                 varavg_btag_disc_btags;
  Float_t                 varmin_dr_jets;
  Float_t                 varhiggsLike_dijet_mass;
  Float_t                 varhiggsLike_dijet_mass2;
  Float_t                 varnumHiggsLike_dijet_15_float;
  Float_t                 varsum_pt;


  vector<TMVA::Reader *> reader;

  for( unsigned int j = 6 ; j < nCat ; ++j ){
      TString label = catList[j];
      reader.push_back( new TMVA::Reader( "!Color:!Silent" ));    
      /////

	reader[0]->AddVariable( "min_dr_jets", &varmin_dr_jets );
	if (j!=1 && j!=4) reader[0]->AddVariable( "higgsLike_dijet_mass", &varhiggsLike_dijet_mass );
	if (j!=1 && j!=4) reader[0]->AddVariable( "higgsLike_dijet_mass2", &varhiggsLike_dijet_mass2 );
	if (j==3 || j==6)  reader[0]->AddVariable( "numHiggsLike_dijet_15_float", &varnumHiggsLike_dijet_15_float );
// 	if (j==2 || j==5)  reader[0]->AddVariable( "numJets_float", &varnumJets_float );
	reader[0]->AddVariable( "sum_pt", &varsum_pt );
	if (j!=2 && j!=5)  reader[0]->AddVariable( "avg_btag_disc_btags",   &varavg_btag_disc_btags   ); 
// 	if (j!=3 && j!=6)  reader[0]->AddVariable( "avg_btag_disc_non_btags",   &varavg_btag_disc_non_btags   ); 



      TString dir    = "../../simpleweights/" + label + "/"; //////
      TString prefix = "TMVAClassification";
      TString wfName = dir + prefix + TString("_CFMlpANN.weights.xml");
      if (3<j && j<7) wfName = dir + prefix + TString("_BDTG.weights.xml");

      std::cout << "Loading  weight file  " << wfName << std::endl; 
      
      // Book method(s)
      if (3<j && j<7) reader[0]->BookMVA( "BDTG method", wfName);


  }// end for each category 
  
  // Print out your config

  std::cout << "Using btag threshold " << btagThres << std::endl;
  
  // Creates a ChainEvent, allowing files to be linked   
  fwlite::ChainEvent ev(fileNames);   

  TFile histofile(outputFileName.c_str(),"recreate");

  histofile.cd();


  //////////////////////////////////////////////////////////////////////////
  ///  Booking Histograms
  //////////////////////////////////////////////////////////////////////////

  
  ///////////////////////////////////////////////////////////////////
  //
  // Make a small tree called summaryTree that stores all your 
  // information event by events
  //
  ///////////////////////////////////////////////////////////////////

  bool CoreVariables = true;
  bool ExtraLeptonVariables = false;
  bool ExtraJetVariables = true;
  bool ExtraKinematicVariables = true;
  //bool ExtraEventVariables = false;
  bool ExtraTriggerVariables = false;
  bool ExtraGenVariables = true; //false;
  bool ExtraHiggsVariables = false;
  bool ExtraSameSignVariables = false; //true;
  
  bool ArtificialJetPt = false;
  float higgs_mass = 115.0;
  
  std::map<TString, int *> intBranches;
  std::map<TString, unsigned int *> uintBranches;  
  std::map<TString, float *> floatBranches;

  // Core Variables
  if (CoreVariables) {

    //event variables
    intBranches["PassTwoLepton"] = new int (0);
    intBranches["TwoMuon"] = new int (0);
    intBranches["TwoEle"] = new int (0);
    intBranches["MuonEle"] = new int (0);

    intBranches["numMuons"] = new int (0);
    intBranches["numTightMuons"] = new int (0);
    intBranches["numLooseMuons"] = new int (0);

    intBranches["numElectrons"] = new int (0);
    intBranches["numTightElectrons"] = new int (0);
    intBranches["numLooseElectrons"] = new int (0);

    intBranches["numJets"] = new int (0);
    intBranches["numTaggedJets"] = new int (0);

    intBranches["isCleanEvent"] = new int (0);
    intBranches["isTriggerPass"] = new int (0);
    intBranches["isDoubleMuTriggerPass"] = new int (0);
    intBranches["isDoubleElectronTriggerPass"] = new int (0);
    intBranches["isMuEGTriggerPass"] = new int (0);
    intBranches["isMETTriggerPass"] = new int (0);


    uintBranches["runNumber"] = new unsigned int (0);
    uintBranches["eventNumber"] = new unsigned int (0);
    uintBranches["luminosityBlock"] = new unsigned int (0);

    //kinematic variables
    ////ANN neural net inputs
//     floatBranches["mass_of_everything"] = new float(0.0);
    floatBranches["avg_btag_disc_btags"] = new float(0.0);
    floatBranches["higgsLike_dijet_mass"] = new float(0.0);
    floatBranches["higgsLike_dijet_mass2"] = new float(0.0);
    intBranches["numHiggsLike_dijet_15"] = new int (0.0);
    floatBranches["sum_pt"] = new float(0.0); 
    floatBranches["min_dr_jets"] = new float(0.0);

    /// MVA output variables
    floatBranches["BDTG_ge3t"] = new float(0.0);

    ////leptons
    floatBranches["mass_leplep"] = new float(0.0);
    floatBranches["dR_leplep"] = new float(0.0);

    intBranches["PassZmask"] = new int (0);
    intBranches["oppositeLepCharge"] = new int (0);

    //pile up
    floatBranches["numPV"] = new float(0.0);
    
    //met
    floatBranches["met"] = new float(0.0);
    floatBranches["MHT"] = new float(0.0);

    //lepton variables
    floatBranches["lep1Pt"] = new float(0.0);
    floatBranches["lep2Pt"] = new float(0.0);
    floatBranches["lep1Eta"] = new float(0.0);
    floatBranches["lep2Eta"] = new float(0.0);
    floatBranches["lep1Phi"] = new float(0.0);
    floatBranches["lep2Phi"] = new float(0.0);
    floatBranches["lep1Iso"] = new float(0.0);
    floatBranches["lep2Iso"] = new float(0.0);
    
    // jet variables
//     floatBranches["first_bjet_pt"] = new float(0.0);
    floatBranches["first_jet_pt"] = new float(0.0);
    floatBranches["second_jet_pt"] = new float(0.0);
    floatBranches["third_jet_pt"] = new float(0.0);
    floatBranches["fourth_jet_pt"] = new float(0.0);

    floatBranches["first_jet_eta"] = new float(0.0);
    floatBranches["second_jet_eta"] = new float(0.0);
    floatBranches["third_jet_eta"] = new float(0.0);
    floatBranches["fourth_jet_eta"] = new float(0.0);

    floatBranches["first_jet_phi"] = new float(0.0);
    floatBranches["second_jet_phi"] = new float(0.0);
    floatBranches["third_jet_phi"] = new float(0.0);
    floatBranches["fourth_jet_phi"] = new float(0.0);

    floatBranches["first_jet_CSV"] = new float(0.0);
    floatBranches["second_jet_CSV"] = new float(0.0);
    floatBranches["third_jet_CSV"] = new float(0.0);
    floatBranches["fourth_jet_CSV"] = new float(0.0);

    ////everything
  } //End if (CoreVariables)



  ////////////////////  
  histofile.cd();


  TTree * summaryTree = new TTree ("summaryTree", "Summary Event Values");

  for (std::map<TString, int*>::iterator iBranchMap = intBranches.begin();
       iBranchMap != intBranches.end();
       iBranchMap ++) {

    cout << "Creating branch  " << (iBranchMap->first).Data() << endl;
    TString tempTreeType = iBranchMap->first;
    tempTreeType += "/I";
    summaryTree->Branch(iBranchMap->first, iBranchMap->second, tempTreeType);

  }

  for (std::map<TString, unsigned int *>::iterator uBranchMap = uintBranches.begin();
       uBranchMap != uintBranches.end();
       uBranchMap ++) {

    cout << "Creating branch  " << (uBranchMap->first).Data() << endl;
    TString tempTreeType = uBranchMap->first;
    tempTreeType += "/i";
    summaryTree->Branch(uBranchMap->first, uBranchMap->second, tempTreeType);

  }

  ////Robin 
  for (std::map<TString, float*>::iterator iDMap = floatBranches.begin();
       iDMap != floatBranches.end();
       iDMap ++) {
    
    std::cout << "Creating branch   " << (iDMap->first).Data() << endl;
    TString ttString = iDMap->first;
    ttString += "/F";
    summaryTree->Branch(iDMap->first, iDMap->second, ttString);       
  }

  //////////////////////////////////////////////////////////////////////////
  ///  Event variables
  //////////////////////////////////////////////////////////////////////////

  ////// ============== trigger ===========
  vstring mc_hlt_DoubleMu_trigger_collection;
  vstring mc_hlt_DoubleElectron_trigger_collection;
  vstring mc_hlt_MuEG_trigger_collection;
  vstring mc_hlt_MET_trigger_collection;

  if( selectionYearStr == "2011"){
    mc_hlt_DoubleMu_trigger_collection.push_back("HLT_DoubleMu7_v");
    mc_hlt_DoubleMu_trigger_collection.push_back("HLT_Mu13_Mu8_v");
    mc_hlt_DoubleMu_trigger_collection.push_back("HLT_Mu17_Mu8_v");
    
    mc_hlt_DoubleElectron_trigger_collection.push_back("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v");
    mc_hlt_DoubleElectron_trigger_collection.push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    mc_hlt_DoubleElectron_trigger_collection.push_back("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v");
    
    mc_hlt_MuEG_trigger_collection.push_back("HLT_Mu17_Ele8_CaloIdL_v");
    mc_hlt_MuEG_trigger_collection.push_back("HLT_Mu8_Ele17_CaloIdL_v");
    mc_hlt_MuEG_trigger_collection.push_back("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v");
    mc_hlt_MuEG_trigger_collection.push_back("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v");
  }
  else if( selectionYearStr == "2012_52x" || selectionYearStr == "2012_53x") {
    mc_hlt_DoubleMu_trigger_collection.push_back("HLT_Mu17_Mu8_v");
    mc_hlt_DoubleMu_trigger_collection.push_back("HLT_Mu17_TkMu8_v");
    mc_hlt_DoubleElectron_trigger_collection.push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    mc_hlt_MuEG_trigger_collection.push_back("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    mc_hlt_MuEG_trigger_collection.push_back("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    mc_hlt_MET_trigger_collection.push_back("HLT_DiCentralPFJet30_PFMET80_BTagCSV07_v");
    mc_hlt_MET_trigger_collection.push_back("HLT_MET120_HBHENoiseCleaned_v");
    mc_hlt_MET_trigger_collection.push_back("HLT_MET200_v");
    mc_hlt_MET_trigger_collection.push_back("HLT_PFMET150_v");
  }
  else {
    assert (selectionYearStr == "either 2012_52x, 2012_53x, or 2011");
  } 

  ///
  float minNDOF = 4;
  float maxAbsZ = 24;
  float maxd0   = 2.;

  int nevents=0;
  float nevents_wgt=0;

  int nentries = ev.size();
  std::cout << "\n\t Number of entries = " << nentries << std::endl;
  std::cout << "\t Max number of entries = " << maxNentries << std::endl;
  std::cout << "\n" << std::endl;

  int cnt = 0;

  bool verbose = false;
  
  //
  // Loop over events
  //
  std::cout << "========  Starting Event Loop  ========" << std::endl;
  try {
    for( ev.toBegin(); !ev.atEnd(); ++ev) {

      cnt++;

      if( cnt==1 )        std::cout << "     Event " << cnt << std::endl;
      if( cnt%10000==0 && cnt!=1 ) std::cout << "Helper events " << cnt << "\t" 
					      << int(float(cnt)/float(nentries)*100) << "% done" << std::endl;

      if( cnt==(maxNentries+1) ) break;

      // initialize tree values 
      for (std::map<TString, int*>::iterator iBranchMap = intBranches.begin();
           iBranchMap != intBranches.end();
           iBranchMap ++) {
        // the memory address whose pointer we've saved should be initialized
        // to -1
        *(iBranchMap->second) = -1;
      }

      for (std::map<TString, unsigned int*>::iterator uBranchMap = uintBranches.begin();
             uBranchMap != uintBranches.end();
           uBranchMap ++) {
        *(uBranchMap->second) = 0;
      }
      
      ////Robin
      for (std::map<TString, float*>::iterator iDMap = floatBranches.begin();
	   iDMap != floatBranches.end();
	   iDMap ++) {
        * (iDMap->second)  = -9.99e3;
      }

      if (verbose) std::cout << "Getting collections... " <<std::endl;
      //std::cout << "========  Event! ========" << std::endl;
      // Get Handle for each collection used
      fwlite::Handle<BNeventCollection> h_event;
      h_event.getByLabel(ev,"BNproducer");
      BNeventCollection const &events = *h_event;

      fwlite::Handle<BNmuonCollection> h_muons;
      if ( selectionYearStr == "2012_52x" || selectionYearStr == "2012_53x" ) h_muons.getByLabel(ev,"BNproducer","selectedPatMuonsPFlow");
      else h_muons.getByLabel(ev,"BNproducer","selectedPatMuonsLoosePFlow");
      BNmuonCollection const &muons = *h_muons;

      fwlite::Handle<BNmuonCollection> h_muons_loose;
      if ( selectionYearStr == "2011" ) h_muons_loose.getByLabel(ev,"BNproducer","selectedPatMuonsLoosePFlow");
      else if ( selectionYearStr == "2012_53x" ) h_muons_loose.getByLabel(ev,"BNproducer","selectedPatMuonsLoosePFlow");
      else if ( selectionYearStr == "2012_52x" ) h_muons_loose.getByLabel(ev,"BNproducer","selectedPatMuonsPFlow");
      else assert ( selectionYearStr == "2012_52x, 2012_53x, or 2011" );
      BNmuonCollection const &looseMuons = *h_muons_loose;
      
      fwlite::Handle<BNmetCollection> h_pfmet;
      if ( selectionYearStr == "2011" ) h_pfmet.getByLabel(ev,"BNproducer","patMETsTypeIPFlow");
      else if ( selectionYearStr == "2012_52x" || selectionYearStr == "2012_53x" ) h_pfmet.getByLabel(ev,"BNproducer","patMETsPFlow");
      else assert (selectionYearStr == "either 2012_52x, 2012_53x, or 2011");
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
      if ( selectionYearStr == "2012_52x" || selectionYearStr == "2012_53x" ) h_electrons.getByLabel(ev,"BNproducer","selectedPatElectronsPFlow");
      else h_electrons.getByLabel(ev,"BNproducer","selectedPatElectronsLoosePFlow");
      BNelectronCollection const &electrons = *h_electrons;

      fwlite::Handle<BNelectronCollection> h_electrons_loose;
      if ( selectionYearStr == "2011" ) h_electrons_loose.getByLabel(ev,"BNproducer","selectedPatElectronsLoosePFlow");
      else if ( selectionYearStr == "2012_53x" ) h_electrons_loose.getByLabel(ev,"BNproducer","selectedPatElectronsLoosePFlow");
      else if ( selectionYearStr == "2012_52x" ) h_electrons_loose.getByLabel(ev,"BNproducer","selectedPatElectronsPFlow");
      else assert ( selectionYearStr == "2012_52x, 2012_53x, or 2011" );
      BNelectronCollection const &looseElectrons = *h_electrons_loose;
      
      fwlite::Handle<BNmcparticleCollection> h_mcparticles;
      h_mcparticles.getByLabel(ev,"BNproducer","MCstatus3");
      BNmcparticleCollection const &mcparticles = *h_mcparticles;

      //fwlite::Handle<BNtrigobjCollection> h_hltobj;
      //h_hltobj.getByLabel(ev,"BNproducer","HLT");
      //BNtrigobjCollection const &hltobjs = *h_hltobj;

      if (verbose) std::cout << "Getting collections... " <<std::endl;


      fwlite::Handle<BNtrackCollection> h_tracks;
      h_tracks.getByLabel(ev,"BNproducer","generalTracks");
      BNtrackCollection const &tracks = *h_tracks;


      /////////
      ///
      /// Event / MET
      ///
      ////////

      EventIter event = events.begin();

      int eventSampleNumber = event->sample;

//       if(cnt<5)  std::cout << "CONFIG: Sample number from ntuple is: " << eventSampleNumber << std::endl;

      if (verbose) std::cout << "met stuff " <<std::endl;
      
      MetIter pfmet = pfmets.begin();
      BNmet pfmetSelected = beanHelper.GetCorrectedMET(pfmets.at(0),pfjets,iSysType);
      
      // Loop over and count the number of primary vertices
      int numPV = 0;
      for( int i=0; i<int(pvs.size()); i++ ){
        bool isGood = ( (pvs.at(i).ndof>=minNDOF) &&
                        (fabs(pvs.at(i).z)<maxAbsZ) &&
                        (fabs(pvs.at(i).rho)<maxd0) );
        bool isFake = ( pvs.at(i).isFake==1 );
        if( isGood && !isFake ) numPV++;
      }

      if (verbose) std::cout << "about to do pu reweight " <<std::endl;

      ///--------------------------------
      // Trigger Requirement
      bool triggerFound = false;
      bool DoubleMuTriggerFound = false;
      bool DoubleElectronTriggerFound = false;
      bool MuEGTriggerFound = false;
      bool METTriggerFound = false;
      bool isTriggerPass = false;
      bool isTriggerPassPrescaled = false;
      bool isDoubleMuTriggerPass = false;
      bool isDoubleElectronTriggerPass = false;
      bool isMuEGTriggerPass = false;
      bool isMETTriggerPass = false;

      if (verbose) std::cout << "about to do pu reweight " <<std::endl;
      //      if( eventSampleNumber>=0 ){
      std::string hlt_name = "holder";

      for( TrigIter hltbit = hlt.begin(); hltbit != hlt.end(); ++hltbit ) {
	hlt_name = hltbit->name;

	for( int t=0; t<int(mc_hlt_DoubleMu_trigger_collection.size()); t++ ){
        //hlt_name = hltbit->name;
	  if( (hlt_name.find(mc_hlt_DoubleMu_trigger_collection[t])!=std::string::npos) ) {
	    if( hltbit->prescale==1 || hltbit->prescale==0 ) {
	      DoubleMuTriggerFound = true;
	      isDoubleMuTriggerPass = ( ( hltbit->pass==1 && hltbit->prescale==1 ) || isDoubleMuTriggerPass);
// 	      isDoubleMuTriggerPS0 = ( ( hltbit->prescale==0 ) || isDoubleMuTriggerPS0);
	    }
	    else isTriggerPassPrescaled = ( hltbit->pass==1 || isTriggerPassPrescaled);
	  }
	  if( isDoubleMuTriggerPass ) break;
	}
	for( int t=0; t<int(mc_hlt_DoubleElectron_trigger_collection.size()); t++ ){
	  //hlt_name = hltbit->name;
	  if( (hlt_name.find(mc_hlt_DoubleElectron_trigger_collection[t])!=std::string::npos) ) {
	    if( hltbit->prescale==1 || hltbit->prescale==0 ) {
	      DoubleElectronTriggerFound = true;
	      isDoubleElectronTriggerPass = ( ( hltbit->pass==1 && hltbit->prescale==1 ) || isDoubleElectronTriggerPass);
// 	      isDoubleElectronTriggerPS0 = ( ( hltbit->prescale==0 ) || isDoubleElectronTriggerPS0);
	    }
	    else isTriggerPassPrescaled = ( hltbit->pass==1 || isTriggerPassPrescaled);
	  }
	  if( isDoubleElectronTriggerPass ) break;
	}
	for( int t=0; t<int(mc_hlt_MuEG_trigger_collection.size()); t++ ){
	  //hlt_name = hltbit->name;
	  if( (hlt_name.find(mc_hlt_MuEG_trigger_collection[t])!=std::string::npos) ) {
	    if( hltbit->prescale==1 || hltbit->prescale==0 ) {
	      MuEGTriggerFound = true;
	      isMuEGTriggerPass = ( ( hltbit->pass==1 && hltbit->prescale==1 ) || isMuEGTriggerPass);
// 	      isMuEGTriggerPS0 = ( ( hltbit->prescale==0 ) || isMuEGTriggerPS0);
	    }
	    else isTriggerPassPrescaled = ( hltbit->pass==1 || isTriggerPassPrescaled);
	  }
	  if( isMuEGTriggerPass ) break;
	}
	for( int t=0; t<int(mc_hlt_MET_trigger_collection.size()); t++ ){
	  //hlt_name = hltbit->name;
	  if( (hlt_name.find(mc_hlt_MET_trigger_collection[t])!=std::string::npos) ) {
	    if( hltbit->prescale==1 || hltbit->prescale==0 ) {
	      METTriggerFound = true;
	      isMETTriggerPass = ( ( hltbit->pass==1 && hltbit->prescale==1 ) || isMETTriggerPass);
	      //             isMETTriggerPS0 = ( ( hltbit->prescale==0 ) || isMETTriggerPS0);
	    }
	    else isTriggerPassPrescaled = ( hltbit->pass==1 || isTriggerPassPrescaled);
	  }
	  if( isMETTriggerPass ) break;
	}
	//if( isTriggerPass ) break;
      }
      if (isDoubleMuTriggerPass || isDoubleElectronTriggerPass || isMuEGTriggerPass) isTriggerPass = true;
      if (DoubleMuTriggerFound || DoubleElectronTriggerFound || MuEGTriggerFound || METTriggerFound) triggerFound = true;
      

      // Print triggers
      //triggerFound = false;
      /// make sure trigger used to tag exists in the event
    if( !triggerFound ){
      std::cout << "  error ===> Trigger not found!!!  Here are the available triggers: " << std::endl;
      for( TrigIter hltbit = hlt.begin(); hltbit != hlt.end(); ++hltbit ){
        hlt_name = hltbit->name;
        std::cout << "\t\t " << hlt_name << "\t\t prescale = " << hltbit->prescale << "\t\t pass = " << hltbit->pass << std::endl;
      }
//       break;
    }

    nevents++;


    ///--------- dil selection trigger
    if (TString(sampleName).Contains("DoubleElectron")){
      if (!isDoubleElectronTriggerPass) continue;
    }
    else if (TString(sampleName).Contains("DoubleMu")){
      if (!isDoubleMuTriggerPass) continue;
    }
    else if (TString(sampleName).Contains("MuEG")){
      if (!isMuEGTriggerPass) continue;
    }
    else std::cout << "error, not data!" << std::endl;

    // Event cleaning requirements
    bool passGoodVertex = ( numPV>0 ) ? true : false;
    bool passFilterOutScraping = ( event->FilterOutScraping==1 ) ? true : false;
    bool passHBHENoiseFilter = ( event->HBHENoiseFilter==1 ) ? true : false;
    
    bool isCleanEvent = ( passGoodVertex && passFilterOutScraping && passHBHENoiseFilter );
    isCleanEvent = (isData) ?  isCleanEvent : true;
    ///--------- dil selection clean event
    if (!isCleanEvent) continue;

      
    /////////
    ///
    /// Electrons
    ///
    ////////
    ////--------- using selectors in "beanHelper.h"
    ////////////////////////////////

    BNelectronCollection electronsTight;
    BNelectronCollection electronsInclusivelyLoose;
    BNelectronCollection electronsInclusivelySide;
    BNelectronCollection electronsLoose;
    BNelectronCollection electronsSide;
    BNelectronCollection electronsTL;
    BNelectronCollection electronsTS;
    BNelectronCollection electronsSelected;

    int numElectrons = dInt;
    int numTightElectrons = dInt;
    int numLooseElectrons = dInt;
    int numSideElectrons = dInt;

    if (selectionYearStr == "2012_53x") {
      electronsTight = beanHelper.GetSelectedElectrons(electrons,electronID::electronTight);
      if (useSideLeptons) electronsInclusivelyLoose = beanHelper.GetSelectedElectrons(looseElectrons,electronID::electronLoose);
      else electronsInclusivelyLoose = beanHelper.GetSelectedElectrons(electrons,electronID::electronLoose);
      electronsInclusivelySide = beanHelper.GetSelectedElectrons(looseElectrons,electronID::electronSide);
    }
    else {
      assert (selectionYearStr == "either 2012_52x, 2012_53x, or 2011");
    }

      
    try { electronsLoose = beanHelper.GetSymmetricDifference(electronsInclusivelyLoose,electronsTight); }
    catch(...) { std::cerr << " exception in GetSymmetricDifference" << std::endl; continue; } //Don't segfault if two leptons have same eta,phi but different pt
    try { electronsTL = beanHelper.GetUnionUnsorted(beanHelper.GetSortedByPt(electronsTight),beanHelper.GetSortedByPt(electronsInclusivelyLoose)); }
    catch(...) { std::cerr << " exception in GetUnionUnsorted" << std::endl; continue; } //Don't segfault if two leptons have same eta,phi but different pt
    if (selectionYearStr == "2011" || selectionYearStr == "2012_53x") {
      try { electronsSide = beanHelper.GetSymmetricDifference(electronsInclusivelySide,electronsInclusivelyLoose); }
      catch(...) { std::cerr << " exception in GetUnionUnsorted" << std::endl; continue; } //Don't segfault if two leptons have same eta,phi but different pt
      try { electronsTS = beanHelper.GetUnionUnsorted(beanHelper.GetSortedByPt(electronsTight),beanHelper.GetSortedByPt(electronsInclusivelySide)); }
      catch(...) { std::cerr << " exception in GetUnionUnsorted" << std::endl; continue; } //Don't segfault if two leptons have same eta,phi but different pt
    }

    if (useSideLeptons) {
      electronsSelected = electronsTS;
      numElectrons = electronsTS.size();
    }
    else {
      electronsSelected = electronsTL;
      numElectrons = electronsTL.size();
    }
      
    numTightElectrons = electronsTight.size();
    numLooseElectrons = electronsTL.size() - electronsTight.size();
    numSideElectrons = electronsTS.size() - electronsTight.size();
    
    /////////
    ///
    /// Muons
    ///
    ////////
    ////--------- using selectors in "beanHelper.h"
    ////////////////////////////////

    BNmuonCollection muonsTight;
    BNmuonCollection muonsInclusivelyLoose;
    BNmuonCollection muonsInclusivelySide;
    BNmuonCollection muonsLoose;
    BNmuonCollection muonsSide;
    BNmuonCollection muonsTL;
    BNmuonCollection muonsTS;
    BNmuonCollection muonsSelected;

    int numMuons = dInt;
    int numTightMuons = dInt;
    int numLooseMuons = dInt;
    int numSideMuons = dInt;
    
    if (selectionYearStr == "2012_53x") {
      muonsTight = beanHelper.GetSelectedMuons(muons,muonID::muonTight);
      muonsInclusivelyLoose = beanHelper.GetSelectedMuons(muons,muonID::muonLoose);
      muonsInclusivelySide = beanHelper.GetSelectedMuons(looseMuons,muonID::muonSide);
    }
    else {
      assert (selectionYearStr == "either 2012_52x, 2012_53x, or 2011");
    }

    try { muonsLoose = beanHelper.GetSymmetricDifference(muonsInclusivelyLoose,muonsTight); }
    catch(...) { std::cerr << " exception in GetSymmetricDifference" << std::endl; continue; } //Don't segfault if two leptons have same eta,phi but different pt
    try { muonsTL = beanHelper.GetUnionUnsorted(beanHelper.GetSortedByPt(muonsTight),beanHelper.GetSortedByPt(muonsInclusivelyLoose)); }
    catch(...) { std::cerr << " exception in GetUnionUnsorted" << std::endl; continue; } //Don't segfault if two leptons have same eta,phi but different pt
    if (selectionYearStr == "2011" || selectionYearStr == "2012_53x") {    
      try { muonsSide = beanHelper.GetSymmetricDifference(muonsInclusivelySide,muonsInclusivelyLoose); }
      catch(...) { std::cerr << " exception in GetSymmetricDifference" << std::endl; continue; } //Don't segfault if two leptons have same eta,phi but different pt
      try { muonsTS = beanHelper.GetUnionUnsorted(beanHelper.GetSortedByPt(muonsTight),beanHelper.GetSortedByPt(muonsInclusivelySide)); }
      catch(...) { std::cerr << " exception in GetUnionUnsorted" << std::endl; continue; } //Don't segfault if two leptons have same eta,phi but different pt
    }

    if (useSideLeptons) {
      muonsSelected = muonsTS;
      numMuons = muonsTS.size();
    }
    else {
      muonsSelected = muonsTL;
      numMuons = muonsTL.size();
    }
      
    numTightMuons = muonsTight.size();
    numLooseMuons = muonsTL.size() - muonsTight.size();
    numSideMuons = muonsTS.size() - muonsSide.size();


    bool PassTwoLepton = (( numTightMuons + numLooseMuons + numTightElectrons + numLooseElectrons )== 2 && ( numTightMuons + numTightElectrons )> 0 );
    ////--------- dil selection lepton

    bool TightMuonLooseMuon = ((numTightMuons == 1) && (numLooseMuons==1));
    bool TightMuonLooseEle = ((numTightMuons == 1) && (numLooseElectrons==1));
    bool TightEleLooseMuon = ((numTightElectrons == 1) && (numLooseMuons==1));
    bool TightEleLooseEle = ((numTightElectrons == 1) && (numLooseElectrons==1));
    
    bool twoTightMuon = (numTightMuons == 2);
    bool oneEleOneMuonTight = ((numTightMuons ==1) && (numTightElectrons ==1));
    bool twoTightEle = (numTightElectrons ==2);
    
    bool TwoMuon = PassTwoLepton && ( twoTightMuon || TightMuonLooseMuon ) ;
    bool TwoEle = PassTwoLepton && ( twoTightEle || TightEleLooseEle ) ;
    bool MuonEle = PassTwoLepton && ( TightMuonLooseEle || TightEleLooseMuon || oneEleOneMuonTight );
    
    if (TString(sampleName).Contains("DoubleElectron")){
      if (!TwoEle) continue;
    }
    else if (TString(sampleName).Contains("DoubleMu")){
      if (!TwoMuon) continue;
    }
    else if (TString(sampleName).Contains("MuEG")){
      if (!MuonEle) continue;
    }
    else std::cout << "error, not data!" << std::endl;


    /////////
    ///
    /// Pfjets
    ///
    ////////

    //beanHelper.GetSelectedJets(const BNjetCollection& iJets, const float iMinPt, const float iMaxAbsEta, const jetID::jetID iJetID, const char iCSVwp)
    //beanHelper.GetUncorrectedJets(const BNjetCollection& iCorrectedJets, const BNjetCollection& iOriginalJets)
    
      std::vector<int> jet_index;
      std::vector<int> tagJet_index;

      std::list<float> jet_CSV_list;      
      std::vector<float> jet_CSV;
      std::vector<int> untagJet_index;
      std::vector<float> tagJet_CSV;

      vfloat jet_px;
      vfloat jet_py;
      vfloat jet_pz;
      vfloat jet_pt;
      vfloat jet_energy;
//       vfloat jet_CHEF;
//       vfloat jet_charge;

      int numGoodJets=0;
      int numBadJets=0;
      vLorentz jetV;

      TLorentzVector jetVector;

      bool jerDebugPrint = false;

      /////////////////////////////////////////////////
      //
      //   Loop over jets
      //
      // 
      /////////////////////////////////////////////////

      if (jerDebugPrint || verbose)
        cout << "\n--------------new event------------------" << endl;

      BNjetCollection const &pfjetsSelected = beanHelper.GetCorrectedJets(pfjets,iSysType);
//       BNjetCollection const &pfjetsSelected_CSV_sorted = beanHelper.GetSortedByCSV( pfjetsSelected );
//       BNjetCollection const &pfjetsSelected_Uncorrected = beanHelper.GetUncorrectedJets(pfjetsSelected,pfjets);


      ///////----
      if (debug_) cout << "Looping over pfjetsSelected" << endl;

      for( int i=0; i<int(pfjetsSelected.size()); i++ ){

	float jetPt = pfjetsSelected.at(i).pt;
	float jetEta = pfjetsSelected.at(i).eta;	
	float jetAbsEta = fabs(jetEta);
	float jetPhi = pfjetsSelected.at(i).phi;

// 	float jetCHEF = pfjetsSelected.at(i).chargedHadronEnergyFraction;
// 	float jetcharge = pfjetsSelected.at(i).charge;
// 	float genJetPT = pfjetsSelected.at(i).genJetPT;
    //float myJER = beanHelper.getJERfactor( jer, jetAbsEta, genJetPT, jetPt);

      jet_px.push_back(pfjetsSelected.at(i).px);
      jet_py.push_back(pfjetsSelected.at(i).py);
      jet_pz.push_back(pfjetsSelected.at(i).pz);
      jet_pt.push_back(pfjetsSelected.at(i).pt);
      jet_energy.push_back(pfjetsSelected.at(i).energy);

//       jet_CHEF.push_back(pfjetsSelected.at(i).chargedHadronEnergyFraction);
//       jet_charge.push_back(pfjetsSelected.at(i).charge);    


    /////////////    
	bool kin = ( jetPt>30. );
	bool eta = ( jetAbsEta<2.4 );
	bool id  = ( pfjetsSelected.at(i).jetIDLoose==1 );

	if (debug_) cout << "Before jet cut" << endl;
	
	if( !(kin && eta && id) ) {
	  numBadJets++;
	  continue;
	}
	
	if (debug_) cout << "Passed jet cut" << endl;
	

	jetVector.SetPxPyPzE(jet_px[i],jet_py[i],jet_pz[i],jet_energy[i]);
	jetV.push_back(jetVector);
	numGoodJets++;
	jet_index.push_back(i);

    
	float csv = pfjetsSelected.at(i).btagCombinedSecVertex;
	bool csvM = ( csv > btagThres );
	
	if( csvM ) {
	  tagJet_index.push_back(i);
	  tagJet_CSV.push_back(csv);
	}
	else { untagJet_index.push_back(i); }
	
// 	int flavor =  pfjetsSelected.at(i).flavour;
	
	jet_CSV.push_back(csv);
	jet_CSV_list.push_back(csv);
	
      }// end for each pf jet
      
      if (debug_) cout << "Finished loop over pfjets" << endl;
      
      //------------------------
      int numJets = int(jet_index.size());
      int numTaggedJets = int(tagJet_index.size());

      ///--------- dil selection jet and tag 
      if (numJets < 3) continue;
      if (numTaggedJets < 3) continue;


      /////////////////////////////////
      ///////
      /////// final selection for neural net training:
      //////
      ////////////////////////////////

      //met

    float metx = pfmet->px;
    float mety = pfmet->py;
    float metpt = pfmet->pt;
    float metx_new = pfmetSelected.px;
    float mety_new = pfmetSelected.py;
    float metpt_new = pfmetSelected.pt;
    
    
      TLorentzVector metV(metx_new,mety_new,0.0,metpt_new);
      float met = metpt_new;
      float unc_met = pfmet->Upt;

      //sort btag discriminator
//       jet_CSV_list.sort();  
//       std::vector<float> jet_CSV_sorted;			
//       std::list<float>::iterator listint;			
//       for (listint = jet_CSV_list.begin(); listint != jet_CSV_list.end(); listint++){
// 	jet_CSV_sorted.push_back(*listint);  //accessible form
//       }


      //sum of all jets
      TLorentzVector sum_jet_vect;
      
      for (int sumv=0; sumv < numJets; sumv++) {
        sum_jet_vect += jetV[sumv];
      }
      
      TLorentzVector sum_higgsLike_dijet_vect;
      
      float higgsLike_dijet_mass1 = dFloat;
      float higgsLike_dijet_mass2 = dFloat;
      float higgsLike_dijet_massX = dFloat;
      int numHiggsLike_dijet_15 = 0;
      float denom_avg_cnt = dFloat;
      float avg_btag_disc_btags = dFloat;     
      float min_dr_jets = dFloat;
      float sum_pt = dFloat;

      TLorentzVector dijet_vect;
      TLorentzVector jet_vect;

      TLorentzVector jet_vect1;
      TLorentzVector jet_vect2;

      float first_jet_pt = dFloat ;
      float second_jet_pt = dFloat ;
      float third_jet_pt = dFloat ;
      float fourth_jet_pt = dFloat ;
      
      float first_jet_eta = dFloat;
      float second_jet_eta = dFloat;
      float third_jet_eta = dFloat;
      float fourth_jet_eta = dFloat;

      float first_jet_CSV = dFloat ;
      float second_jet_CSV = dFloat ;
      float third_jet_CSV = dFloat ;
      float fourth_jet_CSV = dFloat ;

      int first_jet_flavor = dInt ;
      int second_jet_flavor = dInt ;
      int third_jet_flavor = dInt ;
      int fourth_jet_flavor = dInt ;

      ///---------------------	
      TLorentzVector lep_vect1;
      TLorentzVector lep_vect2;

      float lep1Pt = dFloat;
      float lep2Pt = dFloat;
      float lep1Eta = dFloat;
      float lep2Eta = dFloat;
      float lep1Phi = dFloat;
      float lep2Phi = dFloat;
      
      float lep1Iso = dFloat;
      float lep2Iso = dFloat;
      float lep1TkCharge = dFloat;
      float lep2TkCharge = dFloat;
      
    
      if( TwoMuon ) {

	lep1Pt = muonsSelected.at(0).pt;
	lep2Pt = muonsSelected.at(1).pt;
	
	lep1Eta = muonsSelected.at(0).eta;
	lep2Eta = muonsSelected.at(1).eta;  

	lep1Phi = muonsSelected.at(0).phi;
	lep2Phi = muonsSelected.at(1).phi;  
	
	lep1TkCharge = muonsSelected.at(0).tkCharge;
	lep2TkCharge = muonsSelected.at(1).tkCharge;
	
        if (selectionYearStr == "2012_52x" || selectionYearStr == "2012_53x") {
          lep1Iso = (muonsSelected.at(0).pfIsoR04SumChargedHadronPt
                      + max(0.0, muonsSelected.at(0).pfIsoR04SumNeutralHadronEt
                                 + muonsSelected.at(0).pfIsoR04SumPhotonEt
                                 - 0.5*muonsSelected.at(0).pfIsoR04SumPUPt)) * 1.0 /lep1Pt;
          lep2Iso = (muonsSelected.at(1).pfIsoR04SumChargedHadronPt
                      + max(0.0, muonsSelected.at(1).pfIsoR04SumNeutralHadronEt
                                 + muonsSelected.at(1).pfIsoR04SumPhotonEt
                                 - 0.5*muonsSelected.at(1).pfIsoR04SumPUPt)) * 1.0 /lep2Pt;

        }
        else {
          assert (selectionYearStr == "either 2012_52x, 2012_53x, or 2011");
        }

        lep_vect1.SetPxPyPzE(muonsSelected.at(0).px, muonsSelected.at(0).py, muonsSelected.at(0).pz, muonsSelected.at(0).energy);
	lep_vect2.SetPxPyPzE(muonsSelected.at(1).px, muonsSelected.at(1).py, muonsSelected.at(1).pz, muonsSelected.at(1).energy);

      }

      if( TwoEle ) {

	lep1Pt = electronsSelected.at(0).pt;
	lep2Pt = electronsSelected.at(1).pt;
	
	lep1Eta = electronsSelected.at(0).eta;
	lep2Eta = electronsSelected.at(1).eta;  
	
	lep1Phi = electronsSelected.at(0).phi;
	lep2Phi = electronsSelected.at(1).phi;  
	
        lep1TkCharge = electronsSelected.at(0).tkCharge;
        lep2TkCharge = electronsSelected.at(1).tkCharge;
        
        if (selectionYearStr == "2012_52x" || selectionYearStr == "2012_53x") {
          lep1Iso = ( electronsSelected.at(0).chargedHadronIso
                       + max(0.0, electronsSelected.at(0).neutralHadronIso
                                  + electronsSelected.at(0).photonIso
                                  - electronsSelected.at(0).AEffDr03*electronsSelected.at(0).rhoPrime) ) * 1.0 /lep1Pt;
          lep2Iso = ( electronsSelected.at(1).chargedHadronIso
                       + max(0.0, electronsSelected.at(1).neutralHadronIso
                                  + electronsSelected.at(1).photonIso
                                  - electronsSelected.at(1).AEffDr03*electronsSelected.at(1).rhoPrime) ) * 1.0 /lep2Pt;

        }
        else {
          assert (selectionYearStr == "either 2012_52x, 2012_53x, or 2011");
        }

	lep_vect1.SetPxPyPzE(electronsSelected.at(0).px, electronsSelected.at(0).py, electronsSelected.at(0).pz, electronsSelected.at(0).energy);
	lep_vect2.SetPxPyPzE(electronsSelected.at(1).px, electronsSelected.at(1).py, electronsSelected.at(1).pz, electronsSelected.at(1).energy);
        
	}

      // This is the case where the muon is higher pt than the
      // electron
      if( MuonEle ) {

	    lep1Pt = muonsSelected.at(0).pt;
	    lep2Pt = electronsSelected.at(0).pt;
	    
	    lep1Eta = muonsSelected.at(0).eta;
	    lep2Eta = electronsSelected.at(0).eta;  

	    lep1Phi = muonsSelected.at(0).phi;
	    lep2Phi = electronsSelected.at(0).phi;  
	    
        lep1TkCharge = muonsSelected.at(0).tkCharge;
        lep2TkCharge = electronsSelected.at(0).tkCharge;


        if (selectionYearStr == "2012_52x" || selectionYearStr == "2012_53x") {
          lep1Iso = (muonsSelected.at(0).pfIsoR04SumChargedHadronPt
                      + max(0.0, muonsSelected.at(0).pfIsoR04SumNeutralHadronEt
                                 + muonsSelected.at(0).pfIsoR04SumPhotonEt
                                 - 0.5*muonsSelected.at(0).pfIsoR04SumPUPt)) * 1.0 /lep1Pt;
          lep2Iso = ( electronsSelected.at(0).chargedHadronIso
                       + max(0.0, electronsSelected.at(0).neutralHadronIso
                                  + electronsSelected.at(0).photonIso
                                  - electronsSelected.at(0).AEffDr03*electronsSelected.at(0).rhoPrime) ) * 1.0 /lep2Pt;
        }
        else {
          assert (selectionYearStr == "either 2012_52x, 2012_53x, or 2011");
        }
        
	lep_vect1.SetPxPyPzE(muonsSelected.at(0).px, muonsSelected.at(0).py, muonsSelected.at(0).pz, muonsSelected.at(0).energy);
	lep_vect2.SetPxPyPzE(electronsSelected.at(0).px, electronsSelected.at(0).py, electronsSelected.at(0).pz, electronsSelected.at(0).energy);
	
      }
      
      
      int oppositeLepCharge = dInt;
      
      // check to see if the product is negative
      // it can only be negative if the charges
      // have opposite signs
      if ((lep1TkCharge * lep2TkCharge) == -1) {
        oppositeLepCharge = 1;
      } else if ((lep1TkCharge * lep2TkCharge) == 1) {
        oppositeLepCharge = 0;
      } else if (fabs(lep2TkCharge) == 99) {
        oppositeLepCharge = -1;
      } else if (fabs(lep1TkCharge) == 99) {
        oppositeLepCharge = -2;
      }
      else {
        if (debug_) std::cout << "Lep1 has charge " << lep1TkCharge << " and Lep2 has charge " << lep2TkCharge << std::endl;
      }

      ///----- dil selection opposite sign
      if( !(oppositeLepCharge ==1) ) continue;

      ///////      
      if (sum_pt == dFloat) sum_pt = 0.0;
      sum_pt += lep1Pt;
      sum_pt += lep2Pt;
	  
      // two leptons
      TLorentzVector two_lepton_vect = lep_vect1 + lep_vect2;
      
      float mass_leplep = two_lepton_vect.M();
      float dR_leplep = lep_vect1.DeltaR(lep_vect2);
      ///----- dil selection extra cuts
      if (! (dR_leplep > 0.2) ) continue;
      if (! (mass_leplep > 12) ) continue;

      TLorentzVector leps_and_jets_vect = lep_vect1 + lep_vect2 + sum_jet_vect;
      float MHT = leps_and_jets_vect.Pt();

      bool PassBigDiamondZmask = (MuonEle || (mass_leplep < (65.5 + 3*MHT/8)) || (mass_leplep > (108 - MHT/4)) || (mass_leplep < (79 - 3*MHT/4)) || (mass_leplep > (99 + MHT/2)) );

      ///----- dil selection extra cuts
      if (!PassBigDiamondZmask) continue;

      ////////// BDT variables
      for (int i=0; i < numGoodJets; i++) {
        if (debug_) std::cout << "jet_index[" << i << "] = " << jet_index[i] << " , CSV = " << jet_CSV[jet_index[i]] << std::endl;
	int iJet = jet_index[i] ;

	sum_pt += jet_pt[iJet];

	jet_vect.SetPxPyPzE(jet_px[iJet],jet_py[iJet],jet_pz[iJet],jet_energy[iJet]);
        
        for (int j=i+1; j < numGoodJets; j++) {
          sum_higgsLike_dijet_vect = jetV[i] + jetV[j];
          higgsLike_dijet_massX = sum_higgsLike_dijet_vect.M();

          if (pfjetsSelected.at(iJet).btagCombinedSecVertex > btagThres
              || pfjetsSelected.at(jet_index[j]).btagCombinedSecVertex > btagThres) {

            if (fabs(higgs_mass - higgsLike_dijet_mass2) > fabs(higgs_mass - higgsLike_dijet_massX)
                || higgsLike_dijet_mass2 == dFloat) {
              if (fabs(higgs_mass - higgsLike_dijet_mass1) > fabs(higgs_mass - higgsLike_dijet_massX)
                  || higgsLike_dijet_mass1 == dFloat) higgsLike_dijet_mass2 = higgsLike_dijet_mass1;
              else higgsLike_dijet_mass2 = higgsLike_dijet_massX;
            }
            if (fabs(higgs_mass - higgsLike_dijet_mass1) > fabs(higgs_mass - higgsLike_dijet_massX)
                || higgsLike_dijet_mass1 == dFloat) {
              higgsLike_dijet_mass1 = higgsLike_dijet_massX;
            }

            if (fabs(higgs_mass - higgsLike_dijet_massX) < 15) numHiggsLike_dijet_15++;

          }


        } //End j loop over numGoodJets
        
        
	if (i==0)  {        
          first_jet_pt = jet_pt[iJet];
          first_jet_eta = jet_vect.Eta();
          first_jet_CSV = pfjetsSelected.at(iJet).btagCombinedSecVertex;
          first_jet_flavor = pfjetsSelected.at(iJet).flavour;
        }
	if (i==1)  {
          second_jet_pt = jet_pt[iJet];
          second_jet_eta = jet_vect.Eta();
          second_jet_CSV = pfjetsSelected.at(iJet).btagCombinedSecVertex;
          second_jet_flavor = pfjetsSelected.at(iJet).flavour;
        }
	if (i==2)  {
          third_jet_pt = jet_pt[iJet];
          third_jet_eta = jet_vect.Eta();
          third_jet_CSV = pfjetsSelected.at(iJet).btagCombinedSecVertex;
	  third_jet_flavor = pfjetsSelected.at(iJet).flavour;
        }
	if (i==3)  {
          fourth_jet_pt = jet_pt[iJet];
          fourth_jet_eta = jet_vect.Eta();
          fourth_jet_CSV = pfjetsSelected.at(iJet).btagCombinedSecVertex;
	  fourth_jet_flavor = pfjetsSelected.at(iJet).flavour;
        }
	    
	
	if (jet_CSV[i] > btagThres){
          if (avg_btag_disc_btags == dFloat) avg_btag_disc_btags = 0.0;
	  avg_btag_disc_btags += jet_CSV[i];
	}
	    

      } // End of loop over numGoodJets

      /////
      for (int j=0; j < (numJets - 1); j++) {
	int jJet = jet_index[j] ;
	jet_vect1.SetPxPyPzE(jet_px[jJet],jet_py[jJet],jet_pz[jJet],jet_energy[jJet]);
	
	for (int k=j+1; k < numJets; k++) {
	  int kJet = jet_index[k] ; 
	  jet_vect2.SetPxPyPzE(jet_px[kJet],jet_py[kJet],jet_pz[kJet],jet_energy[kJet]);
		
	  dijet_vect = jet_vect1 + jet_vect2;
	  
	  if (min_dr_jets > jet_vect1.DeltaR(jet_vect2) || min_dr_jets == dFloat){
	    min_dr_jets = jet_vect1.DeltaR(jet_vect2);
	  }
	  
	} //end loop over k
      } //end loop over j

      
      if ( numTaggedJets > 0 ) avg_btag_disc_btags /= numTaggedJets;
      

      //------------------------------------------
      //
      // Assign values to the vars the MVA will use
      // 
      //------------------------------------------
      if (debug_) std::cout << "Starting MVA" << std::endl;	  

      //Opposite sign variables
      varavg_btag_disc_btags = avg_btag_disc_btags;
      varmin_dr_jets = min_dr_jets;
      varhiggsLike_dijet_mass = higgsLike_dijet_mass1;
      varhiggsLike_dijet_mass2 = higgsLike_dijet_mass2;
      varnumHiggsLike_dijet_15_float = float(numHiggsLike_dijet_15);
      varsum_pt = sum_pt;

      Float_t bdtValue = 0;
      for( unsigned int j = 6 ; j < nCat ; ++j ) {
        // --- Return the MVA outputs and weights
        TMVA::Reader  *tmpReader = reader[0];  
	TString mvaName = "CFMlpANN";
	if (3<j && j<7) mvaName = "BDTG";
        TString branchName = mvaName + TString("_") + catList[j];
        //        cout << "This is NN category " << catList[j] << " saving into branch " << branchName << endl;
	TString methodName = mvaName + TString(" method");
        Float_t annOut  = tmpReader->EvaluateMVA( methodName );
        *(floatBranches[branchName]) = annOut;  
	bdtValue = annOut;

      } // End category loop

      ////----- dil selection BDT
      if (bdtValue < 0.6) continue;

      
      if (CoreVariables) {

        //event variables
        *(intBranches["PassTwoLepton"]) =  PassTwoLepton ? 1 : 0;
        *(intBranches["TwoMuon"]) = TwoMuon ? 1 : 0;
        *(intBranches["TwoEle"]) =  TwoEle ? 1 : 0;
        *(intBranches["MuonEle"]) = MuonEle ? 1 : 0;

        *(intBranches["numJets"]) = numJets ;
        *(intBranches["numTaggedJets"]) = numTaggedJets;

        *(intBranches["isCleanEvent"]) = isCleanEvent ? 1 : 0;
        *(intBranches["isTriggerPass"]) = isTriggerPass ? 1 : 0;
        *(intBranches["isDoubleMuTriggerPass"]) = isDoubleMuTriggerPass ? 1 : 0;
        *(intBranches["isDoubleElectronTriggerPass"]) = isDoubleElectronTriggerPass ? 1 : 0;
        *(intBranches["isMuEGTriggerPass"]) = isMuEGTriggerPass ? 1 : 0;
//         *(intBranches["isMETTriggerPass"]) = isMETTriggerPass ? 1 : 0;

        *(intBranches["PassZmask"]) = PassBigDiamondZmask ? 1 : 0;
        *(intBranches["oppositeLepCharge"]) = oppositeLepCharge;
        
        *(uintBranches["runNumber"]) = event->run ;
        *(uintBranches["luminosityBlock"]) = event->lumi ;
        *(uintBranches["eventNumber"]) = unsigned(event->evt) ;
//         *(intBranches["sampleNumber"]) = sampleNumber ;

        //met
        *(floatBranches["met"]) = met;
        *(floatBranches["MHT"]) = MHT;

        //lepton variables
        *(floatBranches["lep1Pt"]) =  lep1Pt;
        *(floatBranches["lep2Pt"]) =  lep2Pt;
        *(floatBranches["lep1Eta"]) = lep1Eta;
        *(floatBranches["lep2Eta"]) = lep2Eta;
        *(floatBranches["lep1Phi"]) = lep1Phi;
        *(floatBranches["lep2Phi"]) = lep2Phi;
        *(floatBranches["lep1Iso"]) = lep1Iso;
        *(floatBranches["lep2Iso"]) = lep2Iso;

        
        // jet variables
//         *(floatBranches["first_bjet_pt"]) = first_bjet_pt;
        *(floatBranches["first_jet_pt"]) = first_jet_pt;
        *(floatBranches["second_jet_pt"]) = second_jet_pt;
        *(floatBranches["third_jet_pt"]) = third_jet_pt;
        *(floatBranches["fourth_jet_pt"]) = fourth_jet_pt;
        
        *(floatBranches["first_jet_eta"]) = first_jet_eta;
        *(floatBranches["second_jet_eta"]) = second_jet_eta;
        *(floatBranches["third_jet_eta"]) = third_jet_eta;
        *(floatBranches["fourth_jet_eta"]) = fourth_jet_eta;

        *(floatBranches["first_jet_CSV"]) = first_jet_CSV;
        *(floatBranches["second_jet_CSV"]) = second_jet_CSV;
        *(floatBranches["third_jet_CSV"]) = third_jet_CSV;
        *(floatBranches["fourth_jet_CSV"]) = fourth_jet_CSV;
        
        //kinematic variables
        ////ANN neural net inputs
        *(floatBranches["avg_btag_disc_btags"]) = fmax(avg_btag_disc_btags,-1.0);
        *(floatBranches["higgsLike_dijet_mass"]) = fmax(higgsLike_dijet_mass1,-1.0);
        *(floatBranches["higgsLike_dijet_mass2"]) = fmax(higgsLike_dijet_mass2,-1.0);
        *(intBranches["numHiggsLike_dijet_15"]) = numHiggsLike_dijet_15;
	*(floatBranches["sum_pt"]) = sum_pt;
	*(floatBranches["min_dr_jets"]) = min_dr_jets;
        ////leptons
        *(floatBranches["mass_leplep"]) = mass_leplep;
        *(floatBranches["dR_leplep"]) = dR_leplep;

      } //End if (CoreVariables)

        
        
//       if (debug_) std::cout << "OUTPUT VARIABLES: " <<
      std::cout << tmpName << "   "  << event->run << ",  " <<  event->lumi << ",  " << event->evt << ",  " << bdtValue << ",  "
		<< setprecision(3) << lep1Pt << ";" << setprecision(3) << lep1Eta << ";" << setprecision(3) << lep1Phi << ";  " 
		<< setprecision(3) << lep2Pt << ";" << setprecision(3) << lep2Eta << ";" << setprecision(3) << lep2Phi << ";  "  ;


      for (int j=0; j < numJets ; j++) {
	int jJet = jet_index[j] ;
	double jetPt = pfjetsSelected.at(jJet).pt;
	double jetEta = pfjetsSelected.at(jJet).eta;	
	double jetPhi = pfjetsSelected.at(jJet).phi;
	double jetCSV = pfjetsSelected.at(jJet).btagCombinedSecVertex;

	std::cout << setprecision(3) << jetPt << ";" << setprecision(3) << jetEta << ";" << setprecision(3) << jetPhi << ";" << setprecision(3) << jetCSV << "     " ;
      }

      std::cout << '\n';
      if (verbose) std::cout << "about to fill tree" <<std::endl;
      
      summaryTree->Fill();
      

    } // end loop over events


  }// end try

  catch(std::exception& e) {
    std::cerr << " ==> caught exception " << e.what() << std::endl;

    std::cerr << " Whatever it was, it probably wasn't good, so I will crash" << std::endl;
    bool foundException = true;
    assert (foundException == false);
    //continue;
  }


  std::cout << "\n\nDone with loop over events\n\n" << std::endl;
  
  histofile.Write();
  histofile.Close();

  std::cout << " Done! " << std::endl;

}
