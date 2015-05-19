{
  TH1::SetDefaultSumw2(); 
  TFile * wgtFile = new TFile("../../NtupleMaker/BEANmaker/data/csv_rwt_hf_IT.root");

  int nPt = 5;
  int nEta = 1;

  for (unsigned int iPt = 0; iPt < nPt; iPt++){
    for (unsigned int iEta = 0; iEta < nEta; iEta++){
      cout << "----check pt bin " << iPt << "; and eta bin " << iEta  << endl;

      TH1D* data = (TH1D*)wgtFile->Get(Form("h_csv_Data_Pt%i_Eta%i",iPt,iEta))->Clone(Form("h_csv_Data_Pt%i_Eta%i_tmp",iPt,iEta));
      TH1D* bjets = (TH1D*)wgtFile->Get(Form("h_csv_MC_bjets_Pt%i_Eta%i",iPt,iEta))->Clone(Form("h_csv_MC_bjets_Pt%i_Eta%i_tmp",iPt,iEta));
      TH1D* nonb = (TH1D*)wgtFile->Get(Form("h_csv_MC_nonbjets_Pt%i_Eta%i",iPt,iEta))->Clone(Form("h_csv_MC_nonbjets_Pt%i_Eta%i_tmp",iPt,iEta));

//   double total_data = data->Integral();
//   double total_b = bjets->Integral();
//   double total_nonb = nonb->Integral();

//   cout << "data: " << total_data << endl
//        << "b: " << total_b << endl
//        << "nonb: " << total_nonb << endl;

      TH1D* fit = (TH1D*)wgtFile->Get(Form("csv_ratio_Pt%i_Eta%i_final",iPt,iEta))->Clone(Form("csv_ratio_Pt%i_Eta%i_final_tmp",iPt,iEta));

  TH1D* ratio = (TH1D*)data->Clone(Form("csv_ratio_Pt%i_Eta%i_bin",iPt,iEta));
  ratio->Add(nonb, -1);
  ratio->Divide(bjets);

  TH1D* bjets_binned = (TH1D*)bjets->Clone(Form("bjets_binned_Pt%i_Eta%i",iPt,iEta));
  TH1D* bjets_fit = (TH1D*)bjets->Clone(Form("bjets_fit_Pt%i_Eta%i",iPt,iEta));
  bjets_binned->Multiply(ratio);
//   cout << "error" << endl;

  int NBins = bjets->GetNbinsX();
  cout << "number of bins " << NBins << endl;

  for (unsigned int iBin = 1; iBin < NBins+1; iBin++){
    double Xcenter = bjets_fit->GetBinCenter(iBin);
//     cout << "bin center is" << Xcenter << endl;
//     int useCSVBin = (csv>=0.) ? h_csv_wgt_hf[iSysHF][iPt]->FindBin(csv) : 1;
    double SF = fit->GetBinContent(fit->FindBin(Xcenter));
//     cout << "SF is" << SF << endl;
    double newValue = SF*(bjets->GetBinContent(iBin));
//     cout << "new value is" << newValue << endl;
    bjets_fit->SetBinContent(iBin, newValue);
  }

  bjets->SetLineWidth(2);
  bjets_binned->SetLineWidth(2);
  bjets_fit->SetLineWidth(2);

  bjets->Draw();
  bjets_binned->SetLineColor(2);
  bjets_fit->SetLineColor(3);
  bjets_binned->Draw("same");
  bjets_fit->Draw("same");

  cout << " orig norm is: " << bjets->Integral() << endl
       << " binned norm is: " << bjets_binned->Integral() << endl
       << " fitted norm is: " << bjets_fit->Integral() << endl
       << " fit/bin is: " << bjets_fit->Integral()/bjets_binned->Integral() << endl; 


    }
  }

}
