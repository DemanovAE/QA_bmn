#include "QA_function.C"

void RunByRunAnalis(TFile *inFile, DrawParameters obj_par, std::string path_save, TFile *outFile){

	TH1D *hist;
	TH1D *hist_bad;
	TGraphErrors *Def;
	TGraphErrors *BadRunsGr;
   	TLatex text;

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

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(obj_par.Name=="h2_RunId_stsDigits")pr1->GetYaxis()->SetTitle("FSD digits");
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
		
	for(int i=0; i<(int)pr1->GetNbinsX(); i++){
		int h2RunId = pr1->GetXaxis()->GetBinCenter(i);
		if(h2RunId<RunIdStart)continue;
		hist = (TH1D*)pr1->ProjectionY(Form("run_%s_%i",pr1->GetName(),i),pr1->GetXaxis()->FindBin(h2RunId),pr1->GetXaxis()->FindBin(h2RunId));
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		runId.push_back((int)h2RunId);
		//mean.push_back(hist->Integral());
		mean.push_back(hist->GetMean());
		mean_err.push_back(hist->GetMeanError());
		sigma.push_back(hist->GetStdDev());
		x_err.push_back(0.);
		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<std::endl;
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
	Def->SetName(Form("gr_%s",pr1->GetName()));
	Def->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	Def->SetLineColor(4);
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
	BadRunsGr->SetName(Form("%s_BadRuns",Def->GetName()));
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

	std::string BadRuns;// = Form("%s: ",pr1->GetName());
   	if(BadRunsMean_runId.size()>0)BadRuns = BadRuns + Form("beyond #pm3#sigma: ");
   	for(int dd = 0; dd<BadRunsMean_runId.size(); dd++){
   		if(dd<BadRunsMean_runId.size()-1)BadRuns = BadRuns + Form("%.0f",BadRunsMean_runId[dd]) + ", ";
   		if(dd==BadRunsMean_runId.size()-1)BadRuns = BadRuns + Form("%.0f",BadRunsMean_runId[dd]);
   		BadRunsGlobal.push_back(BadRunsMean_runId[dd]);
   	}

   	//beyond ±3σ   
    TPad *pad1 = new TPad("pad_1", "", 0.  , 0., 0.8, 0.1, 0, 0, 0);
    pad1->Draw();
    pad1->cd();
    text.SetTextSize(0.5);
    text.SetTextColor(kRed);
	text.DrawLatex( 0.01, 0.1, BadRuns.c_str());

   	outFile->cd();
   	Def->Write();
   	BadRunsGr->Write();

	can->SaveAs(Form("./%s/%s.png",path_save.c_str(), pr1->GetName()));
	can->Write();

	TCanvas *can2 = new TCanvas(Form("%s_%i-%i",pr1->GetName(),(int)th1_RanRange.first,(int)th1_RanRange.second),"",960,560);
	can2->cd();
	hist = (TH1D*)pr1->ProjectionY(Form("run_%s_h1",pr1->GetName()),pr1->GetXaxis()->FindBin(th1_RanRange.first),pr1->GetXaxis()->FindBin(th1_RanRange.second));
	hist->GetYaxis()->SetTitle("dN/N");
	hist->GetXaxis()->SetTitleOffset(1);	
	if(obj_par.LogScale==1)can2->SetLogy();
	if(obj_par.Marker!=0)hist->SetMarkerStyle(obj_par.Marker);
	if(obj_par.Color !=0)hist->SetMarkerColor(obj_par.Color);
	if(obj_par.Color !=0)hist->SetLineColor(obj_par.Color);
	if(obj_par.Width !=0)hist->SetLineWidth(obj_par.Width);
	hist->Scale(1./hist->Integral());
	if(obj_par.LogScale==0)hist->GetYaxis()->SetRangeUser(0.,hist->GetMaximum()*1.4);
	hist->Draw("HIST");
	int BadRunsDraw = 0;

	if(BadRunsMean_runId.size()!=0){
		BadRunsDraw = BadRunsMean_runId.back();
		if(obj_par.Name=="h2_RunId_fhcal_e"){
			BadRunsDraw = BadRunsMean_runId[BadRunsMean_runId.size()-3];
		}
		hist_bad = (TH1D*)pr1->ProjectionY(Form("run_%s_h1b",pr1->GetName()),pr1->GetXaxis()->FindBin(BadRunsDraw),pr1->GetXaxis()->FindBin(BadRunsDraw));
		if(obj_par.LogScale==1)can2->SetLogy();
		if(obj_par.Marker!=0)hist_bad->SetMarkerStyle(obj_par.Marker);
		if(obj_par.Color !=0)hist_bad->SetMarkerColor(obj_par.Color);
		if(obj_par.Color !=0)hist_bad->SetLineColor(kRed);
		if(obj_par.Width !=0)hist_bad->SetLineWidth(obj_par.Width);
		hist_bad->Scale(1./hist_bad->Integral());
		hist_bad->Draw("sameHIST");
		if(obj_par.Name=="h2_RunId_fhcal_e"){
			hist_bad->Draw("HIST");
			hist->Draw("HISTsame");
		}
	}
	auto *legend = new TLegend(0.8,0.7,0.89,0.89);
	TLegendEntry* l2 = legend->AddEntry(hist,Form("%i",(int)th1_RanRange.first),"");
    l2->SetTextColor(kBlue);
	
	if(BadRunsMean_runId.size()!=0){
		TLegendEntry* l2 = legend->AddEntry(hist_bad,Form("%i",(int)BadRunsDraw),"");
    	l2->SetTextColor(kRed);
	}
	legend->Draw("same");
	
	gSystem->Exec(Form("mkdir -p ./%s/H1",path_save.c_str()));
    can2->SaveAs(Form("./%s/H1/%s.png",path_save.c_str(), pr1->GetName()));
	can2->Write();

}

void RunByRunAnalisFHcalMod(TH3D *pr1, std::string path_save, TFile *outFile){

	//RunByRunAnalis(TH2D *pr1, path_save, outFile)

}

void RunByRunM2Proton(DrawParameters obj_par, TFile *inFile, string _TOF, string _charge, float min_p, float max_p, int pid, int par, std::string path_save){

	int GetParFit = (pid==2212 ? 4 : 1);
	if(par==2) GetParFit = GetParFit + 1;

	TH2D *pr1 = GetTH2D_M2(obj_par, inFile, _TOF, _charge, min_p, max_p);
	if(pr1==nullptr) return;

	TCanvas *can = new TCanvas(Form("RunId_%s_%i",pr1->GetName(),par),"",960,560);
	TCanvas *can_fit = new TCanvas(Form("fit_RunId_%s_8050_%i",pr1->GetName(),par),"",960,860);
	can->cd();
	gSystem->Exec(Form("mkdir -p ./%s/m2_proton",path_save.c_str()));

	TGraphErrors *Def;
	TGraphErrors *BadRunsGr;
	TGraphErrors *AfterCutMeanErr;

	int nx = pr1->GetXaxis()->GetNbins();
	std::vector<double> x_err;
	std::vector<double> runId;
	std::vector<double> mean;
	std::vector<double> mean_err; //""
	std::vector<double> sigma; //"s"

	std::vector<double> BadRuns_x_err;
	std::vector<double> BadRuns_runId;
	std::vector<double> BadRuns_mean;
	std::vector<double> BadRuns_mean_err; //""
	std::vector<double> BadRuns_sigma; //"s"

	TH1D *hist;

	for(int i=0; i<nx; i++){
		
		int h2RunId = pr1->GetXaxis()->GetBinCenter(i);
		if(h2RunId<RunIdStart)continue;

		hist = (TH1D*)pr1->ProjectionY(Form("run_%s_%i",pr1->GetName(),i),pr1->GetXaxis()->FindBin(h2RunId),pr1->GetXaxis()->FindBin(h2RunId));
		if(hist->GetEntries()==0) continue;

		auto gaus_pion = new TF1(Form("f1_pion_%s_%i",pr1->GetName(),i),"gaus(0)",-0.5,0.4);
		gaus_pion->SetParameter(0,100);
		gaus_pion->SetParameter(1,-0.01);
		gaus_pion->SetParameter(2,0.01);
		hist->Fit(gaus_pion,"QRM");	

		auto gaus_proton = new TF1(Form("f1_pr_%s_%i",pr1->GetName(),i),"gaus(0)",0.6,1.4);
		gaus_proton->SetParameter(0,100);
		gaus_proton->SetParameter(1,PROTON_M*PROTON_M);
		gaus_proton->SetParameter(2,0.13);
		hist->Fit(gaus_proton,"QRM");
		
		auto gaus_all = new TF1(Form("f1_pr_%s_%i",pr1->GetName(),i),"gaus(0)+gaus(3)",-0.6,1.5);
		gaus_all->SetLineColor(kRed);
		gaus_all->SetParameter(0,gaus_pion->GetParameter(0));
		gaus_all->SetParameter(1,gaus_pion->GetParameter(1));
		gaus_all->SetParameter(2,gaus_pion->GetParameter(2));
		gaus_all->SetParameter(3,gaus_proton->GetParameter(0));
		gaus_all->SetParameter(4,gaus_proton->GetParameter(1));
		gaus_all->SetParameter(5,gaus_proton->GetParameter(2));
		hist->Fit(gaus_all,"QRM");

		//std::cout<<gaus_all->GetParameter(GetParFit)<<"\t"<<gaus_all->GetParError(GetParFit)<<"\n";

		x_err.push_back(0.0);
		runId.push_back(h2RunId);
		mean.push_back(gaus_all->GetParameter(GetParFit));
		mean_err.push_back( gaus_all->GetParError(GetParFit) );

		if(h2RunId==8005 && par==1){
		//if(h2RunId==7700 && par==1){
			can_fit->cd();
			auto hist2 = (TH1D*)pr1->ProjectionY(Form("run_%s_%i",pr1->GetName(),i),pr1->GetXaxis()->FindBin(h2RunId),pr1->GetXaxis()->FindBin(h2RunId));
			hist2->SetLineColor(kBlue);
			hist2->GetXaxis()->SetRangeUser(-0.5,2.);
			hist2->Fit(gaus_all,"QRM");
			//TLegend *leg_fit = new TLegend(0.8,);
			can_fit->SaveAs(Form("./%s/m2_proton/Fit8050_%s_%i.png",path_save.c_str(), pr1->GetName(), par));
		}

		//sigma.push_back( 	pr1->GetBinError(i) * sqrt(pr1->GetBinEntries(i)) );
		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<"\t"<<sigma.back()<<"\n";
	}

	can->cd();

	if(mean.size()==0) return;

	double h[2]; // mean
	double s[2]; // std deviations
	double e[2]; // mean err
	parRunByRun(runId,mean,h[0],s[0],e[0]);
	parRunByRun(runId,mean_err,h[1],s[1],e[1]);	
	std::cout<<mean.size()<<"\t"<<h[0]<<"\t"<<s[0]<<"\t"<<e[0]<<"\n";
	std::cout<<mean.size()<<"\t"<<h[1]<<"\t"<<s[1]<<"\t"<<e[1]<<"\n";

	for(int i=0; i<mean.size(); i++){
		double lo = h[0] - nSigma * s[0];
		double hi = h[0] + nSigma * s[0];
		double loStdD = h[1] - nSigma * s[1];
		double hiStdD = h[1] + nSigma * s[1];
		
		if(mean[i] > hi || mean[i] < lo){
		//if(mean[i] > hi || mean[i] < lo || mean_err[i] > hiStdD || mean_err[i] < loStdD){
			BadRuns_x_err.push_back(0.);
			BadRuns_runId.push_back(runId[i]);
			BadRuns_mean.push_back(mean[i]);
			BadRuns_mean_err.push_back(mean_err[i]); //""
			std::cout<<"badRuns\n";
		}
	}
	
	double AxisYMax = h[0]+5*s[0];
	double AxisYMin = h[0]-5*s[0];
	double AxisYH   = (AxisYMax - AxisYMin);
	std::cout<<AxisYMin<<"\t"<<AxisYMax<<"\n";

	Def = new TGraphErrors(mean.size(),&runId[0],&mean[0],&x_err[0],&mean_err[0]);
	if(par==1)Def->GetYaxis()->SetTitle(Form("<m^{2}>, GeV^{2}/c^{4}"));
	if(par==2)Def->GetYaxis()->SetTitle(Form("#sigma_{m^{2}}"));
	Def->GetXaxis()->SetTitle(Form("%s",pr1->GetXaxis()->GetTitle()));
	Def->GetYaxis()->SetTitleOffset(1);
	Def->GetYaxis()->SetRangeUser(AxisYMin,AxisYMax);
	Def->SetLineColor(4);
	Def->SetMarkerStyle(8);
	Def->SetMarkerColor(4);
	auto max = (Def->GetYaxis()->GetXmax()>AxisYMax ? Def->GetYaxis()->GetXmax() : AxisYMax);
	auto min = (Def->GetYaxis()->GetXmin()<AxisYMin ? Def->GetYaxis()->GetXmin() : AxisYMin);
	Def->GetYaxis()->SetRangeUser(min, max);

	TH2D *h2_axis = new TH2D("","",2,RunIdMin,RunIdMax,2,min,max);
	h2_axis->GetYaxis()->SetTitle(Form("Mean %s",Def->GetYaxis()->GetTitle()));
	h2_axis->GetXaxis()->SetTitle(Form("%s",Def->GetXaxis()->GetTitle()));
	h2_axis->Draw();
	Def->Draw("sameP");

	double xMin = Def->GetXaxis()->GetXmin();
	double xH =  (Def->GetXaxis()->GetXmax() - xMin);

	BadRunsGr = new TGraphErrors(BadRuns_mean.size(),&BadRuns_runId[0],&BadRuns_mean[0],&BadRuns_x_err[0],&BadRuns_mean_err[0]);
	BadRunsGr->SetLineColor(2);
	BadRunsGr->SetMarkerStyle(8);
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
   	for(auto dd : BadRuns_runId){
   		BadRuns = BadRuns + Form("%.0f",dd) + ", ";
   	}
   	std::cout<<BadRuns<<std::endl;

	can->SaveAs(Form("./%s/m2_proton/%s_%i.png",path_save.c_str(), pr1->GetName(), par));

}

void RunByRunPionM2(DrawParameters obj_par, TFile *inFile, string _TOF, string _charge, float min_p, float max_p, int pid, int par, std::string path_save){

	int GetParFit = (pid==2212 ? 4 : 1);
	if(par==2) GetParFit = GetParFit + 1;

	TH2D *pr1 = GetTH2D_M2(obj_par, inFile, _TOF, _charge, min_p, max_p);
	if(pr1==nullptr) return;

	TCanvas *can = new TCanvas(Form("RunId_%s_%i",pr1->GetName(),par),"",960,560);
	TCanvas *can_fit = new TCanvas(Form("fit_RunId_%s_8050_%i",pr1->GetName(),par),"",960,860);
	can->cd();
	gSystem->Exec(Form("mkdir -p ./%s/m2_pion",path_save.c_str()));

	TGraphErrors *Def;
	TGraphErrors *BadRunsGr;
	TGraphErrors *AfterCutMeanErr;

	int nx = pr1->GetXaxis()->GetNbins();
	std::vector<double> x_err;
	std::vector<double> runId;
	std::vector<double> mean;
	std::vector<double> mean_err; //""
	std::vector<double> sigma; //"s"

	std::vector<double> BadRuns_x_err;
	std::vector<double> BadRuns_runId;
	std::vector<double> BadRuns_mean;
	std::vector<double> BadRuns_mean_err; //""
	std::vector<double> BadRuns_sigma; //"s"

	TH1D *hist;

	for(int i=0; i<nx; i++){
		
		int h2RunId = pr1->GetXaxis()->GetBinCenter(i);
		if(h2RunId<RunIdStart)continue;

		hist = (TH1D*)pr1->ProjectionY(Form("run_%s_%i",pr1->GetName(),i),pr1->GetXaxis()->FindBin(h2RunId),pr1->GetXaxis()->FindBin(h2RunId));
		if(hist->GetEntries()==0) continue;

		auto gaus_pion = new TF1(Form("f1_pion_%s_%i",pr1->GetName(),i),"gaus(0)",-0.5,0.4);
		gaus_pion->SetLineColor(kRed);
		gaus_pion->SetParameter(0,100);
		gaus_pion->SetParameter(1,-0.01);
		gaus_pion->SetParameter(2,0.01);
		hist->Fit(gaus_pion,"QRM");	

		auto gaus_proton = new TF1(Form("f1_pr_%s_%i",pr1->GetName(),i),"gaus(0)",0.6,1.4);
		gaus_proton->SetParameter(0,100);
		gaus_proton->SetParameter(1,PROTON_M*PROTON_M);
		gaus_proton->SetParameter(2,0.13);
		hist->Fit(gaus_proton,"QRM");
		
		auto gaus_all = new TF1(Form("f1_pr_%s_%i",pr1->GetName(),i),"gaus(0)+gaus(3)",-0.6,1.5);
		gaus_all->SetLineColor(kRed);
		gaus_all->SetParameter(0,gaus_pion->GetParameter(0));
		gaus_all->SetParameter(1,gaus_pion->GetParameter(1));
		gaus_all->SetParameter(2,gaus_pion->GetParameter(2));
		gaus_all->SetParameter(3,gaus_proton->GetParameter(0));
		gaus_all->SetParameter(4,gaus_proton->GetParameter(1));
		gaus_all->SetParameter(5,gaus_proton->GetParameter(2));
		hist->Fit(gaus_all,"QRM");

		//std::cout<<gaus_all->GetParameter(GetParFit)<<"\t"<<gaus_all->GetParError(GetParFit)<<"\n";
		x_err.push_back(0.0);
		runId.push_back(h2RunId);
		mean.push_back(gaus_pion->GetParameter(GetParFit));
		mean_err.push_back( gaus_pion->GetParError(GetParFit) );

		if(h2RunId==7801 && par==1){
			can_fit->cd();
			auto hist2 = (TH1D*)pr1->ProjectionY(Form("run_%s_%i",pr1->GetName(),i),pr1->GetXaxis()->FindBin(h2RunId),pr1->GetXaxis()->FindBin(h2RunId));
			hist2->SetLineColor(kBlue);
			hist2->GetXaxis()->SetRangeUser(-0.5,2.);
			hist2->Fit(gaus_pion,"QRM");
			can_fit->SaveAs(Form("./%s/m2_pion/Fit8050_%s_%i.png",path_save.c_str(), pr1->GetName(), par));
		}

		//sigma.push_back( 	pr1->GetBinError(i) * sqrt(pr1->GetBinEntries(i)) );
		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<"\t"<<sigma.back()<<"\n";
	}

	can->cd();

	if(mean.size()==0) return;

	double h[2]; // mean
	double s[2]; // std deviations
	double e[2]; // mean err
	parRunByRun(runId,mean,h[0],s[0],e[0]);
	parRunByRun(runId,mean_err,h[1],s[1],e[1]);	
	std::cout<<mean.size()<<"\t"<<h[0]<<"\t"<<s[0]<<"\t"<<e[0]<<"\n";
	std::cout<<mean.size()<<"\t"<<h[1]<<"\t"<<s[1]<<"\t"<<e[1]<<"\n";

	for(int i=0; i<mean.size(); i++){
		double lo = h[0] - nSigma * s[0];
		double hi = h[0] + nSigma * s[0];
		double loStdD = h[1] - nSigma * s[1];
		double hiStdD = h[1] + nSigma * s[1];
		
		if(mean[i] > hi || mean[i] < lo){
		//if(mean[i] > hi || mean[i] < lo || mean_err[i] > hiStdD || mean_err[i] < loStdD){
			BadRuns_x_err.push_back(0.);
			BadRuns_runId.push_back(runId[i]);
			BadRuns_mean.push_back(mean[i]);
			BadRuns_mean_err.push_back(mean_err[i]); //""
			std::cout<<"badRuns\n";
		}
	}
	
	double AxisYMax = h[0]+5*s[0];
	double AxisYMin = h[0]-5*s[0];
	double AxisYH   = (AxisYMax - AxisYMin);


	Def = new TGraphErrors(mean.size(),&runId[0],&mean[0],&x_err[0],&mean_err[0]);
	Def->GetYaxis()->SetTitle(Form("<%s>, GeV^{2}/c^{4}",pr1->GetYaxis()->GetTitle()));
	Def->GetXaxis()->SetTitle(Form("%s",pr1->GetXaxis()->GetTitle()));
	Def->GetYaxis()->SetRangeUser(AxisYMin,AxisYMax);
	Def->SetLineColor(4);
	Def->SetMarkerStyle(8);
	Def->SetMarkerColor(4);
	auto max = Def->GetYaxis()->GetXmax();
	auto min = Def->GetYaxis()->GetXmin();
	//Def->GetYaxis()->SetRangeUser((min<AxisYMin ? min : AxisYMin), (max>AxisYMax ? max : AxisYMax));
	Def->Draw("Ap");

	double xMin = Def->GetXaxis()->GetXmin();
	double xH =  (Def->GetXaxis()->GetXmax() - xMin);

	BadRunsGr = new TGraphErrors(BadRuns_mean.size(),&BadRuns_runId[0],&BadRuns_mean[0],&BadRuns_x_err[0],&BadRuns_mean_err[0]);
	BadRunsGr->SetLineColor(2);
	BadRunsGr->SetMarkerStyle(8);
	BadRunsGr->SetMarkerColor(2);
	BadRunsGr->Draw("Psame");

	TLine *lineMean = new TLine(Def->GetXaxis()->GetXmin(),h[0],Def->GetXaxis()->GetXmax(),h[0]);
	lineMean->SetLineStyle(2);
	lineMean->Draw("same");

	TLine *lineUp = new TLine(Def->GetXaxis()->GetXmin(),nSigma*s[0] + h[0],Def->GetXaxis()->GetXmax(),nSigma*s[0] + h[0]);
	lineUp->SetLineColor(kRed);
	lineUp->SetLineStyle(1);
	lineUp->Draw("same");

	TLine *lineDown = new TLine(Def->GetXaxis()->GetXmin(), -nSigma*s[0] + h[0],Def->GetXaxis()->GetXmax(),-nSigma*s[0] + h[0]);
	lineDown->SetLineColor(kRed);
	lineDown->SetLineStyle(1);
	lineDown->Draw("same");

	std::string BadRuns = Form("%s: ",pr1->GetName());
   	for(auto dd : BadRuns_runId){
   		BadRuns = BadRuns + Form("%.0f",dd) + ", ";
   	}
   	std::cout<<BadRuns<<std::endl;

	can->SaveAs(Form("./%s/m2_pion/%s_%i.png",path_save.c_str(), pr1->GetName(), par));

}


void DrawHistoQA_run8_new(){	
	
	makeplotstyle();
	TFile *fstyle = new TFile("./style.root");
	TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
	//tsty->cd();
 	gROOT->SetStyle("Pub");
	gROOT->ForceStyle();

	TFile *file_inFile 		= new TFile("qa_run8_vf_24.04_final.root","READ");	
	TFile *file_outFile = new TFile("./OUT_24.04_new.root","RECREATE");	

	// RunByRun
	std::string path = "QA_RunByRun_24.04_new";
	gSystem->Exec(Form("mkdir -p ./%s/",path.c_str()));
	gSystem->Exec(Form("mkdir -p ./%s/Note",path.c_str()));
	gSystem->Exec(Form("mkdir -p ./%s/H1/",path.c_str()));
	gSystem->Exec(Form("mkdir -p ./%s/H2/",path.c_str()));
	gSystem->Exec(Form("mkdir -p ./%s/H2_NoRuns/",path.c_str()));

	TProfile *pr;
	TH1D *h1;
	TH2D *h2;

	std::string suf = "cut1_";
	std::vector<DrawParameters> DrawObjectParH2RunId;
  
	DrawObjectParH2RunId.push_back({"h2_RunId_nTracks",suf,0,4,3.,{},{0,500},1});                                 	
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_n_tracks",suf,0,4,3.,{},{0,200},1});                                                             
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_chi2",suf,0,4,3.,{},{0,1000},1});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_ndf",suf,0,4,3.,{},{0,200},1});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_chi2_ndf",suf,0,4,3.,{},{0,10},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_x",suf,0,4,3.,{},{-4,4}});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_y",suf,0,4,3.,{},{-4,4}});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_z",suf,0,4,3.,{},{-4,4}});                                                                  
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_nhits",suf,0,4,3.,{},{0,20},0});                                                                
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_eta",suf,0,4,3.,{},{},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_phi",suf,0,4,3.,{},{},0});                              
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_p",suf,0,4,3.,{},{0,40},1});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_px",suf,0,4,3.,{},{-4.,4.},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_py",suf,0,4,3.,{},{-4.,4.},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_pz",suf,0,4,3.,{},{0.,6.},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_pT",suf,0,4,3.,{},{0,2},0});                                                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_chi2_ndf",suf,0,4,3.,{},{0,10},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_dca_r",suf,0,4,3.,{},{0,5.},0});                                                                                                   
/*
	DrawObjectParH2RunId.push_back({"h2_RunId_nStsTracks",suf,0,4,3.,{},{0,500},1});                                 	
	DrawObjectParH2RunId.push_back({"h2_RunId_sts_nhits",suf,0,4,3.,{},{0,20},0});                                                                
	DrawObjectParH2RunId.push_back({"h2_RunId_sts_eta",suf,0,4,3.,{},{},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_sts_phi",suf,0,4,3.,{},{},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_sts_p",suf,0,4,3.,{},{},1});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_sts_px",suf,0,4,3.,{},{},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_sts_py",suf,0,4,3.,{},{},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_sts_pz",suf,0,4,3.,{},{},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_sts_pT",suf,0,4,3.,{},{},1});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_sts_chi2_ndf",suf}); 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_dca_r",suf,0,4,3.,{},{0,5.},0});                                                                                                   
*/
	DrawObjectParH2RunId.push_back({"h2_RunId_bdNum",suf,0,4,3.,{},{0,70},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_bdNumSingle",suf,0,4,3.,{},{0,70},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_nbdMod",suf,0,4,3.,{},{0,41},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_nbdModSingle",suf,0,4,3.,{},{0,41},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_bc1sInt",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_fdInt",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_fdAmp",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_bc1sAmp",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_vcsAmp",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_vcsInt",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_fhcal_e",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_scwall_q",suf,0,4,3.,{},{0,600},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_hodo_q",suf,0,4,3.,{},{},1});

/*
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hitMult_st0",suf});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hitMult_st1",suf});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hitMult_st2",suf});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_x_st0",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_y_st0",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_x_st1",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_y_st1",suf,0,4,2,{},{-5,5}});	
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_x_st2",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_y_st2",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_x",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_y",suf,0,4,2,{},{-5,5}});
*/
	DrawObjectParH2RunId.push_back({"h2_RunId_gemDigits",suf,0,4,3.,{},{0,6000},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_stsDigits",suf,0,4,3.,{},{0,6000},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_tof400Digits",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_tof700Digits",suf,0,4,3.,{},{},1});

	for(auto obj : DrawObjectParH2RunId)
	{
		RunByRunAnalis(file_inFile, obj, path, file_outFile);
		
		h2 = GetTH2(file_inFile,obj);
		if(h2==nullptr){
			std::cout<<"Error!"<<std::endl;
			continue;
		}
		auto *can = new TCanvas("","",960,560);
		can->cd();
		h2->Draw("colz");
		can->SetLogz();		
		can->SaveAs(Form("./%s/H2/%s.png",path.c_str(), h2->GetName()));
	}


	// Draw TH2
	std::vector<DrawParameters> DrawObjectParH2;
	
	DrawObjectParH2.push_back({"h2_bc1sInt_fdInt",suf,0,4,3.,{0,300000},{0,200000}});
	//DrawObjectParH2.push_back({"h2_bc1sIntNorm_fdIntNorm","cut1_",0,4,3.,{},{}});
	//DrawObjectParH2.push_back({"h2_bc1sInt_fdInt","cut2_",0,4,3.,{0,300000},{0,200000}});
	//DrawObjectParH2.push_back({"h2_bc1sIntNorm_fdIntNorm","cut2_",0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_nTracks_stsDigits",suf,0,4,3.,{0,400},{0,4000}});
	DrawObjectParH2.push_back({"h2_nTracks_gemDigits",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_nTracks_stsDigits","cut5_",0,4,3.,{0,400},{0,4000}});
	DrawObjectParH2.push_back({"h2_nTracks_gemDigits","cut5_",0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_nTracks_stsDigits","cut6_",0,4,3.,{0,400},{0,4000}});
	DrawObjectParH2.push_back({"h2_nTracks_gemDigits","cut6_",0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_bc1sIntNorm_fdIntNorm",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_nTracks_tof400Matched",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_nTracks_tof700Matched",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_nTracks_tof400Digits",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_nTracks_tof700Digits",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_tr_phi_tr_eta",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_tr_pT_tr_eta",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_tr_pT_tr_phi",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_tr_pq_tr_m2_tof400",suf,0,4,3.,{-4,5},{-2,6}});
	DrawObjectParH2.push_back({"h2_tr_pq_tr_m2_tof700",suf,0,4,3.,{-4,5},{-2,6}});
	DrawObjectParH2.push_back({"h2_beam_hit_x_st0_beam_hit_y_st0",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_beam_hit_x_st1_beam_hit_y_st1",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_beam_hit_x_st2_beam_hit_y_st2",suf,0,4,3.,{},{}});
	DrawObjectParH2.push_back({"h2_beam_x_beam_y",suf,0,4,3.,{},{}});

	for(auto obj : DrawObjectParH2)
	{
		h2 = GetTH2(file_inFile,obj);
		if(h2==nullptr){
			std::cout<<"Error!"<<std::endl;
			continue;
		}
		auto *can = new TCanvas("","",960,760);
		can->cd();
		can->SetRightMargin(0.12);
		if(obj.Name=="h2_nTracks_stsDigits")h2->GetYaxis()->SetTitle("FSD digits"); // опечатка, подправить STS (cbm) -> FSD (bmn)
		h2->Draw("colz");
		can->SetLogz();
		
		can->SaveAs(Form("./%s/H2_NoRuns/%s.png",path.c_str(), h2->GetName()));
	}

/*
	// Mass sqare
	std::vector<DrawParameters> DrawObjectParM2;
	DrawObjectParM2.push_back({"h2_RunId_tr_","ddM2P_"});//pr

	RunByRunM2Proton(DrawObjectParM2[0], file_inFile, "trM2Tof400","_P", 0.5, 2.0, 2212, 1, path); // Proton m2
	RunByRunM2Proton(DrawObjectParM2[0], file_inFile, "trM2Tof400","_P", 0.5, 2.0, 2212, 2, path); // Proton m2
	RunByRunM2Proton(DrawObjectParM2[0], file_inFile, "trM2Tof700","_P", 0.5, 2.0, 2212, 1, path); // Proton m2
	RunByRunM2Proton(DrawObjectParM2[0], file_inFile, "trM2Tof700","_P", 0.5, 2.0, 2212, 2, path); // Proton m2
	RunByRunPionM2(DrawObjectParM2[0], file_inFile, "trM2Tof400","_P", 0.2, 1.0, 211,  1, path); // Pion m2
	RunByRunPionM2(DrawObjectParM2[0], file_inFile, "trM2Tof400","_P", 0.2, 1.0, 211,  2, path); // Pion m2
	RunByRunPionM2(DrawObjectParM2[0], file_inFile, "trM2Tof400","_M", 0.2, 2.0, 211,  1, path); // Pion m2
	RunByRunPionM2(DrawObjectParM2[0], file_inFile, "trM2Tof400","_M", 0.2, 2.0, 211,  2, path); // Pion m2
	RunByRunPionM2(DrawObjectParM2[0], file_inFile, "trM2Tof700","_P", 0.2, 1.0, 211,  1, path); // Pion m2
	RunByRunPionM2(DrawObjectParM2[0], file_inFile, "trM2Tof700","_P", 0.2, 1.0, 211,  2, path); // Pion m2
	RunByRunPionM2(DrawObjectParM2[0], file_inFile, "trM2Tof700","_M", 0.2, 2.0, 211,  1, path); // Pion m2
	RunByRunPionM2(DrawObjectParM2[0], file_inFile, "trM2Tof700","_M", 0.2, 2.0, 211,  2, path); // Pion m2
*/
	// Output Bad Runs
	PrintBadRuns(file_inFile,suf);

	file_outFile->Close();
	file_inFile->Close();


}
