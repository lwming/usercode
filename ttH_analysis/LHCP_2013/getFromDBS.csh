#!/bin/tcsh -f


#set myTarget = /TTJets_TuneZ2_7TeV-madgraph-tauola/slaunwhj-ttjets_BEAN_V05_CV02_Fall1_toND_fromGlideIn_v1-5a5f035e194011d3a58705a28d5b2f8b/USER
#set myTarget = /DoubleElectron/slaunwhj-DoubleElectron_Run2012A_week02_V01-f9e056d2e35587e92447414aba1e12ec/USER
#set listOutFile = doubleEle2012_week02.list
#set myTarget = 



set listOfSamples = ( /TTH_HtoAll_M_100_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02-4b4d492b3b4660af38ac40a6f45935c1/USER \
/TTH_HtoAll_M_105_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH105-4b4d492b3b4660af38ac40a6f45935c1/USER \
/TTH_HtoAll_M_110_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH110-4b4d492b3b4660af38ac40a6f45935c1/USER \
/TTH_HtoAll_M_115_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH115-4b4d492b3b4660af38ac40a6f45935c1/USER \
/TTH_HtoAll_M_120_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02-4b4d492b3b4660af38ac40a6f45935c1/USER \
/TTH_HtoAll_M_125_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH125-4b4d492b3b4660af38ac40a6f45935c1/USER \
/TTH_HtoAll_M_130_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH130-4b4d492b3b4660af38ac40a6f45935c1/USER \
/TTH_HtoAll_M_135_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH135-4b4d492b3b4660af38ac40a6f45935c1/USER \
/TTH_HtoAll_M_140_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH140-4b4d492b3b4660af38ac40a6f45935c1/USER \
/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_zjets-4b4d492b3b4660af38ac40a6f45935c1/USER \
/TTW_TuneZ2_7TeV-madgraphCMSSW42xPUv2/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttW-4b4d492b3b4660af38ac40a6f45935c1/USER \
/WW_TuneZ2_7TeV_pythia6_tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_WW-4b4d492b3b4660af38ac40a6f45935c1/USER \
/WZ_TuneZ2_7TeV_pythia6_tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_WZ-4b4d492b3b4660af38ac40a6f45935c1/USER \
/ZZ_TuneZ2_7TeV_pythia6_tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ZZ-4b4d492b3b4660af38ac40a6f45935c1/USER \
/T_TuneZ2_s-channel_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_t_schan-4b4d492b3b4660af38ac40a6f45935c1/USER \
/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_t_tW-4b4d492b3b4660af38ac40a6f45935c1/USER \
/T_TuneZ2_t-channel_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_t_tchan-4b4d492b3b4660af38ac40a6f45935c1/USER \
/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_tbar_schan-4b4d492b3b4660af38ac40a6f45935c1/USER \
/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_tbar_tW-4b4d492b3b4660af38ac40a6f45935c1/USER \
/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_tbar_tchan-4b4d492b3b4660af38ac40a6f45935c1/USER )

set listOutFile = "allSamples.list"
touch $listOutFile

if (-e "$listOutFile") then
  rm -f "$listOutFile"
  touch $listOutFile
endif

foreach iTarget ($listOfSamples)
  echo "Sample $iTarget"
  echo "\n\n\n$iTarget\n\n" >> $listOutFile
  dbs search --url='https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet' --query="find file.name where dataset = '$iTarget'" | grep '.root' | sed 's|^|file:|' >> $listOutFile
end 
