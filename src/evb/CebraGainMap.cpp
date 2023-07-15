/*
	CebraGainMap.cpp

	Class for reading, and retrieving CeBrA detector gain matching
	coefficients.

	Written by L.A.. Riley July 2023
*/
#include "EventBuilder.h"
#include "CebraGainMap.h"

namespace EventBuilder {
  
  CebraGainMap::CebraGainMap():
    m_validFlag(false)
  {
  }
	
  CebraGainMap::CebraGainMap(const std::string& name):
    m_validFlag(false)
  {
    FillMap(name);
  }
  
  CebraGainMap::~CebraGainMap() {}

  bool CebraGainMap::FillMap(const std::string& name)
  {
    std::ifstream input(name);
    if(!input.is_open()) 
      {
	m_validFlag = false;
	return m_validFlag;
      }

    int runNum, rangeNum;
    float t1, t2;
    std::string junk;
    GainShift g;
    while(input>>runNum>>rangeNum>>g.t1>>g.t2){
      for(int j=0; j<5; j++){
	input >> g.m[j] >> g.b[j];
	m_map[runNum][rangeNum] = g;
      }
    }

    input.close();
    
    m_validFlag = true;
    return m_validFlag;
  }
}
