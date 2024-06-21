#include <string>
#include <iostream>
#include <fstream> 

using namespace ROOT;
using namespace ROOT::Math;
using namespace ROOT::RDF;

const double RunIdMin = 6600;
const double RunIdMax = 8500;
int RunIdBins= (int)(RunIdMax - RunIdMin);

struct HistoParameter
{
	std::string fName;
	std::string fFillObj;
	std::string fAxisTitle;
	int    fNBins;
	double  fMin;
	double  fMax;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  {Name histo, 		{Name histo, 		name of the column,  	Axis title, 				n bins, 	min, 		max}   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::map< std::string, HistoParameter > QA_variable{
	{"RunId",			{"RunId",			"runId",						";Run ID", 					RunIdBins,	RunIdMin,	RunIdMax}},
	{"bc1sAmp",			{"bc1sAmp",			"bc1sAmplitude",			";bc1 S Amplitude",			800,		0,			4e4}},
	{"bc1bAmp",			{"bc1bAmp",			"bc1bAmplitude",			";bc1 B Amplitude",			800,		0,			4e4}},
	{"bc1tAmp",			{"bc1tAmp",			"bc1tAmplitude",			";bc1 T Amplitude",			800,		0,			4e4}},
	{"bc1sIntNorm",	{"bc1sIntNorm",	"bc1sIntegralNorm",		";bc1 S Integral", 			800,		0,			10}},
	{"bc1sInt",			{"bc1sInt",			"bc1sIntegral",			";bc1 S Integral", 			800,		0,			4e5}},
	{"bc1bInt",			{"bc1bInt",			"bc1bIntegral",			";bc1 B Integral", 			800,		0,			4e5}},
	{"bc1tInt",			{"bc1tInt",			"bc1tIntegral",			";bc1 T Integral", 			800,		0,			4e5}},
	{"fdAmp",			{"fdAmp",			"fdAmplitude",				";fd Amplitude", 				800,		0,			4e4}},
	{"fdInt",			{"fdInt",			"fdIntegral",				";fd Integral", 				1000,		-10000,		4e5}},
	{"fdIntNorm",		{"fdIntNorm",		"fdIntegralNorm",			";fd Integral", 				1000,		-1,			10}},	
	{"vcsAmp",			{"vcsAmp",			"vcsAmplitude",			";vcs Amplitude", 			800,		0,			4e4}},
	{"vcsInt",			{"vcsInt",			"vcsIntegral",				";vcs Integral", 				800,		0,			4e5}},
	{"bdNum",			{"bdNum",			"bd_num",					";BD mult",						200,		0,			200}},
	{"bdNumSingle",	{"bdNumSingle",	"bd_numSingle",			";BD mult",						200,		0,			200}},
	{"nbdMod",			{"nbdMod",			"nbdMod",					";BD modId",					200,		0,			200}},
	{"nbdModSingle",	{"nbdModSingle",	"nbdModSingle",			";BD modId",					200,		0,			200}},
	{"bdMult",			{"bdMult",			"bdMult",					";BD mult",						200,		0,			200}},
	{"bdModId",			{"bdModId",			"bdModId",					";BD modId",					100,		0,			100}},
	{"bdModAmp",		{"bdModAmp",		"bdModAmp",					";BD modAmp",					100,		0,			100}},
	
	// Event parametrs
	{"nTracks",			{"nTracks",			"track_multiplicity",	";N tracks", 				1000,		0,			1000}},
	{"nStsTracks",		{"nStsTracks",		"sts_track_multiplicity",";N sts tracks",			1000,		0,			1000}},
	{"simd_mult",		{"simd_mult",		"simdMult",					";SiMD multiplicity",		200,		0,			200}},
	{"gemDigits",		{"gemDigits",		"gemNdigits",				";GEM digits", 				2000,		0,			20000}},
	{"stsDigits",		{"stsDigits",		"stsNdigits",				";STS digits", 				1000,		0,			10000}},
	{"tof400Digits",	{"tof400Digits",	"tof400Ndigits",			";TOF-400 digits", 			400,		0,			400}},
	{"tof700Digits",	{"tof700Digits",	"tof700Ndigits",			";TOF-700 digits", 			400,		0,			400}},
	{"hodo_q",			{"hodo_q",			"hodoSumQ",					";Hodo Q^{2}", 				1500,		0,			15000}},
	{"scwall_q",		{"scwall_q",		"scwallSumQ",				";ScWall Q_{tot}", 			1000,		0,			1000}},
	{"fhcal&hodo_e",	{"fhcal&hodo_e",	"fhcalhodoSumE",			";E_{tot} (MeV)", 			1500,		0,			15000}},
	{"vtx_x",			{"vtx_x",			"vtxX",						";Vtx_{X} (mm)", 			500,		-10.,		10.}},
	{"vtx_y",			{"vtx_y",			"vtxY",						";Vtx_{Y} (mm)", 			500,		-10.,		10.}},
	{"vtx_z",			{"vtx_z",			"vtxZ",						";Vtx_{Z} (mm)", 			500,		-10.,		10.}},
	{"vtx_n_tracks",	{"vtx_n_tracks",    "vtxNtracks",			";N tracks for Vtx", 		500,		0,			500}},
	{"vtx_chi2",		{"vtx_chi2",    	"vtxChi2",				";Vtx #chi^{2}", 			1000,		0,			4000}},
	{"vtx_ndf",			{"vtx_ndf",    		"vtxNdf",				";Vtx ndf", 				1000,		0,			2000}},
	{"vtx_chi2_ndf",	{"vtx_chi2_ndf",    "vtx_chi2_ndf",			";Vtx #chi^{2}/NDF", 		400,		0,			40}},
	{"tof400_mult",	{"tof400_mult", 	"Tof400_multiplicity",	";TOF-400 multiplicity",	1000,		0,			1000}},
	{"tof700_mult",	{"tof700_mult", 	"Tof700_multiplicity",	";TOF-700 multiplicity",	1000,		0,			1000}},
	{"tof400Matched",	{"tof400Matched", 	"Tof400Matched",		";TOF-400 Matched",			100,		0,			100}},
	{"tof700Matched",	{"tof700Matched", 	"Tof700Matched",		";TOF-700 Matched",			100,		0,			100}},
	// Global Track variables
	{"tr_chi2_vtx",		{"tr_chi2_vtx", 	"trChi2vtx",			";#chi^{2}",				1000,		0,			4000}},
	{"tr_ndf",			{"tr_ndf", 			"trNdf",				";NDF",						100,		0,			50}},
	{"tr_chi2",			{"tr_chi2", 		"trChi2",				";#chi^{2}",				1000,		0,			1000}},
	{"tr_chi2_ndf",		{"tr_chi2_ndf", 	"tr_chi2_ndf",			";#chi^{2}/ndf",			2500,		0,			2500}},
	{"tr_nhits",		{"tr_nhits", 		"trNhits",				";N hits",					40,			0,			40}},
	{"tr_px",			{"tr_px", 			"px",					";p_{x} (GeV/c)",			1000,		-10,		10}},
	{"tr_py",			{"tr_py", 			"py",					";p_{y} (GeV/c)",			1000,		-10,		10}},
	{"tr_pz",			{"tr_pz", 			"pz",					";p_{z} (GeV/c)",			1000,		0,			40}},
	{"tr_pT",			{"tr_pT", 			"pT",					";p_{T} (GeV/c)",			1000,		0,			10}},
	{"tr_p",				{"tr_p", 			"p",					";p (GeV/c)",				2000,		-40,		40}},
	{"tr_eta",			{"tr_eta", 			"eta",					";#eta",					1000,		0,			10}},
	{"tr_phi",			{"tr_phi", 			"phi",					";#phi",					1000,		-5,			5}},
	{"tr_tof700_t",		{"tr_tof700_t",		"tof700hitT",			";T (ns)",					1000,		0,			100}},
	{"tr_tof400_t",		{"tr_tof400_t",		"tof400hitT",			";T (ns)",					1000,		0,			100}},
	{"tr_x",			{"tr_x",			"tr_x",					";x (cm)",					1000,		-40,		40}},
	{"tr_y",			{"tr_y",			"tr_y",					";y (cm)",					1000,		-40,		40}},
	{"tr_z",			{"tr_z",			"tr_z",					";z (cm)",					1000,		-40,		40}},
	{"tr_dca_x",		{"tr_dca_x",		"tr_dca_x",				";DCA_{x} (cm)",			800,		-20,		20}},
	{"tr_dca_y",		{"tr_dca_y",		"tr_dca_y",				";DCA_{y} (cm)",			800,		-20,		20}},
	{"tr_dca_z",		{"tr_dca_z",		"tr_dca_z",				";DCA_{z} (cm)",			800,		-20,		20}},
	{"tr_dca_r",		{"tr_dca_r",		"tr_dca_r",				";DCA_{R} (cm)",			800,		0,			40}},
	// mass2, beta, pid
	{"tr_rapidity",		{"tr_rapidity",		"trRapidity",			";y",						1000, 		-10., 		10.}},
	{"tr_pq",			{"tr_pq", 			"pq",					";p/q (GeV/c)",				1000, 		-10., 		10.}},
	{"tr_m2",			{"tr_m2", 			"trM2",					";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_m2_tof400",	{"tr_m2_tof400", 	"trM2Tof400",			";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_m2_tof700",	{"tr_m2_tof700", 	"trM2Tof700",			";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_beta_tof400",	{"tr_beta_tof400",	"trBetaTof400",			";#beta_{TOF-400}",			600, 		-1., 		5.}},
	{"tr_beta_tof700",	{"tr_beta_tof700",	"trBetaTof700",			";#beta_{TOF-700}",			600, 		-1., 		5.}},
	{"tr_1_beta_tof400",{"tr_1_beta_tof400","beta400",				";1/#beta_{TOF-400}",		600, 		0., 		3.}},
	{"tr_1_beta_tof700",{"tr_1_beta_tof700","beta700",				";1/#beta_{TOF-700}",		600, 		0., 		3.}},
	{"tr_m2_tof400_2212",{"tr_m2_tof400_2212","trM2Tof400_2212",	";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_m2_tof700_2212",{"tr_m2_tof700_2212","trM2Tof700_2212",	";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_m2_tof400_211",{"tr_m2_tof400_211","trM2Tof400_211",		";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_m2_tof700_211",{"tr_m2_tof700_211","trM2Tof700_211",		";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_rapidity_2212",{"tr_rapidity_2212","trProtonY",			";y",						500, 		-5., 		5.}},
	// STS track variables
	{"sts_x",			{"sts_x",			"sts_x",				";x (cm)",					1000,		-40.,		40.}},
	{"sts_y",			{"sts_y",			"sts_y",				";y (cm)",					1000,		-40.,		40.}},
	{"sts_z",			{"sts_z",			"sts_z",				";z (cm)",					1000,		0.,			200.}},
	{"sts_chi2_ndf",	{"sts_chi2_ndf", 	"stsTrackChi2Ndf",		";#chi^{2}/NDF",			1000,		0,			4000}},
	{"sts_nhits",		{"sts_nhits", 		"stsTrackNhits",		";N hits",					40,			0,			40}},
	{"sts_px",			{"sts_px", 			"sts_px",				";p_{x} (GeV/c)",			1000,		-10,		10}},
	{"sts_py",			{"sts_py", 			"sts_py",				";p_{y} (GeV/c)",			1000,		-10,		10}},
	{"sts_pz",			{"sts_pz", 			"sts_pz",				";p_{z} (GeV/c)",			1000,		0,			40}},
	{"sts_pT",			{"sts_pT", 			"sts_pT",				";p_{T} (GeV/c)",			1000,		0,			10}},
	{"sts_p",			{"sts_p", 			"sts_p",				";p (GeV/c)",				1000,		0,			40}},
	{"sts_eta",			{"sts_eta", 		"sts_eta",				";#eta",					1000,		0,			10}},
	{"sts_phi",			{"sts_phi", 		"sts_phi",				";#phi",					1000,		-5,			5}},
	// fhcall 
	{"fhcal_ModId",		{"fhcal_ModId", 	"fhcalModId",			";Mod Id",					60,			0,			60}},
	{"fhcal_ModId_e",	{"fhcal_ModId_e", 	"fhcalModE",			";E (MeV)",					500,		0,			500}},
	{"fhcal_e",			{"fhcal_e", 		"fhcalSumE",			";E_{tot} (MeV)",			1500,		0,			15000}},
	{"fhcal_pos_x",		{"fhcal_pos_x", 	"fhcal_x",				";x (cm)",					100,		-100,		100}},
	{"fhcal_pos_y",		{"fhcal_pos_y", 	"fhcal_y",				";y (cm)",					100,		-100,		100}},
	// beam pipe
	{"beam_hitMult_st0",{"beam_hitMult_st0","beamHitX_St0_mult",	";count",					40,			 0,			40}},
	{"beam_hitMult_st1",{"beam_hitMult_st1","beamHitX_St1_mult",	";count",					40,			 0,			40}},
	{"beam_hitMult_st2",{"beam_hitMult_st2","beamHitX_St2_mult",	";count",					40,			 0,			40}},
	{"beam_hit_x_st0",	{"beam_hit_x_st0", 	"beamHitX_St0",			";x (cm)",					200,		-5,			5}},
	{"beam_hit_y_st0",	{"beam_hit_y_st0", 	"beamHitY_St0",			";y (cm)",					200,		-5,			5}},
	{"beam_hit_x_st1",	{"beam_hit_x_st1", 	"beamHitX_St1",			";x (cm)",					200,		-5,			5}},
	{"beam_hit_y_st1",	{"beam_hit_y_st1", 	"beamHitY_St1",			";y (cm)",					200,		-5,			5}},
	{"beam_hit_x_st2",	{"beam_hit_x_st2", 	"beamHitX_St2",			";x (cm)",					200,		-5,			5}},
	{"beam_hit_y_st2",	{"beam_hit_y_st2", 	"beamHitY_St2",			";y (cm)",					200,		-5,			5}},
	{"beam_x",			{"beam_x", 			"beam_x",				";x (cm)",					200,		-5,			5}},
	{"beam_y",			{"beam_y", 			"beam_y",				";y (cm)",					200,		-5,			5}}
};

const std::vector<std::vector< std::string >> QA_MultDemenPlot{
	{"tr_nhits", 	"Weight:trIsProton"},
	{"bc1sInt",		"fdInt"},
	{"bc1sIntNorm",	"fdIntNorm"},
	{"bc1sAmp",		"fdAmp"},
	{"nTracks",		"bc1sAmp"},
	{"nTracks",		"bc1sInt"},
	{"nTracks",		"fdAmp"},
	{"nTracks",		"fdInt"},
	{"tof400_mult", "tof400Matched"},
	{"tof700_mult", "tof700Matched"},
	{"tof700_mult", "tof400_mult"},
	{"tof700Matched","tof400Matched"},
	{"nTracks",		"tof400Matched"},
	{"nTracks",		"tof700Matched"},
	{"nTracks",		"tof400Digits"},
	{"nTracks",		"tof700Digits"},
	{"nTracks",		"stsDigits"},
	{"nTracks",		"gemDigits"},
	{"nTracks",		"beam_hit_x_st2"},
	{"nTracks",		"beam_hit_y_st2"},

	{"vtx_x",		"vtx_y"},
	{"tr_x",		"tr_y"},
	{"tr_px",		"tr_py"},
	{"tr_eta",		"tr_phi"},
	{"tr_phi",		"tr_eta"},
	{"tr_pT",		"tr_eta"},
	{"tr_pT",		"tr_phi"},
	{"sts_x",		"sts_y"},
	{"nTracks",		"fhcal_e"},
	{"fhcal_ModId",	"fhcal_ModId_e"},
	{"beam_hit_x_st0","beam_hit_y_st0"},
	{"beam_hit_x_st1","beam_hit_y_st1"},
	{"beam_hit_x_st2","beam_hit_y_st2"},
	{"beam_x",		"beam_y"},

	{"RunId", 		"fhcal_ModId",		"fhcal_ModId_e"},
	{"RunId", 		"beam_x",			"beam_y"},
	{"RunId", 		"beam_hit_x_st0",	"beam_hit_y_st0"},
	{"RunId", 		"beam_hit_x_st1",	"beam_hit_y_st1"},
	{"RunId", 		"beam_hit_x_st2",	"beam_hit_y_st2"},
	
	{"tr_pq",		"tr_m2"},
	{"tr_pq",		"tr_m2_tof400"},
	{"tr_pq",		"tr_m2_tof700"},
	{"tr_pq",		"tr_m2_tof400_211"},
	{"tr_pq",		"tr_m2_tof700_211"},
	{"tr_pq",		"tr_m2_tof400_2212"},
	{"tr_pq",		"tr_m2_tof700_2212"},
	{"tr_pq",		"tr_1_beta_tof400"},
	{"tr_pq",		"tr_1_beta_tof700"},
	{"tr_pq",		"tr_beta_tof400"},
	{"tr_pq",		"tr_beta_tof700"},

	{"tr_rapidity",		"tr_pq"},
	{"tr_rapidity",		"tr_pq", 	"Weight:trIsProton"},
	{"tr_rapidity",		"tr_pT"},
	{"tr_rapidity",		"tr_pT", 	"Weight:trIsProton"},
	{"tr_pq",			"tr_m2"},
	{"tr_pq",			"tr_m2", 	"Weight:trIsProton"},

	{"RunId",		"bc1sInt"},
	{"RunId",		"bc1sIntNorm"},
	{"RunId",		"bc1sAmp"},
	{"RunId",		"fdInt"},
	{"RunId",		"fdIntNorm"},
	{"RunId",		"fdAmp"},
	{"RunId",		"vcsInt"},
	{"RunId",		"vcsAmp"},
	{"RunId",		"nTracks"},
	{"RunId",		"nStsTracks"},
	{"RunId",		"bdMult"},
	{"RunId",		"bdModId"},
	{"RunId",		"bdModAmp"},
	{"RunId",		"bdNum"},
	{"RunId",		"bdNumSingle"},
	{"RunId",		"nbdMod"},
	{"RunId",		"nbdModSingle"},
	{"RunId",		"stsDigits"},
	{"RunId",		"gemDigits"},
	
	{"RunId",		"tof400Digits"},
	{"RunId",		"tof700Digits"},
	
	{"RunId",		"fhcal_e"},
	{"RunId",		"fhcal_ModId"},
	{"RunId",		"fhcal_ModId_e"},
	{"RunId",		"fhcal_pos_x"},
	{"RunId",		"fhcal_pos_y"},

	{"RunId",		"hodo_q"},
	{"RunId",		"scwall_q"},
	{"RunId",		"vtx_x"},
	{"RunId",		"vtx_y"},
	{"RunId",		"vtx_z"},
	{"RunId",		"vtx_n_tracks"},
	{"RunId",		"vtx_chi2"},
	{"RunId",		"vtx_ndf"},
	{"RunId",		"vtx_chi2_ndf"},
	{"RunId",		"tof400_mult"},
	{"RunId",		"tof700_mult"},
	{"RunId",		"tof400Matched"},
	{"RunId",		"tof700Matched"},

	{"RunId",		"tr_ndf"},
	{"RunId",		"tr_chi2"},
	{"RunId",		"tr_chi2_ndf"},
	{"RunId",		"tr_nhits"},
	{"RunId",		"tr_px"},
	{"RunId",		"tr_py"},
	{"RunId",		"tr_pz"},
	{"RunId",		"tr_p"},
	{"RunId",		"tr_pT"},
	{"RunId",		"tr_eta"},
	{"RunId",		"tr_phi"},
	{"RunId",		"tr_x"},
	{"RunId",		"tr_y"},
	{"RunId",		"tr_z"},
	{"RunId",		"tr_dca_x"},
	{"RunId",		"tr_dca_y"},
	{"RunId",		"tr_dca_z"},
	{"RunId",		"tr_dca_r"},
	{"RunId",		"tr_tof400_t"},
	{"RunId",		"tr_tof700_t"},
	{"RunId",		"sts_chi2_ndf"},
	{"RunId",		"sts_nhits"},
	{"RunId",		"sts_px"},
	{"RunId",		"sts_py"},
	{"RunId",		"sts_pz"},
	{"RunId",		"sts_p"},
	{"RunId",		"sts_pT"},
	{"RunId",		"sts_eta"},
	{"RunId",		"sts_phi"},
	{"RunId",		"sts_x"},
	{"RunId",		"sts_y"},
	{"RunId",		"sts_z"},

	{"RunId",		"beam_hit_x_st0"},
	{"RunId",		"beam_hit_y_st0"},
	{"RunId",		"beam_hit_x_st1"},
	{"RunId",		"beam_hit_y_st1"},
	{"RunId",		"beam_hit_x_st2"},
	{"RunId",		"beam_hit_y_st2"},
	{"RunId",		"beam_x"},
	{"RunId",		"beam_y"},
	{"RunId",		"beam_hitMult_st0"},
	{"RunId",		"beam_hitMult_st1"},
	{"RunId",		"beam_hitMult_st2"}

};


float NewXRotate(float x, float y, float phi){
	//std::cout<<"x\t"<<x<<"\t"<<y<<"\t"<<phi<<"\t"<<x * TMath::Cos(phi) - y * TMath::Sin(phi)<<std::endl;
	return x * TMath::Cos(phi) - y * TMath::Sin(phi);
}
float NewYRotate(float x, float y, float phi){
	//std::cout<<"y\t"<<x<<"\t"<<y<<"\t"<<phi<<"\t"<<x * TMath::Sin(phi) + y * TMath::Cos(phi)<<std::endl;
	return x * TMath::Sin(phi) + y * TMath::Cos(phi);
}

const ROOT::RDF::TProfile1DModel SetProfile1Dobj(std::string _prefix, const HistoParameter &_par, const HistoParameter &_par_fill){
	return { (_prefix + "pr1_" + _par.fName + "_" + _par_fill.fName).c_str(),_par.fAxisTitle.c_str(), _par.fNBins, _par.fMin, _par.fMax };
}

const ROOT::RDF::TH1DModel SetH1obj(std::string _prefix, const HistoParameter &_par){
	return { (_prefix + "h1_" + _par.fName).c_str(),_par.fAxisTitle.c_str(), _par.fNBins, _par.fMin, _par.fMax };
}

const ROOT::RDF::TH2DModel SetH2obj(std::string _prefix, const HistoParameter &_parX, const HistoParameter &_parY){
	return { (_prefix+"h2_" + _parX.fName + "_" + _parY.fName).c_str(), ( _parX.fAxisTitle + _parY.fAxisTitle).c_str(), _parX.fNBins, _parX.fMin, _parX.fMax, _parY.fNBins, _parY.fMin, _parY.fMax };
}

const ROOT::RDF::TH3DModel SetH3obj(std::string _prefix, const HistoParameter &_parX, const HistoParameter &_parY, const HistoParameter &_parZ){
	return { (_prefix+"h3_" + _parX.fName + "_" + _parY.fName + "_" + _parZ.fName).c_str(), ( _parX.fAxisTitle + _parY.fAxisTitle + _parZ.fAxisTitle).c_str(), _parX.fNBins, _parX.fMin, _parX.fMax, _parY.fNBins, _parY.fMin, _parY.fMax, _parZ.fNBins, _parZ.fMin, _parZ.fMax };
}

template<typename T>
void WriteInFile(T& vector, TFile *file){
	file->cd();
	std::for_each( vector.begin(), vector.end(), []( auto h1 ){ h1->Write(); } );
}

template<typename T>
void AddStandartHisto(T& dd, std::string prefix, TFile *file){
	
	std::vector<ROOT::RDF::RResultPtr<::TH1D>> histo1;
	std::vector<ROOT::RDF::RResultPtr<::TH2D>> histo2;
	std::vector<ROOT::RDF::RResultPtr<::TH3D>> histo3;
	std::vector<ROOT::RDF::RResultPtr<::TProfile>> prof1;

	std::cout<<prefix<<std::endl;

	// Fill TH1D histo all QA_variable
	for(auto qa_obj : QA_variable){
		//std::cout<<qa_obj.first<<"\n";
		histo1.push_back( dd.Histo1D( SetH1obj(prefix,qa_obj.second) , qa_obj.second.fFillObj) );
	}

	// Fill TH1D, TH2D, TH3D and TProfile1D with and without weight
	for(auto qa_obj : QA_MultDemenPlot){
		
		// Fill TH1D,TH2D,TH3D without weight 
		if(strncmp(qa_obj.back().c_str(),"Weight:",7) != 0){
			switch( (int)qa_obj.size() ){
				case 2:{
					histo2.push_back( dd.Histo2D( SetH2obj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1])), 
						QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj));
					break;
				}
				case 3:{
					histo3.push_back( dd.Histo3D( SetH3obj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1]),QA_variable.at(qa_obj[2])), 
						QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj, QA_variable.at(qa_obj[2]).fFillObj));
					break;
				}
			}
		}
		
		// Fill TH1D,TH2D,TH3D with weight
		if(strncmp(qa_obj.back().c_str(),"Weight:",7) == 0){
			
			std::string _fw = qa_obj.back();
			_fw.erase(_fw.begin(), _fw.begin()+7);
			
			switch( (int)qa_obj.size() ){
				case 2:{
					histo1.push_back( dd.Histo1D( SetH1obj(prefix,QA_variable.at(qa_obj[0])), QA_variable.at(qa_obj[0]).fFillObj, _fw) );
					histo1.back()->SetName(Form("%s_W=%s",histo1.back()->GetName(),_fw.c_str()));
					break;
				}
				case 3:{
					histo2.push_back( dd.Histo2D( SetH2obj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1])), 
						QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj, _fw));
					histo2.back()->SetName(Form("%s_W=%s",histo2.back()->GetName(),_fw.c_str()));
					break;
				}
				case 4:{
					histo3.push_back( dd.Histo3D( SetH3obj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1]),QA_variable.at(qa_obj[2])), 
						QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj, QA_variable.at(qa_obj[2]).fFillObj,_fw));
					histo3.back()->SetName(Form("%s_W=%s",histo3.back()->GetName(),_fw.c_str()));
					break;
				}
			}
		}

		//Fill TProfile1D
		if(qa_obj.size()==2 && qa_obj[0]=="RunId"){
			prof1.push_back( dd.Profile1D( SetProfile1Dobj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1])),
				QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj));
		}
	}
	
	WriteInFile(histo1,file);
	WriteInFile(histo2,file);
	WriteInFile(histo3,file);
	WriteInFile(prof1,file);

}

// Fill slice M2 
template<typename T>
void AddHistoM2vsRunId(T& dd, std::string prefix, std::string NameM2, std::string pqCharge, TFile *file){
	
	std::vector<ROOT::RDF::RResultPtr<::TH2D>> histo2;
	std::vector<std::string> NameFillPar;
	
	std::cout<<prefix<<"\t"<<NameM2<<std::endl;

	double fP_start		= 0.1;
	double fP_finish	= 5.1;
	double fP_step  	= 0.2;
	int    fP_nbins		= (int)(abs( fP_finish - fP_start)/fP_step);
	string fChName = "P";
	if(pqCharge=="pq<0") fChName = "M";

	auto ddM2 = dd.Define("cut_pAll", Form("abs(p)>%f && abs(p)<%f && %s && tr_dca_r<5",fP_start,fP_finish,pqCharge.c_str()))
   	  					.Define(Form("%s_cut_pAll",NameM2.c_str()),Form("%s[cut_pAll]",NameM2.c_str()));

	for(int ip=0; ip<fP_nbins; ip++){
		
		double fPmin = fP_start + fP_step * ip;
		double fPmax = fP_start + fP_step * (ip+1);
		std::string fCut  			= Form("cut_p%i",ip);
		std::string fCutFormul		= Form("abs(p)>%f && abs(p)<%f && %s && tr_dca_r<5",fPmin,fPmax, pqCharge.c_str());
		std::string fM2WithCut 		= Form("%s_p%i_%s",	NameM2.c_str(),ip,fChName.c_str());
		std::string fM2WithCutFormul= Form("%s[%s]",	NameM2.c_str(),fCut.c_str());
		//std::cout<<fCut<<"\t"<<fCutFormul<<"\t"<<fM2WithCut<<"\t"<<fM2WithCutFormul<<"\n";

		ddM2 = ddM2.Define(fCut, fCutFormul)
				   .Define(fM2WithCut, fM2WithCutFormul);
   	  	NameFillPar.push_back(fM2WithCut);
	}

	histo2.push_back(ddM2.Histo2D({Form("%sh2_RunId_m2%s_pAll_%s",prefix.c_str(), NameM2.c_str(), fChName.c_str()),";run ID;m^{2} (GeV^{2}/c^{4})", RunIdBins, RunIdMin, RunIdMax, 750, -5., 10.}, "runId", Form("%s_cut_pAll",NameM2.c_str())) );
	for(int ip=0; ip<fP_nbins; ip++){
		histo2.push_back( ddM2.Histo2D({Form("%sh2_RunId_tr_m2_%s_p%i_%s",prefix.c_str(), NameM2.c_str(), ip, fChName.c_str()), 
											  ";run ID;m^{2} (GeV^{2}/c^{4})", RunIdBins, RunIdMin, RunIdMax, 750, -5., 10.}, 
											  "runId", NameFillPar[ip]) );
	}

	WriteInFile(histo2,file);

}

void RemoveBadRuns(std::vector<int> &_Runs, std::vector<int> _BadRUns){
	for(auto _run : _BadRUns){
		_Runs.erase(remove(_Runs.begin(), _Runs.end(), _run), _Runs.end());
	}
}

void run8_qa_new( std::string str_in_list, std::string out_file_name="qa.root", std::string in_fit_file="inFit.root"){
	
	TStopwatch timer1;
  	timer1.Start();

	// In git File
	auto file_fit = TFile::Open( in_fit_file.c_str(), "READ" );
	file_fit->cd();
	TGraphErrors *f1_FitBC1 = (TGraphErrors*)file_fit->Get("gr_cut1_h2_RunId_bc1sInt_1");
	TGraphErrors *f1_FitFD  = (TGraphErrors*)file_fit->Get("gr_cut1_h2_RunId_bdInt_1");

	TFileCollection collection( "collection", "", str_in_list.c_str() );
    auto* chain = new TChain( "t" );
    chain->AddFileInfoList( collection.GetList() );
    ROOT::RDataFrame d( *chain );

    float angleR = -1.*TMath::ATan2(700.,50.);
    std::cout<<angleR<<std::endl;

    const float PROTON_M = 0.93827208;
    const float PION_M = 0.13957061;
    const float Y_CM = 1.15141;

    auto f1_m2_400 = new TF1( "m2_p_400", "pol2", 0, 10 );
    f1_m2_400->SetParameter( 0, 0.965499 );
    f1_m2_400->SetParameter( 1, -0.0625193 );
    f1_m2_400->SetParameter( 2, -0.0217673 );

	auto f1_s_400 = new TF1( "s_p_400", "pol3", 0, 10 );
    f1_s_400->SetParameter( 0, 0.220837 );
    f1_s_400->SetParameter( 1, -0.214113 );
    f1_s_400->SetParameter( 2, 0.161722 );
    f1_s_400->SetParameter( 3, -0.0251886 );
    
    auto f1_m2_700 = new TF1( "m2_p_700", "pol4", 0, 10 );
    f1_m2_700->SetParameter( 0, 1.0847 );
    f1_m2_700->SetParameter( 1, -0.330513 );
    f1_m2_700->SetParameter( 2, 0.220286 );
    f1_m2_700->SetParameter( 3, -0.064973 );
    f1_m2_700->SetParameter( 4, 0.00705849 );

    auto f1_s_700 = new TF1( "s_p_700", "pol3", 0, 10 );
    f1_s_700->SetParameter( 0,  0.102933 );
    f1_s_700->SetParameter( 1,  -0.115384 );
    f1_s_700->SetParameter( 2,  0.088186 );
    f1_s_700->SetParameter( 3,  -0.0115386 );


	std::vector<int> physical_runs{6667, 6668, 6669, 6670, 6671, 6672, 6673, 6674, 6675, 6676, 6677, 6678, 6679, 6680, 6681, 6683, 6684, 6685, 6686, 6687, 6689, 6690, 6691, 6692, 6694, 6695, 6696, 6698, 6699, 6732, 6733, 6734, 6737, 6738, 6739, 6740, 6745, 6752, 6753, 6760, 6761, 6765, 6766, 6767, 6768, 6769, 6771, 6772, 6773, 6774, 6779, 6780, 6782, 6783, 6785, 6786, 6788, 6794, 6795, 6797, 6799, 6800, 6803, 6815, 6816, 6817, 6818, 6819, 6820, 6821, 6822, 6879, 6882, 6883, 6884, 6886, 6887, 6889, 6891, 6900, 6901, 6902, 6903, 6904, 6905, 6906, 6907, 6908, 6909, 6910, 6911, 6915, 6916, 6918, 6919, 6920, 6921, 6923, 6924, 6926, 6927, 6928, 6929, 6930, 6931, 6932, 6933, 6934, 6935, 6936, 6937, 6939, 6940, 6968, 6970, 6972, 6973, 6975, 6976, 6977, 6978, 6979, 6980, 6981, 6982, 6983, 6984, 6990, 6991, 6992, 6993, 6994, 6995, 6997, 6998, 6999, 7000, 7002, 7003, 7004, 7005, 7006, 7008, 7009, 7010, 7011, 7012, 7030, 7031, 7032, 7033, 7034, 7035, 7037, 7038, 7040, 7041, 7042, 7043, 7044, 7046, 7047, 7048, 7049, 7050, 7051, 7052, 7053, 7054, 7055, 7056, 7075, 7076, 7077, 7078, 7081, 7082, 7083, 7084, 7086, 7087, 7091, 7092, 7093, 7094, 7096, 7097, 7098, 7100, 7101, 7102, 7103, 7104, 7125, 7126, 7127, 7128, 7129, 7130, 7131, 7132, 7133, 7135, 7136, 7137, 7138, 7146, 7149, 7150, 7151, 7154, 7155, 7156, 7157, 7159, 7160, 7161, 7162, 7163, 7164, 7165, 7166, 7167, 7168, 7173, 7174, 7175, 7176, 7177, 7178, 7179, 7180, 7181, 7182, 7184, 7186, 7187, 7188, 7191, 7192, 7193, 7194, 7195, 7200, 7202, 7203, 7205, 7206, 7207, 7208, 7209, 7211, 7212, 7213, 7214, 7215, 7216, 7217, 7218, 7219, 7220, 7223, 7225, 7255, 7258, 7261, 7263, 7265, 7267, 7268, 7269, 7271, 7272, 7274, 7276, 7278, 7279, 7281, 7284, 7286, 7288, 7290, 7291, 7312, 7313, 7320, 7321, 7322, 7323, 7325, 7326, 7327, 7328, 7337, 7342, 7343, 7344, 7345, 7346, 7348, 7349, 7351, 7352, 7353, 7354, 7355, 7356, 7357, 7358, 7359, 7361, 7363, 7364, 7365, 7367, 7369, 7374, 7376, 7377, 7378, 7379, 7380, 7381, 7382, 7386, 7387, 7388, 7389, 7390, 7391, 7392, 7393, 7395, 7396, 7397, 7398, 7399, 7400, 7401, 7402, 7403, 7405, 7406, 7408, 7409, 7410, 7411, 7412, 7413, 7414, 7415, 7417, 7418, 7419, 7421, 7422, 7423, 7425, 7427, 7428, 7429, 7431, 7432, 7433, 7434, 7435, 7437, 7439, 7440, 7441, 7442, 7444, 7445, 7446, 7447, 7449, 7451, 7452, 7453, 7454, 7455, 7456, 7457, 7458, 7460, 7461, 7469, 7471, 7472, 7473, 7474, 7477, 7478, 7480, 7481, 7482, 7483, 7484, 7487, 7488, 7489, 7490, 7491, 7492, 7493, 7495, 7497, 7498, 7500, 7501, 7502, 7513, 7514, 7515, 7517, 7519, 7520, 7521, 7528, 7529, 7530, 7531, 7532, 7533, 7534, 7537, 7538, 7539, 7542, 7543, 7545, 7546, 7547, 7549, 7550, 7551, 7552, 7553, 7554, 7564, 7565, 7566, 7567, 7569, 7570, 7572, 7573, 7574, 7575, 7577, 7579, 7581, 7584, 7585, 7586, 7587, 7590, 7591, 7592, 7596, 7597, 7599, 7600, 7604, 7605, 7606, 7607, 7608, 7609, 7611, 7612, 7613, 7622, 7623, 7625, 7626, 7627, 7628, 7630, 7631, 7633, 7634, 7635, 7636, 7638, 7639, 7640, 7641, 7643, 7644, 7645, 7646, 7647, 7649, 7655, 7656, 7657, 7659, 7660, 7662, 7663, 7664, 7665, 7666, 7668, 7669, 7670, 7671, 7673, 7674, 7675, 7676, 7677, 7678, 7679, 7681, 7682, 7684, 7685, 7687, 7688, 7689, 7690, 7692, 7693, 7694, 7696, 7698, 7700, 7701, 7702, 7703, 7704, 7705, 7710, 7712, 7713, 7714, 7715, 7716, 7717, 7718, 7721, 7723, 7724, 7725, 7726, 7727, 7728, 7729, 7730, 7732, 7733, 7734, 7735, 7736, 7737, 7751, 7752, 7753, 7755, 7756, 7761, 7762, 7763, 7764, 7766, 7767, 7768, 7769, 7771, 7772, 7775, 7776, 7778, 7779, 7780, 7781, 7783, 7784, 7785, 7786, 7788, 7789, 7790, 7791, 7794, 7795, 7796, 7797, 7798, 7801, 7802, 7803, 7814, 7816, 7819, 7821, 7824, 7825, 7828, 7829, 7830, 7831, 7832, 7834, 7835, 7836, 7842, 7843, 7845, 7846, 7847, 7848, 7850, 7851, 7852, 7853, 7855, 7856, 7857, 7858, 7859, 7865, 7868, 7869, 7870, 7871, 7873, 7874, 7876, 7877, 7878, 7880, 7882, 7883, 7884, 7885, 7886, 7887, 7890, 7891, 7892, 7893, 7894, 7896, 7897, 7898, 7899, 7900, 7901, 7903, 7904, 7905, 7906, 7907, 7908, 7910, 7911, 7912, 7913, 7914, 7931, 7932, 7933, 7935, 7937, 7938, 7939, 7941, 7942, 7944, 7948, 7949, 7950, 7952, 7954, 7955, 7957, 7958, 7960, 7961, 7962, 7963, 7965, 7966, 7967, 7975, 7977, 7978, 7979, 7981, 7982, 7986, 7988, 7989, 7990, 7991, 7992, 7995, 7996, 7997, 7998, 7999, 8000, 8001, 8002, 8004, 8005, 8006, 8007, 8008, 8009, 8013, 8014, 8015, 8016, 8018, 8020, 8021, 8022, 8023, 8026, 8027, 8028, 8029, 8030, 8031, 8032, 8033, 8038, 8039, 8040, 8041, 8042, 8044, 8045, 8046, 8047, 8048, 8050, 8051, 8052, 8053, 8055, 8056, 8057, 8058, 8059, 8061, 8063, 8064, 8065, 8066, 8068, 8069, 8070, 8071, 8072, 8074, 8075, 8076, 8077, 8079, 8080, 8081, 8082, 8084, 8086, 8087, 8088, 8089, 8090, 8097, 8100, 8101, 8102, 8104, 8106, 8108, 8109, 8110, 8111, 8112, 8113, 8115, 8116, 8117, 8118, 8119, 8121, 8122, 8123, 8124, 8129, 8130, 8131, 8133, 8137, 8138, 8139, 8140, 8141, 8142, 8144, 8156, 8157, 8158, 8159, 8160, 8161, 8162, 8165, 8166, 8167, 8168, 8169, 8170, 8173, 8174, 8175, 8176, 8177, 8180, 8183, 8184, 8186, 8188, 8190, 8191, 8192, 8193, 8195, 8196, 8198, 8199, 8201, 8202, 8203, 8204, 8205, 8206, 8207, 8208, 8209, 8210, 8211, 8212, 8213, 8215, 8217, 8219, 8220, 8221, 8228, 8229, 8230, 8231, 8235, 8236, 8238, 8239, 8240, 8242, 8244, 8245, 8246, 8247, 8248, 8250, 8251, 8253, 8254, 8255, 8256, 8257, 8258, 8265, 8266, 8267, 8268, 8270, 8271, 8273, 8274, 8275, 8276, 8277, 8278, 8279, 8281, 8284, 8286, 8287, 8288, 8289, 8290, 8292, 8293, 8294, 8295, 8297, 8298, 8299, 8300, 8305, 8306};
    std::vector<int> badFd_runs{7075, 7076, 7077, 7078, 7081, 7083, 7084, 7086, 7087, 7091, 7092, 7093, 7096, 7097, 7098, 7100, 7101, 7102, 7103, 7104};

	auto dd = d
		//.Range(1000,1005)
		//.Filter("runId < 8312")
		//.Filter([&physical_runs]( UInt_t run_id ){ 
		//	return std::find( physical_runs.begin(), physical_runs.end(), run_id) != physical_runs.end();}, 
		//	{"runId"} )
		.Define( "track_multiplicity", "return trMom.size();" )
		.Define( "sts_track_multiplicity", "return stsTrackMomentum.size();" )
		.Define( "vtx_chi2_ndf", "return vtxChi2/vtxNdf;" )
      .Define( "gemNdigits","return gemDigits.size()" )
      .Define( "stsNdigits","return stsDigits.size()" )
      .Define( "tof400Ndigits","return tof400Digits.size()" )
      .Define( "tof700Ndigits","return tof700Digits.size()" )
      .Define( "Tof400_multiplicity", "return trBetaTof400.size()" )
		.Define( "Tof700_multiplicity", "return trBetaTof700.size()" )
		//.Define( "track_multiplicity_new", [angleR](unsigned long x, unsigned long y){ return NewXRotate((float)x,(float)y,(float)angleR);},{"track_multiplicity","stsNdigits"} )
		//.Define( "stsNdigitsNew", 		   [angleR](unsigned long x, unsigned long y){ return NewYRotate((float)x,(float)y,(float)angleR);},{"track_multiplicity","stsNdigits"} )
      // Triggers
      .Define( "bc1sTdcHits", " return bc1sTdcTimes.size(); " )
      .Define( "bc1tTdcHits", " return bc1tTdcTimes.size(); " )
      .Define( "bc1bTdcHits", " return bc1bTdcTimes.size(); " )
      .Define( "vcsTdcHits", 	" return vcsTdcTimes.size(); " )
      .Define( "fdTdcHits", 	" return fdTdcTimes.size(); " )
      // BC1S
      .Define( "bc1sIntegralNorm",[f1_FitBC1](float _bc1, UInt_t _runId){ return _bc1 / f1_FitBC1->Eval((double)_runId);},{"bc1sIntegral","runId"} )
      .Define( "fdIntegralNorm",	[f1_FitFD] (float _fd,  UInt_t _runId){ return _fd  / f1_FitFD ->Eval((double)_runId);},{"fdIntegral",	"runId"} )
      //BD
      .Define( "bd_num", 		  " return bdModAmp.size(); " )
		.Define( "nbdMod", []( ROOT::RVecD vec_bdModAmp, ROOT::VecOps::RVec<short> vec_bdModId ){
				ROOT::RVecD vec_nbdMod{};
				vec_nbdMod.reserve( vec_bdModAmp.size() );
				for( int i=0; i<vec_bdModAmp.size(); ++i ){
					int mod = vec_bdModId.at(i);
					vec_nbdMod.push_back( mod );
				}
				return vec_nbdMod;
			}, {"bdModAmp","bdModId"} )
		.Define( "nbdModSingle", []( ROOT::RVecD vec_bdModAmp, ROOT::VecOps::RVec<short> vec_bdModId ){
				ROOT::RVecD vec_nbdMod{};
      			int n[70]={0};
				vec_nbdMod.reserve( vec_bdModAmp.size() );
				for( int i=0; i<vec_bdModAmp.size(); ++i ){
					int mod = vec_bdModId.at(i);
					n[mod]++;
				}
				for(int i=0; i<70; i++){
					if(n[i]>0)vec_nbdMod.push_back(i);
				}
				return vec_nbdMod;
			}, {"bdModAmp","bdModId"} )
		.Define( "bd_numSingle", []( ROOT::RVecD vec_bdModAmp, ROOT::VecOps::RVec<short> vec_bdModId ){
				ROOT::RVecD vec_nbdMod{};
      			int n[70]={0};
				vec_nbdMod.reserve( vec_bdModAmp.size() );
				for( int i=0; i<vec_bdModAmp.size(); ++i ){
					int mod = vec_bdModId.at(i);
					n[mod]++;
				}
				int nbd1 = 0;
				for(int i=0; i<70; i++){
					if(n[i]>0)nbd1++;;
				}
				return nbd1;
			}, {"bdModAmp","bdModId"} )
		//BeamHit and track
		.Define( "beamHitX_St0_mult"," int hit{}; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==0) hit=hit+1; } return hit;" )
		.Define( "beamHitX_St1_mult"," int hit{}; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==1) hit=hit+1; } return hit;" )
		.Define( "beamHitX_St2_mult"," int hit{}; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==2) hit=hit+1; } return hit;" )
		.Define( "beamHitX_St0", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==0) hit.push_back( beamHitXYZ.at(i).X() ); } return hit;" )
		.Define( "beamHitY_St0", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==0) hit.push_back( beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beamHitX_St1", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==1) hit.push_back( cos(TMath::Pi()/6.)*beamHitXYZ.at(i).X() - sin(TMath::Pi()/6.)*beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beamHitY_St1", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==1) hit.push_back( sin(TMath::Pi()/6.)*beamHitXYZ.at(i).X() + cos(TMath::Pi()/6.)*beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beamHitX_St2", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==2) hit.push_back( cos(TMath::Pi()/3.)*beamHitXYZ.at(i).X() - sin(TMath::Pi()/3.)*beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beamHitY_St2", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==2) hit.push_back( sin(TMath::Pi()/3.)*beamHitXYZ.at(i).X() + cos(TMath::Pi()/3.)*beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beam_x", " ROOT::RVecF beam_x; for( auto bm_tr : beamTrackParameters ){ beam_x.push_back( bm_tr.at(0) ); } return beam_x; " )
		.Define( "beam_y", " ROOT::RVecF beam_y; for( auto bm_tr : beamTrackParameters ){ beam_y.push_back( bm_tr.at(1) ); } return beam_y; " )
		// Global Tracks
		.Define( "p",  " ROOT::RVecF p;   for( auto mom : trMom ){ p.push_back( mom.P() ); } 	 return p; " )
		.Define( "px", " ROOT::RVecF px;  for( auto mom : trMom ){ px.push_back( mom.Px() ); } 	 return px; " )
		.Define( "py", " ROOT::RVecF py;  for( auto mom : trMom ){ py.push_back( mom.Py() ); } 	 return py; " )
		.Define( "pz", " ROOT::RVecF pz;  for( auto mom : trMom ){ pz.push_back( mom.Pz() ); } 	 return pz; " )
		.Define( "pT", " ROOT::RVecF pT;  for( auto mom : trMom ){ pT.push_back( mom.Pt() ); } 	 return pT; " )
		.Define( "phi"," ROOT::RVecF phi; for( auto mom : trMom ){ phi.push_back( mom.Phi() ); } return phi; " )
		.Define( "eta"," ROOT::RVecF eta; for( auto mom : trMom ){ eta.push_back( mom.Eta() ); } return eta; " )
		.Define( "pq", " ROOT::RVecF pq;  for( int i=0; i<trMom.size(); i++ ){ pq.push_back( trMom.at(i).P()/trCharge.at(i) ); } return pq;" )
		.Define( "Tof400Matched", "int Ntr{}; for( int i=0; i<trMom.size(); i++ ){ if(trBetaTof400.at(i) > -990.)Ntr++; } return Ntr; " )
		.Define( "Tof700Matched", "int Ntr{}; for( int i=0; i<trMom.size(); i++ ){ if(trBetaTof700.at(i) > -990.)Ntr++; } return Ntr; " )		
		.Define( "beta400", " ROOT::RVecF beta; for( int i=0; i<trMom.size(); i++ ){auto b = trBetaTof400.at(i); beta.push_back( b > -990. ? 1 / b : -999.0 ) ; } return beta;" )
		.Define( "beta700", " ROOT::RVecF beta; for( int i=0; i<trMom.size(); i++ ){auto b = trBetaTof700.at(i); beta.push_back( b > -990. ? 1 / b : -999.0 ) ; } return beta;" )
		.Define( "beta400_2212","ROOT::RVecF vec_m2;\n"
							"for( int i=0; i<trMom.size(); i++ ){\n"
							" auto p = trMom.at(i).P();\n"
							" auto p2 = p*p;\n"
							" auto beta = beta400.at(i);\n"
							" auto m2 = 0.93827208 * 0.93827208;\n"
							" auto res = p2 > -990. ? beta - sqrt(m2 / p2 + 1) : -999.0;\n"
							" vec_m2.push_back( res );\n"
							"}\n"
							"return vec_m2;" )
		.Define( "trM2Tof400","ROOT::RVecF vec_m2;\n"
							"for( int i=0; i<trMom.size(); i++ ){\n"
							" auto p = trMom.at(i).P();\n"
							" auto p2 = p*p;\n"
							" auto beta = trBetaTof400.at(i);\n"
							" auto beta2 = beta*beta;\n"
							" auto gamma2 = 1 - beta2;\n"
							" auto m2 = beta > -990. ? p2 / beta2 * gamma2 : -999.0;\n"
							" vec_m2.push_back( m2 );\n"
							"}\n"
							"return vec_m2;" )
		.Define( "trM2Tof400_2212", [PROTON_M]( ROOT::RVecF vec_m2_tof400 ){
				ROOT::RVecF vec_m2_2212{};
				vec_m2_2212.reserve( vec_m2_tof400.size() );
				for( int i=0; i<vec_m2_tof400.size(); ++i ){
					auto m2 = vec_m2_tof400.at(i);
					auto m2_2212 = PROTON_M*PROTON_M;
					vec_m2_2212.push_back( m2 - m2_2212 );
				}
				return vec_m2_2212;
			}, {"trM2Tof400"} )
		.Define( "trBeta_2212", [PROTON_M]( ROOT::RVecF vec_m2_tof400 ){
				ROOT::RVecF vec_m2_2212{};
				vec_m2_2212.reserve( vec_m2_tof400.size() );
				for( int i=0; i<vec_m2_tof400.size(); ++i ){
					auto m2 = vec_m2_tof400.at(i);
					auto m2_2212 = PROTON_M*PROTON_M;
					vec_m2_2212.push_back( m2 - m2_2212 );
				}
				return vec_m2_2212;
			}, {"trM2Tof400"} )
		.Define( "trM2Tof400_211", [PION_M]( ROOT::RVecF vec_m2_tof400 ){
				ROOT::RVecF vec_m2_211{};
				vec_m2_211.reserve( vec_m2_tof400.size() );
				for( int i=0; i<vec_m2_tof400.size(); ++i ){
					auto m2 = vec_m2_tof400.at(i);
					auto m2_211 = PION_M*PION_M;
					vec_m2_211.push_back( m2 - m2_211 );
				}
				return vec_m2_211;
			}, {"trM2Tof400"} )
		.Define( "beta700_2212","ROOT::RVecF vec_m2;\n"
							"for( int i=0; i<trMom.size(); i++ ){\n"
							" auto p = trMom.at(i).P();\n"
							" auto p2 = p*p;\n"
							" auto beta = beta700.at(i);\n"
							" auto m2 = 0.93827208 * 0.93827208;\n"
							" auto res = p2 > -990. ? beta - sqrt(m2 / p2 + 1) : -999.0;\n"
							" vec_m2.push_back( res );\n"
							"}\n"
							"return vec_m2;" )
		.Define( "trM2Tof700","ROOT::RVecF vec_m2;\n"
							"for( int i=0; i<trMom.size(); i++ ){\n"
							" auto p = trMom.at(i).P();\n"
							" auto p2 = p*p;\n"
							" auto beta = trBetaTof700.at(i);\n"
							" auto beta2 = beta*beta;\n"
							" auto gamma2 = 1 - beta2;\n"
							" auto m2 = beta > -990. ? p2 / beta2 * gamma2 : -999.0;\n"
							" vec_m2.push_back( m2 );\n"
							"}\n"
							"return vec_m2;" )
		.Define( "trM2Tof700_2212", [PROTON_M]( ROOT::RVecF vec_m2_tof700 ){
				ROOT::RVecF vec_m2_2212{};
				vec_m2_2212.reserve( vec_m2_tof700.size() );
				for( int i=0; i<vec_m2_tof700.size(); ++i ){
					auto m2 = vec_m2_tof700.at(i);
					auto m2_2212 = PROTON_M*PROTON_M;
					vec_m2_2212.push_back( m2 - m2_2212 );
				}
				return vec_m2_2212;
			}, {"trM2Tof700"} )
		.Define( "trM2Tof700_211", [PION_M]( ROOT::RVecF vec_m2_tof700 ){
				ROOT::RVecF vec_m2_211{};
				vec_m2_211.reserve( vec_m2_tof700.size() );
				for( int i=0; i<vec_m2_tof700.size(); ++i ){
					auto m2 = vec_m2_tof700.at(i);
					auto m2_211 = PION_M*PION_M;
					vec_m2_211.push_back( m2 - m2_211 );
				}
				return vec_m2_211;
			}, {"trM2Tof700"} )
		.Define( "trM2", [](ROOT::RVecF vec_400, ROOT::RVecF vec_700){
							ROOT::RVecF vec_m2{};
							vec_m2.reserve( vec_400.size() );
							for( int i=0; i<vec_400.size(); ++i ){
								auto m2_400 = vec_400.at(i);
								auto m2_700 = vec_700.at(i);
								if( m2_400 < -990. ){
									vec_m2.push_back( m2_700 );
									continue;
								}
								if( m2_700 < -990. ){
									vec_m2.push_back( m2_400 );
									continue;
								}
								vec_m2.push_back( 0.5*(m2_400 + m2_700) );
							}
							return vec_m2;
						}, { "trM2Tof400", "trM2Tof700" } )
		.Define( "trM2_2212", [PROTON_M]( ROOT::RVecF vec_m2 ){
				ROOT::RVecF vec_m2_2212{};
				vec_m2_2212.reserve( vec_m2.size() );
				for( int i=0; i<vec_m2.size(); ++i ){
					auto m2 = vec_m2.at(i);
					auto m2_2212 = PROTON_M*PROTON_M;
					vec_m2_2212.push_back( m2 - m2_2212 );
				}
				return vec_m2_2212;
			}, {"trM2"} )
		.Define( "trM2_211", [PION_M]( ROOT::RVecF vec_m2 ){
				ROOT::RVecF vec_m2_2212{};
				vec_m2_2212.reserve( vec_m2.size() );
				for( int i=0; i<vec_m2.size(); ++i ){
					auto m2 = vec_m2.at(i);
					auto m2_2212 = PION_M*PION_M;
					vec_m2_2212.push_back( m2 - m2_2212 );
				}
				return vec_m2_2212;
			}, {"trM2"} )
		.Define( "trIsProton400", 
				[ f1_m2_400, f1_s_400 ]( 
					ROOT::RVecF vec_m2, 
					ROOT::RVecF vec_pq 
					){
						ROOT::RVecI vec_is{};
						vec_is.reserve( vec_pq.size() );
						for( int i=0; i < vec_pq.size(); ++i ){
							auto pq = vec_pq.at(i);
							auto m2 = vec_m2.at(i);
							if( pq < 0 ){ vec_is.push_back(0); continue; }
							auto mean = f1_m2_400->Eval(pq);
							auto sigma = f1_s_400->Eval(pq);
							auto lo = mean - 2*sigma;
							auto hi = mean + 2*sigma;
							vec_is.push_back( lo < m2 && m2 < hi ? 1 : 0 );
						}
						return vec_is;
					}
					,{ "trM2Tof400", "pq" } )
		.Define( "trIsProton700", 
				[ f1_m2_700, f1_s_700 ]( 
					ROOT::RVecF vec_m2, 
					ROOT::RVecF vec_pq 
					){
						ROOT::RVecI vec_is{};
						vec_is.reserve( vec_pq.size() );
						for( int i=0; i < vec_pq.size(); ++i ){
							auto pq = vec_pq.at(i);
							auto m2 = vec_m2.at(i);
							if( pq < 0 ){ vec_is.push_back(0); continue; }
							auto mean = f1_m2_700->Eval(pq);
							auto sigma = f1_s_700->Eval(pq);
							auto lo = mean - 2*sigma;
							auto hi = mean + 2*sigma;
							vec_is.push_back( lo < m2 && m2 < hi ? 1 : 0 );
						}
						return vec_is;
					}
					,{ "trM2Tof700", "pq" } )
		.Define( "trIsProton", []( ROOT::RVecI is_400, ROOT::RVecI is_700 ){
			ROOT::RVecI vec_is{};
			vec_is.reserve( is_400.size() );
			for( int i=0; i<is_400.size(); ++i ){ vec_is.push_back( is_400.at(i) == 1 || is_700.at(i) == 1 ? 1 : 0 ); }
			return vec_is;
		}, {"trIsProton400", "trIsProton700"} )
		.Define( "trProtonY", [PROTON_M, Y_CM]( ROOT::RVecF vec_pz, ROOT::RVecF vec_pq ){
			ROOT::RVecF vec_y{};
			vec_y.reserve( vec_pz.size() );
			for( int i=0; i<vec_pz.size(); ++i ){
				auto pz = vec_pz.at(i);
				auto p = vec_pq.at(i);
				auto E = sqrt( p*p + PROTON_M*PROTON_M );
				auto y = 0.5 * log( ( E + pz )/( E - pz ) ) - Y_CM;
				vec_y.push_back( y );
			}
			return vec_y;
		}, {"pz", "pq"} )
		.Define( "trRapidity", [Y_CM]( ROOT::RVecF vec_pz, ROOT::RVecF vec_pq, ROOT::RVecF vec_m2 ){
			ROOT::RVecF vec_y{};
			vec_y.reserve( vec_pz.size() );
			for( int i=0; i<vec_pz.size(); ++i ){
				auto pz = vec_pz.at(i);
				auto p = vec_pq.at(i);
				auto E = sqrt( p*p + vec_m2.at(i) );
				auto y = 0.5 * log( ( E + pz )/( E - pz ) ) - Y_CM;
				vec_y.push_back( y );
			}
			return vec_y;
		}, {"pz", "pq", "trM2"} )
		// STS tracks
		.Define( "tr_chi2_ndf", " ROOT::RVecF vec_par; for( int i=0; i<trChi2.size(); ++i ){ vec_par.push_back( trChi2.at(i) / trNdf.at(i) ); } return vec_par; " )
		.Define( "tr_x", 	  	" ROOT::RVecF vec_par; for( auto par : trParamFirst ){ vec_par.push_back( par.at(0) ); } return vec_par; " )
		.Define( "tr_y", 		" ROOT::RVecF vec_par; for( auto par : trParamFirst ){ vec_par.push_back( par.at(1) ); } return vec_par; " )
		.Define( "tr_z", 		" ROOT::RVecF vec_par; for( auto par : trParamFirst ){ vec_par.push_back( par.at(2) ); } return vec_par; " )
		.Define( "tr_dca_x", 	" ROOT::RVecF vec_par; for( auto par : trParamFirst ){ vec_par.push_back( par.at(0) - vtxX ); } return vec_par; " )
		.Define( "tr_dca_y", 	" ROOT::RVecF vec_par; for( auto par : trParamFirst ){ vec_par.push_back( par.at(1) - vtxY ); } return vec_par; " )
		.Define( "tr_dca_z",	" ROOT::RVecF vec_par; for( auto par : trParamFirst ){ vec_par.push_back( par.at(2) - vtxZ ); } return vec_par; " )
		.Define( "tr_dca_r", [](ROOT::RVecF vec_x, ROOT::RVecF vec_y){ 
			ROOT::RVecF vec_r{};
			vec_r.reserve( vec_x.size() );
			for( int i=0; i<vec_x.size(); ++i ){
				auto x = vec_x.at(i);
				auto y = vec_y.at(i);
				auto r = sqrt(x*x + y*y);
				vec_r.push_back(r);
			}
			return vec_r;
		}, { "tr_dca_x", "tr_dca_y" } )
		.Define( "tr_pq_difference", [](ROOT::RVec<std::vector<float>> vec_vtx_parameters, ROOT::RVec<std::vector<float>> vec_trk_parameters){
			ROOT::RVecF vec_dp{};
			vec_dp.reserve( vec_vtx_parameters.size() );
			for( int i=0; i<vec_vtx_parameters.size(); ++i ){
				auto qp_vtx = vec_vtx_parameters.at(i).at(5);
				auto qp_trk = vec_trk_parameters.at(i).at(5);
				auto p_vtx = 1.0/qp_vtx;
				auto p_trk = 1.0/qp_trk;
				auto dp = fabs( p_vtx - p_trk ) / p_trk;
				vec_dp.push_back( dp );
			}
			return vec_dp;
		}, {"trParamFirst", "stsTrackParameters"} )
		.Define( "sts_x", 	" ROOT::RVecF sts_x;  for( auto par : stsTrackParameters ){ sts_x.push_back( par.at(0) ); } return sts_x; " )
		.Define( "sts_y", 	" ROOT::RVecF sts_y;  for( auto par : stsTrackParameters ){ sts_y.push_back( par.at(1) ); } return sts_y; " )
		.Define( "sts_z", 	" ROOT::RVecF sts_z;  for( auto par : stsTrackParameters ){ sts_z.push_back( par.at(2) ); } return sts_z; " )
		.Define( "sts_px", 	" ROOT::RVecF sts_px; for( auto mom : stsTrackMomentum ){ sts_px.push_back( mom.Px() ); } return sts_px; " )
		.Define( "sts_py", 	" ROOT::RVecF sts_py; for( auto mom : stsTrackMomentum ){ sts_py.push_back( mom.Py() ); } return sts_py; " )
		.Define( "sts_pz", 	" ROOT::RVecF sts_pz; for( auto mom : stsTrackMomentum ){ sts_pz.push_back( mom.Pz() ); } return sts_pz; " )
		.Define( "sts_pT", 	" ROOT::RVecF sts_pT; for( auto mom : stsTrackMomentum ){ sts_pT.push_back( mom.Pt() ); } return sts_pT; " )
		.Define( "sts_p", 	" ROOT::RVecF sts_p;  for( auto mom : stsTrackMomentum ){ sts_p.push_back( mom.P() ); }   return sts_p; " )
		.Define( "sts_eta", " ROOT::RVecF sts_eta;for( auto mom : stsTrackMomentum ){ sts_eta.push_back( mom.Eta() ); } return sts_eta; " )
		.Define( "sts_phi", " ROOT::RVecF sts_phi;for( auto mom : stsTrackMomentum ){ sts_phi.push_back( mom.Phi() ); } return sts_phi; " )
		.Define( "tx_err", 	" ROOT::RVecF tx_err; for( auto cov : globalTrackCovMatrix ){ tx_err.push_back( cov.at(5) ); } return tx_err;")
        .Define( "ty_err", 	" ROOT::RVecF ty_err; for( auto cov : globalTrackCovMatrix ){ ty_err.push_back( cov.at(9) ); } return ty_err;")
        .Define( "qp_err", 	" ROOT::RVecF qp_err; for( auto cov : globalTrackCovMatrix ){ qp_err.push_back( cov.at(14) ); } return qp_err;")		          
		// BeamPipe
		// FHCal modules
		.Define( "fhcal_x", " ROOT::RVecF fhcal_x; for( auto pos : fhcalModPos ){ fhcal_x.push_back( pos.X() ); } return fhcal_x; " )
		.Define( "fhcal_y", " ROOT::RVecF fhcal_y; for( auto pos : fhcalModPos ){ fhcal_y.push_back( pos.Y() ); } return fhcal_y; " )
		.Define( "fhcalSumE", "float fhcalE{}; for( auto modE : fhcalModE ){ fhcalE+=modE; } return fhcalE; " )
		.Define( "scwallSumQ", "float scwallQ{}; for( auto modQ : scwallModQ ){ scwallQ+=modQ; } return scwallQ; " )
        .Define( "hodoSumQ", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return hodoQ; " )
        .Define( "fhcalhodoSumE", "float fhcalE{}; for( auto modId = 0; modId < (fhcalModId.size()-1); modId++ ){ fhcalE+=fhcalModE.at(modId); } float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return fhcalE+0.35*hodoQ; " )
        .Define( "hodoIsCentral", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return (hodoQ<40); " )
        .Define( "hodoIsPeripheral", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return (hodoQ>=40); " )

		;

	auto mask_vector 	 = [](const ROOT::RVecF &vector, const ROOT::RVecI &mask) { return vector[mask]; };

	auto PhisRunCut = [&physical_runs]( UInt_t run_id ){return std::find( physical_runs.begin(), physical_runs.end(), run_id) != physical_runs.end();};
	auto BRCut = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<5); };	//
	auto ARCut = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<7); }; //
	auto ARCutMinB = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<5); }; // AR - 5 бит minimum-bias
	
	auto bc1fdCut = []( ROOT::VecOps::RVec<float> bc1_int, ROOT::VecOps::RVec<float> fd_int ){
                        float trigger = fd_int[0]-bc1_int[0]*0.602;
                        return -25900 < trigger && trigger < -6030;
                	};

    auto bc1fdCut_new = [](double x, double y){
    	std::vector<double> meanX = {1.,2.,3.,4.,5.,6.,7.,8.,9.};
    	std::vector<double> meanY = {0.,1.,2.,3.,4.,5.,6.,7.,8.};
    	double sigma = 0.5;
    	bool _key = false;
    	for(int i=0; i<meanX.size(); i++){
    		if(x > (meanX[i] - sigma) && x < (meanX[i] + sigma) && y > (meanY[i] - sigma) && y < (meanY[i] + sigma)) _key=true;
    	}
    	return _key;
    };

    auto bc1Cut = []( ROOT::VecOps::RVec<float> bc1_int ){ return 1e4 < bc1_int[0] && bc1_int[0] < 4e4; };
	auto stsNdigitsMultCut = []( unsigned long sts_digits, unsigned long n_tracks ){ 
	       double sts_min = sts_digits-n_tracks*(4.81632+0.0332792*n_tracks-9.62078e-05*n_tracks*n_tracks);
	       double sts_max = sts_digits-n_tracks*(19.4203-0.0518774*n_tracks+4.56033e-05*n_tracks*n_tracks);
	       return -74.0087 < sts_min && sts_max < 188.248; 
	      };

	auto stsNdigitsMultCut2 = []( unsigned long sts_digits, unsigned long n_tracks ){ 
	       double sts_min = sts_digits-n_tracks*(17.6212-0.065777*n_tracks+8.84404e-05*n_tracks*n_tracks);
	       double sts_max = sts_digits-n_tracks*(21.6309-0.0774261*n_tracks+0.000141619*n_tracks*n_tracks);
	       return -549.584 < sts_min && sts_max < 354.607; 
	      };

	// Out File
	auto file_out = TFile::Open( out_file_name.c_str(), "RECREATE" );
	file_out->cd();


	// Without all cuts
	AddStandartHisto(dd,"def_",file_out);

	// Phis Runs
	auto dd_cut0 = dd
		.Filter("runId < 8312")
		.Filter(PhisRunCut,{"runId"} )
	;
	AddStandartHisto(dd_cut0,"cut0_",file_out);

	// CCT2
	auto dd_cut1 = dd
		.Filter("runId < 8312")
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		;
	AddStandartHisto(dd_cut1,"cut1_",file_out);
/*
	// Bad Runs
	std::cout<<physical_runs.size()<<"\t"<<badFd_runs.size()<<"\n";
    RemoveBadRuns(physical_runs,badFd_runs);
    std::cout<<physical_runs.size()<<"\n";
	auto dd_cut2 = dd
		.Filter("runId < 8312")
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		;
	AddStandartHisto(dd_cut2,"cut2_",file_out);

	// bc1 vs fd
	auto dd_cut3 = dd
		.Filter("runId < 8312")
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		//.Filter(bc1fdCut_new,{"bc1sIntegralNorm","fdIntegralNorm"})
		;
	AddStandartHisto(dd_cut3,"cut3_",file_out);
*/
	// n tracks vs sts difits
	auto dd_cut4 = dd
		.Filter("runId < 8312")
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		//.Filter(bc1fdCut_new,{"bc1sIntegralNorm","fdIntegralNorm"})
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		;
	AddStandartHisto(dd_cut4,"cut4_",file_out);

	// n tracks for vertex
	auto dd_cut5 = dd
		.Filter("runId < 8312")
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		//.Filter(bc1fdCut_new,{"bc1sIntegralNorm","fdIntegralNorm"})
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		.Filter(stsNdigitsMultCut,{"stsNdigits", "track_multiplicity"})
		;
	AddStandartHisto(dd_cut5,"cut5_",file_out);

	// n tracks for vertex
	auto dd_cut6 = dd
		.Filter("runId < 8312")
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		//.Filter(bc1fdCut_new,{"bc1sIntegralNorm","fdIntegralNorm"})
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		.Filter(stsNdigitsMultCut2,{"stsNdigits", "track_multiplicity"})
		;
	AddStandartHisto(dd_cut6,"cut6_",file_out);

	// Centrality
	auto dd_cent1 = dd
		.Filter("runId < 8312")
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter(bc1fdCut_new,{"bc1sIntegralNorm","fdIntegralNorm"})
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtx_chi2_ndf > 0.1")
		.Filter(stsNdigitsMultCut,{"stsNdigits", "track_multiplicity"})
		;
	AddStandartHisto(dd_cent1,"cent1_",file_out);

	// Centrality
	auto dd_cent2 = dd
		.Filter("runId < 8312")
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter(bc1fdCut_new,{"bc1sIntegralNorm","fdIntegralNorm"})
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtx_chi2_ndf > 0.1")
		.Filter(stsNdigitsMultCut2,{"stsNdigits", "track_multiplicity"})
		;
	AddStandartHisto(dd_cent2,"cent2_",file_out);

	// m2 cuts
	auto ddM2P = dd
				.Filter("runId < 8312")
				.Filter(PhisRunCut,{"runId"} )
				.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
				.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
				.Filter("vtxNtracks > 1")
				//.Filter(bc1fdCut_new,{"bc1sIntegralNorm","fdIntegralNorm"})
				.Filter(stsNdigitsMultCut,{"stsNdigits", "track_multiplicity"})
				.Filter("abs(vtxX)<90")
				.Filter("abs(vtxY)<90")
				.Filter("abs(vtxZ)<90")
				.Filter("Tof400Matched > 0")
				.Filter("Tof700Matched > 0");


	AddStandartHisto (ddM2P,"ddM2P_",file_out);
	AddHistoM2vsRunId(ddM2P,"ddM2P_","trM2Tof400","pq>0",file_out);
	AddHistoM2vsRunId(ddM2P,"ddM2P_","trM2Tof400","pq<0",file_out);
	AddHistoM2vsRunId(ddM2P,"ddM2P_","trM2Tof700","pq>0",file_out);
	AddHistoM2vsRunId(ddM2P,"ddM2P_","trM2Tof700","pq<0",file_out);

	file_out->Close();

	std::cout<<"QA_completed!"<<std::endl;
	
	timer1.Stop();
  	timer1.Print();
}
