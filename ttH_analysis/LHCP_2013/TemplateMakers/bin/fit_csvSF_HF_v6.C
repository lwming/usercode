//
//   Example of a program to fit non-equidistant data points
//   =======================================================
//
//   The fitting function fcn is a simple chisquare function
//   The data consists of 5 data points (arrays x,y,z) + the errors in errorsz
//   More details on the various functions or parameters for these functions
//   can be obtained in an interactive ROOT session with:
//    Root > TMinuit *minuit = new TMinuit(10);
//    Root > minuit->mnhelp("*")  to see the list of possible keywords
//    Root > minuit->mnhelp("SET") explains most parameters
//Author: Rene Brun

#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TVirtualFitter.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"

#include "TMatrixDSym.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

#include "TCanvas.h"

#include "Math/MinimizerOptions.h"
#include <TSpline.h>
#include "Math/Interpolator.h"
#define MAXPOINTS 200

//______________________________________________________________________________
void fit_csvSF_HF_v6(){


  TH1::SetDefaultSumw2();

  TString dirprefix = "Images/Images_2013_04_16_v1_8TeV_csvSF_fit_hf/";

  struct stat st;
  if( stat(dirprefix.Data(),&st) != 0 )  mkdir(dirprefix.Data(),0777);


  TFile *file = TFile::Open("HistoFiles/csv_rwt_hf_IT_v2.root");
  TFile *file_JESUp = TFile::Open("HistoFiles/csv_rwt_hf_IT_v2_JESUp.root");
  TFile *file_JESDown = TFile::Open("HistoFiles/csv_rwt_hf_IT_v2_JESDown.root");


  std::string histofilename = "HistoFiles/csv_rwt_hf_final_IT_v2.root";
  TFile histofile(histofilename.c_str(),"recreate");
  histofile.cd();


  double useUp = 1.2;
  double useDown = 0.8;

  int ncsvbins = 18;
  // double csvbins[] = { -10., 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.723, 0.767, 0.810, 0.854, 0.898, 0.918, 0.939, 0.959, 0.980, 1.01 };
  double csvbins_new[] = { -0.04, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.723, 0.767, 0.810, 0.854, 0.898, 0.918, 0.939, 0.959, 0.980, 1.01 };

  std::vector<TString> hist_name;
  std::vector<TString> data_hist_name;
  std::vector<TString> mc_b_hist_name;
  std::vector<TString> mc_nonb_hist_name;

  int maxPt  = 5;
  int maxEta = 1;

  for( int iPt=0; iPt<maxPt; iPt++ ){
    for( int iEta=0; iEta<maxEta; iEta++ ){
      hist_name.push_back( Form("csv_ratio_Pt%d_Eta%d", iPt, iEta) );

      data_hist_name.push_back( Form("csv_Data_Pt%d_Eta%d", iPt, iEta) );
      mc_b_hist_name.push_back( Form("csv_MC_bjets_Pt%d_Eta%d", iPt, iEta) );
      mc_nonb_hist_name.push_back( Form("csv_MC_nonbjets_Pt%d_Eta%d", iPt, iEta) );
    }
  }

  int NumHists_normal = int( hist_name.size() );
  int numHists = NumHists_normal+2;

  TH1D* h_csv_ratio[numHists];
  TH1D* h_csv_ratio_LF[numHists];
  TH1D* h_csv_ratio_LFUp[numHists];
  TH1D* h_csv_ratio_LFDown[numHists];
  TH1D* h_csv_ratio_JESUp[numHists];
  TH1D* h_csv_ratio_JESDown[numHists];
  TH1D* h_csv_ratio_Stats1Up[numHists];
  TH1D* h_csv_ratio_Stats1Down[numHists];
  TH1D* h_csv_ratio_Stats2Up[numHists];
  TH1D* h_csv_ratio_Stats2Down[numHists];

  TH1D* h_csv_data[NumHists_normal];
  TH1D* h_csv_mc_b[NumHists_normal];
  TH1D* h_csv_mc_nonb[NumHists_normal];

  TH1D* h_csv_mc_b_JESUp[NumHists_normal];
  TH1D* h_csv_mc_nonb_JESUp[NumHists_normal];
  TH1D* h_csv_mc_b_JESDown[NumHists_normal];
  TH1D* h_csv_mc_nonb_JESDown[NumHists_normal];


  TH1D* h_csv_data_all;
  TH1D* h_csv_mc_b_all;
  TH1D* h_csv_mc_nonb_all;

  TH1D* h_csv_data_all_JES;

  TH1D* h_csv_mc_b_all_JESUp;
  TH1D* h_csv_mc_nonb_all_JESUp;

  TH1D* h_csv_mc_b_all_JESDown;
  TH1D* h_csv_mc_nonb_all_JESDown;

  for( int iHist=0; iHist<NumHists_normal; iHist++ ){
    TH1D* h_csv_data_temp0 = (TH1D*)file->Get( data_hist_name[iHist] )->Clone( Form("h_%s_temp0",data_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_b_temp0 = (TH1D*)file->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_temp0 = (TH1D*)file->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_temp0",mc_nonb_hist_name[iHist].Data()) );

    // JES up/down
    TH1D* h_csv_mc_b_JESUp_temp0 = (TH1D*)file_JESUp->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_JESUp_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_JESUp_temp0 = (TH1D*)file_JESUp->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_JESUp_temp0",mc_nonb_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_b_JESDown_temp0 = (TH1D*)file_JESDown->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_JESDown_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_JESDown_temp0 = (TH1D*)file_JESDown->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_JESDown_temp0",mc_nonb_hist_name[iHist].Data()) );

    h_csv_data[iHist] = new TH1D( Form("h_%s",data_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_b[iHist] = new TH1D( Form("h_%s",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_nonb[iHist] = new TH1D( Form("h_%s",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );

    h_csv_mc_b_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_nonb_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_b_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_nonb_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );

    for( int iBin=0; iBin<ncsvbins; iBin++ ){
      h_csv_data[iHist]->SetBinContent(iBin+1, h_csv_data_temp0->GetBinContent(iBin+1));
      h_csv_data[iHist]->SetBinError(iBin+1, h_csv_data_temp0->GetBinError(iBin+1));

      h_csv_mc_b[iHist]->SetBinContent(iBin+1, h_csv_mc_b_temp0->GetBinContent(iBin+1));
      h_csv_mc_b[iHist]->SetBinError(iBin+1, h_csv_mc_b_temp0->GetBinError(iBin+1));

      h_csv_mc_nonb[iHist]->SetBinContent(iBin+1, h_csv_mc_nonb_temp0->GetBinContent(iBin+1));
      h_csv_mc_nonb[iHist]->SetBinError(iBin+1, h_csv_mc_nonb_temp0->GetBinError(iBin+1));

      h_csv_mc_b_JESUp[iHist]->SetBinContent(iBin+1, h_csv_mc_b_JESUp_temp0->GetBinContent(iBin+1));
      h_csv_mc_b_JESUp[iHist]->SetBinError(iBin+1, h_csv_mc_b_JESUp_temp0->GetBinError(iBin+1));
      h_csv_mc_nonb_JESUp[iHist]->SetBinContent(iBin+1, h_csv_mc_nonb_JESUp_temp0->GetBinContent(iBin+1));
      h_csv_mc_nonb_JESUp[iHist]->SetBinError(iBin+1, h_csv_mc_nonb_JESUp_temp0->GetBinError(iBin+1));

      h_csv_mc_b_JESDown[iHist]->SetBinContent(iBin+1, h_csv_mc_b_JESDown_temp0->GetBinContent(iBin+1));
      h_csv_mc_b_JESDown[iHist]->SetBinError(iBin+1, h_csv_mc_b_JESDown_temp0->GetBinError(iBin+1));
      h_csv_mc_nonb_JESDown[iHist]->SetBinContent(iBin+1, h_csv_mc_nonb_JESDown_temp0->GetBinContent(iBin+1));
      h_csv_mc_nonb_JESDown[iHist]->SetBinError(iBin+1, h_csv_mc_nonb_JESDown_temp0->GetBinError(iBin+1));
    }


    if( iHist==0 ){
      h_csv_data_all = (TH1D*)h_csv_data[iHist]->Clone("h_csv_data_all");
      h_csv_mc_b_all = (TH1D*)h_csv_mc_b[iHist]->Clone("h_csv_mc_b_all");
      h_csv_mc_nonb_all = (TH1D*)h_csv_mc_nonb[iHist]->Clone("h_csv_mc_nonb_all");

      h_csv_mc_b_all_JESUp = (TH1D*)h_csv_mc_b_JESUp[iHist]->Clone("h_csv_mc_b_all_JESUp");
      h_csv_mc_nonb_all_JESUp = (TH1D*)h_csv_mc_nonb_JESUp[iHist]->Clone("h_csv_mc_nonb_all_JESUp");
      h_csv_mc_b_all_JESDown = (TH1D*)h_csv_mc_b_JESDown[iHist]->Clone("h_csv_mc_b_all_JESDown");
      h_csv_mc_nonb_all_JESDown = (TH1D*)h_csv_mc_nonb_JESDown[iHist]->Clone("h_csv_mc_nonb_all_JESDown");
    }
    else{
      h_csv_data_all->Add(h_csv_data[iHist]);
      h_csv_mc_b_all->Add(h_csv_mc_b[iHist]);
      h_csv_mc_nonb_all->Add(h_csv_mc_nonb[iHist]);

      h_csv_mc_b_all_JESUp->Add(h_csv_mc_b_JESUp[iHist]);
      h_csv_mc_nonb_all_JESUp->Add(h_csv_mc_nonb_JESUp[iHist]);
      h_csv_mc_b_all_JESDown->Add(h_csv_mc_b_JESDown[iHist]);
      h_csv_mc_nonb_all_JESDown->Add(h_csv_mc_nonb_JESDown[iHist]);
    }


    h_csv_ratio[iHist]        = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_%d",iHist));
    h_csv_ratio_LFUp[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_LFUp_%d",iHist));
    h_csv_ratio_LFDown[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_LFDown_%d",iHist));

    h_csv_ratio_JESUp[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_JESUp_%d",iHist));
    h_csv_ratio_JESDown[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_JESDown_%d",iHist));

    TH1D* h_csv_mc_b_temp0_LFUp = (TH1D*)h_csv_mc_b[iHist]->Clone(Form("h_csv_mc_b_temp0_LFUp_%d",iHist));
    TH1D* h_csv_mc_b_temp0_LFDown = (TH1D*)h_csv_mc_b[iHist]->Clone(Form("h_csv_mc_b_temp0_LFDown_%d",iHist));

    TH1D* h_csv_mc_nonb_temp0_LFUp = (TH1D*)h_csv_mc_nonb[iHist]->Clone(Form("h_csv_mc_nonb_temp0_LFUp_%d",iHist));
    TH1D* h_csv_mc_nonb_temp0_LFDown = (TH1D*)h_csv_mc_nonb[iHist]->Clone(Form("h_csv_mc_nonb_temp0_LFDown_%d",iHist));

    h_csv_mc_b_temp0_LFUp->Scale( h_csv_data[iHist]->Integral() / ( h_csv_mc_b[iHist]->Integral() + useUp*h_csv_mc_nonb[iHist]->Integral() ) );
    h_csv_mc_b_temp0_LFDown->Scale( h_csv_data[iHist]->Integral() / ( h_csv_mc_b[iHist]->Integral() + useDown*h_csv_mc_nonb[iHist]->Integral() ) );

    h_csv_mc_nonb_temp0_LFUp->Scale( h_csv_data[iHist]->Integral() / ( h_csv_mc_b[iHist]->Integral() + useUp*h_csv_mc_nonb[iHist]->Integral() ) );
    h_csv_mc_nonb_temp0_LFDown->Scale( h_csv_data[iHist]->Integral() / ( h_csv_mc_b[iHist]->Integral() + useDown*h_csv_mc_nonb[iHist]->Integral() ) );

    h_csv_ratio[iHist]->Add(h_csv_mc_nonb[iHist],-1);
    h_csv_ratio_LFUp[iHist]->Add(h_csv_mc_nonb_temp0_LFUp,-useUp);
    h_csv_ratio_LFDown[iHist]->Add(h_csv_mc_nonb_temp0_LFDown,-useDown);
    h_csv_ratio_JESUp[iHist]->Add(h_csv_mc_nonb_JESUp[iHist],-1);
    h_csv_ratio_JESDown[iHist]->Add(h_csv_mc_nonb_JESDown[iHist],-1);

    h_csv_ratio[iHist]->Divide(h_csv_mc_b[iHist]);
    h_csv_ratio_LFUp[iHist]->Divide(h_csv_mc_b_temp0_LFUp);
    h_csv_ratio_LFDown[iHist]->Divide(h_csv_mc_b_temp0_LFDown);
    h_csv_ratio_JESUp[iHist]->Divide(h_csv_mc_b_JESUp[iHist]);
    h_csv_ratio_JESDown[iHist]->Divide(h_csv_mc_b_JESDown[iHist]);

  }


  TH1D* h_csv_ratio_all = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_temp");
  TH1D* h_csv_ratio_all_LFUp   = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_LFUp_temp");
  TH1D* h_csv_ratio_all_LFDown = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_LFDown_temp");

  TH1D* h_csv_ratio_all_JESUp   = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_JESUp_temp");
  TH1D* h_csv_ratio_all_JESDown = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_JESDown_temp");

  TH1D* h_csv_ratio_cumulative = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative");
  TH1D* h_csv_ratio_cumulative_LFUp = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_LFUp");
  TH1D* h_csv_ratio_cumulative_LFDown = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_LFDown");
  TH1D* h_csv_ratio_cumulative_JESUp = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_JESUp");
  TH1D* h_csv_ratio_cumulative_JESDown = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_JESDown");
  TH1D* h_csv_ratio_cumulative_Stats1Up = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_Stats1Up");
  TH1D* h_csv_ratio_cumulative_Stats1Down = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_Stats1Down");
  TH1D* h_csv_ratio_cumulative_Stats2Up = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_Stats2Up");
  TH1D* h_csv_ratio_cumulative_Stats2Down = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_Stats2Down");

  int nBins = h_csv_ratio_cumulative->GetNbinsX();

  TH1D* h_csv_mc_b_all_LFUp = (TH1D*)h_csv_mc_b_all->Clone("h_csv_mc_b_all_LFUp");
  TH1D* h_csv_mc_b_all_LFDown = (TH1D*)h_csv_mc_b_all->Clone("h_csv_mc_b_all_LFDown");

  TH1D* h_csv_mc_nonb_all_LFUp = (TH1D*)h_csv_mc_nonb_all->Clone("h_csv_mc_nonb_all_LFUp");
  TH1D* h_csv_mc_nonb_all_LFDown = (TH1D*)h_csv_mc_nonb_all->Clone("h_csv_mc_nonb_all_LFDown");


  h_csv_mc_b_all_LFUp->Scale( h_csv_ratio_all->Integral() / (h_csv_mc_b_all->Integral() +  useUp*h_csv_mc_nonb_all->Integral()) );
  h_csv_mc_b_all_LFDown->Scale( h_csv_ratio_all->Integral() / (h_csv_mc_b_all->Integral() +  useDown*h_csv_mc_nonb_all->Integral()) );

  h_csv_mc_nonb_all_LFUp->Scale( useUp * h_csv_ratio_all->Integral() / (h_csv_mc_b_all->Integral() +  useUp*h_csv_mc_nonb_all->Integral()) );
  h_csv_mc_nonb_all_LFDown->Scale( useDown * h_csv_ratio_all->Integral() / (h_csv_mc_b_all->Integral() +  useDown*h_csv_mc_nonb_all->Integral()) );

  h_csv_ratio_all->Add(h_csv_mc_nonb_all,-1);
  h_csv_ratio_all_LFUp->Add(h_csv_mc_nonb_all_LFUp,-1);
  h_csv_ratio_all_LFDown->Add(h_csv_mc_nonb_all_LFDown,-1);
  h_csv_ratio_all_JESUp->Add(h_csv_mc_nonb_all_JESUp,-1);
  h_csv_ratio_all_JESDown->Add(h_csv_mc_nonb_all_JESDown,-1);


  for( int iBin=0; iBin<nBins; iBin++ ){
    h_csv_ratio_cumulative->SetBinContent( iBin+1, h_csv_ratio_all->Integral(iBin+1,nBins) );
    h_csv_ratio_cumulative_LFUp->SetBinContent( iBin+1, h_csv_ratio_all_LFUp->Integral(iBin+1,nBins) );
    h_csv_ratio_cumulative_LFDown->SetBinContent( iBin+1, h_csv_ratio_all_LFDown->Integral(iBin+1,nBins) );
    h_csv_ratio_cumulative_JESUp->SetBinContent( iBin+1, h_csv_ratio_all_JESUp->Integral(iBin+1,nBins) );
    h_csv_ratio_cumulative_JESDown->SetBinContent( iBin+1, h_csv_ratio_all_JESDown->Integral(iBin+1,nBins) );
  }


  h_csv_ratio_all->Divide(h_csv_mc_b_all);
  h_csv_ratio_all_LFUp->Divide(h_csv_mc_b_all_LFUp);
  h_csv_ratio_all_LFDown->Divide(h_csv_mc_b_all_LFDown);
  h_csv_ratio_all_JESUp->Divide(h_csv_mc_b_all_JESUp);
  h_csv_ratio_all_JESDown->Divide(h_csv_mc_b_all_JESDown);



  TH1D* h_mc_nonb_cumulative = (TH1D*)h_csv_mc_nonb_all->Clone("h_mc_nonb_cumulative");
  TH1D* h_mc_b_cumulative = (TH1D*)h_csv_mc_b_all->Clone("h_mc_b_cumulative");
  TH1D* h_mc_b_cumulative_LFUp = (TH1D*)h_csv_mc_b_all_LFUp->Clone("h_mc_b_cumulative_LFUp");
  TH1D* h_mc_b_cumulative_LFDown = (TH1D*)h_csv_mc_b_all_LFDown->Clone("h_mc_b_cumulative_LFDown");
  TH1D* h_mc_b_cumulative_JESUp = (TH1D*)h_csv_mc_b_all_JESUp->Clone("h_mc_b_cumulative_JESUp");
  TH1D* h_mc_b_cumulative_JESDown = (TH1D*)h_csv_mc_b_all_JESDown->Clone("h_mc_b_cumulative_JESDown");

  for( int iBin=0; iBin<nBins; iBin++ ){
    h_mc_nonb_cumulative->SetBinContent( iBin+1, h_csv_mc_nonb_all->Integral(iBin+1,nBins) );

    h_mc_b_cumulative->SetBinContent( iBin+1, h_csv_mc_b_all->Integral(iBin+1,nBins) );
    h_mc_b_cumulative_LFUp->SetBinContent( iBin+1, h_csv_mc_b_all_LFUp->Integral(iBin+1,nBins) );
    h_mc_b_cumulative_LFDown->SetBinContent( iBin+1, h_csv_mc_b_all_LFDown->Integral(iBin+1,nBins) );
    h_mc_b_cumulative_JESUp->SetBinContent( iBin+1, h_csv_mc_b_all_JESUp->Integral(iBin+1,nBins) );
    h_mc_b_cumulative_JESDown->SetBinContent( iBin+1, h_csv_mc_b_all_JESDown->Integral(iBin+1,nBins) );
  }

  h_csv_ratio_cumulative->Divide(h_mc_b_cumulative);
  h_csv_ratio_cumulative_LFUp->Divide(h_mc_b_cumulative_LFUp);
  h_csv_ratio_cumulative_LFDown->Divide(h_mc_b_cumulative_LFDown);
  h_csv_ratio_cumulative_JESUp->Divide(h_mc_b_cumulative_JESUp);
  h_csv_ratio_cumulative_JESDown->Divide(h_mc_b_cumulative_JESDown);


  for( int iBin=0; iBin<nBins; iBin++ ){
    printf("\t iBin=%d,\t D=%.0f,\t LF=%.0f,\t HF=%.0f,\t iD=%.0f,\t iLF=%.0f,\t iHF=%.0f,\t iLFUp=%.0f,\t iHFUp=%.0f,\t iLFDown=%.0f,\t iHFDown=%.0f,\t (D-LF)/HF=%.2f,\t i(D-LF)/HF=%.3f,\t LFUp=%.3f,\t LFDown=%.3f,\t cum=%.2f,\t cumLFUp=%.2f,\t cumLFDown=%.2f \n", 
	   iBin, h_csv_data_all->GetBinContent(iBin+1), h_csv_mc_nonb_all->GetBinContent(iBin+1), h_csv_mc_b_all->GetBinContent(iBin+1),
	   h_csv_data_all->Integral(iBin+1,nBins), h_csv_mc_nonb_all->Integral(iBin+1,nBins), h_csv_mc_b_all->Integral(iBin+1,nBins),
	   h_csv_mc_nonb_all_LFUp->Integral(iBin+1,nBins), h_csv_mc_b_all_LFUp->Integral(iBin+1,nBins),
	   h_csv_mc_nonb_all_LFDown->Integral(iBin+1,nBins), h_csv_mc_b_all_LFDown->Integral(iBin+1,nBins),
	   (h_csv_data_all->GetBinContent(iBin+1) - h_csv_mc_nonb_all->GetBinContent(iBin+1))/h_csv_mc_b_all->GetBinContent(iBin+1),
	   (h_csv_data_all->Integral(iBin+1,nBins) - h_csv_mc_nonb_all->Integral(iBin+1,nBins))/h_csv_mc_b_all->Integral(iBin+1,nBins),
	   (h_csv_data_all->Integral(iBin+1,nBins) - h_csv_mc_nonb_all_LFUp->Integral(iBin+1,nBins))/h_csv_mc_b_all_LFUp->Integral(iBin+1,nBins),
	   (h_csv_data_all->Integral(iBin+1,nBins) - h_csv_mc_nonb_all_LFDown->Integral(iBin+1,nBins))/h_csv_mc_b_all_LFDown->Integral(iBin+1,nBins),
	   h_csv_ratio_cumulative->GetBinContent(iBin+1), h_csv_ratio_cumulative_LFUp->GetBinContent(iBin+1), h_csv_ratio_cumulative_LFDown->GetBinContent(iBin+1) );

  }

  double csvL = h_csv_ratio_cumulative->GetBinContent(4);
  double csvL_LFUp = h_csv_ratio_cumulative_LFUp->GetBinContent(4) - csvL;
  double csvL_JESUp = h_csv_ratio_cumulative_JESUp->GetBinContent(4) - csvL;
  double csvL_LFDown = h_csv_ratio_cumulative_LFDown->GetBinContent(4) - csvL;
  double csvL_JESDown = h_csv_ratio_cumulative_JESDown->GetBinContent(4) - csvL;

  double csvL_Up = sqrt( csvL_LFUp*csvL_LFUp + csvL_JESUp*csvL_JESUp );
  double csvL_Down = sqrt( csvL_LFDown*csvL_LFDown + csvL_JESDown*csvL_JESDown );

  double csvL_Err = 0.5 * (csvL_Up + csvL_Down);

  printf("\t CSVL: %.3f +/- %.3f \n", csvL, csvL_Err );

  double csvM = h_csv_ratio_cumulative->GetBinContent(9);
  double csvM_LFUp = h_csv_ratio_cumulative_LFUp->GetBinContent(9) - csvM;
  double csvM_JESUp = h_csv_ratio_cumulative_JESUp->GetBinContent(9) - csvM;
  double csvM_LFDown = h_csv_ratio_cumulative_LFDown->GetBinContent(9) - csvM;
  double csvM_JESDown = h_csv_ratio_cumulative_JESDown->GetBinContent(9) - csvM;

  double csvM_Up = sqrt( csvM_LFUp*csvM_LFUp + csvM_JESUp*csvM_JESUp );
  double csvM_Down = sqrt( csvM_LFDown*csvM_LFDown + csvM_JESDown*csvM_JESDown );

  double csvM_Err = 0.5 * (csvM_Up + csvM_Down);

  printf("\t CSVL: %.3f +/- %.3f \n", csvM, csvM_Err );

  double csvT = h_csv_ratio_cumulative->GetBinContent(14);
  double csvT_LFUp = h_csv_ratio_cumulative_LFUp->GetBinContent(14) - csvT;
  double csvT_JESUp = h_csv_ratio_cumulative_JESUp->GetBinContent(14) - csvT;
  double csvT_LFDown = h_csv_ratio_cumulative_LFDown->GetBinContent(14) - csvT;
  double csvT_JESDown = h_csv_ratio_cumulative_JESDown->GetBinContent(14) - csvT;

  double csvT_Up = sqrt( csvT_LFUp*csvT_LFUp + csvT_JESUp*csvT_JESUp );
  double csvT_Down = sqrt( csvT_LFDown*csvT_LFDown + csvT_JESDown*csvT_JESDown );

  double csvT_Err = 0.5 * (csvT_Up + csvT_Down);

  printf("\t CSVL: %.3f +/- %.3f \n", csvT, csvT_Err );


  hist_name.push_back("csv_ratio_all");

  hist_name.push_back("csv_ratio_cumulative_all");


  h_csv_ratio[numHists-2] = (TH1D*)h_csv_ratio_all->Clone( Form("h_%s",hist_name[numHists-2].Data()) );

  h_csv_ratio_LFUp[numHists-2] = (TH1D*)h_csv_ratio_all_LFUp->Clone( Form("h_%s_LFUp",hist_name[numHists-2].Data()) );
  h_csv_ratio_LFDown[numHists-2] = (TH1D*)h_csv_ratio_all_LFDown->Clone( Form("h_%s_LFDown",hist_name[numHists-2].Data()) );

  h_csv_ratio_JESUp[numHists-2] = (TH1D*)h_csv_ratio_all_JESUp->Clone( Form("h_%s_JESUp",hist_name[numHists-2].Data()) );
  h_csv_ratio_JESDown[numHists-2] = (TH1D*)h_csv_ratio_all_JESDown->Clone( Form("h_%s_JESDown",hist_name[numHists-2].Data()) );


  h_csv_ratio[numHists-1] = (TH1D*)h_csv_ratio_cumulative->Clone( Form("h_%s",hist_name[numHists-1].Data()) );
  h_csv_ratio[numHists-1]->SetMarkerStyle(20);


  for( int iHist=0; iHist<numHists-1; iHist++ ){
    h_csv_ratio_LF[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_LF",hist_name[numHists-2].Data()) );

    for( int iBin=0; iBin<nBins; iBin++ ){
      double content = h_csv_ratio_LF[iHist]->GetBinContent(iBin+1);
      double hfUp = fabs( content - h_csv_ratio_LFUp[iHist]->GetBinContent(iBin+1) );
      double hfDown = fabs( content - h_csv_ratio_LFDown[iHist]->GetBinContent(iBin+1) );

      double hfUnc = 0.5 * ( hfUp + hfDown );
      h_csv_ratio_LF[iHist]->SetBinError(iBin+1,hfUnc);
    }
  }


  for( int iHist=0; iHist<numHists-1; iHist++ ){
    h_csv_ratio_Stats1Up[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats1Up",hist_name[numHists-2].Data()) );
    h_csv_ratio_Stats1Down[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats1Down",hist_name[numHists-2].Data()) );
    h_csv_ratio_Stats2Up[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats2Up",hist_name[numHists-2].Data()) );
    h_csv_ratio_Stats2Down[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats2Down",hist_name[numHists-2].Data()) );

    for( int iBin=0; iBin<nBins; iBin++ ){
      double center = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
      double content = h_csv_ratio[iHist]->GetBinContent(iBin+1);

      double delta = h_csv_ratio[iHist]->GetBinError(iBin+1);
      double stat1Up   = content + delta * ( 1 - 2*center );
      double stat1Down = content + delta * ( 2*center - 1 );

      double stat2Up   = content + delta * ( 1 - 6*center*(1-center) );
      double stat2Down = content - delta * ( 1 - 6*center*(1-center) );

      h_csv_ratio_Stats1Up[iHist]->SetBinContent(iBin+1,stat1Up);
      h_csv_ratio_Stats1Down[iHist]->SetBinContent(iBin+1,stat1Down);

      h_csv_ratio_Stats2Up[iHist]->SetBinContent(iBin+1,stat2Up);
      h_csv_ratio_Stats2Down[iHist]->SetBinContent(iBin+1,stat2Down);
    }
  }



  int n = 100000;
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(n);



  TH1D* h_csv_ratio_final[numHists];
  TH1D* h_csv_ratio_final_LF[numHists];
  TH1D* h_csv_ratio_final_LFUp[numHists];
  TH1D* h_csv_ratio_final_LFDown[numHists];
  TH1D* h_csv_ratio_final_JESUp[numHists];
  TH1D* h_csv_ratio_final_JESDown[numHists];
  TH1D* h_csv_ratio_final_Stats1Up[numHists];
  TH1D* h_csv_ratio_final_Stats1Down[numHists];
  TH1D* h_csv_ratio_final_Stats2Up[numHists];
  TH1D* h_csv_ratio_final_Stats2Down[numHists];

  int NumFinalBins = 1000;


  TCanvas *c1 = new TCanvas("c1");
  TF1* btagSF = new TF1("btagSF","0.607239*x*x*x*x+-1.49984*x*x*x+1.3473*x*x+-0.599888*x+1.09396",0.15,0.95);
  TF1* btagSFUp   = new TF1("btagSFUp","1.02*(0.607239*x*x*x*x+-1.49984*x*x*x+1.3473*x*x+-0.599888*x+1.09396)",0.15,0.95);
  TF1* btagSFDown = new TF1("btagSFUp","0.98*(0.607239*x*x*x*x+-1.49984*x*x*x+1.3473*x*x+-0.599888*x+1.09396)",0.15,0.95);
  h_csv_ratio_cumulative->SetStats(0);
  h_csv_ratio_cumulative->GetYaxis()->SetRangeUser(0.88,1.1);
  h_csv_ratio_cumulative->GetXaxis()->SetRangeUser(0.1,1.00);
  h_csv_ratio_cumulative->SetMarkerStyle(20);
  h_csv_ratio_cumulative->SetTitle(";CSV;Heavy flavor scale factor");

  btagSFUp->SetLineStyle(2);
  btagSFDown->SetLineStyle(2);

  btagSFUp->SetLineColor(kMagenta-2);
  btagSFDown->SetLineColor(kMagenta-2);


  //TH1D* h_csv_ratio_cumulative_LF = (TH1D*)h_csv_ratio_cumulative_LFUp->Clone("h_csv_ratio_cumulative_LF");

  for( int iBin=0; iBin<nBins; iBin++ ){
    double center = h_csv_ratio_cumulative->GetBinCenter(iBin+1);
    if( center>0.95 || center<0.15 ){
      h_csv_ratio_cumulative->SetBinContent( iBin+1, 0 );
      h_csv_ratio_cumulative_LFUp->SetBinContent( iBin+1, 0 );
      h_csv_ratio_cumulative_LFDown->SetBinContent( iBin+1, 0 );
      h_csv_ratio_cumulative_JESUp->SetBinContent( iBin+1, 0 );
      h_csv_ratio_cumulative_JESDown->SetBinContent( iBin+1, 0 );
    }
  }


  h_csv_ratio_cumulative_LFUp->SetLineColor(kGreen+3);
  h_csv_ratio_cumulative_LFDown->SetLineColor(kGreen+3);
  h_csv_ratio_cumulative_JESUp->SetLineColor(kRed);
  h_csv_ratio_cumulative_JESDown->SetLineColor(kBlue);

  h_csv_ratio_cumulative_LFUp->SetLineWidth(2);
  h_csv_ratio_cumulative_LFDown->SetLineWidth(2);
  h_csv_ratio_cumulative_JESUp->SetLineWidth(2);
  h_csv_ratio_cumulative_JESDown->SetLineWidth(2);


  h_csv_ratio_cumulative->Draw("pe1");
  btagSF->Draw("same");
  btagSFUp->Draw("same");
  btagSFDown->Draw("same");

  h_csv_ratio_cumulative_LFUp->Draw("][histsame");
  h_csv_ratio_cumulative_LFDown->Draw("][histsame");
  h_csv_ratio_cumulative_JESUp->Draw("][histsame");
  h_csv_ratio_cumulative_JESDown->Draw("][histsame");
  
  TLegend *legend2 = new TLegend(0.2,0.75,0.87,0.89);

  legend2->SetFillColor(kWhite);
  legend2->SetLineColor(kWhite);
  legend2->SetShadowColor(kWhite);
  legend2->SetTextFont(42);
  legend2->SetTextSize(0.035);

  legend2->SetNColumns(3);

  legend2->AddEntry(h_csv_ratio_cumulative,"Scale Factor","pe1");
  legend2->AddEntry(h_csv_ratio_cumulative_JESUp,"JES Up","l");
  legend2->AddEntry(h_csv_ratio_cumulative_JESDown,"JES Down","l");
  legend2->AddEntry(h_csv_ratio_cumulative_LFUp,"LF Err","l");
  legend2->AddEntry(btagSF,"BTAG POG","l");
  legend2->AddEntry(btagSFUp,"BTAG POG Err","l");

  legend2->Draw();

  TString myimg = dirprefix + "hfSF_cumulative.png";
  c1->Print(myimg);

  for( int iHist=0; iHist<numHists-1; iHist++ ){
    //continue;
    h_csv_ratio_final[iHist] = new TH1D( Form("h_%s_final",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_JESUp[iHist] = new TH1D( Form("h_%s_final_JESUp",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_JESDown[iHist] = new TH1D( Form("h_%s_final_JESDown",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_LFUp[iHist] = new TH1D( Form("h_%s_final_LFUp",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_LFDown[iHist] = new TH1D( Form("h_%s_final_LFDown",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats1Up[iHist] = new TH1D( Form("h_%s_final_Stats1Up",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats1Down[iHist] = new TH1D( Form("h_%s_final_Stats1Down",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats2Up[iHist] = new TH1D( Form("h_%s_final_Stats2Up",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats2Down[iHist] = new TH1D( Form("h_%s_final_Stats2Down",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );


    std::vector<double> x; x.clear();
    std::vector<double> y; y.clear();
    std::vector<double> y_JESUp; y_JESUp.clear();
    std::vector<double> y_JESDown; y_JESDown.clear();
    std::vector<double> y_LFUp; y_LFUp.clear();
    std::vector<double> y_LFDown; y_LFDown.clear();
    std::vector<double> y_Stats1Up; y_Stats1Up.clear();
    std::vector<double> y_Stats1Down; y_Stats1Down.clear();
    std::vector<double> y_Stats2Up; y_Stats2Up.clear();
    std::vector<double> y_Stats2Down; y_Stats2Down.clear();

    for( int iBin=0; iBin<nBins; iBin++ ){
      double center = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
      if( center<0 ) continue;
      double sf = h_csv_ratio[iHist]->GetBinContent(iBin+1);
      double sf_JESUp = h_csv_ratio_JESUp[iHist]->GetBinContent(iBin+1);
      double sf_JESDown = h_csv_ratio_JESDown[iHist]->GetBinContent(iBin+1);
      double sf_LFUp = h_csv_ratio_LFUp[iHist]->GetBinContent(iBin+1);
      double sf_LFDown = h_csv_ratio_LFDown[iHist]->GetBinContent(iBin+1);
      double sf_Stats1Up = h_csv_ratio_Stats1Up[iHist]->GetBinContent(iBin+1);
      double sf_Stats1Down = h_csv_ratio_Stats1Down[iHist]->GetBinContent(iBin+1);
      double sf_Stats2Up = h_csv_ratio_Stats2Up[iHist]->GetBinContent(iBin+1);
      double sf_Stats2Down = h_csv_ratio_Stats2Down[iHist]->GetBinContent(iBin+1);

      x.push_back(center);
      y.push_back(sf);
      y_JESUp.push_back(sf_JESUp);
      y_JESDown.push_back(sf_JESDown);
      y_LFUp.push_back(sf_LFUp);
      y_LFDown.push_back(sf_LFDown);
      y_Stats1Up.push_back(sf_Stats1Up);
      y_Stats1Down.push_back(sf_Stats1Down);
      y_Stats2Up.push_back(sf_Stats2Up);
      y_Stats2Down.push_back(sf_Stats2Down);
    }

    //ROOT::Math::Interpolation::Type iType = ROOT::Math::Interpolation::kLINEAR;
    //ROOT::Math::Interpolation::Type iType = ROOT::Math::Interpolation::kCSPLINE;
    //ROOT::Math::Interpolation::Type iType = ROOT::Math::Interpolation::kPOLYNOMIAL;
    ROOT::Math::Interpolation::Type iType = ROOT::Math::Interpolation::kAKIMA;

    ROOT::Math::Interpolator * m_i =  new ROOT::Math::Interpolator(x,y,iType);
    ROOT::Math::Interpolator * m_i_JESUp =  new ROOT::Math::Interpolator(x,y_JESUp,iType);
    ROOT::Math::Interpolator * m_i_JESDown =  new ROOT::Math::Interpolator(x,y_JESDown,iType);
    ROOT::Math::Interpolator * m_i_LFUp =  new ROOT::Math::Interpolator(x,y_LFUp,iType);
    ROOT::Math::Interpolator * m_i_LFDown =  new ROOT::Math::Interpolator(x,y_LFDown,iType);
    ROOT::Math::Interpolator * m_i_Stats1Up =  new ROOT::Math::Interpolator(x,y_Stats1Up,iType);
    ROOT::Math::Interpolator * m_i_Stats1Down =  new ROOT::Math::Interpolator(x,y_Stats1Down,iType);
    ROOT::Math::Interpolator * m_i_Stats2Up =  new ROOT::Math::Interpolator(x,y_Stats2Up,iType);
    ROOT::Math::Interpolator * m_i_Stats2Down =  new ROOT::Math::Interpolator(x,y_Stats2Down,iType);



    h_csv_ratio_final[iHist]->SetLineWidth(2);
    h_csv_ratio_final_JESUp[iHist]->SetLineWidth(2);
    h_csv_ratio_final_JESDown[iHist]->SetLineWidth(2);
    h_csv_ratio_final_LFUp[iHist]->SetLineWidth(2);
    h_csv_ratio_final_LFDown[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats1Up[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats1Down[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats2Up[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats2Down[iHist]->SetLineWidth(2);


    h_csv_ratio_final[iHist]->SetLineColor(kBlack);
    h_csv_ratio_final_JESUp[iHist]->SetLineColor(kRed);
    h_csv_ratio_final_JESDown[iHist]->SetLineColor(kBlue);
    h_csv_ratio_final_LFUp[iHist]->SetLineColor(kGreen+3);
    h_csv_ratio_final_LFDown[iHist]->SetLineColor(kGreen+3);
    h_csv_ratio_final_Stats1Up[iHist]->SetLineColor(kMagenta-2);
    h_csv_ratio_final_Stats1Down[iHist]->SetLineColor(kMagenta-2);
    h_csv_ratio_final_Stats2Up[iHist]->SetLineColor(kRed-2);
    h_csv_ratio_final_Stats2Down[iHist]->SetLineColor(kRed-2);



    double firstPoint = x[0];
    double lastPoint  = x[int(x.size())-1];
    for( int iBin=0; iBin<n; iBin++ ){
      double center = h_csv_ratio_final[iHist]->GetBinCenter(iBin+1);
      if( center<0 ){
	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1));
	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,h_csv_ratio_JESUp[iHist]->GetBinContent(1));
	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,h_csv_ratio_JESDown[iHist]->GetBinContent(1));
	h_csv_ratio_final_LFUp[iHist]->SetBinContent(iBin+1,h_csv_ratio_LFUp[iHist]->GetBinContent(1));
	h_csv_ratio_final_LFDown[iHist]->SetBinContent(iBin+1,h_csv_ratio_LFDown[iHist]->GetBinContent(1));
	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) + h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) - h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) + h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) - h_csv_ratio[iHist]->GetBinError(1));
      }
      else if( center<firstPoint ){
      	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,m_i->Eval( firstPoint ));
      	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,m_i_JESUp->Eval( firstPoint ));
      	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,m_i_JESDown->Eval( firstPoint ));
      	h_csv_ratio_final_LFUp[iHist]->SetBinContent(iBin+1,m_i_LFUp->Eval( firstPoint ));
      	h_csv_ratio_final_LFDown[iHist]->SetBinContent(iBin+1,m_i_LFDown->Eval( firstPoint ));
      	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,m_i_Stats1Up->Eval( firstPoint ));
      	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,m_i_Stats1Down->Eval( firstPoint ));
      	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,m_i_Stats2Up->Eval( firstPoint ));
      	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,m_i_Stats2Down->Eval( firstPoint ));
      }
      else if( center>lastPoint ){
      	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,m_i->Eval( lastPoint ));
      	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,m_i_JESUp->Eval( lastPoint ));
      	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,m_i_JESDown->Eval( lastPoint ));
      	h_csv_ratio_final_LFUp[iHist]->SetBinContent(iBin+1,m_i_LFUp->Eval( lastPoint ));
      	h_csv_ratio_final_LFDown[iHist]->SetBinContent(iBin+1,m_i_LFDown->Eval( lastPoint ));
      	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,m_i_Stats1Up->Eval( lastPoint ));
      	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,m_i_Stats1Down->Eval( lastPoint ));
      	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,m_i_Stats2Up->Eval( lastPoint ));
      	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,m_i_Stats2Down->Eval( lastPoint ));
      }
      else {
	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,m_i->Eval( center ));
	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,m_i_JESUp->Eval( center ));
	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,m_i_JESDown->Eval( center ));
	h_csv_ratio_final_LFUp[iHist]->SetBinContent(iBin+1,m_i_LFUp->Eval( center ));
	h_csv_ratio_final_LFDown[iHist]->SetBinContent(iBin+1,m_i_LFDown->Eval( center ));
	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,m_i_Stats1Up->Eval( center ));
	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,m_i_Stats1Down->Eval( center ));
	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,m_i_Stats2Up->Eval( center ));
	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,m_i_Stats2Down->Eval( center ));
      }
    }


    h_csv_ratio[iHist]->SetMarkerStyle(20);

    h_csv_ratio[iHist]->SetTitle( Form("HF %s", hist_name[iHist].Data() ) );
    if( iHist==numHists-1 ) h_csv_ratio[iHist]->SetTitle( Form("LF %s", hist_name[iHist].Data() ) );
    h_csv_ratio[iHist]->GetYaxis()->SetTitle("Data/MC SF");
    h_csv_ratio[iHist]->GetXaxis()->SetTitle("CSV");

    h_csv_ratio[iHist]->SetStats(0);

    double maxY = 1.3 * h_csv_ratio[iHist]->GetMaximum();
    maxY = std::min( maxY, 5. );

    h_csv_ratio[iHist]->SetMaximum(maxY);
    h_csv_ratio[iHist]->SetMinimum(0.2);

    TLegend *legend = new TLegend(0.2,0.75,0.87,0.89);


    h_csv_ratio_LFUp[iHist]->SetLineColor(kGreen+1);
    h_csv_ratio_LFDown[iHist]->SetLineColor(kGreen+1);

    h_csv_ratio_JESUp[iHist]->SetLineColor(kRed);
    h_csv_ratio_JESDown[iHist]->SetLineColor(kBlue);



    legend->SetFillColor(kWhite);
    legend->SetLineColor(kWhite);
    legend->SetShadowColor(kWhite);
    legend->SetTextFont(42);
    legend->SetTextSize(0.035);

    legend->SetNColumns(3);

    legend->AddEntry(h_csv_ratio[iHist],"HF SF","p");
    legend->AddEntry(h_csv_ratio_final_JESUp[iHist],"JES Up","l");
    legend->AddEntry(h_csv_ratio_final_JESDown[iHist],"JES Down","l");
    legend->AddEntry(h_csv_ratio_final[iHist],"HF Fit","l");
    legend->AddEntry(h_csv_ratio_final_LFUp[iHist],"LF Err","l");
    legend->AddEntry(h_csv_ratio_final_Stats1Up[iHist],"Stats1 Err","l");
    legend->AddEntry(h_csv_ratio_final_Stats2Up[iHist],"Stats2 Err","l");

    TString img;



    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_only.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_only.pdf";
    c1->Print(img);



    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_JESUp[iHist]->Draw("histsame");
    h_csv_ratio_final_JESDown[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_JES.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_JES.pdf";
    c1->Print(img);


    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_LFUp[iHist]->Draw("histsame");
    h_csv_ratio_final_LFDown[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_LF.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_LF.pdf";
    c1->Print(img);


    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_Stats1.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_Stats1.pdf";
    c1->Print(img);



    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_Stats2.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_Stats2.pdf";
    c1->Print(img);


    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_JESUp[iHist]->Draw("histsame");
    h_csv_ratio_final_JESDown[iHist]->Draw("histsame");
    h_csv_ratio_final_LFUp[iHist]->Draw("histsame");
    h_csv_ratio_final_LFDown[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Down[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_All.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_All.pdf";
    c1->Print(img);


    h_csv_ratio_final[iHist]->Write(Form("%s_final",hist_name[iHist].Data()));
    h_csv_ratio_final_JESUp[iHist]->Write(Form("%s_final_JESUp",hist_name[iHist].Data()));
    h_csv_ratio_final_JESDown[iHist]->Write(Form("%s_final_JESDown",hist_name[iHist].Data()));
    h_csv_ratio_final_LFUp[iHist]->Write(Form("%s_final_LFUp",hist_name[iHist].Data()));
    h_csv_ratio_final_LFDown[iHist]->Write(Form("%s_final_LFDown",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats1Up[iHist]->Write(Form("%s_final_Stats1Up",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats1Down[iHist]->Write(Form("%s_final_Stats1Down",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats2Up[iHist]->Write(Form("%s_final_Stats2Up",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats2Down[iHist]->Write(Form("%s_final_Stats2Down",hist_name[iHist].Data()));


    delete m_i;
    delete m_i_JESUp;
    delete m_i_JESDown;
    delete m_i_LFUp;
    delete m_i_LFDown;
    delete m_i_Stats1Up;
    delete m_i_Stats1Down;
    delete m_i_Stats2Up;
    delete m_i_Stats2Down;

    delete legend;

  }


  std::cout << " Done! " << std::endl;

  histofile.Write();
  histofile.Close();


}

/*
 FCN=4.37714 FROM IMProve     STATUS=RESET          48 CALLS         261 TOTAL
                     EDM=7.07195e-09    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.18246e+00   4.56664e-02   4.56664e-04   4.69042e+02
   2  p1           4.63312e+02   2.03551e+02   2.03551e+00  -4.44797e-01
   3  p2           2.10061e-06   1.49933e-06   1.49933e-08  -7.16288e+07
	 f1->Eval(463.312) = 0.731548
		 f1->GetMinimum(0,1000) = 0.731548
 FCN=0.0571179 FROM IMProve     STATUS=RESET          35 CALLS         207 TOTAL
                     EDM=1.04253e-08    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.21615e+00   4.59417e-02   4.59417e-04   1.71784e+02
   2  p1           3.82802e+02   4.77352e+01   4.77352e-01  -2.76792e-01
   3  p2           2.90873e-06   8.85574e-07   8.85574e-09  -2.20034e+07
 FCN=0.0615925 FROM IMProve     STATUS=RESET          43 CALLS         154 TOTAL
                     EDM=5.13031e-11    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.14857e+00   4.50183e-02   4.50183e-04   6.00544e+02
   2  p1           6.47355e+02   2.73547e+02   2.73547e+00  -4.90261e-01
   3  p2           1.28422e-06   8.54454e-07   8.54454e-09  -1.81000e+08



 TF1* f0 = new TF1("f0","[0] + [1]*TMath::Log([2]*x + [3]) + [4]*x + [5]*x*x + [6]*x*x*x + [7]*x*x*x*x + [8]*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 8.98,    ndof = 6,       prob = 0.17 
 csv_ratio_Pt0_Eta1:     chi2 = 3.89,    ndof = 6,       prob = 0.69 
 csv_ratio_Pt0_Eta2:     chi2 = 11.51,   ndof = 6,       prob = 0.07 
 csv_ratio_Pt1_Eta0:     chi2 = 5.51,    ndof = 6,       prob = 0.48 
 csv_ratio_Pt1_Eta1:     chi2 = 9.67,    ndof = 6,       prob = 0.14 
 csv_ratio_Pt1_Eta2:     chi2 = 12.62,   ndof = 6,       prob = 0.05 
 csv_ratio_Pt2_Eta0:     chi2 = 5.62,    ndof = 6,       prob = 0.47 
 csv_ratio_Pt2_Eta1:     chi2 = 7.55,    ndof = 6,       prob = 0.27 
 csv_ratio_Pt2_Eta2:     chi2 = 4.37,    ndof = 6,       prob = 0.63 
 csv_ratio_all:  chi2 = 12.21,   ndof = 6,       prob = 0.06 
 csv_ratio_cumulative_all:       chi2 = 24.52,   ndof = 6,       prob = 0.00


 TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 8.98,    ndof = 9,       prob = 0.44,    SF (CSV=1) = 2.38 
 csv_ratio_Pt0_Eta1:     chi2 = 3.89,    ndof = 9,       prob = 0.92,    SF (CSV=1) = 3.19 
 csv_ratio_Pt0_Eta2:     chi2 = 12.73,   ndof = 9,       prob = 0.18,    SF (CSV=1) = 1.79 
 csv_ratio_Pt1_Eta0:     chi2 = 5.51,    ndof = 9,       prob = 0.79,    SF (CSV=1) = 1.98 
 csv_ratio_Pt1_Eta1:     chi2 = 9.67,    ndof = 9,       prob = 0.38,    SF (CSV=1) = 0.75 
 csv_ratio_Pt1_Eta2:     chi2 = 12.62,   ndof = 9,       prob = 0.18,    SF (CSV=1) = 1.56 
 csv_ratio_Pt2_Eta0:     chi2 = 5.62,    ndof = 9,       prob = 0.78,    SF (CSV=1) = 5.59 
 csv_ratio_Pt2_Eta1:     chi2 = 7.55,    ndof = 9,       prob = 0.58,    SF (CSV=1) = 2.55 
 csv_ratio_Pt2_Eta2:     chi2 = 4.37,    ndof = 9,       prob = 0.89,    SF (CSV=1) = 3.32 
 csv_ratio_all:  chi2 = 12.21,   ndof = 9,       prob = 0.20,    SF (CSV=1) = 2.67 
 csv_ratio_cumulative_all:       chi2 = 24.52,   ndof = 9,       prob = 0.00,    SF (CSV=1) = 2.67 

 TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 6.87,    ndof = 8,       prob = 0.55,    SF (CSV=1) = 3.94 
 csv_ratio_Pt0_Eta1:     chi2 = 3.82,    ndof = 8,       prob = 0.87,    SF (CSV=1) = 2.91 
 csv_ratio_Pt0_Eta2:     chi2 = 10.35,   ndof = 8,       prob = 0.24,    SF (CSV=1) = 0.56 
 csv_ratio_Pt1_Eta0:     chi2 = 3.78,    ndof = 8,       prob = 0.88,    SF (CSV=1) = 4.21 
 csv_ratio_Pt1_Eta1:     chi2 = 8.93,    ndof = 8,       prob = 0.35,    SF (CSV=1) = 2.17 
 csv_ratio_Pt1_Eta2:     chi2 = 10.30,   ndof = 8,       prob = 0.24,    SF (CSV=1) = 3.70 
 csv_ratio_Pt2_Eta0:     chi2 = 3.34,    ndof = 8,       prob = 0.91,    SF (CSV=1) = 8.36 
 csv_ratio_Pt2_Eta1:     chi2 = 7.57,    ndof = 8,       prob = 0.48,    SF (CSV=1) = 2.66 
 csv_ratio_Pt2_Eta2:     chi2 = 3.93,    ndof = 8,       prob = 0.86,    SF (CSV=1) = 4.03 
 csv_ratio_all:  chi2 = 9.51,    ndof = 8,       prob = 0.30,    SF (CSV=1) = 3.38 
 csv_ratio_cumulative_all:       chi2 = 9.84,    ndof = 8,       prob = 0.28,    SF (CSV=1) = 3.63 

 TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x + [7]*x*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 6.87,    ndof = 7,       prob = 0.44,    SF (CSV=1) = 3.94 
 csv_ratio_Pt0_Eta1:     chi2 = 3.82,    ndof = 7,       prob = 0.80,    SF (CSV=1) = 2.91 
 csv_ratio_Pt0_Eta2:     chi2 = 10.35,   ndof = 7,       prob = 0.17,    SF (CSV=1) = 0.56 
 csv_ratio_Pt1_Eta0:     chi2 = 3.78,    ndof = 7,       prob = 0.80,    SF (CSV=1) = 4.21 
 csv_ratio_Pt1_Eta1:     chi2 = 8.93,    ndof = 7,       prob = 0.26,    SF (CSV=1) = 2.17 
 csv_ratio_Pt1_Eta2:     chi2 = 10.30,   ndof = 7,       prob = 0.17,    SF (CSV=1) = 3.70 
 csv_ratio_Pt2_Eta0:     chi2 = 3.34,    ndof = 7,       prob = 0.85,    SF (CSV=1) = 8.36 
 csv_ratio_Pt2_Eta1:     chi2 = 7.57,    ndof = 7,       prob = 0.37,    SF (CSV=1) = 2.66 
 csv_ratio_Pt2_Eta2:     chi2 = 3.93,    ndof = 7,       prob = 0.79,    SF (CSV=1) = 4.04 
 csv_ratio_all:  chi2 = 9.51,    ndof = 7,       prob = 0.22,    SF (CSV=1) = 3.38 
 csv_ratio_cumulative_all:       chi2 = 9.84,    ndof = 7,       prob = 0.20,    SF (CSV=1) = 3.63 


 */
