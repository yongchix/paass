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
}

YapProcessor::YapProcessor(const double &a) : EventProcessor(OFFSET, RANGE, "TemplateProcessor") {
    associatedTypes.insert("yap");
    a_ = a;
}

void YapProcessor::DeclarePlots(void) {
    DeclareHistogram1D(D_ENERGY, SA, "Energy");
    DeclareHistogram2D(DD_TEMPLATE_VS_PULSER, SA, SA, "Template Energy vs. Pulser Energy");
}

bool YapProcessor::PreProcess(RawEvent &event) {
    if (!EventProcessor::PreProcess(event))
        return false;

    evts_ = event.GetSummary("yap")->GetList();

    for (vector<ChanEvent *>::const_iterator it = evts_.begin(); it != evts_.end(); it++) {
    }
    return true;
}

bool YapProcessor::Process(RawEvent &event) {
    if (!EventProcessor::Process(event))
        return false;

    for (vector<ChanEvent *>::const_iterator it = evts_.begin(); it != evts_.end(); it++) {
    }


    return true;
}
