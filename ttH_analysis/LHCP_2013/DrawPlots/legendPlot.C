//////////////////////////////////////////////
// Draw Legend Only Plot
void MyPlot::drawLegendPlot(int nColumns){

  int width  = 800;
  int height = 800;
  if(nColumns==1) width = 500;
  if(nColumns==2) width = 700;
  if(nColumns==3) width = 1200;
  if(nColumns==4) width = 1400;

  TCanvas* myC = new TCanvas("myC", "myC", width,height);

  // SET UP LEGEND
  TLegend *legend = new TLegend(0.1, 0.1, 0.9, 0.9);

  legend->SetFillColor(kWhite);
  legend->SetLineColor(kWhite);
  legend->SetShadowColor(kWhite);
  legend->SetTextFont(42);
  legend->SetTextSize(0.035);

  legend->SetNColumns(nColumns);

  this->h_bkg_err_1sig->SetFillStyle(3654);
  this->h_bkg_err_1sig->SetFillColor(kBlack);

  legend->AddEntry(this->h_ttH,std::string("t#bar{t}H(125) x 30").c_str(),"l");
  legend->AddEntry(this->h_ttbar_lf,std::string("t#bar{t} + lf").c_str(),"f");
  legend->AddEntry(this->h_ttbar_ccbar,std::string("t#bar{t} + c#bar{c}").c_str(),"f");
  legend->AddEntry(this->h_ttbar_b,std::string("t#bar{t} + b").c_str(),"f");
  legend->AddEntry(this->h_ttbar_bbbar,std::string("t#bar{t} + b#bar{b}").c_str(),"f");
  legend->AddEntry(this->h_singlet,std::string("Single t").c_str(),"f");
  legend->AddEntry(this->h_ttV,std::string("t#bar{t} + W,Z").c_str(),"f");
  legend->AddEntry(this->h_ewk,std::string("EWK").c_str(),"f");
  legend->AddEntry(this->h_bkg_err_1sig,std::string("Bkg. Unc.").c_str(),"f");
  legend->AddEntry(this->h_data,std::string("Data").c_str(),"lpe");

  legend->Draw();

  TString saveMe = dirprefix + "ttH_legend_";
  saveMe += nColumns;
  saveMe += "columns";
  saveMe += ".pdf";
  myC->SaveAs(saveMe);

  return;
}

