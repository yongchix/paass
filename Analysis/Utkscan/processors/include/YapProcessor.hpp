/** \file TemplateProcessor.hpp
 * \brief A Template class to be used to build others.
 * \author S. V. Paulauskas
 * \date October 26, 2014
 */
#ifndef __YAPPROCESSOR_HPP__
#define __YAPPROCESSOR_HPP__

#include "EventProcessor.hpp"

//! A generic processor to be used as a template for others
class YapProcessor : public EventProcessor {
public:
    /** Default Constructor */
    YapProcessor();

    /** Constructor Accepting an argument */
    YapProcessor(const double &a);

    /** Default Destructor */
    ~YapProcessor() {};

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
	processor_struct::YAP YAPstruct; 
};

#endif // __YAPPROCESSOR_HPP__
