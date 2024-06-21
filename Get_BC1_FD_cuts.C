#include "QA_function.C"

void RunByRunAnalis(TFile *inFile, DrawParameters obj_par, string BC1_or_FD, int MeanSigma, std::string path_save, TFile *outFile){

	TH1D *hist;
	TGraphErrors *Def;
	TGraphErrors *BadRunsGr;

	std::vector<double> x_err;
	std::vector<double> runId;
	std::vector<double> mean;
	std::vector<double> mean_err; //""
	std::vector<double> sigma; //"s"

	std::vector<double> BadRunsMean_x_err;
	std::vector<double> BadRunsMean_runId;
	std::vector<double> BadRunsMean_mean;
	std::vector<double> BadRunsMean_mean_err; //""
	std::vector<double> BadRunsMean_sigma; //"s"

	int id_detecor = (BC1_or_FD=="bc1" ? 0 : 1);	
	double first_mean[2] = {28571.,	19284.};
	double first_sigma[2]= {3192.,	2940.};

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
		
	for(int i=0; i<(int)pr1->GetNbinsX(); i++){
		int h2RunId = pr1->GetXaxis()->GetBinCenter(i);
		if(h2RunId<RunIdStart)continue;
		hist = (TH1D*)pr1->ProjectionY(Form("run_%s_%i",pr1->GetName(),h2RunId),pr1->GetXaxis()->FindBin(h2RunId),pr1->GetXaxis()->FindBin(h2RunId));
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		
		hist->SetLineColor(kBlue);
		hist->SetTitle(Form("Run %i",h2RunId));

		double minFitRange = 0.;
		double maxFitRange = 0.;

		if(id_detecor==0){
			minFitRange = 22500.;
			maxFitRange = 35000.;
		}else if(id_detecor==1){
			minFitRange = 13000.;
			maxFitRange = 25000.;
			if(h2RunId>7550 && h2RunId<7593){
				minFitRange = 5000.;
				maxFitRange = 13000.;
			}
		}
		auto gaus = new TF1(Form("f1_pr_%s_%i",pr1->GetName(),i),"gaus(0)",minFitRange,maxFitRange);
		gaus->SetLineColor(kRed);
		gaus->SetParameter(0,10000);
		gaus->SetParameter(1,first_mean[id_detecor]);
		gaus->SetParameter(2,first_sigma[id_detecor]);
		hist->Fit(gaus,"QRMH");
		
		gaus->SetParameter(0,gaus->GetParameter(0));
		gaus->SetParameter(1,gaus->GetParameter(1));
		gaus->SetParameter(2,gaus->GetParameter(2));
		hist->Fit(gaus,"QRM");

		runId.push_back((int)h2RunId);
		//mean.push_back(hist->Integral());
		mean.push_back(gaus->GetParameter(MeanSigma));
		mean_err.push_back(0.);
		sigma.push_back(0.);
		x_err.push_back(0.);
		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<std::endl;
		outFile->cd();
		hist->Write();
		if((int)h2RunId==8005){
			TCanvas *can1 = new TCanvas(Form("fit_%s",pr1->GetName()),"",960,560);
			can1->cd();
			hist->Draw();
			can1->SetLogy();
			can1->SaveAs(Form("./%s/BC1_FD/fit_%s_%i.png",path_save.c_str(), pr1->GetName(), (int)h2RunId));
		};
	}

	if(mean.size()==0) return;

	double h[2]; // mean
	double s[2]; // std deviations
	double e[2]; // mean err
	parRunByRun(runId,mean,h[0],s[0],e[0]);
	parRunByRun(runId,mean_err,h[1],s[1],e[1]);	
	//std::cout<<mean.size()<<"\t"<<h[0]<<"\t"<<s[0]<<"\t"<<e[0]<<"\n";
	//std::cout<<mean.size()<<"\t"<<h[1]<<"\t"<<s[1]<<"\t"<<e[1]<<"\n";

	for(int i=0; i<mean.size(); i++){

		if(runId[i]<RunIdStart)continue;

		double lo = 	h[0] - nSigma * s[0];
		double hi = 	h[0] + nSigma * s[0];
		double loStdD = h[1] - nSigma * s[1];
		double hiStdD = h[1] + nSigma * s[1];
		
		if(mean[i] > hi || mean[i] < lo){
		//if((mean[i] > hi || mean[i] < lo) || (mean_err[i] > hiStdD || mean_err[i] < loStdD)){
			BadRunsMean_x_err.push_back(0.);
			BadRunsMean_runId.push_back(runId[i]);
			BadRunsMean_mean.push_back(mean[i]);
			BadRunsMean_mean_err.push_back(mean_err[i]); //""
			//std::cout<<"badRuns\n";
		}
	}

	double AxisYMax = h[0]+5*s[0];
	double AxisYMin = h[0]-5*s[0];
	double AxisYH   = (AxisYMax - AxisYMin);

	TCanvas *can = new TCanvas(Form("RunId_%s",pr1->GetName()),"",960,560);
	can->cd();

	Def = new TGraphErrors(mean.size(),&runId[0],&mean[0],&x_err[0],&mean_err[0]);
	Def->SetName(Form("gr_%s_%i",pr1->GetName(),MeanSigma));
	Def->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	Def->SetLineColor(1);
	Def->SetMarkerSize(1);
	Def->SetMarkerStyle(8);
	Def->SetMarkerColor(4);
	auto max = (Def->GetYaxis()->GetXmax()>AxisYMax ? Def->GetYaxis()->GetXmax() : AxisYMax);
	auto min = (Def->GetYaxis()->GetXmin()<AxisYMin ? Def->GetYaxis()->GetXmin() : AxisYMin);
	Def->GetYaxis()->SetRangeUser(min, max);
	//std::cout<<min<<"\t"<<max<<"\t"<<AxisYMin<<"\t"<<AxisYMax<<"\n";
	TH2D *h2_axis = new TH2D("","",2,RunIdMin,RunIdMax,2,min,max);
	h2_axis->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	h2_axis->GetXaxis()->SetTitle(Form("%s",pr1->GetXaxis()->GetTitle()));
	h2_axis->Draw();
	Def->Draw("sameP");

	BadRunsGr = new TGraphErrors(BadRunsMean_mean.size(),&BadRunsMean_runId[0],&BadRunsMean_mean[0],&BadRunsMean_x_err[0],&BadRunsMean_mean_err[0]);
	BadRunsGr->SetName(Form("%s_BadRuns_%i",Def->GetName(),MeanSigma));
	BadRunsGr->SetLineColor(2);
	BadRunsGr->SetMarkerStyle(Def->GetMarkerStyle());
	BadRunsGr->SetMarkerColor(2);
	BadRunsGr->Draw("Psame");

	TLine *lineMean = new TLine(RunIdMin,h[0],RunIdMax,h[0]);
	lineMean->SetLineStyle(2);
	lineMean->Draw("same");

	TLine *lineUp = new TLine(RunIdMin,nSigma*s[0] + h[0],RunIdMax,nSigma*s[0] + h[0]);
	lineUp->SetLineColor(kRed);
	lineUp->SetLineStyle(1);
	lineUp->Draw("same");

	TLine *lineDown = new TLine(RunIdMin, -nSigma*s[0] + h[0],RunIdMax,-nSigma*s[0] + h[0]);
	lineDown->SetLineColor(kRed);
	lineDown->SetLineStyle(1);
	lineDown->Draw("same");

	std::string BadRuns = Form("%s: ",pr1->GetName());
   	for(auto dd : BadRunsMean_runId){
   		BadRuns = BadRuns + Form("%.0f",dd) + ", ";
   		BadRunsGlobal.push_back(dd);
   	}
   	std::cout<<BadRuns<<std::endl;

   	outFile->cd();
   	Def->Write();
   	BadRunsGr->Write();

	gSystem->Exec(Form("mkdir -p ./%s/BC1_FD",path_save.c_str()));
	can->SaveAs(Form("./%s/BC1_FD/%s_%i.png",path_save.c_str(), pr1->GetName(), MeanSigma));
	//can->Write();

}


void Get_BC1_FD_cuts(){	
	
	makeplotstyle();
	TFile *fstyle = new TFile("./style.root");
	TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
	//tsty->cd();
 	gROOT->SetStyle("Pub");
	gROOT->ForceStyle();

	TFile *file_inFile 	= new TFile("qa_run8_vf_24.04_final.root","READ");
	TFile *file_outFile = new TFile("./OUT_24.04_bc1fd.root","RECREATE");	

	// RunByRun
	std::string path = "QA_RunByRun_24.04_new";
	gSystem->Exec(Form("mkdir -p ./%s/",path.c_str()));

	std::string suf = "cut1_";
	std::vector<DrawParameters> DrawObjectParH2;

	DrawObjectParH2.push_back({"h2_RunId_bc1sInt",suf,0,4,2.,{},{},1});
	DrawObjectParH2.push_back({"h2_RunId_bdInt",suf,0,4,2.,{},{},1});

	RunByRunAnalis(file_inFile, DrawObjectParH2[0],"bc1", 1, path, file_outFile);
	RunByRunAnalis(file_inFile, DrawObjectParH2[1],"bd",  1, path, file_outFile);
	RunByRunAnalis(file_inFile, DrawObjectParH2[0],"bc1", 2, path, file_outFile);
	RunByRunAnalis(file_inFile, DrawObjectParH2[1],"bd",  2, path, file_outFile);

	// Output Bad Runs
	PrintBadRuns(file_inFile,suf);

	file_outFile->Close();
	file_inFile->Close();

}