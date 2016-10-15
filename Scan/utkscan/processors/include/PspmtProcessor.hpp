/** \file PspmtProcessor.hpp
 *  \brief A processor to handle pixelated PMTs
 *  \author Shintaro Go
 *  \date August 24, 2016
 */
#ifndef __PSPMTPROCESSOR_HPP__
#define __PSPMTPROCESSOR_HPP__

#include <string>
#include <map>

#include "EventProcessor.hpp"
#include "RawEvent.hpp"

///Class to handle processing of position sensitive pmts
class PspmtProcessor : public EventProcessor {
public:
    /** Default Constructor */
    PspmtProcessor(void){};

    ///@brief Constructor that sets the scale and offset for histograms
    ///@param[in] scale : The multiplicative scaling factor
    ///@param[in] offset : The additave offset for the histogram
    PspmtProcessor(const std::string &vd, const double &scale,
                   const unsigned int &offset, const double &threshold);

    /** Default Destructor */
    ~PspmtProcessor() {};

    /** Declare the plots used in the analysis */
    void DeclarePlots(void);

    /** Preprocess the PSPMT data
     * \param [in] event : the event to preprocess
     * \return true if successful */
    bool PreProcess(RawEvent &event);

    ///Enumeration that describes the different voltage dividers that have
    /// been used. Each board may have a different method to calculate the
    /// position of the interaction.
    enum VDTYPES {
        SIB064_1018, SIB064_0926, UNKNOWN
    };

    ///@return The x,y position of the interaction as calculated from the QDC
    /// found from the waveform in the WaveformAnalyzer, the on-board
    /// Pixie-16 energy filter, or the TraceFilterAnalyzer applying a
    /// trapezoidal filter to the trace.
    std::pair<double, double> GetPosition(const std::string &type) {
        if (type == "qdc")
            return posQdc_;
        else if (type == "pixie")
            return posEnergy_;
        else if (type == "trace")
            return posTrace_;
        else
            return std::pair<double, double>(0., 0.);
    }

private:
    ///@brief A method to wrap the call to std::map::insert(). This makes the
    /// code a little clearer (not by much...)
    ///@param[in] map : The map that we are going to insert into
    ///@param[in] key : The key for the key/value pair to insert.
    ///@param[in] value : The value for the key/value pair to insert.
    ///@return A pair containing (1) an interator to the newly inserted element
    /// or the already existing element and (2) a bool that is true if the
    /// element was inserted and false if an entry already existed.
    std::pair<std::map<std::string, double>::iterator, bool> InsertMapValue
            (std::map<std::string, double> &map, const std::string &key, const
            double &value);

    ///@brief A method to calculate the x position of the interaction with
    /// the scintillator
    ///@param[in] map : The map containing the set of x and y points that we
    /// will use to calculate the position.
    ///@return The x,y position of the interaction
    std::pair<double, double> CalculatePosition(const std::map<std::string,
            double> &map, const VDTYPES &vdtype);

    std::pair<double, double> posQdc_; ///< The x,y pair calculated from the
    ///< QDC of the waveform calculated by
    ///< the WaveformAnalyzer
    std::pair<double, double> posEnergy_; ///< X,Y pair calculated from the
    ///< Pixie-16 on-board energy filter
    std::pair<double, double> posTrace_; ///< X,Y pair calculated from the
    ///< TraceFilterAnalyzer
    VDTYPES vdtype_; ///< Local variable to store the type of voltage divider
    ///< we're using.
    double histogramScale_; ///< The scale that we need for the DAMM output
    unsigned int histogramOffset_; ///< The offset that we need for the the
    ///< DAMM output
    double threshold_; ///< The threshold that the energy calculated by
                       ///< the Pixie-16 trapezoidal filter needs to reach
                       ///< before we can analyze the signals.
};

#endif // __PSPMTPROCESSOR_HPP__