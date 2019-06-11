/** \file YapProcessor.cpp
 *\brief A Template processor class that can be used to build your own.
 *\author S. V. Paulauskas
 *\date October 26, 2014
 */
#include <iostream>

#include "DammPlotIds.hpp"
#include "DetectorSummary.hpp"
#include "RawEvent.hpp"
#include "YapProcessor.hpp"
#include "RootDevProcessor.hpp"

#include "DetectorDriver.hpp"
#include "DetectorLibrary.hpp"
#include "HelperFunctions.hpp"

namespace dammIds {
    namespace pspmt {
        const int D_ENERGY = 0; //!< ID for the energy of the template detector
        const int DD_TEMPLATE_VS_PULSER = 1; //!< Energy Template vs. Energy Pulser
    }
}//namespace dammIds

using namespace std;
using namespace dammIds::pspmt;

YapProcessor::YapProcessor() : EventProcessor(OFFSET, RANGE, "TemplateProcessor") {
    associatedTypes.insert("yap");
    associatedTypes.insert("mcp");
}

YapProcessor::YapProcessor(const double &a) : EventProcessor(OFFSET, RANGE, "TemplateProcessor") {
    associatedTypes.insert("yap");
    a_ = a;
    associatedTypes.insert("mcp");

}

void YapProcessor::DeclarePlots(void) {
    DeclareHistogram1D(D_ENERGY, SA, "Energy");
    DeclareHistogram2D(DD_TEMPLATE_VS_PULSER, SA, SA, "Template Energy vs. Pulser Energy");
}

// for position calibrations                                                                       
const double parX[4] = {1.67e-8, -2.49e-5, 4.11e-2, -4.91};
const double parY[4] = {3.16e-8, -4.75e-5, 5.30e-2, -7.20};
//                                                               
const double parXIon[4] = {1.27e-8, -4.25e-5, 6.94e-2, -3.08e1};
const double parYIon[4] = {1.82e-8, -6.15e-5, 9.2e-2, -4.03e1};

bool YapProcessor::PreProcess(RawEvent &event) {
    if (!EventProcessor::PreProcess(event))
        return false;

	// get summary of events
    static const auto &yapevents = event.GetSummary("yap", true)->GetList();
	unsigned int dyMult = event.GetSummary("yap:dynode", true)->GetMult(); 
	bool isPileup = false; 
	unsigned int an0Mult = event.GetSummary("yap:anode0", true)->GetMult(); 
	unsigned int an1Mult = event.GetSummary("yap:anode1", true)->GetMult(); 
	unsigned int an2Mult = event.GetSummary("yap:anode2", true)->GetMult(); 
	unsigned int an3Mult = event.GetSummary("yap:anode3", true)->GetMult(); 
	bool isGoodYapEvent = false; 
	unsigned int mcpMult = event.GetSummary("mcp:1time", true)->GetMult(); 
	bool isIon = false; 

	if(dyMult > 1) {
		isPileup = true; 
	}
	if(an0Mult > 0 
	   && an1Mult > 0
	   && an2Mult > 0
	   && an3Mult > 0
	   && dyMult > 0) {
		isGoodYapEvent = true; 
	}
	if(mcpMult > 0) {
		isIon = true; 
	}

	double time = -999; 
	double dyEnergy = -999; 
	double anEnergyTotal = -999; 
	unsigned int dyQDC = -999; 
	unsigned int anQDCTotal = -999; 
	unsigned int anQDC[4] = {}; 
	//
	std::vector<unsigned int> dyTrace = {};
	std::vector<unsigned int> anTrace0 = {};
	std::vector<unsigned int> anTrace1 = {};
	std::vector<unsigned int> anTrace2 = {};
	std::vector<unsigned int> anTrace3 = {};

	if(isGoodYapEvent) {
		//		printf("Number of triggers in YAP(dy:an0-3): %d, %d, %d, %d, %d \n", 
		//	   dyMult, 
		//	   an0Mult, an1Mult, an2Mult, an3Mult);  

		for(auto it = yapevents.begin(); 
			it != yapevents.end(); 
			it++) {
			// get channel number
			unsigned int chanNum = (*it)->GetID(); 
			if(chanNum == 4) { // this is dynode
				time = (*it)->GetTime()*Globals::get()->GetAdcClockInSeconds((*it)->GetChanID().GetModFreq()) * 1e9; 
				dyEnergy = (*it)->GetEnergy(); 
				dyQDC = (*it)->GetTrace().DoQDCSimple(1, 120); 

				if(!isIon && isPileup) {
					// take traces only for
					// pile-up decays
					dyTrace = (*it)->GetTrace();
				}
			} else { // these are anodes
				anEnergyTotal += (*it)->GetEnergy(); 
				anQDCTotal += (*it)->GetTrace().DoQDCSimple(1, 120); 

				anQDC[chanNum] = (*it)->GetTrace().DoQDCSimple(1, 120); 

				if(!isIon && isPileup) { 
					// take traces only for
					// pile-up decays
					switch(chanNum) {
					case 0: anTrace0 = (*it)->GetTrace(); break; 
					case 1: anTrace1 = (*it)->GetTrace(); break; 
					case 2: anTrace2 = (*it)->GetTrace(); break; 
					case 3: anTrace3 = (*it)->GetTrace(); break; 
					}
				}
			}
		} // end(loop over YAP events)

		// fill the output tree
		YAPstruct.time = time; 
		YAPstruct.dyEnergy = dyEnergy; 
		YAPstruct.anEnergyTotal = anEnergyTotal; 
		// qdc
		YAPstruct.dyQDC = dyQDC; 
		YAPstruct.anQDCTotal = anQDCTotal; 
		// traces
		if(!isIon && isPileup) {
			YAPstruct.dyTrace = dyTrace; 
			YAPstruct.anTrace0 = anTrace0; 
			YAPstruct.anTrace1 = anTrace1; 
			YAPstruct.anTrace2 = anTrace2; 
			YAPstruct.anTrace3 = anTrace3; 
		}
		// position
		// 1     0
		//
		//
		// 2     3
		YAPstruct.posRawX = ((double)(anQDC[0]+anQDC[1]))/anQDCTotal; 
		YAPstruct.posRawY = ((double)(anQDC[0]+anQDC[3]))/anQDCTotal; 
		YAPstruct.posRawX *= 1000; 
		YAPstruct.posRawY *= 1000; 
        YAPstruct.posRawX = (parX[0]*pow(YAPstruct.posRawX,3) 
							 + parX[1]*pow(YAPstruct.posRawX,2) 
							 + parX[2]*YAPstruct.posRawX 
							 + parX[3] 
							 + 0.5);
        YAPstruct.posRawY = (parY[0]*pow(YAPstruct.posRawY,3) 
							 + parY[1]*pow(YAPstruct.posRawY,2) 
							 + parY[2]*YAPstruct.posRawY 
							 + parY[3] 
							 + 0.5);
		// pixelization
		YAPstruct.posX = trunc(YAPstruct.posRawX); 
		YAPstruct.posY = trunc(YAPstruct.posRawY); 

		printf("Pixel (%d, %d) \n", 
			   YAPstruct.posX, 
			   YAPstruct.posY); 
		
	} // end(isGoodYapEvent)
	
	// fill other info.
	YAPstruct.isIon = isIon; 
	YAPstruct.isGoodYap = isGoodYapEvent; 


	// complete filling
	// and save to the tree
	pixie_tree_event_->yap_vec_.emplace_back(YAPstruct); 



    return true;
}

bool YapProcessor::Process(RawEvent &event) {
    if (!EventProcessor::Process(event))
        return false;

    for (vector<ChanEvent *>::const_iterator it = evts_.begin(); it != evts_.end(); it++) {
    }


    return true;
}
