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
					Double_t cebraRelT = ev.cebraTime[i] - ev.anodeBackTime;
					Double_t cebraRelT_toScint = ev.cebraTime[i] - ev.scintLeftTime;
					MyFill(table,"xavg_cebrafcoinc_NoCuts",600,-300,300, ev.xavg);
					MyFill(table,"cebraRelTime_NoCuts",3000,-15000,15000, cebraRelT); // was 1000, -3000, 3500
					MyFill(table,"cebraRelTime_toScint",3000,-15000,15000,cebraRelT_toScint);
					MyFill(table,"cebraRelRTScint_cebraRelRTAnode",3000,-15000,15000,cebraRelT_toScint,3000,-15000,15000,cebraRelT);
					MyFill(table,"cebraRelRTScint_cebraChannel",3000,-15000,15000,cebraRelT_toScint,144,0,144,ev.cebraChannel[i]);
					MyFill(table,"cebraRelRTAnode_cebraChannel",3000,-15000,15000,cebraRelT,144,0,144,ev.cebraChannel[i]);
					MyFill(table,"cebraRelRTScint_anodeRelT",3000,-15000,15000,cebraRelT_toScint,3000,-15000,15000,anodeRelBT);


					const char *cebrai_RelTime_toScint[] = {"cebra0_RelTime_toScint_noCuts",
															"cebra1_RelTime_toScint_noCuts",
															"cebra2_RelTime_toScint_noCuts",
															"cebra3_RelTime_toScint_noCuts",
															"cebra4_RelTime_toScint_noCuts"
					};
					MyFill(table,cebrai_RelTime_toScint[i],3000,-15000,15000,cebraRelT_toScint);


				}
			}

			MyFill(table,"anodeBackRelTime_toScint",1000,-3000,3500,anodeRelBT);
			MyFill(table,"delayRelBackTime_toScint",1000,-3000,3500,delayRelBT_toScint);
			MyFill(table,"delayRelFrontTime_toScint",1000,-3000,3500,delayRelFT_toScint);
			} else {
			MyFill(table,"noscinttime_counter_NoCuts",2,0,1,1); 
			}
	
		
			//plots for CeBrA that are relative to Detector 0
			for(int i=1; i<5; i++) {

				// Fills plots with the full CeBrA Coinciedence window

				const char *cebra_TimeDif_0_i[] = {	"cebraTime0-cebraTime1_noCuts",
													"cebraTime0-cebraTime2_noCuts",
													"cebraTime0-cebraTime3_noCuts",
													"cebraTime0-cebraTime4_noCuts"
				};

				if(ev.cebraE[0]!=-1 && ev.cebraE[i]!=-1){
					MyFill(table,cebra_TimeDif_0_i[i-1],3200,-1600,1600,ev.cebraTime[0]-ev.cebraTime[i]);
				}

				// Fills plots with some time gate and shift

				const char *cebra_E0_Ei_cal_TGate_noCuts[] = {	"cebra_E0_E1_cal_TGate_noCuts",
																"cebra_E0_E2_cal_TGate_noCuts",
																"cebra_E0_E3_cal_TGate_noCuts",
																"cebra_E0_E4_cal_TGate_noCuts"
				};

				const char *cebra_T0_Ti_TGate_shifted_noCuts[] = {	"cebra_Time0-Time1_TGate_shifted_noCuts",
																	"cebra_Time0-Time2_TGate_shifted_noCuts",
																	"cebra_Time0-Time3_TGate_shifted_noCuts",
																	"cebra_Time0-Time4_TGate_shifted_noCuts"
				};

				const char *cebra_Ei_cebraT0_Ti_TGate_shifted_noCuts[] = {	"cebra_E1_vs_T0-T1_TGate_shifted_noCuts",
																			"cebra_E2_vs_T0-T2_TGate_shifted_noCuts",
																			"cebra_E3_vs_T0-T3_TGate_shifted_noCuts",
																			"cebra_E4_vs_T0-T4_TGate_shifted_noCuts"
				};


				int tWindow = 20;					//half of the time gate								
				double cebra_tShift[4] = {	3.0, 	//detector 1 relative to detector 0
											3.0, 	//detector 2 relative to detector 0
											68.0, 	//detector 3 relative to detector 0
											80.0	//detector 4 relative to detector 0			
				};

				Double_t cebra_TimeDif_0_i_shift = ev.cebraTime[0]-ev.cebraTime[i]+cebra_tShift[i-1];

				if(cebra_TimeDif_0_i_shift > -tWindow && cebra_TimeDif_0_i_shift < tWindow){
					if(ev.cebraE[0]!=-1 && ev.cebraE[i]!=-1){

						MyFill(table,cebra_E0_Ei_cal_TGate_noCuts[i-1],4096,0,4096,ev.cebraE[0],4096,0,4096,ev.cebraE[i]);
						MyFill(table,cebra_T0_Ti_TGate_shifted_noCuts[i-1],tWindow*2,-tWindow,tWindow,cebra_TimeDif_0_i_shift);
						MyFill(table,cebra_Ei_cebraT0_Ti_TGate_shifted_noCuts[i-1],tWindow*2,-tWindow,tWindow,cebra_TimeDif_0_i_shift,4096,0,4096,ev.cebraE[i]);
					}
				}


			}


			for(int i=0; i<5; i++)
			{
				if(ev.cebraE[i] != -1) 
				{
					const char *cebraEi_noCuts[] = {"cebraE0_noCuts","cebraE1_noCuts","cebraE2_noCuts","cebraE3_noCuts","cebraE4_noCuts"};
					
					Double_t cebraRelT_i_toScint = ev.cebraTime[i] - ev.scintLeftTime;
					Double_t cebraE_cal=ev.cebraE[i]*2.6113656+5.621562404;

					MyFill(table,cebraEi_noCuts[i],4096,0,4096,ev.cebraE[i]);
					MyFill(table,"cebraE_NoCuts",4096,0,4096,ev.cebraE[i]);
					MyFill(table,"cebraChannel_cebraE_NoCuts",20,0,20,ev.cebraChannel[i],4096,0,4096,ev.cebraE[i]);

				}
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
			

		
			//plots for CeBrA that are relative to Detector 0
			for(int i=1; i<5; i++) {

				// Fills plots with the full CeBrA Coinciedence window

				const char *cebra_TimeDif_0_i[] = {	"cebraTime0-cebraTime1_Cut",
													"cebraTime0-cebraTime2_Cut",
													"cebraTime0-cebraTime3_Cut",
													"cebraTime0-cebraTime4_Cut"
				};

				if(ev.cebraE[0]!=-1 && ev.cebraE[i]!=-1){
					MyFill(table,cebra_TimeDif_0_i[i-1],3200,-1600,1600,ev.cebraTime[0]-ev.cebraTime[i]);
				}

				// Fills plots with some time gate and shift

				const char *cebra_E0_Ei_cal_TGate_Cut[] = {	"cebra_E0_E1_cal_TGate_Cut",
																"cebra_E0_E2_cal_TGate_Cut",
																"cebra_E0_E3_cal_TGate_Cut",
																"cebra_E0_E4_cal_TGate_Cut"
				};

				const char *cebra_T0_Ti_TGate_shifted_Cut[] = {	"cebra_Time0-Time1_TGate_shifted_Cut",
																	"cebra_Time0-Time2_TGate_shifted_Cut",
																	"cebra_Time0-Time3_TGate_shifted_Cut",
																	"cebra_Time0-Time4_TGate_shifted_Cut"
				};

				const char *cebra_Ei_cebraT0_Ti_TGate_shifted_Cut[] = {	"cebra_E1_vs_T0-T1_TGate_shifted_Cut",
																			"cebra_E2_vs_T0-T2_TGate_shifted_Cut",
																			"cebra_E3_vs_T0-T3_TGate_shifted_Cut",
																			"cebra_E4_vs_T0-T4_TGate_shifted_Cut"
				};


				int tWindow = 20;					//half of the time gate								
				double cebra_tShift[4] = {	3.0, 	//detector 1 relative to detector 0
											3.0, 	//detector 2 relative to detector 0
											68.0, 	//detector 3 relative to detector 0
											80.0	//detector 4 relative to detector 0			
				};

				Double_t cebra_TimeDif_0_i_shift = ev.cebraTime[0]-ev.cebraTime[i]+cebra_tShift[i-1];

				if(cebra_TimeDif_0_i_shift > -tWindow && cebra_TimeDif_0_i_shift < tWindow){
					if(ev.cebraE[0]!=-1 && ev.cebraE[i]!=-1){

						MyFill(table,cebra_E0_Ei_cal_TGate_Cut[i-1],4096,0,4096,ev.cebraE[0],4096,0,4096,ev.cebraE[i]);
						MyFill(table,cebra_T0_Ti_TGate_shifted_Cut[i-1],tWindow*2,-tWindow,tWindow,cebra_TimeDif_0_i_shift);
						MyFill(table,cebra_Ei_cebraT0_Ti_TGate_shifted_Cut[i-1],tWindow*2,-tWindow,tWindow,cebra_TimeDif_0_i_shift,4096,0,4096,ev.cebraE[i]);
					}
				}

			}
			


			for(int i=0; i<5; i++)
			{
				if(ev.cebraE[i] != -1) 
				{
					const char *cebraEi_Cut[] = {"cebraE0_Cut","cebraE1_Cut","cebraE2_Cut","cebraE3_Cut","cebraE4_Cut"};
					
					Double_t cebraRelT_i_toScint = ev.cebraTime[i] - ev.scintLeftTime;
					Double_t cebraE_cal=ev.cebraE[i]*2.6113656+5.621562404;

					MyFill(table,cebraEi_Cut[i],4096,0,4096,ev.cebraE[i]);
					MyFill(table,"cebraE_Cut",4096,0,4096,ev.cebraE[i]);
					MyFill(table,"cebraChannel_cebraE_Cut",20,0,20,ev.cebraChannel[i],4096,0,4096,ev.cebraE[i]);

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