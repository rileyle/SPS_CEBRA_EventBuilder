/*SFPPlotter.h
 *Class for generating histogram files for SPS-CEBRA data
 *Intended use case is generating a TChain of multiple analyzed files and making
 *histograms of the larger data set.
 *
 *Created Jan 2020 by GWM
 */

#include "EventBuilder.h"
#include "SFPPlotter.h"
#include <TSystem.h>

namespace EventBuilder {

	/*Generates storage and initializes pointers*/
	SFPPlotter::SFPPlotter() :
		event_address(new ProcessedEvent()), m_progressFraction(0.1)
	{
	}
	
	SFPPlotter::~SFPPlotter() 
	{
		delete event_address;
	}
	
	/*2D histogram fill wrapper*/
	void SFPPlotter::MyFill(THashTable* table, const std::string& name, int binsx, double minx, double maxx, double valuex,
							int binsy, double miny, double maxy, double valuey)
	{
		TH2F *histo = (TH2F*) table->FindObject(name.c_str());
		if(histo != nullptr) 
			histo->Fill(valuex, valuey);
		else
		{
			TH2F *h = new TH2F(name.c_str(), name.c_str(), binsx, minx, maxx, binsy, miny, maxy);
			h->Fill(valuex, valuey);
			table->Add(h);
		}
	}
	
	/*1D histogram fill wrapper*/
	void SFPPlotter::MyFill(THashTable* table, const std::string& name, int binsx, double minx, double maxx, double valuex)
	{
		TH1F *histo = (TH1F*) table->FindObject(name.c_str());
		if(histo != nullptr)
			histo->Fill(valuex);
		else 
		{
			TH1F *h = new TH1F(name.c_str(), name.c_str(), binsx, minx, maxx);
			h->Fill(valuex);
			table->Add(h);
		}
	}
	
	/*Makes histograms where only rejection is unset data*/
	void SFPPlotter::MakeUncutHistograms(const ProcessedEvent& ev, THashTable* table)
	{
		MyFill(table,"x1NoCuts_bothplanes",600,-300,300,ev.x2);
		MyFill(table,"x2NoCuts_bothplanes",600,-300,300,ev.x2);
		MyFill(table,"xavgNoCuts_bothplanes",600,-300,300,ev.xavg);
		MyFill(table,"xavgNoCuts_theta_bothplanes",600,-300,300,ev.xavg,100,0,TMath::Pi()/2.,ev.theta);
		
		MyFill(table,"x1_delayBackRightE_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.delayBackRightE);
		MyFill(table,"x2_delayBackRightE_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.delayBackRightE);
		MyFill(table,"xavg_delayBackRightE_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.delayBackRightE);
		MyFill(table,"x1_x2_NoCuts",600,-300,300,ev.x1,600,-300,300,ev.x2);
	
		Double_t delayBackAvgE = (ev.delayBackRightE+ev.delayBackLeftE)/2.0;
		MyFill(table,"x1_delayBackAvgE_NoCuts",600,-300,300,ev.x1,512,0,4096,delayBackAvgE);
		MyFill(table,"x2_delayBackAvgE_NoCuts",600,-300,300,ev.x2,512,0,4096,delayBackAvgE);
		MyFill(table,"xavg_delayBackAvgE_NoCuts",600,-300,300,ev.xavg,512,0,4096,delayBackAvgE);
		Double_t delayFrontAvgE = (ev.delayFrontRightE+ev.delayFrontLeftE)/2.0;
		MyFill(table,"x1_delayFrontAvgE_NoCuts",600,-300,300,ev.x1,512,0,4096,delayFrontAvgE);
		MyFill(table,"x2_delayFrontAvgE_NoCuts",600,-300,300,ev.x2,512,0,4096,delayFrontAvgE);
		MyFill(table,"xavg_delayFrontAvgE_NoCuts",600,-300,300,ev.xavg,512,0,4096,delayFrontAvgE);
	
		MyFill(table,"scintLeft_anodeBack_NoCuts",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeBack);
		MyFill(table,"scintLeft_anodeFront_NoCuts",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeFront);
		MyFill(table,"scintLeft_cathode_NoCuts",512,0,4096,ev.scintLeft,512,0,4096,ev.cathode);
	
		MyFill(table,"x1_scintLeft_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.scintLeft);
		MyFill(table,"x2_scintLeft_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.scintLeft);
		MyFill(table,"xavg_scintLeft_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.scintLeft);
	
		MyFill(table,"x1_anodeBack_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.anodeBack);
		MyFill(table,"x2_anodeBack_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.anodeBack);
		MyFill(table,"xavg_anodeBack_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.anodeBack);
	
		MyFill(table,"x1_anodeFront_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.anodeFront);
		MyFill(table,"x2_anodeFront_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.anodeFront);
		MyFill(table,"xavg_anodeFront_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.anodeFront);
	
		MyFill(table,"x1_cathode_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.cathode);
		MyFill(table,"x2_cathode_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.cathode);
		MyFill(table,"xavg_cathode_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.cathode);
	
		/****Timing relative to back anode****/
		if(ev.anodeBackTime != -1 && ev.scintLeftTime != -1)
		{
			Double_t anodeRelFT = ev.anodeFrontTime - ev.anodeBackTime;
			Double_t delayRelFT = ev.delayFrontMaxTime - ev.anodeBackTime;
			Double_t delayRelBT = ev.delayBackMaxTime - ev.anodeBackTime;
			Double_t anodeRelBT = ev.anodeBackTime - ev.scintLeftTime;
			Double_t delayRelFT_toScint = ev.delayFrontMaxTime - ev.scintLeftTime;
			Double_t delayRelBT_toScint = ev.delayBackMaxTime - ev.scintLeftTime;
			MyFill(table,"anodeRelFrontTime_NoCuts",1000,-3000,3500, anodeRelFT);
			MyFill(table,"delayRelFrontTime_NoCuts",1000,-3000,-3500,delayRelFT);
			MyFill(table,"delayRelBackTime_NoCuts",1000,-3000,-3500,delayRelBT);

	
			for(int i=0; i<5; i++) {
       			if(ev.cebraE[i] != -1 && ev.x1 != -1e6 && ev.x2 != -1e6){

					std::string I = std::to_string(i);


					//Cebra Relative Time to Left Scint Plots
					Double_t cebraRelT_toScint = ev.cebraTime[i] - ev.scintLeftTime;
					std::string cebra_reltime_toscint = "cebra_RelTime_toScint_" + I + "_noCuts";
					const char *cebra_RelTime_toScint_I_noCuts = cebra_reltime_toscint.c_str();	
					MyFill(table,cebra_RelTime_toScint_I_noCuts,8000,-4000,4000,cebraRelT_toScint);
					// MyFill(table,"cebra_RelTime_toScint_noCuts",8000,-4000,4000,cebraRelT_toScint);


					//Cebra Energy Plot with no Cuts
					std::string cebra_E = "cebra_E_"+ I + "_noCuts"; 
					const char *cebra_E_I_noCuts = cebra_E.c_str();	
					MyFill(table,cebra_E_I_noCuts,1024,0,4096,ev.cebraE[i]);


					//Cebra ADC Channel shift (to make all the detectors line up with each other) with the corresponding plots
					//good for run 82 - 123
					double cebra_E_ADCShift[5] = {	1.15672328675213*ev.cebraE[0]	-16.6919499054403,
													1.3021940822188	*ev.cebraE[1]	-17.935158566721,
													1.14400591545883*ev.cebraE[2]	-2.44528388259982,
													1.13872748785169*ev.cebraE[3]	-5.59919193459234,
													1.0				*ev.cebraE[4]	-0};

					std::string cebra_E_i_ADCShift = "cebra_E_"+ I + "_ADCShift_noCuts"; 
					const char *cebra_E_I_ADCShift_noCuts = cebra_E_i_ADCShift.c_str();	
					MyFill(table,cebra_E_I_ADCShift_noCuts,1024,0,4096,cebra_E_ADCShift[i]);
					MyFill(table,"cebra_E_ADCShift_noCuts",1024,0,4096,cebra_E_ADCShift[i]);


					// //CeBrA vs x1 with no cuts
					// std::string x1_cebraE_i_noCuts = "x1_cebraE_" + I + "_noCuts";
					// const char *x1_cebraE_I_noCuts = x1_cebraE_i_noCuts.c_str();
					// MyFill(table,x1_cebraE_I_noCuts,600,-300,300,ev.x1,1024,0,1024,ev.cebraE[i]);
					// MyFill(table,"x1_cebraE_noCuts",600,-300,300,ev.x1,1024,0,1024,ev.cebraE[i]);

				}
			}

			MyFill(table,"anodeBackRelTime_toScint",1000,-3000,3500,anodeRelBT);
			MyFill(table,"delayRelBackTime_toScint",1000,-3000,3500,delayRelBT_toScint);
			MyFill(table,"delayRelFrontTime_toScint",1000,-3000,3500,delayRelFT_toScint);
			} else {
			MyFill(table,"noscinttime_counter_NoCuts",2,0,1,1); 
			}


		if(ev.x1 != -1e6 && ev.x2 == -1e6)
			MyFill(table,"x1NoCuts_only1plane",600,-300,300,ev.x1);
		else if(ev.x2 != -1e6 && ev.x1 == -1e6)
			MyFill(table,"x2NoCuts_only1plane",600,-300,300,ev.x2);
		else if(ev.x1 == -1e6 && ev.x2 == -1e6)
			MyFill(table,"nopos_counter",2,0,1,1);
	}
	
	/*Makes histograms with cuts & gates implemented*/
	void SFPPlotter::MakeCutHistograms(const ProcessedEvent& ev, THashTable* table) 
	{
		if(!cutter.IsInside(&ev)) 
			return;
	
		MyFill(table,"x1_bothplanes_Cut",600,-300,300,ev.x1);
		MyFill(table,"x2_bothplanes_Cut",600,-300,300,ev.x2);
		MyFill(table,"xavg_bothplanes_Cut",600,-300,300,ev.xavg);
		MyFill(table,"x1_x2_Cut",600,-300,300,ev.x1, 600,-300,300,ev.x2);
		MyFill(table,"xavg_theta_Cut_bothplanes",600,-300,300,ev.xavg,100,0,TMath::Pi()/2.,ev.theta);
		
		MyFill(table,"x1_delayBackRightE_Cut",600,-300,300,ev.x1,512,0,4096,ev.delayBackRightE);
		MyFill(table,"x2_delayBackRightE_Cut",600,-300,300,ev.x2,512,0,4096,ev.delayBackRightE);
		MyFill(table,"xavg_delayBackRightE_Cut",600,-300,300,ev.xavg,512,0,4096,ev.delayBackRightE);
	
		Double_t delayBackAvgE = (ev.delayBackRightE+ev.delayBackLeftE)/2.0;
		MyFill(table,"x1_delayBackAvgE_Cut",600,-300,300,ev.x1,512,0,4096,delayBackAvgE);
		MyFill(table,"x2_delayBackAvgE_Cut",600,-300,300,ev.x2,512,0,4096,delayBackAvgE);
		MyFill(table,"xavg_delayBackAvgE_Cut",600,-300,300,ev.xavg,512,0,4096,delayBackAvgE);
		Double_t delayFrontAvgE = (ev.delayFrontRightE+ev.delayFrontLeftE)/2.0;
		MyFill(table,"x1_delayFrontAvgE_Cut",600,-300,300,ev.x1,512,0,4096,delayFrontAvgE);
		MyFill(table,"x2_delayFrontAvgE_Cut",600,-300,300,ev.x2,512,0,4096,delayFrontAvgE);
		MyFill(table,"xavg_delayFrontAvgE_Cut",600,-300,300,ev.xavg,512,0,4096,delayFrontAvgE);
	
		MyFill(table,"scintLeft_anodeBack_Cut",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeBack);
		MyFill(table,"scintLeft_anodeFront_Cut",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeFront);
		MyFill(table,"scintLeft_cathode_Cut",512,0,4096,ev.scintLeft,512,0,4096,ev.cathode);
	
		MyFill(table,"x1_scintLeft_Cut",600,-300,300,ev.x1,512,0,4096,ev.scintLeft);
		MyFill(table,"x2_scintLeft_Cut",600,-300,300,ev.x2,512,0,4096,ev.scintLeft);
		MyFill(table,"xavg_scintLeft_Cut",600,-300,300,ev.xavg,512,0,4096,ev.scintLeft);
	
		MyFill(table,"x1_anodeBack_Cut",600,-300,300,ev.x1,512,0,4096,ev.anodeBack);
		MyFill(table,"x2_anodeBack_Cut",600,-300,300,ev.x2,512,0,4096,ev.anodeBack);
		MyFill(table,"xavg_anodeBack_Cut",600,-300,300,ev.xavg,512,0,4096,ev.anodeBack);
		
		MyFill(table,"x1_anodeFront_Cut",600,-300,300,ev.x1,512,0,4096,ev.anodeFront);
		MyFill(table,"x2_anodeFront_Cut",600,-300,300,ev.x2,512,0,4096,ev.anodeFront);
		MyFill(table,"xavg_anodeFront_Cut",600,-300,300,ev.xavg,512,0,4096,ev.anodeFront);
		
		MyFill(table,"x1_cathode_Cut",600,-300,300,ev.x1,512,0,4096,ev.cathode);
		MyFill(table,"x2_cathode_Cut",600,-300,300,ev.x2,512,0,4096,ev.cathode);
		MyFill(table,"xavg_cathode_Cut",600,-300,300,ev.xavg,512,0,4096,ev.cathode);
	
		/****Timing relative to back anode****/
		if(ev.anodeBackTime != -1 && ev.scintLeftTime != -1) 
		{
			Double_t anodeRelFT = ev.anodeFrontTime - ev.anodeBackTime;
			Double_t anodeRelBT = ev.anodeBackTime - ev.anodeBackTime;
			Double_t anodeRelFT_toScint = ev.anodeFrontTime-ev.scintLeftTime;
			MyFill(table,"anodeRelBackTime_Cut",1000,-3000,3500, anodeRelBT);
			MyFill(table,"anodeRelFrontTime_Cut",1000,-3000,3500, anodeRelFT);
			MyFill(table,"anodeRelTime_toScint_Cut",1000,-3000,3500,anodeRelFT_toScint);
			
			//aligned all the the peaks in det 4 no cuts
			//good for run 82 - 123
			double cebra_E_ADCShift[5] = {	1.15672328675213*ev.cebraE[0]	-16.6919499054403,
											1.3021940822188	*ev.cebraE[1]	-17.935158566721,
											1.14400591545883*ev.cebraE[2]	-2.44528388259982,
											1.13872748785169*ev.cebraE[3]	-5.59919193459234,
											1.0				*ev.cebraE[4]	-0};

			
			for(int i=0; i<5; i++) {
       			if(ev.cebraE[i] != -1 && ev.x1 != -1e6 && ev.x2 != -1e6){

					std::string I = std::to_string(i);

					//CeBrA Relative Time to Left Scint Cut Plots
					Double_t cebraRelT_toScint = ev.cebraTime[i] - ev.scintLeftTime;

					std::string cebra_reltime_toscint = "cebra_RelTime_toScint_" + I + "_Cut";
					const char *cebra_RelTime_toScint_I_Cut = cebra_reltime_toscint.c_str();	
					MyFill(table,cebra_RelTime_toScint_I_Cut,8000,-4000,4000,cebraRelT_toScint);
					// MyFill(table,"cebra_RelTime_toScint_Cut",8000,-4000,4000,cebraRelT_toScint);

					//CeBrA Energy Spectrum with Cut
					std::string cebra_E = "cebra_E_"+ I + "_Cut"; 
					const char *cebra_E_I_Cut = cebra_E.c_str();	
					MyFill(table,cebra_E_I_Cut,1024,0,4096,ev.cebraE[i]);

					//CeBrA plots with ADC Shift and cut
					std::string cebra_E_i_ADCShift = "cebra_E_"+ I + "_ADCShift_Cut"; 
					const char *cebra_E_I_ADCShift_Cut = cebra_E_i_ADCShift.c_str();	
					MyFill(table,cebra_E_I_ADCShift_Cut,1024,0,4096,cebra_E_ADCShift[i]);
					MyFill(table,"cebra_E_ADCShift_Cut",1024,0,4096,cebra_E_ADCShift[i]);

					//CeBrA energy vs xavg without time cut
					std::string xavg_cebraE_i = "xavg_cebraE_" + I + "_Cut";
					const char *xavg_cebraE_I_Cut = xavg_cebraE_i.c_str();
					MyFill(table,xavg_cebraE_I_Cut,600,-300,300,ev.xavg,1024,0,4096,ev.cebraE[i]);
					MyFill(table,"xavg_cebraE_Cut",600,-300,300,ev.xavg,1024,0,4096,ev.cebraE[i]);

					//CeBrA energy vs x1
					std::string x1_cebraE_i = "x1_cebraE_" + I + "_Cut";
					const char *x1_cebraE_I_Cut = x1_cebraE_i.c_str();
					MyFill(table,x1_cebraE_I_Cut,600,-300,300,ev.x1,1024,0,4096,ev.cebraE[i]);
					MyFill(table,"x1_cebraE_Cut",600,-300,300,ev.x1,1024,0,4096,ev.cebraE[i]);



					//RelTime to Left Scint Shifts (shifts peak to 0 for each detector)
					//adjust accordingly using the "cebra_RelTime_toScint_I" plots
					double cebra_RelTime_toScint_Shift[5] = {	687.0,  
																685.0,
																685.0,
																685.0,
																655.0};

					double cebra_RelTime_Width = 5.0;
					double cebraRelT_toScint_Shifted = cebraRelT_toScint + cebra_RelTime_toScint_Shift[i];

					if(cebraRelT_toScint_Shifted > -cebra_RelTime_Width && cebraRelT_toScint_Shifted < cebra_RelTime_Width){
						
						
						MyFill(table,"xavg_TimeCut_ProtonCut",600,-300,300,ev.xavg);

						//CeBrA energy with the RelTime toScint Cut
						std::string cebra_E_i_TimeCutShift_Cut = "cebra_E_" + I + "_TimeCutShift_Cut";
						const char *cebra_E_I_TimeCutShift_Cut = cebra_E_i_TimeCutShift_Cut.c_str();
						MyFill(table,cebra_E_I_TimeCutShift_Cut,1024,0,4096,ev.cebraE[i]);

						//CeBrA vs xavg with the Time Cut applied
						std::string xavg_cebraE_i_TimeCutShift_Cut = "xavg_cebraE_" + I + "_TimeCutShift_Cut";
						const char *xavg_cebraE_I_TimeCutShift_Cut = xavg_cebraE_i_TimeCutShift_Cut.c_str();
						MyFill(table,xavg_cebraE_I_TimeCutShift_Cut,600,-300,300,ev.xavg,1024,0,4096,ev.cebraE[i]);

						//CeBrA vs x1 with the Time Cut applied
						std::string x1_cebraE_i_TimeCutShift_Cut = "x1_cebraE_" + I + "_TimeCutShift_Cut";
						const char *x1_cebraE_I_TimeCutShift_Cut = x1_cebraE_i_TimeCutShift_Cut.c_str();
						MyFill(table,x1_cebraE_I_TimeCutShift_Cut,600,-300,300,ev.x1,1024,0,4096,ev.cebraE[i]);

						//Plot to show the relative time shift (should be centered on 0)
						std::string cebra_reltime_toscint_TimeCutShift_Cut = "cebra_RelTime_toScint_" + I + "TimeCutShift_Cut";
						const char *cebra_RelTime_toScint_I_TimeCutShift_Cut = cebra_reltime_toscint_TimeCutShift_Cut.c_str();	
						MyFill(table,cebra_RelTime_toScint_I_TimeCutShift_Cut,400,-100,100,cebraRelT_toScint_Shifted);


						//All the detectors summed using the ADC Shift values
						MyFill(table,"AA_xavg_cebraE_Sum_TimeCutShift_Cut",600,-300,300,ev.xavg,512,0,4096,cebra_E_ADCShift[i]);
						MyFill(table,"AA_x1_cebraE_Sum_TimeCutShift_Cut",600,-300,300,ev.x1,512,0,4096,cebra_E_ADCShift[i]);

						//Energy Calibrated to the GS band 
						double cebra_E_EnergyCalibrated = 2.230153412*cebra_E_ADCShift[i]+25.09473527;
						MyFill(table,"AA_xavg_cebraE_Sum_EnergyCalibrated_TimeCutShift_Cut",600,-300,300,ev.xavg,2000,0,10000,cebra_E_EnergyCalibrated);
						MyFill(table,"AA_x1_cebraE_Sum_EnergyCalibrated_TimeCutShift_Cut",600,-300,300,ev.x1,2000,0,10000,cebra_E_EnergyCalibrated);


						//Zony plots (small CeBr3 detectors, small zebras, like a small horses or a pony)
						if(i==0){
							for(int j=0; j<4; j++) {
								//gamma-gamma coincience matrix with the adult vs zonys
								MyFill(table,"cebraE_zony_cebraE4_TimeCut_Cut",512,0,4096,cebra_E_ADCShift[j],512,0,4096,cebra_E_ADCShift[4]);

								MyFill(table,"AA_xavg_cebraE_zony_TimeCutShift_Cut",600,-300,300,ev.xavg,512,0,4096,cebra_E_ADCShift[j]);
								MyFill(table,"AA_x1_cebraE_zony_TimeCutShift_Cut",600,-300,300,ev.x1,512,0,4096,cebra_E_ADCShift[j]);
							}
						}

					} 

					//CeBrA vs xavg without the time cut
					MyFill(table,"AA_xavg_cebraE_Sum_Cut",600,-300,300,ev.xavg,512,0,4096,cebra_E_ADCShift[i]);

				}



				
			}


			//plots for CeBrA that are relative to Detector 0
			for(int i=1; i<5; i++) {
				// Fills plots with the full CeBrA Coinciedence window
				const char *cebra_TimeDif_0_i[] = {	"cebraTime0_cebraTime1_Cut",
													"cebraTime0_cebraTime2_Cut",
													"cebraTime0_cebraTime3_Cut",
													"cebraTime0_cebraTime4_Cut"};

				if(ev.cebraE[0]!=-1 && ev.cebraE[i]!=-1){
					MyFill(table,cebra_TimeDif_0_i[i-1],5000,-15000,15000,ev.cebraTime[0]-ev.cebraTime[i]);
				}
			}


			

		}

	}
	
	/*Runs a list of files given from a RunCollector class*/
	void SFPPlotter::Run(const std::vector<std::string>& files, const std::string& output)
	{
		TFile *outfile = TFile::Open(output.c_str(), "RECREATE");
		TChain* chain = new TChain("SPSTree");
		for(unsigned int i=0; i<files.size(); i++)
			chain->Add(files[i].c_str()); 
		chain->SetBranchAddress("event", &event_address);
		THashTable* table = new THashTable();
	
		long blentries = chain->GetEntries();
		long count=0, flush_val=blentries*m_progressFraction, flush_count=0;
	
	
		for(long i=0; i<chain->GetEntries(); i++) 
		{
			count++;
			if(count == flush_val)
			{
				flush_count++;
				count=0;
				m_progressCallback(flush_count*flush_val, blentries);
			}
			chain->GetEntry(i);
			MakeUncutHistograms(*event_address, table);
			if(cutter.IsValid()) MakeCutHistograms(*event_address, table);
		}
		outfile->cd();
		table->Write();
		if(cutter.IsValid()) 
		{
			auto clist = cutter.GetCuts();
			for(unsigned int i=0; i<clist.size(); i++) 
			  clist[i]->Write();
		}
		delete table;
		outfile->Close();
		delete outfile;
	}

}