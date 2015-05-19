{
  TFile * f0 = new TFile("csv_rwt_lf_IT_v2.root");
  TFile * f1 = new TFile("csv_rwt_lf_IT_v2_JESUp.root");
  TFile * f2 = new TFile("csv_rwt_lf_IT_v2_JESDown.root");

  int nPt = 3;
  int nEta = 3;
  double xBins[17] = {-0.1, -0.05, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 1.01};
  int nBins = 16;
  // heavy flavor Bins
//   int nPt = 5;
//   int nEta = 1;
//   double xBins[18] = {0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};
//   int nBins = 17;


  for (unsigned int iPt = 0; iPt < nPt; iPt++){
    for (unsigned int iEta = 0; iEta < nEta; iEta++){
 
      TString canvName = Form("lf_Pt%i_Eta%i_JEScompare",iPt,iEta);  
      TCanvas * canv = new TCanvas (canvName, canvName);
      TString histName = Form("csv_ratio_Pt%i_Eta%i",iPt,iEta);  

      TH1F * h0temp = (TH1F*)f0->Get(histName);
      TH1F * h1temp = (TH1F*)f1->Get(histName);
      TH1F * h2temp = (TH1F*)f2->Get(histName);

      TString h0Name = histName + "v0";
      TString h1Name = histName + "v1";
      TString h2Name = histName + "v2";
      TH1F * h0 = new TH1F(h0Name,histName,nBins, xBins);
      TH1F * h1 = new TH1F(h1Name,histName,nBins, xBins);
      TH1F * h2 = new TH1F(h2Name,histName,nBins, xBins);
      h0->Sumw2();
      h1->Sumw2();
      h2->Sumw2();

      for (unsigned int iBin = 1; iBin < nBins+1; iBin++){
	h0->SetBinContent(iBin, h0temp->GetBinContent(iBin));
	h1->SetBinContent(iBin, h1temp->GetBinContent(iBin));
	h2->SetBinContent(iBin, h2temp->GetBinContent(iBin));

	h0->SetBinError(iBin, h0temp->GetBinError(iBin));
	h1->SetBinError(iBin, h1temp->GetBinError(iBin));
	h2->SetBinError(iBin, h2temp->GetBinError(iBin));
      }

      h0->SetLineWidth(2);
      h1->SetLineWidth(2);
      h2->SetLineWidth(2);

      h1->SetLineColor(2);
      h2->SetLineColor(3);
      h1->SetMarkerColor(2);
      h2->SetMarkerColor(3);
      if ( h0->GetMaximum() < h1->GetMaximum()) h0->SetMaximum( 1.2*(h1->GetMaximum())); ///
      if ( h0->GetMinimum() > h1->GetMinimum()) h0->SetMinimum( 0.8*(h1->GetMinimum())); ///
      canv->cd();
      gStyle->SetOptStat(0);
      h0->Draw("pe");
      h1->Draw("histsame");
      h2->Draw("histsame");

      canv->Print(canvName +".png");
    }
  }
}
