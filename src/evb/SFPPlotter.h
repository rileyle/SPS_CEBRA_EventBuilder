/*SFPPlotter.h
 *Class for generating histogram files for SPS-SABRE data
 *Intended use case is generating a TChain of multiple analyzed files and making
 *histograms of the larger data set.
 *
 *Created Jan 2020 by GWM
 */

#ifndef SFPCLEANER_H
#define SFPCLEANER_H

#include "DataStructs.h"
#include "ProgressCallback.h"
#include "CutHandler.h"
#include "CebraGainMap.h"

namespace EventBuilder {

	class SFPPlotter 
	{
	public:
		SFPPlotter();
		~SFPPlotter();
		inline void ApplyCutlist(const std::string& listname) { cutter.SetCuts(listname); }
	        inline void ReadCebraGains(const std::string& name) { gains.FillMap(name); }
		void Run(const std::vector<std::string>& files, const std::string& output);
		inline void SetProgressCallbackFunc(const ProgressCallbackFunc& function) { m_progressCallback = function; }
		inline void SetProgressFraction(double frac) { m_progressFraction = frac; }
	
	private:
		void Chain(const std::vector<std::string>& files); //Form TChain
		void MakeUncutHistograms(const ProcessedEvent& ev, THashTable* table, int runNum);
                void MakeCutHistograms(const ProcessedEvent& ev, THashTable* table, int runNum);
	
		/*Histogram fill wrapper functions*/
		void MyFill(THashTable* table, const std::string& name, int binsx, double minx, double maxx, double valuex,
					int binsy, double miny, double maxy, double valuey);
		void MyFill(THashTable* table, const std::string& name, int binsx, double minx, double maxx, double valuex);
	
		ProcessedEvent *event_address;
	
		/*Cuts*/
		CutHandler cutter;

                CebraGainMap gains;
	  
		ProgressCallbackFunc m_progressCallback;
		double m_progressFraction;
	
	};

}

#endif
