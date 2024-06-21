#include "QA_function.C"

void DigitNTracksFit(TFile *inFile, DrawParameters obj_par, std::string path_save, TFile *outFile){

	TH1D *hist;

	std::vector<double> x;
	std::vector<double> x_err;
	std::vector<double> y;
	std::vector<double> y_err;
	std::vector<double> y_low;
	std::vector<double> y_high;

	int StopFit = 350;
	double nSigma = 3.;
	double minFitRange = 10.;
	double maxFitRange = 6000.;
	
	TF1 *fit_mean = new TF1("fm","pol3(0)",0,1000);
	TF1 *fit_low  = new TF1("fl","pol3(0)",21,1000);
	TF1 *fit_high = new TF1("fh","pol3(0)",0,1000);

	fit_mean -> SetLineColor(kRed);
	fit_low  -> SetLineColor(kRed);
	fit_high -> SetLineColor(kRed);

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
		
	for(int i=0; i<StopFit; i++){
		int h2BinX = pr1->GetXaxis()->GetBinCenter(i);
		if(h2BinX<1)continue;
		hist = (TH1D*)pr1->ProjectionY(Form("nTrack_%s_%i",pr1->GetName(),h2BinX),pr1->GetXaxis()->FindBin(h2BinX),pr1->GetXaxis()->FindBin(h2BinX));
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		
		hist->SetLineColor(kBlue);
		hist->SetTitle(Form("nTracks=%i",h2BinX));

		auto gaus = new TF1(Form("f1_pr_%s_%i",pr1->GetName(),i),"gaus(0)",minFitRange,maxFitRange);
		gaus->SetLineColor(kRed);
		hist->Fit(gaus,"QRMH");
		
		auto gausPol = new TF1(Form("f1_1_pr_%s_%i",pr1->GetName(),i),"gaus(0)+pol1(3)",minFitRange,maxFitRange);
		gausPol->SetParameter(0,gaus->GetParameter(0));
		gausPol->SetParameter(1,gaus->GetParameter(1));
		gausPol->SetParameter(2,gaus->GetParameter(2));
		hist->Fit(gausPol,"QRM");

		x.push_back((double)h2BinX);
		y.push_back(gaus->GetParameter(1));
		y_low.push_back( gaus->GetParameter(1) - nSigma * gaus->GetParameter(2) );
		y_high.push_back(gaus->GetParameter(1) + nSigma * gaus->GetParameter(2) );

		x_err.push_back(0.);
		y_err.push_back(0.);

		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<std::endl;
		outFile->cd();
		hist->Write();
	}

	if(x.size()==0) return;

	TGraphErrors *gr_mean = new TGraphErrors(x.size(),&x[0],&y[0],&x_err[0],&y_err[0]);
	gr_mean->SetName(Form("gr_%s",pr1->GetName()));
	gr_mean->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_mean->SetLineColor(1);
	gr_mean->SetMarkerSize(1);
	gr_mean->SetMarkerStyle(8);
	gr_mean->SetMarkerColor(4);
	gr_mean->Fit(fit_mean,"QRM");

	TGraphErrors *gr_low = new TGraphErrors(x.size(),&x[0],&y_low[0],&x_err[0],&y_err[0]);
	gr_low->SetName(Form("gr_%s",pr1->GetName()));
	gr_low->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_low->SetLineColor(1);
	gr_low->SetMarkerSize(1);
	gr_low->SetMarkerStyle(8);
	gr_low->SetMarkerColor(4);
	gr_low->Fit(fit_low,"QRM");


	TGraphErrors *gr_high = new TGraphErrors(x.size(),&x[0],&y_high[0],&x_err[0],&y_err[0]);
	gr_high->SetName(Form("gr_%s",pr1->GetName()));
	gr_high->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_high->SetLineColor(1);
	gr_high->SetMarkerSize(1);
	gr_high->SetMarkerStyle(8);
	gr_high->SetMarkerColor(4);
	gr_high->Fit(fit_high,"QRM");


	TCanvas *can = new TCanvas(Form("StsDigitCut_%s",pr1->GetName()),"",960,560);
	can->cd();
	pr1->Draw("colz");
	can->SetLogz();
	gr_mean->Draw("Lsame");
	gr_low->Draw("Lsame");
	gr_high->Draw("Lsame");
	fit_mean->Draw("same");
	fit_low ->Draw("same");
	fit_high->Draw("same");

	std::cout<<fit_low->GetParameter(0)<<"\t\t"<<fit_low->GetParameter(1)<<"\t\t"<<fit_low->GetParameter(2)<<"\t\t"<<fit_low->GetParameter(3)<<"\n";
	std::cout<<fit_high->GetParameter(0)<<"\t\t"<<fit_high->GetParameter(1)<<"\t\t"<<fit_high->GetParameter(2)<<"\t\t"<<fit_high->GetParameter(3)<<"\n";

	gSystem->Exec(Form("mkdir -p ./%s/StsDigitCut",path_save.c_str()));
	can->SaveAs(Form("./%s/StsDigitCut/%s1.png",path_save.c_str(), pr1->GetName()));
	
	gr_mean->Write();
	gr_low->Write();
	gr_high->Write();
	can->Write();

}

void Get_GraphiCuts_cuts(){	
	
	makeplotstyle();
	TFile *fstyle = new TFile("./style.root");
	TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
	//tsty->cd();
 	gROOT->SetStyle("Pub");
	gROOT->ForceStyle();
	//qa_run8_vf_24.04_8005.root
	//TFile *file_inFile 	= new TFile("qa_run8_vf_24.04_new2.root","READ");
	TFile *file_inFile 		= new TFile("qa_run8_vf_24.04_final.root","READ");
	TFile *file_outFile = new TFile("./OUT_24.04_GraphCuts.root","RECREATE");	

	// RunByRun
	std::string path = "QA_RunByRun_24.04_new";
	gSystem->Exec(Form("mkdir -p ./%s/",path.c_str()));

	std::vector<DrawParameters> DrawObjectParH2;
	DrawObjectParH2.push_back({"h2_nTracks_stsDigits","cut4_",0,4,2.,{0,500},{0,6000}});

	DigitNTracksFit(file_inFile, DrawObjectParH2[0], path, file_outFile);

	file_outFile->Close();
	file_inFile->Close();

}