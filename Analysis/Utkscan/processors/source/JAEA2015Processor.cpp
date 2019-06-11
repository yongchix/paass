/** \file JAEA2015Processor.cpp
 *\brief A Template processor class that can be used to build your own.
 *\author S. V. Paulauskas
 *\date October 26, 2014
 */
#include <iostream>

#include "DammPlotIds.hpp"
#include "DetectorSummary.hpp"
#include "RawEvent.hpp"
#include "JAEA2015Processor.hpp"

namespace dammIds {
    namespace experiment {
        const int D_ENERGY = 0; //!< ID for the energy of the template detector
        const int DD_TEMPLATE_VS_PULSER = 1; //!< Energy Template vs. Energy Pulser
    }
}//namespace dammIds

using namespace std;
using namespace dammIds::experiment;

JAEA2015Processor::JAEA2015Processor() : EventProcessor(OFFSET, RANGE, "TemplateExpProcessor") {
    associatedTypes.insert("yap");
}

JAEA2015Processor::JAEA2015Processor(const double &a) : EventProcessor(OFFSET, RANGE, "TemplateExpProcessor") {
    associatedTypes.insert("yap");
    a_ = a;
}

void JAEA2015Processor::DeclarePlots(void) {
    DeclareHistogram1D(D_ENERGY, SA, "Energy");
    DeclareHistogram2D(DD_TEMPLATE_VS_PULSER, SA, SA, "Template Energy vs. Pulser Energy");
}

bool JAEA2015Processor::PreProcess(RawEvent &event) {
    if (!EventProcessor::PreProcess(event))
        return false;

    evts_ = event.GetSummary("yap")->GetList();

    for (vector<ChanEvent *>::const_iterator it = evts_.begin(); it != evts_.end(); it++) {
        unsigned int location = (*it)->GetChanID().GetLocation();
    }
    return true;
}

bool JAEA2015Processor::Process(RawEvent &event) {
    if (!EventProcessor::Process(event))
        return false;

    return true;
}
