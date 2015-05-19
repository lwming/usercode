{
  TH1::SetDefaultSumw2(); 
  TFile * wgtFile = new TFile("../../NtupleMaker/BEANmaker/data/csv_rwt_lf_IT.root");

  int nPt = 3;
  int nEta = 3;

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
  ratio->Add(bjets, -1);
  ratio->Divide(nonb);

  TH1D* nonb_binned = (TH1D*)nonb->Clone(Form("nonb_binned_Pt%i_Eta%i",iPt,iEta));
  TH1D* nonb_fit = (TH1D*)nonb->Clone(Form("nonb_fit_Pt%i_Eta%i",iPt,iEta));
  nonb_binned->Multiply(ratio);
//   cout << "error" << endl;

  int NBins = nonb->GetNbinsX();
  cout << "number of bins " << NBins << endl;

  for (unsigned int iBin = 1; iBin < NBins+1; iBin++){
    double Xcenter = nonb_fit->GetBinCenter(iBin);
//     cout << "bin center is" << Xcenter << endl;
//     int useCSVBin = (csv>=0.) ? h_csv_wgt_hf[iSysHF][iPt]->FindBin(csv) : 1;
    double SF = fit->GetBinContent(fit->FindBin(Xcenter));
//     cout << "SF is" << SF << endl;
    double newValue = SF*(nonb->GetBinContent(iBin));
//     cout << "new value is" << newValue << endl;
    nonb_fit->SetBinContent(iBin, newValue);
  }

  nonb->SetLineWidth(2);
  nonb_binned->SetLineWidth(2);
  nonb_fit->SetLineWidth(2);

  nonb->Draw();
  nonb_binned->SetLineColor(2);
  nonb_fit->SetLineColor(3);
  nonb_binned->Draw("same");
  nonb_fit->Draw("same");

  cout << " orig norm is: " << nonb->Integral() << endl
       << " binned norm is: " << nonb_binned->Integral() << endl
       << " fitted norm is: " << nonb_fit->Integral() << endl
       << " fit/bin is: " << nonb_fit->Integral()/nonb_binned->Integral() << endl; 


    }
  }

}
