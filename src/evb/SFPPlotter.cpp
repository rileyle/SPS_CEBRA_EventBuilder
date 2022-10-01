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
			
			// for(int i=0; i<5; i++) {
       		// 	if(ev.cebraE[i] != -1 && ev.x1 != -1e6 && ev.x2 != -1e6){
			// 		Double_t cebraRelT = ev.cebraTime[i] - ev.anodeBackTime;
			// 		Double_t cebraRelT_toScint = ev.cebraTime[i] - ev.scintLeftTime;
			// 		MyFill(table,"xavg_cebrafcoinc_NoCuts",600,-300,300, ev.xavg);
			// 		MyFill(table,"cebraRelTime_NoCuts",3000,-15000,15000, cebraRelT); // was 1000, -3000, 3500
			// 		MyFill(table,"cebraRelTime_toScint_NoCuts",3000,-15000,15000,cebraRelT_toScint);
			// 		MyFill(table,"cebraRelRTScint_cebraRelRTAnode_NoCuts",3000,-15000,15000,cebraRelT_toScint,3000,-15000,15000,cebraRelT);
			// 		MyFill(table,"cebraRelRTScint_cebraChannel_NoCuts",3000,-15000,15000,cebraRelT_toScint,144,0,144,ev.cebraChannel[i]);
			// 		MyFill(table,"cebraRelRTAnode_cebraChannel_NoCuts",3000,-15000,15000,cebraRelT,144,0,144,ev.cebraChannel[i]);
			// 		MyFill(table,"cebraRelRTScint_anodeRelT_NoCuts",3000,-15000,15000,cebraRelT_toScint,3000,-15000,15000,anodeRelBT);


			// 		const char *cebrai_RelTime_toScint[] = {"cebra0_RelTime_toScint_noCuts",
			// 												"cebra1_RelTime_toScint_noCuts",
			// 												"cebra2_RelTime_toScint_noCuts",
			// 												"cebra3_RelTime_toScint_noCuts",
			// 												"cebra4_RelTime_toScint_noCuts"};

			// 		MyFill(table,cebrai_RelTime_toScint[i],3000,-15000,15000,cebraRelT_toScint);


			// 	}
			// }

			MyFill(table,"anodeBackRelTime_toScint",1000,-3000,3500,anodeRelBT);
			MyFill(table,"delayRelBackTime_toScint",1000,-3000,3500,delayRelBT_toScint);
			MyFill(table,"delayRelFrontTime_toScint",1000,-3000,3500,delayRelFT_toScint);
			} else {
			MyFill(table,"noscinttime_counter_NoCuts",2,0,1,1); 
			}

	
		/*
			//plots for CeBrA that are relative to Detector 0
			for(int i=1; i<5; i++) {

				double cebra_tShift[4] = {	3.0, 	//detector 1 relative to detector 0
											2.0, 	//detector 2 relative to detector 0
											2.5, 	//detector 3 relative to detector 0
											33.5	//detector 4 relative to detector 0			
				};


				// Fills plots with the full CeBrA Coinciedence window
				const char *cebra_TimeDif_0_i[] = {	"cebraTime0_cebraTime1_noCuts",
													"cebraTime0_cebraTime2_noCuts",
													"cebraTime0_cebraTime3_noCuts",
													"cebraTime0_cebraTime4_noCuts"};

				const char *cebra_E0_Ei_noCuts[] = {"cebra_E0_E1_noCuts",
													"cebra_E0_E2_noCuts",
													"cebra_E0_E3_noCuts",
													"cebra_E0_E4_noCuts"};

				if(ev.cebraE[0]!=-1 && ev.cebraE[i]!=-1){

					MyFill(table,cebra_TimeDif_0_i[i-1],3000,-3000,3000,ev.cebraTime[0]-ev.cebraTime[i]);
					MyFill(table,cebra_E0_Ei_noCuts[i-1],1024,0,4096,ev.cebraE[0],1024,0,4096,ev.cebraE[i]);

				}

				// int tWindow = 20;	//half of the time gate								

				// // Fills plots with some time gate and shift
				// const char *cebra_E0_Ei_cal_TGate_noCuts[] = {	"cebra_E0_E1_cal_TGate_noCuts",
				// 												"cebra_E0_E2_cal_TGate_noCuts",
				// 												"cebra_E0_E3_cal_TGate_noCuts",
				// 												"cebra_E0_E4_cal_TGate_noCuts"};

				// const char *cebra_T0_Ti_TGate_shifted_noCuts[] = {	"cebra_Time0-Time1_TGate_shifted_noCuts",
				// 													"cebra_Time0-Time2_TGate_shifted_noCuts",
				// 													"cebra_Time0-Time3_TGate_shifted_noCuts",
				// 													"cebra_Time0-Time4_TGate_shifted_noCuts"};

				// const char *cebra_Ei_cebraT0_Ti_TGate_shifted_noCuts[] = {	"cebra_E1_vs_T0_T1_TGate_shifted_noCuts",
				// 															"cebra_E2_vs_T0_T2_TGate_shifted_noCuts",
				// 															"cebra_E3_vs_T0_T3_TGate_shifted_noCuts",
				// 															"cebra_E4_vs_T0_T4_TGate_shifted_noCuts"};

			
				// Double_t cebra_TimeDif_0_i_shift = ev.cebraTime[0]-ev.cebraTime[i]+cebra_tShift[i-1];

				// if(cebra_TimeDif_0_i_shift > -tWindow && cebra_TimeDif_0_i_shift < tWindow){
				// 	if(ev.cebraE[0]!=-1 && ev.cebraE[i]!=-1){

				// 		MyFill(table,cebra_E0_Ei_cal_TGate_noCuts[i-1],4096,0,4096,ev.cebraE[0],4096,0,4096,ev.cebraE[i]);
				// 		MyFill(table,cebra_T0_Ti_TGate_shifted_noCuts[i-1],tWindow*2,-tWindow,tWindow,cebra_TimeDif_0_i_shift);
				// 		MyFill(table,cebra_Ei_cebraT0_Ti_TGate_shifted_noCuts[i-1],tWindow*2,-tWindow,tWindow,cebra_TimeDif_0_i_shift,4096,0,4096,ev.cebraE[i]);
						
				// 	}
				// }


			} */
		

			for(int i=0; i<5; i++){
				if(ev.cebraE[i] != -1) {
					
					//run 82 to 123 --> 12C shifts
					// double cebraE_ADC_Shift[5] = {	1.15672328675213*ev.cebraE[0]	-16.6919499054403,
					// 									1.3021940822188	*ev.cebraE[1]	-17.935158566721,
					// 									1.14400591545883*ev.cebraE[2]	-2.44528388259982,
					// 									1.13872748785169*ev.cebraE[3]	-5.59919193459234,
					// 									1.0				*ev.cebraE[4]	-0};


					// double cebraE_ADC_Shift[5] = {	1*ev.cebraE[0]+0,
					// 								1*ev.cebraE[1]+0,
					// 								1*ev.cebraE[2]+0,
					// 								1*ev.cebraE[3]+0,
					// 								1*ev.cebraE[4]+0};


					//run 162-183
					double cebraE_ADC_Shift[5] = {1.14759314191149	*ev.cebraE[0]-33.6481331086578,
											1.13138046963966	*ev.cebraE[1]-38.3795474557953,
											1.10901267613132	*ev.cebraE[2]-4.52471201077913,
											1.10283776422464	*ev.cebraE[3]-14.7328089971076,
											1	*ev.cebraE[4]+0};


					const char *cebraEi_noCuts[] = {"cebraE0_noCuts","cebraE1_noCuts","cebraE2_noCuts","cebraE3_noCuts","cebraE4_noCuts"};
					
					MyFill(table,cebraEi_noCuts[i],1024,0,4096,ev.cebraE[i]);

					const char *cebraEi_Shifted_noCuts[] = {"cebraE0_Shifted_noCuts","cebraE1_Shifted_noCuts","cebraE2_Shifted_noCuts","cebraE3_Shifted_noCuts","cebraE4_Shifted_noCuts"};
					
					MyFill(table,cebraEi_Shifted_noCuts[i],1024,0,4096,cebraE_ADC_Shift[i]);


					// MyFill(table,"cebraE_noCuts",1024,0,4096,ev.cebraE[i]);
					// MyFill(table,"cebraChannel_cebraE_noCuts",20,0,20,ev.cebraChannel[i],1024,0,4096,ev.cebraE[i]);

					// if(ev.x1 != -1e6 && ev.x2 == -1e6 && ev.cebraE[i] !=-1){

					// 	const char *x1_cebraEi_noCuts[] = {	"x1_cebraE0_noCuts",
					// 										"x1_cebraE1_noCuts",
					// 										"x1_cebraE2_noCuts",
					// 										"x1_cebraE3_noCuts",
					// 										"x1_cebraE4_noCuts"};

					// 	MyFill(table,x1_cebraEi_noCuts[i],600,-300,300,ev.x1,1024,0,1024,ev.cebraE[i]);
					// 	MyFill(table,"x1_cebraE_noCuts",600,-300,300,ev.x1,1024,0,1024,ev.cebraE[i]);
					//}

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
			
			//aligned all the the peaks in det 4 no cuts

			//run 82 to 123 --> 12C shifts
			// double cebraE_ADC_Shift[5] = {	1.15672328675213*ev.cebraE[0]	-16.6919499054403,
			// 								1.3021940822188	*ev.cebraE[1]	-17.935158566721,
			// 								1.14400591545883*ev.cebraE[2]	-2.44528388259982,
			// 								1.13872748785169*ev.cebraE[3]	-5.59919193459234,
			// 								1.0				*ev.cebraE[4]	-0};

			//run 140 - 
			// double cebraE_ADC_Shift[5] = {	1			*ev.cebraE[0]+0,
			// 								0.948342641	*ev.cebraE[1]-0.696411121,
			// 								0.95950388	*ev.cebraE[2]+2.965761686,
			// 								0.958168245	*ev.cebraE[3]+2.109440511,
			// 								0.914171408	*ev.cebraE[4]-5.19425177};

			//run162-183
				double cebraE_ADC_Shift[5] = {1.14759314191149	*ev.cebraE[0]-33.6481331086578,
											1.13138046963966	*ev.cebraE[1]-38.3795474557953,
											1.10901267613132	*ev.cebraE[2]-4.52471201077913,
											1.10283776422464	*ev.cebraE[3]-14.7328089971076,
											1	*ev.cebraE[4]+0};




			
			for(int i=0; i<5; i++) {
       			if(ev.cebraE[i] != -1 && ev.x1 != -1e6 && ev.x2 != -1e6){

					Double_t cebraRelT = ev.cebraTime[i] - ev.anodeBackTime;
					Double_t cebraRelT_toScint = ev.cebraTime[i] - ev.scintLeftTime;
					MyFill(table,"xavg_cebrafcoinc_Cut",600,-300,300, ev.xavg);
					MyFill(table,"cebraRelTime_Cut",3000,-15000,15000, cebraRelT); // was 1000, -3000, 3500
					MyFill(table,"cebraRelTime_toScint_Cut",3000,-15000,15000,cebraRelT_toScint);
					MyFill(table,"cebraRelRTScint_cebraRelRTAnode_Cut",3000,-15000,15000,cebraRelT_toScint,3000,-15000,15000,cebraRelT);
					// MyFill(table,"cebraRelRTScint_cebraChannel_Cut",3000,-15000,15000,cebraRelT_toScint,144,0,144,ev.cebraChannel[i]);
					// MyFill(table,"cebraRelRTAnode_cebraChannel_Cut",3000,-15000,15000,cebraRelT,144,0,144,ev.cebraChannel[i]);
					MyFill(table,"cebraRelRTScint_anodeRelT_Cut",3000,-15000,15000,cebraRelT_toScint,3000,-15000,15000,anodeRelBT);

					const char *cebraEi_Cut[] = {"cebraE0_Cut","cebraE1_Cut","cebraE2_Cut","cebraE3_Cut","cebraE4_Cut"};
					
					Double_t cebraRelT_i_toScint = ev.cebraTime[i] - ev.scintLeftTime;

					MyFill(table,cebraEi_Cut[i],1024,0,4096,ev.cebraE[i]);
					MyFill(table,"cebraE_Cut",1024,0,4096,ev.cebraE[i]);

					const char *cebraEi_ADC_Shift_Cut[] = {	"cebraE0_ADC_Shift_Cut",
															"cebraE1_ADC_Shift_Cut",
															"cebraE2_ADC_Shift_Cut",
															"cebraE3_ADC_Shift_Cut",
															"cebraE4_ADC_Shift_Cut"};

					MyFill(table,cebraEi_ADC_Shift_Cut[i],1024,0,4096,cebraE_ADC_Shift[i]);
					MyFill(table,"cebraE_ADC_Shift_Cut",1024,0,4096,cebraE_ADC_Shift[i]);

					if(ev.x1 != -1e6 && ev.x2 == -1e6 && ev.cebraE[i] !=-1){

						const char *xavg_cebraEi_Cut[] = {	"xavg_cebraE0_Cut",
															"xavg_cebraE1_Cut",
															"xavg_cebraE2_Cut",
															"xavg_cebraE3_Cut",
															"xavg_cebraE4_Cut"};

						MyFill(table,xavg_cebraEi_Cut[i],600,-300,300,ev.xavg,1024,0,4096,ev.cebraE[i]);
						MyFill(table,"xavg_cebraE_Cut",600,-300,300,ev.xavg,1024,0,4096,ev.cebraE[i]);
					}

					// if(ev.x1 != -1e6 && ev.cebraE[i] !=-1){

					// 	const char *x1_cebraEi_Cut[] = {	"x1_cebraE0_Cut",
					// 										"x1_cebraE1_Cut",
					// 										"x1_cebraE2_Cut",
					// 										"x1_cebraE3_Cut",
					// 										"x1_cebraE4_Cut"};

					// 	MyFill(table,x1_cebraEi_Cut[i],600,-300,300,ev.x1,1024,0,4096,ev.cebraE[i]);
					// 	MyFill(table,"x1_cebraE_Cut",600,-300,300,ev.x1,1024,0,4096,ev.cebraE[i]);
					// }


					const char *cebrai_RelTime_toScint[] = {"cebra0_RelTime_toScint_Cut",
															"cebra1_RelTime_toScint_Cut",
															"cebra2_RelTime_toScint_Cut",
															"cebra3_RelTime_toScint_Cut",
															"cebra4_RelTime_toScint_Cut"
					};

					const char *cebrai_RelTime_toScint_theta[] = { "cebra0_RelTime_toScint_theta_Cut",
																   "cebra1_RelTime_toScint_theta_Cut",
																   "cebra2_RelTime_toScint_theta_Cut",
																   "cebra3_RelTime_toScint_theta_Cut",
																   "cebra4_RelTime_toScint_theta_Cut"
					};

					MyFill(table,cebrai_RelTime_toScint[i],8000,-4000,4000,cebraRelT_toScint);

					MyFill(table,cebrai_RelTime_toScint_theta[i],8000,-4000,4000,cebraRelT_toScint,100,0,TMath::Pi()/2.,ev.theta);

					//Not Shifted Time Cut Code
					// if(cebraRelT_toScint > -750 && cebraRelT_toScint < -600){

					// 	const char *cebraE_i_TimeCut_ProtonCut[] = {	"cebraE_0_TimeCut_ProtonCut",
					// 													"cebraE_1_TimeCut_ProtonCut",
					// 													"cebraE_2_TimeCut_ProtonCut",
					// 													"cebraE_3_TimeCut_ProtonCut",
					// 													"cebraE_4_TimeCut_ProtonCut"};

					// 	const char *xavg_cebraEi_TimeCut_ProtonCut[] = {"xavg_cebraE0_TimeCut_ProtonCut",
					// 													"xavg_cebraE1_TimeCut_ProtonCut",
					// 													"xavg_cebraE2_TimeCut_ProtonCut",
					// 													"xavg_cebraE3_TimeCut_ProtonCut",
					// 													"xavg_cebraE4_TimeCut_ProtonCut"};

					// 	const char *x1_cebraEi_TimeCut_ProtonCut[] = {	"x1_cebraE0_TimeCut_ProtonCut",
					// 													"x1_cebraE1_TimeCut_ProtonCut",
					// 													"x1_cebraE2_TimeCut_ProtonCut",
					// 													"x1_cebraE3_TimeCut_ProtonCut",
					// 													"x1_cebraE4_TimeCut_ProtonCut"};

					// 	const char *cebrai_RelTime_toScint_TimeCut_ProtonCut[] = {	"cebra0_RelTime_toScint_TimeCut_ProtonCut",
					// 																"cebra1_RelTime_toScint_TimeCut_ProtonCut",
					// 																"cebra2_RelTime_toScint_TimeCut_ProtonCut",
					// 																"cebra3_RelTime_toScint_TimeCut_ProtonCut",
					// 																"cebra4_RelTime_toScint_TimeCut_ProtonCut"};

					// 	MyFill(table,"xavg_TimeCut_ProtonCut",600,-300,300,ev.xavg);
					// 	MyFill(table,cebraE_i_TimeCut_ProtonCut[i],1024,0,4096,ev.cebraE[i]);
					// 	MyFill(table,xavg_cebraEi_TimeCut_ProtonCut[i],600,-300,300,ev.xavg,1024,0,4096,ev.cebraE[i]);
					// 	MyFill(table,x1_cebraEi_TimeCut_ProtonCut[i],600,-300,300,ev.x1,1024,0,4096,ev.cebraE[i]);
					// 	MyFill(table,cebrai_RelTime_toScint_TimeCut_ProtonCut[i],8000,-4000,4000,cebraRelT_toScint);

					// } 




					//RelTime to Scint Shifts
					double cebra_RelTime_Shift[5] = {687.0,    //12C shifts
													685.0,
													685.0,
													685.0,
													655.0};

					// double cebra_RelTime_Shift[5] = {672.0,    //49Ti
					// 								 670.0,
					// 								 670.0,
					// 								 670.0,
					// 								 640.0};

					double cebra_RelTime_Width = 20.0;
					double cebraRelT_toScint_Shifted = cebraRelT_toScint + cebra_RelTime_Shift[i];


					if(cebraRelT_toScint_Shifted > -cebra_RelTime_Width && cebraRelT_toScint_Shifted < cebra_RelTime_Width){

						const char *cebraE_i_TimeCut_Shifted_ProtonCut[] = {"cebraE_0_TimeCut_Shifted_ProtonCut",
																			"cebraE_1_TimeCut_Shifted_ProtonCut",
																			"cebraE_2_TimeCut_Shifted_ProtonCut",
																			"cebraE_3_TimeCut_Shifted_ProtonCut",
																			"cebraE_4_TimeCut_Shifted_ProtonCut"};

						const char *xavg_cebraEi_TimeCut_Shifted_ProtonCut[] = {"xavg_cebraE0_TimeCut_Shifted_ProtonCut",
																				"xavg_cebraE1_TimeCut_Shifted_ProtonCut",
																				"xavg_cebraE2_TimeCut_Shifted_ProtonCut",
																				"xavg_cebraE3_TimeCut_Shifted_ProtonCut",
																				"xavg_cebraE4_TimeCut_Shifted_ProtonCut"};

						const char *x1_cebraEi_TimeCut_Shifted_ProtonCut[] = {	"x1_cebraE0_TimeCut_Shifted_ProtonCut",
																				"x1_cebraE1_TimeCut_Shifted_ProtonCut",
																				"x1_cebraE2_TimeCut_Shifted_ProtonCut",
																				"x1_cebraE3_TimeCut_Shifted_ProtonCut",
																				"x1_cebraE4_TimeCut_Shifted_ProtonCut"};

						// const char *x1_cebraEi_TimeCut_ProtonCut[] = {	"x1_cebraE0_TimeCut_ProtonCut",
						// 														"x1_cebraE1_TimeCut_ProtonCut",
						// 														"x1_cebraE2_TimeCut_ProtonCut",
						// 														"x1_cebraE3_TimeCut_ProtonCut",
						// 														"x1_cebraE4_TimeCut_ProtonCut"};

						const char *cebrai_RelTime_toScint_TimeCut_Shifted_ProtonCut[] = {	"cebra0_RelTime_toScint_TimeCut_Shifted_ProtonCut",
																							"cebra1_RelTime_toScint_TimeCut_Shifted_ProtonCut",
																							"cebra2_RelTime_toScint_TimeCut_Shifted_ProtonCut",
																							"cebra3_RelTime_toScint_TimeCut_Shifted_ProtonCut",
																							"cebra4_RelTime_toScint_TimeCut_Shifted_ProtonCut"};

						MyFill(table,"xavg_TimeCut_ProtonCut",600,-300,300,ev.xavg);
						MyFill(table,cebraE_i_TimeCut_Shifted_ProtonCut[i],1024,0,4096,ev.cebraE[i]);
						MyFill(table,xavg_cebraEi_TimeCut_Shifted_ProtonCut[i],600,-300,300,ev.xavg,1024,0,4096,ev.cebraE[i]);
						MyFill(table,x1_cebraEi_TimeCut_Shifted_ProtonCut[i],600,-300,300,ev.x1,1024,0,4096,ev.cebraE[i]);
						MyFill(table,cebrai_RelTime_toScint_TimeCut_Shifted_ProtonCut[i],400,-100,100,cebraRelT_toScint_Shifted);


						//Energy calibrated on the ground state band
						MyFill(table,"AA_xavg_cebraE_TimeCut_ProtonCut",600,-300,300,ev.xavg,512,0,4096,cebraE_ADC_Shift[i]);
						MyFill(table,"AA_x1_cebraE_TimeCut_ProtonCut",600,-300,300,ev.x1,512,0,4096,cebraE_ADC_Shift[i]);

						MyFill(table,"AA_xavg_cebraE_TimeCut_Cal_ProtonCut",600,-300,300,ev.xavg,2000,0,10000,2.230153412*cebraE_ADC_Shift[i]+25.09473527);
						MyFill(table,"AA_x1_cebraE_TimeCut_Cal_ProtonCut",600,-300,300,ev.x1,2000,0,10000,2.230153412*cebraE_ADC_Shift[i]+25.09473527);



						if(i==0){
							for(int j=0; j<4; j++) {
								MyFill(table,"cebraE_zony_cebraE4_TimeCut_Cal_ProtonCut",2000,0,10000,2.230153412*cebraE_ADC_Shift[j]+25.09473527,2000,0,10000,2.230153412*cebraE_ADC_Shift[4]+25.09473527);

								MyFill(table,"AA_xavg_cebraE_zony_TimeCut_ProtonCut",600,-300,300,ev.xavg,512,0,4096,cebraE_ADC_Shift[j]);
								MyFill(table,"AA_x1_cebraE_zony_TimeCut_ProtonCut",600,-300,300,ev.x1,512,0,4096,cebraE_ADC_Shift[j]);
							}
						}

					} 

					MyFill(table,"AA_xavg_cebraE_ProtonCut",600,-300,300,ev.xavg,512,0,4096,cebraE_ADC_Shift[i]);

				}



				
			}


			for(int j=0; j<4; j++) {
				MyFill(table,"cebraE_zony_cebraE4_Cal_ProtonCut",2000,0,10000,2.230153412*cebraE_ADC_Shift[j]+25.09473527,2000,0,10000,2.230153412*cebraE_ADC_Shift[4]+25.09473527);

				}

			

	
			//plots for CeBrA that are relative to Detector 0
			for(int i=1; i<5; i++) {

				double cebra_tShift[4] = {	3.0, 	//detector 1 relative to detector 0
											2.0, 	//detector 2 relative to detector 0
											2.5, 	//detector 3 relative to detector 0
											33.5	//detector 4 relative to detector 0		
				};

				// Fills plots with the full CeBrA Coinciedence window
				const char *cebra_TimeDif_0_i[] = {	"cebraTime0_cebraTime1_Cut",
													"cebraTime0_cebraTime2_Cut",
													"cebraTime0_cebraTime3_Cut",
													"cebraTime0_cebraTime4_Cut"};

				const char *cebra_E0_Ei_Cut[] = {	"cebra_E0_E1_Cut",
													"cebra_E0_E2_Cut",
													"cebra_E0_E3_Cut",
													"cebra_E0_E4_Cut"};

				if(ev.cebraE[0]!=-1 && ev.cebraE[i]!=-1){

					MyFill(table,cebra_TimeDif_0_i[i-1],3200,-1600,1600,ev.cebraTime[0]-ev.cebraTime[i]);
					MyFill(table,cebra_E0_Ei_Cut[i-1],1024,0,4096,ev.cebraE[0],1024,0,4096,ev.cebraE[i]);

				}

			}
			


			for(int i=0; i<5; i++){
				if(ev.cebraE[i] != -1) {

					const char *cebraEi_Cut[] = {"cebraE0_Cut","cebraE1_Cut","cebraE2_Cut","cebraE3_Cut","cebraE4_Cut"};
					
					// Double_t cebraRelT_i_toScint = ev.cebraTime[i] - ev.scintLeftTime;

					// MyFill(table,cebraEi_Cut[i],1024,0,4096,ev.cebraE[i]);
					// MyFill(table,"cebraE_Cut",1024,0,4096,ev.cebraE[i]);
					// MyFill(table,"cebraChannel_cebraE_Cut",20,0,20,ev.cebraChannel[i],1024,0,4096,ev.cebraE[i]);

					if(ev.x1 != -1e6 && ev.x2 == -1e6 && ev.cebraE[i] !=-1){

						const char *xavg_cebraEi_Cut[] = {	"xavg_cebraE0_Cut",
															"xavg_cebraE1_Cut",
															"xavg_cebraE2_Cut",
															"xavg_cebraE3_Cut",
															"xavg_cebraE4_Cut"};

						MyFill(table,xavg_cebraEi_Cut[i],600,-300,300,ev.xavg,1024,0,4096,ev.cebraE[i]);
						MyFill(table,"xavg_cebraE_Cut",600,-300,300,ev.xavg,1024,0,4096,ev.cebraE[i]);
					}

					if(ev.x1 != -1e6 && ev.cebraE[i] !=-1){

						const char *x1_cebraEi_Cut[] = {	"x1_cebraE0_Cut",
															"x1_cebraE1_Cut",
															"x1_cebraE2_Cut",
															"x1_cebraE3_Cut",
															"x1_cebraE4_Cut"};

						MyFill(table,x1_cebraEi_Cut[i],600,-300,300,ev.x1,1024,0,4096,ev.cebraE[i]);
						MyFill(table,"x1_cebraE_Cut",600,-300,300,ev.x1,1024,0,4096,ev.cebraE[i]);
					}

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