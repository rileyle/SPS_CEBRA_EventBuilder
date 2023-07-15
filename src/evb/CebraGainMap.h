/*
	CebraGainMap.h

	Class for reading, and retrieving CeBrA detector gain matching
	coefficients.

	Written by L.A.. Riley July 2023
*/
#ifndef CEBRAGAINMAP_H
#define CEBRAGAINMAP_H

namespace EventBuilder {

  struct GainShift
  {
    float t1;
    float t2;
    float m[5];
    float b[5];
  };
  
  class CebraGainMap
  {
  
  public:
    CebraGainMap();
    CebraGainMap(const std::string& filename);
    ~CebraGainMap();
    bool FillMap(const std::string& filename);
    float GetT1(int run, int range){ return m_map[run][range].t1; }
    float GetT2(int run, int range){ return m_map[run][range].t2; }
    float GetSlope(int run, int range, int det){
      return m_map[run][range].m[det];
    }
    float GetIntercept(int run, int range, int det){
      return m_map[run][range].b[det];
    }
    inline bool IsValid() { return m_validFlag; };
  
  private:
    std::unordered_map<int, std::unordered_map< int, GainShift> > m_map;
    bool m_validFlag;
  };
  
}
#endif
