

set year = "2012_53x"
set zmask = "Zmask"
#set zmask = "noZmask"
#foreach zmask ("noZmask" "noZmask_lowPV" "noZmask_medPV" "noZmask_highPV")

foreach lepCat ("TwoLep")
  cd $lepCat
  echo "Now in $PWD"

  foreach sample ("TwoLep" "singlet_s" "singlet_tW" "singlet_t" "singletbar_s" "singletbar_tW" "singletbar_t" "ttbarW" "ttbarZ" "ttbar_b" "ttbar_bb" "ttbar_cc" "ttbar" "wjets" "ww" "wz" "zjets" "zjets_lowmass" "zz" "ttH110" "ttH115" "ttH120" "ttH125" "ttH130" "ttH135" "ttH140")
    set allcatString = ""
    foreach kind ( "eq2jeq2t" "eq3jeq2t" "ge4jeq2t" "ge3t" )
#    foreach kind ("eq2jeq2t" "eq3jeq2t" "ge4jeq2t" )

	set allcatString = "${allcatString} ${sample}_${year}_${zmask}_OS_${kind}_${lepCat}.root" 
    end

#    hadd -f ${sample}_${year}_${zmask}_ge2t_${lepCat}.root ${allcatString}
    hadd -f ${sample}_${year}_${zmask}_OS_ge2t_${lepCat}.root ${allcatString}
    #echo "hadd -f zjets_${year}_${zmask}_${kind}_${lepCat}.root ${zjetsPartString}"
    #echo "hadd -f ttbar_${year}_${zmask}_${kind}_${lepCat}.root ${allcatString}"
  end
  cd ..
end

#end
