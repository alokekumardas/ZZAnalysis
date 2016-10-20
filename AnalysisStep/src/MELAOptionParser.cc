#include <ZZAnalysis/AnalysisStep/interface/MELAOptionParser.h>

using namespace std;


MELAOptionParser::MELAOptionParser(string stropts) :
includePAux(false),
isGenProb(false),
hmass(-1),
h2mass(-1),
hwidth(0.),
h2width(0.)
{
  // Split all options by whitespace
  splitOptionRecursive(stropts, rawOptions, ' ');
  analyze();
}
void MELAOptionParser::analyze(){
  char rawdelimiter = ':';
  for (unsigned int opt=1; opt<rawOptions.size(); opt++){
    string wish, value;
    splitOption(rawOptions.at(opt), wish, value, rawdelimiter);
    interpretOption(wish, value);
  }
}
void MELAOptionParser::splitOption(string rawoption, string& wish, string& value, char delimiter){
  size_t posEq = rawoption.find(delimiter);
  if (posEq!=string::npos){
    wish=rawoption;
    value=rawoption.substr(posEq+1);
    wish.erase(wish.begin()+posEq, wish.end());
  }
  else{
    wish="";
    value=rawoption;
  }
}
void MELAOptionParser::splitOptionRecursive(string rawoption, vector<string>& splitoptions, char delimiter){
  string suboption=rawoption, result=rawoption;
  string remnant;
  while (result!=""){
    splitOption(suboption, result, remnant, delimiter);
    if (result!="") splitoptions.push_back(result);
    suboption = remnant;
  }
  if (remnant!="") splitoptions.push_back(remnant);
}
Bool_t MELAOptionParser::checkListVariable(vector<string>& list, string var){
  for (unsigned int v=0; v<list.size(); v++){
    if (list.at(v)==var) return true; // Look for exact match
  }
  return false;
}

void MELAOptionParser::interpretOption(string wish, string value){
  if (wish.empty()){
    cerr << "Unknown option with value " << value << endl;
  }
  else if (wish=="Process") setProcess(value);
  else if (wish=="Production") setProduction(value);
  else if (wish=="MatrixElement") setME(value);
  else if (wish=="SuperMelaSyst") setSuperMelaSyst(value);

  else if (wish=="isGen") isGenProb = (UInt_t)atoi(value.c_str());

  else if (wish=="hmass" || wish=="MH") hmass = (Float_t)atof(value.c_str());
  else if (wish=="h2mass" || wish=="MH2") h2mass = (Float_t)atof(value.c_str());
  else if (wish=="hwidth" || wish=="GaH") hwidth = (Float_t)atof(value.c_str());
  else if (wish=="h2width" || wish=="GaH2") h2width = (Float_t)atof(value.c_str());

  else if (wish=="Options"){
    vector<string> vtmp;
    splitOptionRecursive(value, vtmp, ';');
    for (unsigned int iopt=0; iopt<vtmp.size(); iopt++){
      string opt = vtmp.at(iopt);
      if (opt=="AddPAux") includePAux=true;
    }
  }

  else if (wish=="Couplings"){
    vector<string> vtmp;
    splitOptionRecursive(value, vtmp, ';');
    for (unsigned int iopt=0; iopt<vtmp.size(); iopt++) extractCoupling(vtmp.at(iopt));
  }

  else cerr << "Unknown specified argument: " << value << " with specifier " << wish << endl;
}

void MELAOptionParser::setProcess(string wish){
  if (wish=="HSMHiggs") proc = TVar::HSMHiggs;
  else if (wish=="H0minus") proc = TVar::H0minus;
  else if (wish=="H0hplus") proc = TVar::H0hplus;
  else if (wish=="H0_g1prime2") proc = TVar::H0_g1prime2;
  else if (wish=="H0_Zgs") proc = TVar::H0_Zgs;
  else if (wish=="H0_gsgs") proc = TVar::H0_gsgs;
  else if (wish=="H0_Zgs_PS") proc = TVar::H0_Zgs_PS;
  else if (wish=="H0_gsgs_PS") proc = TVar::H0_gsgs_PS;
  else if (wish=="H0_Zgsg1prime2") proc = TVar::H0_Zgsg1prime2;

  else if (wish=="D_g1g4") proc = TVar::D_g1g4;
  else if (wish=="D_g1g4_pi_2") proc = TVar::D_g1g4_pi_2;
  else if (wish=="D_g1g2") proc = TVar::D_g1g2;
  else if (wish=="D_g1g2_pi_2") proc = TVar::D_g1g2_pi_2;
  else if (wish=="D_g1g1prime2") proc = TVar::D_g1g1prime2;
  else if (wish=="D_zzzg") proc = TVar::D_zzzg;
  else if (wish=="D_zzgg") proc = TVar::D_zzgg;
  else if (wish=="D_zzzg_PS") proc = TVar::D_zzzg_PS;
  else if (wish=="D_zzgg_PS") proc = TVar::D_zzgg_PS;
  else if (wish=="D_zzzg_g1prime2") proc = TVar::D_zzzg_g1prime2;
  else if (wish=="D_zzzg_g1prime2_pi_2") proc = TVar::D_zzzg_g1prime2_pi_2;

  else if (wish=="H1minus") proc = TVar::H1minus;
  else if (wish=="H1plus") proc = TVar::H1plus;

  else if (wish=="H2_g1") proc = TVar::H2_g1;
  else if (wish=="H2_g2") proc = TVar::H2_g2;
  else if (wish=="H2_g3") proc = TVar::H2_g3;
  else if (wish=="H2_g4") proc = TVar::H2_g4;
  else if (wish=="H2_g5") proc = TVar::H2_g5;
  else if (wish=="H2_g1g5") proc = TVar::H2_g1g5;
  else if (wish=="H2_g6") proc = TVar::H2_g6;
  else if (wish=="H2_g7") proc = TVar::H2_g7;
  else if (wish=="H2_g8") proc = TVar::H2_g8;
  else if (wish=="H2_g9") proc = TVar::H2_g9;
  else if (wish=="H2_g10") proc = TVar::H2_g10;

  else if (wish=="bkgZGamma") proc = TVar::bkgZGamma;
  else if (wish=="bkgZJets") proc = TVar::bkgZJets;
  else if (wish=="bkgZZ") proc = TVar::bkgZZ;
  else if (wish=="bkgWW") proc = TVar::bkgWW;
  else if (wish=="bkgWWZZ") proc = TVar::bkgWWZZ;

  else if (wish=="bkgZZ_SMHiggs") proc = TVar::bkgZZ_SMHiggs;
  else if (wish=="bkgWW_SMHiggs") proc = TVar::bkgWW_SMHiggs;
  else if (wish=="bkgWWZZ_SMHiggs") proc = TVar::bkgWWZZ_SMHiggs;

  else if (wish=="HSMHiggs_WWZZ") proc = TVar::HSMHiggs_WWZZ;

  else if (wish=="D_gg10") proc = TVar::D_gg10;

  else if (wish=="SelfDefine_spin0") proc = TVar::SelfDefine_spin0;
  else if (wish=="SelfDefine_spin1") proc = TVar::SelfDefine_spin1;
  else if (wish=="SelfDefine_spin2") proc = TVar::SelfDefine_spin2;
  else cerr << "MELAOptionParser::setProcess(" << wish << "): Failed to find the proper process." << endl;
}
void MELAOptionParser::setProduction(string wish){
  if (wish=="ZZGG" || wish=="GG") prod = TVar::ZZGG;
  else if (wish=="ZZQQB" || wish=="QQB") prod = TVar::ZZQQB;
  else if (wish=="ZZQQB_STU" || wish=="QQB_STU") prod = TVar::ZZQQB_STU;
  else if (wish=="ZZQQB_S" || wish=="QQB_S") prod = TVar::ZZQQB_S;
  else if (wish=="ZZQQB_TU" || wish=="QQB_TU") prod = TVar::ZZQQB_TU;
  else if (wish=="ZZINDEPENDENT" || wish=="INDEPENDENT") prod = TVar::ZZINDEPENDENT;
  else if (wish=="JJQCD") prod = TVar::JJQCD;
  else if (wish=="JJVBF") prod = TVar::JJVBF;
  else if (wish=="JQCD") prod = TVar::JQCD;
  else if (wish=="Lep_ZH") prod = TVar::Lep_ZH;
  else if (wish=="Lep_WH") prod = TVar::Lep_WH;
  else if (wish=="Had_ZH" || wish=="ZH") prod = TVar::Had_ZH;
  else if (wish=="Had_WH" || wish=="WH") prod = TVar::Had_WH;
  else if (wish=="GammaH") prod = TVar::GammaH;
  else if (wish=="ttH") prod = TVar::ttH;
  else if (wish=="bbH") prod = TVar::bbH;
  else cerr << "MELAOptionParser::setProduction(" << wish << "): Failed to find the proper production." << endl;
}
void MELAOptionParser::setME(string wish){
  if (wish=="MCFM") ME = TVar::MCFM;
  else if (wish=="JHUGen") ME = TVar::JHUGen;
  else if (wish=="Analytical" || wish=="ANALYTICAL") ME = TVar::ANALYTICAL;
  else cerr << "MELAOptionParser::setME(" << wish << "): Failed to find the proper matrix element." << endl;
}
void MELAOptionParser::setSuperMelaSyst(string wish){
  if (wish=="SMSyst_None") superSyst = TVar::SMSyst_None;
  else if (wish=="SMSyst_ScaleUp") superSyst = TVar::SMSyst_ScaleUp;
  else if (wish=="SMSyst_ResUp") superSyst = TVar::SMSyst_ResUp;
  else if (wish=="SMSyst_ScaleDown") superSyst = TVar::SMSyst_ScaleDown;
  else if (wish=="SMSyst_ResDown") superSyst = TVar::SMSyst_ResDown;
  else cerr << "MELAOptionParser::setSuperMelaSyst(" << wish << "): Failed to find the proper SuperMELA systematics case." << endl;
}
void MELAOptionParser::extractCoupling(string opt){
  string wish, strValRe, strValIm;
  float valRe=0;
  float valIm=0;
  splitOption(opt, wish, strValRe, '=');
  // Lambda and cz/cw couplings have no imaginary components, so do not expect to parse them with ','.
  if (
    wish.find("Lambda")==string::npos
    &&
    wish.find("q1sq")==string::npos
    &&
    wish.find("q2sq")==string::npos
    &&
    wish.find("q12sq")==string::npos
    &&
    wish!="separateWWZZcouplings"
    ){
    splitOption(opt, strValRe, strValIm, ',');
    valIm = atof(strValIm.c_str());
  }
  valRe = atof(strValRe.c_str());

  // Here we go again, sillions of couplings
  if (wish=="separateWWZZcouplings"){
    coupl_H.allow_WWZZSeparation((bool)valRe);
  }

  // Spin-0 couplings, first resonance
  else if (wish=="kappa"){ coupl_H.Hqqcoupl[gHIGGS_KAPPA][0]=valRe; coupl_H.Hqqcoupl[gHIGGS_KAPPA][1]=valIm; }
  else if (wish=="kappa_tilde"){ coupl_H.Hqqcoupl[gHIGGS_KAPPA_TILDE][0]=valRe; coupl_H.Hqqcoupl[gHIGGS_KAPPA_TILDE][1]=valIm; }
  else if (wish=="ghg2"){ coupl_H.Hggcoupl[gHIGGS_GG_2][0]=valRe; coupl_H.Hggcoupl[gHIGGS_GG_2][1]=valIm; }
  else if (wish=="ghg3"){ coupl_H.Hggcoupl[gHIGGS_GG_3][0]=valRe; coupl_H.Hggcoupl[gHIGGS_GG_3][1]=valIm; }
  else if (wish=="ghg4"){ coupl_H.Hggcoupl[gHIGGS_GG_4][0]=valRe; coupl_H.Hggcoupl[gHIGGS_GG_4][1]=valIm; }

  else if (wish=="cz_q1sq"){ coupl_H.HzzCLambda_qsq[cLambdaHIGGS_VV_QSQ1]=(int)valRe; }
  else if (wish=="cz_q2sq"){ coupl_H.HzzCLambda_qsq[cLambdaHIGGS_VV_QSQ2]=(int)valRe; }
  else if (wish=="cz_q12sq"){ coupl_H.HzzCLambda_qsq[cLambdaHIGGS_VV_QSQ12]=(int)valRe; }
  else if (wish=="Lambda_z11"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda_z21"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda_z31"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda_z41"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda_z12"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda_z22"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda_z32"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda_z42"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda_z10"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda_z20"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda_z30"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda_z40"){ coupl_H.HzzLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="ghz1"){ coupl_H.Hzzcoupl[gHIGGS_VV_1][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_1][1]=valIm; }
  else if (wish=="ghz2"){ coupl_H.Hzzcoupl[gHIGGS_VV_2][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_2][1]=valIm; }
  else if (wish=="ghz3"){ coupl_H.Hzzcoupl[gHIGGS_VV_3][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_3][1]=valIm; }
  else if (wish=="ghz4"){ coupl_H.Hzzcoupl[gHIGGS_VV_4][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_4][1]=valIm; }
  else if (wish=="ghzgs1_prime2"){ coupl_H.Hzzcoupl[gHIGGS_ZA_1_PRIME2][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_ZA_1_PRIME2][1]=valIm; }
  else if (wish=="ghzgs2"){ coupl_H.Hzzcoupl[gHIGGS_ZA_2][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_ZA_2][1]=valIm; }
  else if (wish=="ghzgs3"){ coupl_H.Hzzcoupl[gHIGGS_ZA_3][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_ZA_3][1]=valIm; }
  else if (wish=="ghzgs4"){ coupl_H.Hzzcoupl[gHIGGS_ZA_4][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_ZA_4][1]=valIm; }
  else if (wish=="ghgsgs2"){ coupl_H.Hzzcoupl[gHIGGS_AA_2][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_AA_2][1]=valIm; }
  else if (wish=="ghgsgs3"){ coupl_H.Hzzcoupl[gHIGGS_AA_3][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_AA_3][1]=valIm; }
  else if (wish=="ghgsgs4"){ coupl_H.Hzzcoupl[gHIGGS_AA_4][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_AA_4][1]=valIm; }
  else if (wish=="ghz1_prime"){ coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME][1]=valIm; }
  else if (wish=="ghz1_prime2"){ coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME2][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME2][1]=valIm; }
  else if (wish=="ghz1_prime3"){ coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME3][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME3][1]=valIm; }
  else if (wish=="ghz1_prime4"){ coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME4][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME4][1]=valIm; }
  else if (wish=="ghz1_prime5"){ coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME5][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME5][1]=valIm; }
  else if (wish=="ghz2_prime"){ coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME][1]=valIm; }
  else if (wish=="ghz2_prime2"){ coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME2][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME2][1]=valIm; }
  else if (wish=="ghz2_prime3"){ coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME3][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME3][1]=valIm; }
  else if (wish=="ghz2_prime4"){ coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME4][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME4][1]=valIm; }
  else if (wish=="ghz2_prime5"){ coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME5][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME5][1]=valIm; }
  else if (wish=="ghz3_prime"){ coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME][1]=valIm; }
  else if (wish=="ghz3_prime2"){ coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME2][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME2][1]=valIm; }
  else if (wish=="ghz3_prime3"){ coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME3][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME3][1]=valIm; }
  else if (wish=="ghz3_prime4"){ coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME4][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME4][1]=valIm; }
  else if (wish=="ghz3_prime5"){ coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME5][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME5][1]=valIm; }
  else if (wish=="ghz4_prime"){ coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME][1]=valIm; }
  else if (wish=="ghz4_prime2"){ coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME2][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME2][1]=valIm; }
  else if (wish=="ghz4_prime3"){ coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME3][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME3][1]=valIm; }
  else if (wish=="ghz4_prime4"){ coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME4][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME4][1]=valIm; }
  else if (wish=="ghz4_prime5"){ coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME5][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME5][1]=valIm; }
  else if (wish=="ghz1_prime6"){ coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME6][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME6][1]=valIm; }
  else if (wish=="ghz1_prime7"){ coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME7][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_1_PRIME7][1]=valIm; }
  else if (wish=="ghz2_prime6"){ coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME6][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME6][1]=valIm; }
  else if (wish=="ghz2_prime7"){ coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME7][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_2_PRIME7][1]=valIm; }
  else if (wish=="ghz3_prime6"){ coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME6][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME6][1]=valIm; }
  else if (wish=="ghz3_prime7"){ coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME7][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_3_PRIME7][1]=valIm; }
  else if (wish=="ghz4_prime6"){ coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME6][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME6][1]=valIm; }
  else if (wish=="ghz4_prime7"){ coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME7][0]=valRe; coupl_H.Hzzcoupl[gHIGGS_VV_4_PRIME7][1]=valIm; }
  else if (wish=="cw_q1sq"){ coupl_H.HwwCLambda_qsq[cLambdaHIGGS_VV_QSQ1]=(int)valRe; }
  else if (wish=="cw_q2sq"){ coupl_H.HwwCLambda_qsq[cLambdaHIGGS_VV_QSQ2]=(int)valRe; }
  else if (wish=="cw_q12sq"){ coupl_H.HwwCLambda_qsq[cLambdaHIGGS_VV_QSQ12]=(int)valRe; }
  else if (wish=="Lambda_w11"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda_w21"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda_w31"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda_w41"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda_w12"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda_w22"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda_w32"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda_w42"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda_w10"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda_w20"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda_w30"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda_w40"){ coupl_H.HwwLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="ghw1"){ coupl_H.Hwwcoupl[gHIGGS_VV_1][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_1][1]=valIm; }
  else if (wish=="ghw2"){ coupl_H.Hwwcoupl[gHIGGS_VV_2][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_2][1]=valIm; }
  else if (wish=="ghw3"){ coupl_H.Hwwcoupl[gHIGGS_VV_3][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_3][1]=valIm; }
  else if (wish=="ghw4"){ coupl_H.Hwwcoupl[gHIGGS_VV_4][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_4][1]=valIm; }
  else if (wish=="ghw1_prime"){ coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME][1]=valIm; }
  else if (wish=="ghw1_prime2"){ coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME2][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME2][1]=valIm; }
  else if (wish=="ghw1_prime3"){ coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME3][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME3][1]=valIm; }
  else if (wish=="ghw1_prime4"){ coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME4][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME4][1]=valIm; }
  else if (wish=="ghw1_prime5"){ coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME5][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME5][1]=valIm; }
  else if (wish=="ghw2_prime"){ coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME][1]=valIm; }
  else if (wish=="ghw2_prime2"){ coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME2][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME2][1]=valIm; }
  else if (wish=="ghw2_prime3"){ coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME3][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME3][1]=valIm; }
  else if (wish=="ghw2_prime4"){ coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME4][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME4][1]=valIm; }
  else if (wish=="ghw2_prime5"){ coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME5][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME5][1]=valIm; }
  else if (wish=="ghw3_prime"){ coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME][1]=valIm; }
  else if (wish=="ghw3_prime2"){ coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME2][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME2][1]=valIm; }
  else if (wish=="ghw3_prime3"){ coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME3][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME3][1]=valIm; }
  else if (wish=="ghw3_prime4"){ coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME4][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME4][1]=valIm; }
  else if (wish=="ghw3_prime5"){ coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME5][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME5][1]=valIm; }
  else if (wish=="ghw4_prime"){ coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME][1]=valIm; }
  else if (wish=="ghw4_prime2"){ coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME2][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME2][1]=valIm; }
  else if (wish=="ghw4_prime3"){ coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME3][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME3][1]=valIm; }
  else if (wish=="ghw4_prime4"){ coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME4][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME4][1]=valIm; }
  else if (wish=="ghw4_prime5"){ coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME5][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME5][1]=valIm; }
  else if (wish=="ghw1_prime6"){ coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME6][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME6][1]=valIm; }
  else if (wish=="ghw1_prime7"){ coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME7][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_1_PRIME7][1]=valIm; }
  else if (wish=="ghw2_prime6"){ coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME6][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME6][1]=valIm; }
  else if (wish=="ghw2_prime7"){ coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME7][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_2_PRIME7][1]=valIm; }
  else if (wish=="ghw3_prime6"){ coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME6][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME6][1]=valIm; }
  else if (wish=="ghw3_prime7"){ coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME7][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_3_PRIME7][1]=valIm; }
  else if (wish=="ghw4_prime6"){ coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME6][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME6][1]=valIm; }
  else if (wish=="ghw4_prime7"){ coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME7][0]=valRe; coupl_H.Hwwcoupl[gHIGGS_VV_4_PRIME7][1]=valIm; }

  // Spin-0 couplings, second resonance
  else if (wish=="c2z_q1sq"){ coupl_H.H2zzCLambda_qsq[cLambdaHIGGS_VV_QSQ1]=(int)valRe; }
  else if (wish=="c2z_q2sq"){ coupl_H.H2zzCLambda_qsq[cLambdaHIGGS_VV_QSQ2]=(int)valRe; }
  else if (wish=="c2z_q12sq"){ coupl_H.H2zzCLambda_qsq[cLambdaHIGGS_VV_QSQ12]=(int)valRe; }
  else if (wish=="Lambda2_z11"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda2_z21"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda2_z31"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda2_z41"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda2_z12"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda2_z22"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda2_z32"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda2_z42"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda2_z10"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda2_z20"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda2_z30"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda2_z40"){ coupl_H.H2zzLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="gh2z1"){ coupl_H.H2zzcoupl[gHIGGS_VV_1][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_1][1]=valIm; }
  else if (wish=="gh2z2"){ coupl_H.H2zzcoupl[gHIGGS_VV_2][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_2][1]=valIm; }
  else if (wish=="gh2z3"){ coupl_H.H2zzcoupl[gHIGGS_VV_3][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_3][1]=valIm; }
  else if (wish=="gh2z4"){ coupl_H.H2zzcoupl[gHIGGS_VV_4][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_4][1]=valIm; }
  else if (wish=="gh2zgs1_prime2"){ coupl_H.H2zzcoupl[gHIGGS_ZA_1_PRIME2][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_ZA_1_PRIME2][1]=valIm; }
  else if (wish=="gh2zgs2"){ coupl_H.H2zzcoupl[gHIGGS_ZA_2][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_ZA_2][1]=valIm; }
  else if (wish=="gh2zgs3"){ coupl_H.H2zzcoupl[gHIGGS_ZA_3][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_ZA_3][1]=valIm; }
  else if (wish=="gh2zgs4"){ coupl_H.H2zzcoupl[gHIGGS_ZA_4][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_ZA_4][1]=valIm; }
  else if (wish=="gh2gsgs2"){ coupl_H.H2zzcoupl[gHIGGS_AA_2][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_AA_2][1]=valIm; }
  else if (wish=="gh2gsgs3"){ coupl_H.H2zzcoupl[gHIGGS_AA_3][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_AA_3][1]=valIm; }
  else if (wish=="gh2gsgs4"){ coupl_H.H2zzcoupl[gHIGGS_AA_4][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_AA_4][1]=valIm; }
  else if (wish=="gh2z1_prime"){ coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME][1]=valIm; }
  else if (wish=="gh2z1_prime2"){ coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME2][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME2][1]=valIm; }
  else if (wish=="gh2z1_prime3"){ coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME3][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME3][1]=valIm; }
  else if (wish=="gh2z1_prime4"){ coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME4][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME4][1]=valIm; }
  else if (wish=="gh2z1_prime5"){ coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME5][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME5][1]=valIm; }
  else if (wish=="gh2z2_prime"){ coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME][1]=valIm; }
  else if (wish=="gh2z2_prime2"){ coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME2][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME2][1]=valIm; }
  else if (wish=="gh2z2_prime3"){ coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME3][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME3][1]=valIm; }
  else if (wish=="gh2z2_prime4"){ coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME4][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME4][1]=valIm; }
  else if (wish=="gh2z2_prime5"){ coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME5][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME5][1]=valIm; }
  else if (wish=="gh2z3_prime"){ coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME][1]=valIm; }
  else if (wish=="gh2z3_prime2"){ coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME2][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME2][1]=valIm; }
  else if (wish=="gh2z3_prime3"){ coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME3][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME3][1]=valIm; }
  else if (wish=="gh2z3_prime4"){ coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME4][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME4][1]=valIm; }
  else if (wish=="gh2z3_prime5"){ coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME5][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME5][1]=valIm; }
  else if (wish=="gh2z4_prime"){ coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME][1]=valIm; }
  else if (wish=="gh2z4_prime2"){ coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME2][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME2][1]=valIm; }
  else if (wish=="gh2z4_prime3"){ coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME3][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME3][1]=valIm; }
  else if (wish=="gh2z4_prime4"){ coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME4][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME4][1]=valIm; }
  else if (wish=="gh2z4_prime5"){ coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME5][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME5][1]=valIm; }
  else if (wish=="gh2z1_prime6"){ coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME6][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME6][1]=valIm; }
  else if (wish=="gh2z1_prime7"){ coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME7][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_1_PRIME7][1]=valIm; }
  else if (wish=="gh2z2_prime6"){ coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME6][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME6][1]=valIm; }
  else if (wish=="gh2z2_prime7"){ coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME7][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_2_PRIME7][1]=valIm; }
  else if (wish=="gh2z3_prime6"){ coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME6][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME6][1]=valIm; }
  else if (wish=="gh2z3_prime7"){ coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME7][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_3_PRIME7][1]=valIm; }
  else if (wish=="gh2z4_prime6"){ coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME6][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME6][1]=valIm; }
  else if (wish=="gh2z4_prime7"){ coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME7][0]=valRe; coupl_H.H2zzcoupl[gHIGGS_VV_4_PRIME7][1]=valIm; }
  else if (wish=="c2w_q1sq"){ coupl_H.H2wwCLambda_qsq[cLambdaHIGGS_VV_QSQ1]=(int)valRe; }
  else if (wish=="c2w_q2sq"){ coupl_H.H2wwCLambda_qsq[cLambdaHIGGS_VV_QSQ2]=(int)valRe; }
  else if (wish=="c2w_q12sq"){ coupl_H.H2wwCLambda_qsq[cLambdaHIGGS_VV_QSQ12]=(int)valRe; }
  else if (wish=="Lambda2_w11"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda2_w21"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda2_w31"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda2_w41"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ1]=valRe; }
  else if (wish=="Lambda2_w12"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda2_w22"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda2_w32"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda2_w42"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ2]=valRe; }
  else if (wish=="Lambda2_w10"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_1][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda2_w20"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_2][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda2_w30"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_3][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="Lambda2_w40"){ coupl_H.H2wwLambda_qsq[LambdaHIGGS_QSQ_VV_4][cLambdaHIGGS_VV_QSQ12]=valRe; }
  else if (wish=="gh2w1"){ coupl_H.H2wwcoupl[gHIGGS_VV_1][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_1][1]=valIm; }
  else if (wish=="gh2w2"){ coupl_H.H2wwcoupl[gHIGGS_VV_2][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_2][1]=valIm; }
  else if (wish=="gh2w3"){ coupl_H.H2wwcoupl[gHIGGS_VV_3][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_3][1]=valIm; }
  else if (wish=="gh2w4"){ coupl_H.H2wwcoupl[gHIGGS_VV_4][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_4][1]=valIm; }
  else if (wish=="gh2w1_prime"){ coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME][1]=valIm; }
  else if (wish=="gh2w1_prime2"){ coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME2][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME2][1]=valIm; }
  else if (wish=="gh2w1_prime3"){ coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME3][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME3][1]=valIm; }
  else if (wish=="gh2w1_prime4"){ coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME4][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME4][1]=valIm; }
  else if (wish=="gh2w1_prime5"){ coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME5][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME5][1]=valIm; }
  else if (wish=="gh2w2_prime"){ coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME][1]=valIm; }
  else if (wish=="gh2w2_prime2"){ coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME2][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME2][1]=valIm; }
  else if (wish=="gh2w2_prime3"){ coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME3][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME3][1]=valIm; }
  else if (wish=="gh2w2_prime4"){ coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME4][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME4][1]=valIm; }
  else if (wish=="gh2w2_prime5"){ coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME5][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME5][1]=valIm; }
  else if (wish=="gh2w3_prime"){ coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME][1]=valIm; }
  else if (wish=="gh2w3_prime2"){ coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME2][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME2][1]=valIm; }
  else if (wish=="gh2w3_prime3"){ coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME3][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME3][1]=valIm; }
  else if (wish=="gh2w3_prime4"){ coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME4][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME4][1]=valIm; }
  else if (wish=="gh2w3_prime5"){ coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME5][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME5][1]=valIm; }
  else if (wish=="gh2w4_prime"){ coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME][1]=valIm; }
  else if (wish=="gh2w4_prime2"){ coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME2][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME2][1]=valIm; }
  else if (wish=="gh2w4_prime3"){ coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME3][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME3][1]=valIm; }
  else if (wish=="gh2w4_prime4"){ coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME4][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME4][1]=valIm; }
  else if (wish=="gh2w4_prime5"){ coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME5][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME5][1]=valIm; }
  else if (wish=="gh2w1_prime6"){ coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME6][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME6][1]=valIm; }
  else if (wish=="gh2w1_prime7"){ coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME7][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_1_PRIME7][1]=valIm; }
  else if (wish=="gh2w2_prime6"){ coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME6][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME6][1]=valIm; }
  else if (wish=="gh2w2_prime7"){ coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME7][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_2_PRIME7][1]=valIm; }
  else if (wish=="gh2w3_prime6"){ coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME6][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME6][1]=valIm; }
  else if (wish=="gh2w3_prime7"){ coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME7][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_3_PRIME7][1]=valIm; }
  else if (wish=="gh2w4_prime6"){ coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME6][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME6][1]=valIm; }
  else if (wish=="gh2w4_prime7"){ coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME7][0]=valRe; coupl_H.H2wwcoupl[gHIGGS_VV_4_PRIME7][1]=valIm; }

  // Spin-1 couplings, only for the first resonance
  else if (wish=="zprime_qq_left"){ coupl_Zp.Zqqcoupl[gZPRIME_QQ_LEFT][0]=valRe; coupl_Zp.Zqqcoupl[gZPRIME_QQ_LEFT][1]=valIm; }
  else if (wish=="zprime_qq_right"){ coupl_Zp.Zqqcoupl[gZPRIME_QQ_RIGHT][0]=valRe; coupl_Zp.Zqqcoupl[gZPRIME_QQ_RIGHT][1]=valIm; }
  else if (wish=="zprime_zz_1"){ coupl_Zp.Zvvcoupl[gZPRIME_VV_1][0]=valRe; coupl_Zp.Zvvcoupl[gZPRIME_VV_1][1]=valIm; }
  else if (wish=="zprime_zz_2"){ coupl_Zp.Zvvcoupl[gZPRIME_VV_2][0]=valRe; coupl_Zp.Zvvcoupl[gZPRIME_VV_2][1]=valIm; }

  // Spin-2 couplings, only for the first resonance
  else if (wish=="graviton_qq_left"){ coupl_X.Gqqcoupl[gGRAVITON_QQ_LEFT][0]=valRe; coupl_X.Gqqcoupl[gGRAVITON_QQ_LEFT][1]=valIm; }
  else if (wish=="graviton_qq_right"){ coupl_X.Gqqcoupl[gGRAVITON_QQ_RIGHT][0]=valRe; coupl_X.Gqqcoupl[gGRAVITON_QQ_RIGHT][1]=valIm; }
  else if (wish=="a1"){ coupl_X.Gggcoupl[gGRAVITON_GG_1][0]=valRe; coupl_X.Gggcoupl[gGRAVITON_GG_1][1]=valIm; }
  else if (wish=="a2"){ coupl_X.Gggcoupl[gGRAVITON_GG_2][0]=valRe; coupl_X.Gggcoupl[gGRAVITON_GG_2][1]=valIm; }
  else if (wish=="a3"){ coupl_X.Gggcoupl[gGRAVITON_GG_3][0]=valRe; coupl_X.Gggcoupl[gGRAVITON_GG_3][1]=valIm; }
  else if (wish=="a4"){ coupl_X.Gggcoupl[gGRAVITON_GG_4][0]=valRe; coupl_X.Gggcoupl[gGRAVITON_GG_4][1]=valIm; }
  else if (wish=="a5"){ coupl_X.Gggcoupl[gGRAVITON_GG_5][0]=valRe; coupl_X.Gggcoupl[gGRAVITON_GG_5][1]=valIm; }
  else if (wish=="b1"){ coupl_X.Gvvcoupl[gGRAVITON_VV_1][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_1][1]=valIm; }
  else if (wish=="b2"){ coupl_X.Gvvcoupl[gGRAVITON_VV_2][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_2][1]=valIm; }
  else if (wish=="b3"){ coupl_X.Gvvcoupl[gGRAVITON_VV_3][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_3][1]=valIm; }
  else if (wish=="b4"){ coupl_X.Gvvcoupl[gGRAVITON_VV_4][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_4][1]=valIm; }
  else if (wish=="b5"){ coupl_X.Gvvcoupl[gGRAVITON_VV_5][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_5][1]=valIm; }
  else if (wish=="b6"){ coupl_X.Gvvcoupl[gGRAVITON_VV_6][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_6][1]=valIm; }
  else if (wish=="b7"){ coupl_X.Gvvcoupl[gGRAVITON_VV_7][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_7][1]=valIm; }
  else if (wish=="b8"){ coupl_X.Gvvcoupl[gGRAVITON_VV_8][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_8][1]=valIm; }
  else if (wish=="b9"){ coupl_X.Gvvcoupl[gGRAVITON_VV_9][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_9][1]=valIm; }
  else if (wish=="b10"){ coupl_X.Gvvcoupl[gGRAVITON_VV_10][0]=valRe; coupl_X.Gvvcoupl[gGRAVITON_VV_10][1]=valIm; }

  else cerr << "MELAOptionParser::extractCoupling: Coupling " << wish << " is not supported!" << endl;
}
