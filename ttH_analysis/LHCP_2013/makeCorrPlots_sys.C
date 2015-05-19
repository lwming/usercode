
void makeCorrPlots_sys(TString catName = "e3je2t") {

  gStyle->SetOptStat(0);

  TLatex plotText;
  plotText.SetNDC();
  plotText.SetTextSize(0.052);
 
  //input variables
  std::vector<string> varNames;
  varNames.push_back("sum_pt");
  varNames.push_back("min_dr_jets");
  varNames.push_back("avg_btag_disc_non_btags");
  if (catName == "e3je2t"){
    varNames.push_back("avg_btag_disc_btags");
    varNames.push_back("BDTG_e3je2t");
  }
  else if(catName == "ge4je2t"){
    varNames.push_back("higgsLike_dijet_mass");
    varNames.push_back("higgsLike_dijet_mass2");
    varNames.push_back("numJets");
    varNames.push_back("BDTG_ge4je2t");
  }
  else std::cout << "Error, wrong category name" << std::endl;

  int nVar = varNames.size();
  std::cout << "number of variables: " << nVar << std::endl;
//   char *varNames[] = {"sum_pt",
//                       "min_dr_jets",
//                       "avg_btag_disc_non_btags",
//                       "avg_btag_disc_btags",
//                       "BDTG_e3je2t",};

  TString inputFile = "corrPlots_JESDown_";
  inputFile += catName;
  inputFile += ".root";
  TFile *corrPlotFile = TFile::Open(inputFile);

  for (int i = 0; i < nVar; ++i) {

    TCanvas *canv = new TCanvas(Form("JESDown_Yvar_VS_%s_",varNames[i].c_str()) + catName,"",990,660);
    canv->Divide(3,2,0.01,0.01);

    int numPad = 0;

    for (int j = 0; j < nVar; ++j) {
      if (i == j) continue;

      TString profName = "profiles_%s_";
      profName += varNames[i];
      profName += "_";
      profName += varNames[j];
//       cout << "profName is "<< profName << endl;
      TProfile *dataProf = (TProfile *)corrPlotFile->Get(Form(profName.Data(),"Data"));
      dataProf->GetYaxis()->SetTitleOffset(1.5);
      dataProf->SetMarkerColor(kBlack);
      dataProf->SetLineColor(kBlack);

      TProfile *mcProf = (TProfile *)corrPlotFile->Get(Form(profName.Data(),"MC"));
      mcProf->GetYaxis()->SetTitleOffset(1.5);

      ++numPad;
      canv->cd(numPad);

      gPad->SetRightMargin(0.02);

      TLegend *leg = new TLegend(0.65, 0.75, 0.9, 0.88);
      leg->SetFillColor(0);
      leg->SetLineColor(0);
      leg->SetLineWidth(0);
      leg->AddEntry(dataProf,"Data","le");
      leg->AddEntry(mcProf,"MC","f");

      mcProf->Draw("e2");
      dataProf->Draw("esame");
      leg->Draw();

      if (numPad==1) plotText.DrawLatex(0.17,0.91,"CMS Preliminary");
      if (numPad==2) plotText.DrawLatex(0.3,0.91,"#sqrt{s} = 8 TeV, L = 19.45 fb^{-1}");

    }

//     canv->Print(".pdf");
    canv->Print(".png");
  }
  
}

