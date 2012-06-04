/** \file TraceFilterer.h
 *  \brief Declaration of analysis class which does the filtering of traces.
 *
 *  David Miller, January 2011
 */

#ifndef __TRACEFILTERER_H_
#define __TRACEFILTERER_H_

#include <string>

#include "Trace.h"
#include "TracePlotter.h"

class TraceFilterer : public TracePlotter {
 private:
    static const std::string defaultFilterFile;

 protected:    
    static const int energyBins;
    static const double energyScaleFactor;

    TrapezoidalFilterParameters  fastParms;
    Trace::value_type            fastThreshold;
    TrapezoidalFilterParameters  energyParms;
    TrapezoidalFilterParameters  thirdParms;
    Trace::value_type            slowThreshold;

    Trace fastFilter;   ///< fast filter of trace
    Trace energyFilter; ///< slow filter of trace
    Trace thirdFilter;  ///< second slow filter of trace
    
    bool useThirdFilter;

    struct PulseInfo {
    public:
	Trace::size_type time;
	double energy;
	bool isFound;

	PulseInfo(void);
	PulseInfo(Trace::size_type theTime, double theEnergy);
    };
    PulseInfo pulse;

    virtual const PulseInfo& FindPulse(Trace::iterator begin, Trace::iterator end);
 public:
    TraceFilterer();
    virtual ~TraceFilterer();

    virtual bool Init(const std::string &filterFile = defaultFilterFile);
    virtual void DeclarePlots(void);
    virtual void Analyze(Trace &trace, 
			 const std::string &type, const std::string &subtype);
};

#endif // __TRACEFILTERER_H_