 //Root includes                                   
 #include "TROOT.h"
 #include "Riostream.h"
 #include "TFile.h"
 #include "TH1.h"
 #include "TF1.h"
 #include "TH1F.h"
 #include "TSystem.h"
 #include "TStyle.h"
 #include "TTree.h"
 #include "TString.h"
 #include "TMath.h"
 #include "TAxis.h"
 #include "TKey.h"
 #include "TList.h"

//
//
// To use:
// root [0] .L getCombineInfo_dilep_ljets_new.C

//
// To generate the "combination" datacard with all channels enabled, call like this:
//     root [1] getCombineInfo_dilep_ljets("ttH120","combined_ljets_dilep","result_combined.root",7,7,7,0,0,0,1,1);
//     ===> the outout data card is then: combined_ljets_dilep_mu777_el777_dilep3t1_dilep2t1.dat
//
// To generate a datacard with all channels enabled, call like this:
//     root [1] getCombineInfo_dilep_ljets("ttH120","combined_ljets_dilep","result_combined.root",7,7,7,7,7,7,1,1);
//     ===> the outout data card is then: combined_ljets_dilep_mu777_el777_dilep3t1_dilep2t1.dat
//
// To generate a datacard with just l+jets channels enabled, call like this:
//     root [1] getCombineInfo_dilep_ljets("ttH120","combined_ljets_dilep","combined_ljets_dilep.root",7,7,7,7,7,7,0,0);
//     ===> the outout data card is then: combined_ljets_dilep_mu777_el777_dilep3t0_dilep2t0.dat
//
// To generate a datacard with just 6 jet channels enabled, call like this:
//     root [1] getCombineInfo_dilep_ljets("ttH120","combined_ljets_dilep","combined_ljets_dilep.root",0,0,7,0,0,7,0,0);
//     ===> the outout data card is then: combined_ljets_dilep_mu007_el007_dilep3t0_dilep2t0.dat
//
//
//
void getCombineInfo_dilep_ljets_awesome_8TeV(TString hmass="ttH120",TString outputTextFileName="dataCardOutput_ttH_ljets_dilep_120",
					     TString histName = "result_combined.root",
					     int ljets4cat=7,int ljets5cat=7,int ljets6cat=7,
					     int dilepCat1 = 1, int dilepCat2 = 1, bool noNorm = true,
					     int systFlagEnable = 0x3FFFFF)
{
//
//	TString discName = "MLP";
	TString discName = "CFMlpANN";
	const int numChannels = 13;
	TString channelNames[] = {
										"ljets_j4_t2", 		//	 "j4_2t",
										"ljets_j4_t3", 		//	 "j4_3t",
										"ljets_j4_t4", 		//	 "j4_3t",
										"ljets_j5_t2", 		//	 "j5_2t",
										"ljets_j5_t3", 		//	 "j5_3t",
										"ljets_j5_tge4", 		//	 "j5_3t",
										"ljets_jge6_t2", 		//	 "j6_2t",
										"ljets_jge6_t3", 		//	 "j6_3t",
										"ljets_jge6_tge4", 	//	 "j6_3t",
										"ge3t",				// dilep >= 3tags
										"e2je2t",				// dilep 2 jets 2 tags
                                        "e3je2t",           // dilep 3 jets, 2 tags
                                        "ge4je2t"           // dilep >=4 jets, 2 tags
										};
	TString channelNamesOutput[] = {
										"ljets_j4_t2", 		//	 "j4_2t",
										"ljets_j4_t3", 		//	 "j4_3t",
										"ljets_j4_t4", 		//	 "j4_3t",
										"ljets_j5_t2", 		//	 "j5_2t",
										"ljets_j5_t3", 		//	 "j5_3t",
										"ljets_j5_tge4", 		//	 "j5_3t",
										"ljets_jge6_t2", 		//	 "j6_2t",
										"ljets_jge6_t3", 		//	 "j6_3t",
										"ljets_jge6_tge4", 	//	 "j6_3t",
										"ge3t",				// dilep >= 3tags
										"e2je2t",				// dilep 2 jets 2 tags
                                        "e3je2t",           // dilep 3 jets, 2 tags
                                        "ge4je2t"           // dilep >=4 jets, 2 tags
										};
	int fileNumber[] = {
							0,0,0,0,0,0,0,0,0,
							1,1,1,1};												// last 4 are from dilepton
	
//
// Show which categories are included
	bool channelEnable[numChannels];
	for (int i=0; i<numChannels; i++) {
		channelEnable[i] = false;
	}
	for (int i=0; i<3; i++){
		if (ljets4cat & (1<< i)) channelEnable[i] = true;
		if (ljets5cat & (1<< i)) channelEnable[i+3] = true;
		if (ljets6cat & (1<< i)) channelEnable[i+6] = true;
	}
	if (dilepCat1 & 1) channelEnable[9] = true;
	if (dilepCat2 & 1) channelEnable[10] = true;
	if (dilepCat2 & 2) channelEnable[11] = true;
	if (dilepCat2 & 4) channelEnable[12] = true;

	for (int i=0; i<numChannels; i++) {
		cout << channelNames[i] << ": enabled? " << channelEnable[i] << endl;
	}
	
	char catVals[50];     // "_muXXX_elXXX.dat"
	int nnn = sprintf(catVals,"_ljet%d%d%d_dilep3t%d_dilep2t%d.dat",ljets4cat,ljets5cat,ljets6cat,dilepCat1,dilepCat2);
	outputTextFileName.Append(catVals);
	cout << "File name = " << outputTextFileName << endl;
//
// Setup the output file
	ofstream of;
	of.open(outputTextFileName.Data());
// 
	int numSamples = 10;
	TString sampleNames[] = {"ttH120","ttbar","ttbarPlusBBbar","ttbarPlusCCbar","singlet","wjets","zjets","ttbarW","ttbarZ","diboson"};
	sampleNames[0] = hmass;
	TString signalName = "ttH";

//	TString histName = "multinet_reh.root";
	
	TFile* inputFile = new TFile(histName);
	//TFile* inputfile[2];
//	inputFile[0] = new TFile("ttH_ljets_scale_7TeV.root");
//	inputFile[1] = new TFile("result_combined.root");
	//inputFile[0] = new TFile("ttH_7TeV.root");
	//inputFile[1] = new TFile("ttH_7TeV.root");
	
	//inputFile->ls();
//
// Print out header info
//	of << "imax " << numChannels << " # number of channels" << endl;
//	of << "jmax " << numSamples-1 << " # number of backgrounds" << endl;
//	of << "kmax " << "*" << " # number of nuisance parameters" << endl;
//	of << "---------------" << endl;

	of << "imax * # number of channels" << endl;
	of << "jmax * # number of backgrounds" << endl;
	of << "kmax * # number of nuisance parameters" << endl;
	of << "---------------" << endl;

//
// Do observed data
//
// Loop over output lines
	for (int n=0; n<2; n++) {
		if (n==0) {
			of << "bin                ";
		} else {
			of << "observation       ";
		}
		
//
// get the data
		TString hhname = "data_obs";
		hhname.Append("_");
		hhname.Append(discName);
		hhname.Append("_");
//
// Loop over the channels for each sample
		for (int j=0; j<numChannels; j++) {
			TString hname = hhname;
			hname.Append(channelNames[j]);
			if (n==0) {
				if (channelEnable[j]) of << " " << channelNamesOutput[j];
//				of << setw(10) << channelNames[j];
			} else {
              if (channelEnable[j]) {
				cout << "Getting hist " << hname << " " << fileNumber[j] << endl;
				TH1F* th = (TH1F*)inputFile->Get(hname);
//				TH1F* th = (TH1F*)inputFile[fileNumber[j]]->Get(hname);
				float num = th->Integral();
				cout << "data " << channelNames[j] << " " << num << endl;
//				of.setf(ios::fixed, ios::floatfield);
//				of << setprecision(0) << setw(12) << num;
				of << "   " << Form("%f",num);
                              }
			}
		}
		of << endl;
	}  	
	of << "---------------" << endl;
//
// 
	of << "shapes * * " << histName << " $PROCESS_CFMlpANN_$CHANNEL $PROCESS_CFMlpANN_$CHANNEL_$SYSTEMATIC" << endl;
	of << "shapes ttH * " << histName << " $PROCESS$MASS_CFMlpANN_$CHANNEL $PROCESS$MASS_CFMlpANN_$CHANNEL_$SYSTEMATIC" << endl;
	of << "---------------" << endl;
//
// NOw do backgrounds
//
// Loop over output lines
	for (int n=0; n<4; n++) {
		if (n==0) {
			of << "bin        ";
		} else if ((n==1) || (n==2)) {
			of << "process    ";
		} else if (n==3) {
			of << "rate       ";
		}
		
//
// Loop over the samples
		for (int i=0; i<numSamples; i++) {
			TString hhname = sampleNames[i];
			hhname.Append("_");
			hhname.Append(discName);
			hhname.Append("_");
//
// Loop over the channels for each sample
			for (int j=0; j<numChannels; j++) {
				TString hname = hhname;
				hname.Append(channelNames[j]);
				if (n==0) {
//					of << setw(12) << channelNames[j];
					if (channelEnable[j]) of << " " << channelNamesOutput[j];
				} else if (n==1) {
//					of << setw(12) << sampleNames[i];
					if (i == 0) {
						if (channelEnable[j]) of << " " << signalName;
					} else {
						if (channelEnable[j]) of << " " << sampleNames[i];
					}
				} else if (n==2) {
//					of << setw(12) << i;
					if (channelEnable[j]) of << " " << i;
				} else if (n==3) {
                                  if (channelEnable[j]) {
                                    cout << "hanme " << hname << endl;
                                    TH1F* th = (TH1F*)inputFile->Get(hname);
                                    //TH1F* th = (TH1F*)inputFile[fileNumber[j]]->Get(hname);
                                   float num = th->Integral();
                                   //of.setf(ios::fixed, ios::floatfield);
                                    //					of << setprecision(2) << setw(12) << num;
                                   if (noNorm && i == 0) {
                                     of << "  -1";
                                   } else {
                                     of << "  " << num;
                                   }
                                  }
				}
			}
		}
		of << endl;
	}  	
	of << "---------------" << endl;
//
// Loop over the samples
	for (int j=0; j<numChannels; j++) {
		if (channelEnable[j]) {
			cout << "channel " << channelNames[j] << " ";
			float sum = 0.0;
			for (int i=0; i<numSamples; i++) {
				TString hhname = sampleNames[i];
				hhname.Append("_");
				hhname.Append(discName);
				hhname.Append("_");
				TString hname = hhname;
				hname.Append(channelNames[j]);
				TH1F* th = (TH1F*)inputFile->Get(hname);
//				TH1F* th = (TH1F*)inputFile[fileNumber[j]]->Get(hname);
				float num = th->Integral();
				if (i == 0) cout << "  ttH: " << num;
				if (i == 1) cout << "; summed background: ";
				if (i >= 1) sum = sum + num;
			}
			cout << sum << endl;
		}
	}
//
// Loop over the channels for each sample

	int numSysts = 39;//21;
	TString systNames[] = {
	  "lumi",
	  "QCDscale_ttH",
	  "QCDscale_ttbar",
	  "pdf_gg",
	  "pdf_qqbar",
	  "pdf_qg",
	  "QCDscale_V",
	  "QCDscale_VV",
	  "CMS_ttH_pu",
	  "CMS_res_j",
	  "CMS_ttH_eff_lep",
	  "Q2scale_ttH_ttbar0p",
	  "Q2scale_ttH_ttbar_bb",
	  "Q2scale_ttH_ttbar_cc",
	  "Q2scale_ttH_V",
	  "CMS_fake_bRate",
	  "CMS_eff_bRate",
	  "CMS_scale_j",
	  "CMS_ttH_PUcorr",
	  "ljets_j4_t3_fake_bShape",
	  "ljets_j4_t4_fake_bShape",
	  "ljets_j5_t3_fake_bShape",
	  "ljets_j5_tge4_fake_bShape",
	  "ljets_jge6_t2_fake_bShape",
	  "ljets_jge6_t3_fake_bShape",
	  "ljets_jge6_tge4_fake_bShape",
	  "ge3t_fake_bShape",
	  "e2je2t_fake_bShape",
	  "ljets_j4_t3_eff_bShape",
	  "ljets_j4_t4_eff_bShape",
	  "ljets_j5_t3_eff_bShape",
	  "ljets_j5_tge4_eff_bShape",
	  "ljets_jge6_t2_eff_bShape",
	  "ljets_jge6_t3_eff_bShape",
	  "ljets_jge6_tge4_eff_bShape",
	  "ge3t_eff_bShape",
	  "e2je2t_eff_bShape",
      "Q2scale_ttH_ttbar1p",
      "Q2scale_ttH_ttbar2p"
	};

	TString sysTypes[] = {
										"lnN",			 //	"lumi",
										"lnN",			 //	"QCDscale_ttH",
										"lnN",			 //	"QCDscale_ttbar",
										"lnN",			 //	"pdf_gg",
										"lnN",			 //	"pdf_qqbar",
										"lnN",			 //	"pdf_qg",
										"lnN",			 //	"QCDscale_V",
										"lnN",			 //	"QCDscale_VV",
										"lnN",			 //	"CMS_ttH_pu",
										"lnN",			 //   "CMS_res_j",
										"lnN",			 //   "CMS_ttH_eff_lep",
										"shape",			 //   "Q2scale_ttH_ttbar",
										"shape",    	 //   "Q2scale_ttH_ttbar_bb",
										"shape", 		 //	"Q2scale_ttH_ttbar_cc",
										"lnN", 		 //	"Q2scale_ttH_V",
										"shape", 		 //	"CMS_fake_bRate",
										"shape",			 //	"CMS_eff_bRate",
										"shape",			 //	"CMS_scale_j",
										"shape",              // "CMS_ttH_PUcorr"
										"shape", 		 //	"CMS_fake_bShape",
										"shape", 		 //	"CMS_fake_bShape",
										"shape", 		 //	"CMS_fake_bShape",
										"shape", 		 //	"CMS_fake_bShape",
										"shape", 		 //	"CMS_fake_bShape",
										"shape", 		 //	"CMS_fake_bShape",
										"shape", 		 //	"CMS_fake_bShape",
										"shape", 		 //	"CMS_fake_bShape",
										"shape", 		 //	"CMS_fake_bShape",
										"shape",			 //	"CMS_eff_bShape",
										"shape",			 //	"CMS_eff_bShape",
										"shape",			 //	"CMS_eff_bShape",
										"shape",			 //	"CMS_eff_bShape",
										"shape",			 //	"CMS_eff_bShape",
										"shape",			 //	"CMS_eff_bShape",
										"shape",			 //	"CMS_eff_bShape",
										"shape",			 //	"CMS_eff_bShape",
										"shape",			 //	"CMS_eff_bShape",
                                        "shape",              // 1p
                                        "shape"              // 2p
        };									

//                       "ttH120", "ttbar","ttbarPlusBBbar","ttbarPlusCCbar", "singlet", "wjets", "zjets", "ttbarW", "ttbarZ", "diboson"};
	TString valSyst[] = {  
	                     "1.044",  "1.044",     "1.044",  	 "1.044", 	 	  "1.044",   "1.044",  "1.044", "1.044", "1.044",    "1.044", // "lumi",
	                      "1.125",      "-",   	 "-", 		 "-", 	 	 	  "-",		  "-",		"-",  	"-",  	"-",  	  "-",     // "QCDscale_ttH",
	                         "-",   "1.12",       "1.12",    "1.12",      	  "1.02",	  "-",		"-",  	"1.15",  	"1.15",  	  "-",     // "QCDscale_ttbar",
	                         "1.08",   "1.09",       "1.09",   	"1.09", 	    	  "-",		  "-",		"-",  	"-",  	"1.09",  	  "-",     // "pdf_gg",
	                         "-",      "-",       "-", 		 "-",  	    	  "-",	     "1.048",	"1.042",   "-",   "1.07",		  "-",     // "pdf_qqbar",
	                         "-",      "-",       "-", 		 "-",  		 	 "1.046",	  "-",   	"-",	   "-",	   "-",		  "-",     // "pdf_qg",
	                         "-",      "-",   	 "-", 		 "-",     	 	  "-",	     "1.013",	"1.012",   "-",   "-", 		  "-",     // "QCDscale_V",
	                         "-",      "-",   	 "-", 		 "-", 	 	 	  "-",		  "-",		 "-", 	"-",  	"-",  	  "1.035", // "QCDscale_VV",
	                         "1.01",   "1.01",	 "1.01", 	 "1.01",     	   "1.01",    "1.01",   "1.01",  "1.01",  "1.01",	  "1.01",  // "CMS_ttH_pu",
	                         "1.015",   "1.015",	 "1.015",	 "1.015",    	  "1.015",    "1.015",  "1.015", "1.015", "1.015",	  "1.015", // "CMS_res_j",
	                         "1.04",   "1.04",	 "1.04",	 "1.04",    	  "1.04",    "1.04",  "1.04", "1.04", "1.04",	  "1.04", // "CMS_ttH_eff_lep",
	                         "-",      "1",   	 "-", 		 "-", 	 	 	  "-",		  "-",   	"-", 	   "-", 	   "-",  	  "-",     // "Q2scale_ttH_ttbar",
	                         "-",      "-",   	 "1", 		 "-", 	 	 	  "-",		  "-",   	"-", 	   "-", 	   "-",  	  "-",     // "Q2scale_ttH_ttbar_bb",
	                         "-",      "-",   	 "-", 		 "1", 	 	 	  "-",		  "-",   	"-", 	   "-", 	   "-",  	  "-",     // "Q2scale_ttH_ttbar_cc",
	                         "-",      "-",   	 "-", 		 "-", 	 	 	  "-",		  "-",	   "-", 	   "-",  	"-",  	  "-",     // "Q2scale_ttH_V" (value set below),
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bRate",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bRate",
                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",	     // "CMS_scale_j",
                             "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",	     // CMS_ttH_PUcorr,
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_fake_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bShape",
	                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bShape",
                         "1",      "1",   	 "1", 		 "1", 	 	 	  "1",		  "1",   	"1", 	   "1", 	   "1",  	  "1",     // "CMS_eff_bShape",
                         "-",      "1",   	 "-", 		 "-", 	 	 	  "-",		  "-",   	"-", 	   "-", 	   "-",  	  "-",     // "Q2scale_ttH_ttbar1p",
                         "-",      "1",   	 "-", 		 "-", 	 	 	  "-",		  "-",   	"-", 	   "-", 	   "-",  	  "-"     // "Q2scale_ttH_ttbar2p",
        };

        
	bool systEnabled[] = {
								true,	  //  "lumi",
								true,	  //  "QCDscale_ttH",
								true,	  //  "QCDscale_ttbar",
								true,	  //  "pdf_gg", 												
								true,	  //  "pdf_qqbar", 												
								true,	  //  "pdf_qg", 												
								true,	  //  "QCDscale_V",  										
								true,	  //  "QCDscale_VV",
								true,	  //  "CMS_ttH_pu",
								true,	  //  "CMS_res_j",
								true,	  //  "CMS_ttH_eff_lep",
								true,   //  "Q2scale_ttH_ttbar",
								true,   //  "Q2scale_ttH_ttbar_bb",
								true,	  //  "Q2scale_ttH_ttbar_cc",
								true,	  //  "Q2scale_ttH_V",
								true,	  //  "CMS_fake_bRate",
								true,   //  "CMS_eff_bRate",
								true,	  //  "CMS_scale_j",
								true,      // CMS_ttH_PUcorr
								true,	  //  "CMS_fake_bShape",
								true,	  //  "CMS_fake_bShape",
								true,	  //  "CMS_fake_bShape",
								true,	  //  "CMS_fake_bShape",
								true,	  //  "CMS_fake_bShape",
								true,	  //  "CMS_fake_bShape",
								true,	  //  "CMS_fake_bShape",
								true,	  //  "CMS_fake_bShape",
								true,	  //  "CMS_fake_bShape",
								true,   //  "CMS_eff_bShape",
								true,   //  "CMS_eff_bShape",
								true,   //  "CMS_eff_bShape",
								true,   //  "CMS_eff_bShape",
								true,   //  "CMS_eff_bShape",
								true,   //  "CMS_eff_bShape",
								true,   //  "CMS_eff_bShape",
								true,   //  "CMS_eff_bShape",
								true,   //  "CMS_eff_bShape",
                                true,   // 1p
                                true
								}; 			
								

//
// Loop over systematica
	for (int n=0; n<numSysts; n++) {
          // if (!(systFlagEnable & (1<<n)))
          //   systEnabled[n] = false;
          printf("setting enables: %d %x %x; %d\n",n,systFlagEnable,(1<<n),systEnabled[n]);
	}							
    //
    // Loop over systematica
	for (int n=0; n<numSysts; n++) {
      if (systEnabled[n]) {
        of <<	systNames[n] << "  " << 	sysTypes[n] << "  ";
        //
        // Loop over the samples
        for (int i=0; i<numSamples; i++) {
          //
          // Loop over the channels for each sample
          for (int j=0; j<numChannels; j++) {
		
            if (channelEnable[j]) {
                  
              const char *syst = valSyst[n*numSamples+i];
                  
              if( systNames[n].Contains("bShape") ){
                if( systNames[n].Contains(channelNames[j]) ) syst = Form("1");
                else                                         syst = Form("-");
              }


                                              
              // Q2_ttH_ttbar0p only for ljets_j4_t3, ljets_j4_t4, e2je2t
              if (systNames[n] == "Q2scale_ttH_ttbar0p"){
                // if you're not any of these, then skip
                if (!channelNames[j].Contains("e2je2t")
                    && !channelNames[j].Contains("ljets_j4_t2")
                    && !channelNames[j].Contains("ljets_j4_t3")
                    && !channelNames[j].Contains("ljets_j4_t4")) {                      
                  syst = Form("-");
                } else {
                  cout << systNames[n] << ": using channel " << channelNames[j]  << "  sample name  " << sampleNames[i] << " value " << syst  <<  endl;
                }
              }
                  
              if (systNames[n] == "Q2scale_ttH_ttbar1p"){
                // if you're not any of these, then skip
                if (!channelNames[j].Contains("ge3t")
                    && !channelNames[j].Contains("ljets_j5_t2")
                    && !channelNames[j].Contains("ljets_j5_t3")
                    && !channelNames[j].Contains("ljets_j5_tge4")) {                      
                  syst = Form("-"); // off
                } else {
                  cout << systNames[n] << ": using channel " << channelNames[j]  << "  sample name  " << sampleNames[i] << " value " << syst  <<  endl;
                }
              }

              if (systNames[n] == "Q2scale_ttH_ttbar2p"){
                // if you're not any of these, then skip
                if (   !channelNames[j].Contains("ljets_jge6_t2")
                       && !channelNames[j].Contains("ljets_jge6_t3")
                       && !channelNames[j].Contains("ljets_jge6_tge4")) {                      
                  syst = Form("-");
                } else {
                  cout << systNames[n] << ": using channel " << channelNames[j]  << "  sample name  " << sampleNames[i] << " value " << syst  <<  endl;
                }
              }


          


          
              //Make this a little more complicated so that some of the systematics
              //get bigger as the jet bin increases
              if (systNames[n] == "Q2scale_ttH_V") {
                if (sampleNames[i] == "wjets" ||
                    sampleNames[i] == "zjets") {
                  int iJet = 0;
                  if (channelNames[j].Contains("e2je2t")) {
                    iJet = 2;
                    syst = Form("%f",1+ 0.1 * iJet); //+10% per jet
                  } else if (channelNames[j].Contains("e3je2t")) {
                    iJet = 3;
                    syst = Form("%f",1+ 0.1 * iJet); //+10% per jet
                  } else if (channelNames[j].Contains("ge4je2t")) {
                    iJet = 4;
                    syst = Form("%f",1+ 0.1 * iJet); //+10% per jet
                  } else if (channelNames[j].Contains("ge3t")) {
                    iJet = 3;
                    syst = Form("%f",1+ 0.1 * iJet); //+10% per jet
                  } else if (channelNames[j].Contains("j4_t")) {
                    iJet = 4;
                    syst = Form("%f",1+ 0.1 * iJet); //+10% per jet
                  } else if  (channelNames[j].Contains("j5_t")) {
                    iJet = 5;
                    syst = Form("%f",1+ 0.1 * iJet); //+10% per jet
                  } else if  (channelNames[j].Contains("jge6_t")) {
                    iJet = 6;
                    syst = Form("%f",1+ 0.1 * iJet); //+10% per jet
                  } else {
                    syst = "-";
                  }
                }
              }
              of << syst << "  ";
              //					of << setprecision(2) << setw(12) <<  valSyst[n*numSamples + i] ;
            }      
          }
        }
            
        of << endl;
      } else {
        cout << systNames[n] << " " << 	sysTypes[n] << " not enabled." << endl;
      }
    }   

    of << "---------------" << endl;
       
    inputFile->Close();
    //        inputFile[1]->Close();
    //
    
        
}
