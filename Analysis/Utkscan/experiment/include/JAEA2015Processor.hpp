/** \file TemplateProcessor.hpp
 * \brief A Template class to be used to build others.
 * \author S. V. Paulauskas
 * \date October 26, 2014
 */
#ifndef __JAEA2015Processor_HPP__
#define __JAEA2015Processor_HPP__

#include "EventProcessor.hpp"

//! A generic processor to be used as a template for others
class JAEA2015Processor : public EventProcessor {
public:
    /** Default Constructor */
    JAEA2015Processor();

    /** Constructor Accepting an argument */
    JAEA2015Processor(const double &a);

    /** Default Destructor */
    ~JAEA2015Processor() {};

    /** Declares the plots for the processor */
    virtual void DeclarePlots(void);

    /** Performs the preprocessing, which cannot depend on other processors
    * \param [in] event : the event to process
    * \return true if preprocessing was successful */
    virtual bool PreProcess(RawEvent &event);

    /** Performs the main processsing, which may depend on other processors
    * \param [in] event : the event to process
    * \return true if processing was successful */
    virtual bool Process(RawEvent &event);

    /** \return The processed Template events */
    std::vector<ChanEvent *> GetTemplateEvents(void) const {
        return (evts_);
    }

private:
    double a_; //!< a variable global to the class
    std::vector<ChanEvent *> evts_; //!< vector of events for people to get
};

#endif // __JAEA2015Processor_HPP__
