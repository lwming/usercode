
import os

listOfSamples = {
"/TTH_HtoAll_M_100_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttH100.list",
"/TTH_HtoAll_M_105_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH105-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttH105.list",
"/TTH_HtoAll_M_110_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH110-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttH110.list",
"/TTH_HtoAll_M_115_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH115-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttH115.list",
"/TTH_HtoAll_M_120_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttH120.list",
"/TTH_HtoAll_M_125_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH125-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttH125.list",
"/TTH_HtoAll_M_130_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH130-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttH130.list",
"/TTH_HtoAll_M_135_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH135-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttH135.list",
"/TTH_HtoAll_M_140_7TeV_pythia6/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttH140-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttH140.list",
"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_zjets-4b4d492b3b4660af38ac40a6f45935c1/USER":"zjets.list",
"/TTW_TuneZ2_7TeV-madgraphCMSSW42xPUv2/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ttW-4b4d492b3b4660af38ac40a6f45935c1/USER":"ttbarW.list",
"/WW_TuneZ2_7TeV_pythia6_tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_WW-4b4d492b3b4660af38ac40a6f45935c1/USER":"ww.list",
"/WZ_TuneZ2_7TeV_pythia6_tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_WZ-4b4d492b3b4660af38ac40a6f45935c1/USER":"wz.list",
"/ZZ_TuneZ2_7TeV_pythia6_tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_ZZ-4b4d492b3b4660af38ac40a6f45935c1/USER":"zz.list",
"/T_TuneZ2_s-channel_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_t_schan-4b4d492b3b4660af38ac40a6f45935c1/USER":"singlet_s.list",
"/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_t_tW-4b4d492b3b4660af38ac40a6f45935c1/USER":"singlet_tW.list",
"/T_TuneZ2_t-channel_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_t_tchan-4b4d492b3b4660af38ac40a6f45935c1/USER":"singlet_t.list",
"/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_tbar_schan-4b4d492b3b4660af38ac40a6f45935c1/USER":"singletbar_s.list",
"/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_tbar_tW-4b4d492b3b4660af38ac40a6f45935c1/USER":"singletbar_tW.list",
"/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/lannon-skimDilep_START42_V11_v2_BEAN_V05_CV02_tbar_tchan-4b4d492b3b4660af38ac40a6f45935c1/USER":"singletbar_t.list"
}

for iTarget, iList in listOfSamples.iteritems():
	print "Target = %s\nList = %s\n" % (iTarget, iList)

	command="dbs search --url='https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet'"' --query="find file.name where dataset ='" '%s'"'" | grep ".root" | sed "s|^|file:|" > %s' % (iTarget,iList)
	print command
	print "--"
	os.popen(command)

print "Done"
