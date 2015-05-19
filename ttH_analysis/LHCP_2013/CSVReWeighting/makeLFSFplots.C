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

//______________________________________________________________________________
void makeLFSFplots(){
  TH1::SetDefaultSumw2(); 
 TFile * f0 = new TFile("csv_rwt_lf_IT_v2.root");
//   TFile * f1 = new TFile("csv_rwt_lf_IT_v2_JESUp.root");
//   TFile * f2 = new TFile("csv_rwt_lf_IT_v2_JESDown.root");

  int nPt = 3;
  int nEta = 3;

  int ncsvbins = 17;
  double csvbins[] = { -10.0, 0.0, 0.040, 0.080, 0.120, 0.160, 0.200, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.010 };
  double csvbins_new[] = { -0.04, 0.0, 0.040, 0.080, 0.120, 0.160, 0.200, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.010 };



  for (unsigned int iPt = 0; iPt < nPt; iPt++){
    for (unsigned int iEta = 0; iEta < nEta; iEta++){

      TString dataName = Form("csv_Data_Pt%i_Eta%i",iPt,iEta);  
      TString bName = Form("csv_MC_bjets_Pt%i_Eta%i",iPt,iEta);  
      TString nonbName = Form("csv_MC_nonbjets_Pt%i_Eta%i",iPt,iEta);  
//       TString subName = Form("csv_sub_Pt%i_Eta%i",iPt,iEta); 

      TH1F * h0temp = (TH1F*)f0->Get(dataName)->Clone(Form("h_%s_temp",dataName.Data()));
      TH1F * h1temp = (TH1F*)f0->Get(bName)->Clone(Form("h_%s_temp",bName.Data()));
      TH1F * h2temp = (TH1F*)f0->Get(nonbName)->Clone(Form("h_%s_temp",nonbName.Data()));

      TH1F * h0temp_rebin = (TH1F*)h0temp->Rebin(ncsvbins,Form("h_%s_temp_rebin",dataName.Data()),csvbins);
      TH1F * h1temp_rebin = (TH1F*)h1temp->Rebin(ncsvbins,Form("h_%s_temp_rebin",bName.Data()),csvbins);
      TH1F * h2temp_rebin = (TH1F*)h2temp->Rebin(ncsvbins,Form("h_%s_temp_rebin",nonbName.Data()),csvbins);

//       TString h0Name = histName + "v0";
//       TString h1Name = histName + "v1";
//       TString h2Name = histName + "v2";

      TH1F * h0 = new TH1F(Form("h_%s",dataName.Data()),";CSV",ncsvbins,csvbins_new );
      TH1F * h1 = new TH1F(Form("h_%s",bName.Data()),";CSV",ncsvbins,csvbins_new );
      TH1F * h2 = new TH1F(Form("h_%s",nonbName.Data()),";CSV",ncsvbins,csvbins_new );
//       TH1F * h3 = new TH1F(Form("h_%s",subName.Data()),";CSV",ncsvbins,csvbins_new );
      h0->Sumw2();
      h1->Sumw2();
      h2->Sumw2();
//       h3->Sumw2();

      for (unsigned int iBin = 1; iBin < ncsvbins+1; iBin++){
	h0->SetBinContent(iBin, h0temp_rebin->GetBinContent(iBin));
	h1->SetBinContent(iBin, h1temp_rebin->GetBinContent(iBin));
	h2->SetBinContent(iBin, h2temp_rebin->GetBinContent(iBin));

	h0->SetBinError(iBin, h0temp_rebin->GetBinError(iBin));
	h1->SetBinError(iBin, h1temp_rebin->GetBinError(iBin));
	h2->SetBinError(iBin, h2temp_rebin->GetBinError(iBin));
      }

      TH1F * hsub = (TH1F*)h0->Clone(Form("csv_sub_Pt%i_Eta%i",iPt,iEta));
      hsub->Add(h1,-1);
      hsub->SetLineWidth(2);
      hsub->SetLineColor(4);
      double plotMax2 = TMath::Max(hsub->GetMaximum(),h2->GetMaximum());
      hsub->SetMaximum(plotMax2*1.2);
      hsub->SetTitle(Form("PtBin%i_EtaBin%i",iPt,iEta));

      h0->SetLineWidth(2);
      h1->SetLineWidth(2);
      h2->SetLineWidth(2);

      h0->SetLineColor(1);
      h0->SetMarkerColor(1);
      h0->SetMarkerSize(0.5);
      h0->SetMarkerStyle(20);

      h1->SetLineColor(2);
      h2->SetLineColor(3);
      h1->SetMarkerColor(2);
      h2->SetMarkerColor(3);
      h1->SetFillColor(2);
      h2->SetFillColor(3);

      ///
      TLegend *legend = new TLegend(0.2,0.75,0.77,0.89);
      legend->SetFillColor(kWhite);
      legend->SetLineColor(kWhite);
      legend->SetShadowColor(kWhite);
      legend->SetTextFont(42);
      legend->SetTextSize(0.04);
      
      legend->SetNColumns(3);
      legend->AddEntry(h0,"Data","pl");
      legend->AddEntry(h1,"HF","l");
      legend->AddEntry(h2,"LF","l");

      THStack * mcstack = new THStack("MC_stack",";CSV");
      mcstack->SetTitle(Form("PtBin%i_EtaBin%i",iPt,iEta));
      mcstack->Add(h2,"hist");
      mcstack->Add(h1,"hist");
//       if ( h0->GetMaximum() < h1->GetMaximum()) h0->SetMaximum( 1.2*(h1->GetMaximum())); ///
//       if ( h0->GetMinimum() > h1->GetMinimum()) h0->SetMinimum( 0.8*(h1->GetMinimum())); ///
      double plotMax = TMath::Max(mcstack->GetMaximum(),h0->GetMaximum());
      mcstack->SetMaximum(plotMax*1.2);

      gStyle->SetOptStat(0);

      TString canvName = Form("lf_Pt%i_Eta%i_stack",iPt,iEta);  
      TCanvas * canv = new TCanvas (canvName, canvName);
      canv->cd();
      mcstack->Draw();
      h0->Draw("pesame");
      legend->Draw("same");
      canv->Print(canvName +".pdf");


      TLegend *legend2 = new TLegend(0.5,0.75,0.87,0.89);
      legend2->SetFillColor(kWhite);
      legend2->SetLineColor(kWhite);
      legend2->SetShadowColor(kWhite);
      legend2->SetTextFont(42);
      legend2->SetTextSize(0.04);
      
      legend2->SetNColumns(3);
      legend2->AddEntry(hsub,"(Data - HF)","pl");
//       legend2->AddEntry(h1,"HF","l");
      legend2->AddEntry(h2,"LF","l");

      TString canvName2 = Form("lf_Pt%i_Eta%i_overlay",iPt,iEta);  
      TCanvas * canv2 = new TCanvas (canvName2, canvName2);
      canv2->cd();
      hsub->Draw();
      h2->Draw("same");
      legend2->Draw("same");
      canv2->Print(canvName2 +".pdf");

    }
  }
}
