#ifndef SJD_ROLLINGFIXINGRATESOURCE_INCLUDED
#define SJD_ROLLINGFIXINGRATESOURCE_INCLUDED
#pragma once

#include <sjd\RateSource\Fixing\FixingRateSource.h>

using namespace std;
using namespace QuantLib;

namespace sjd 
{

    /*======================================================================================
    FixingRateSource

    A curve that contains historic rate sets. 

    A fixing curve must NOT interpolate, either there is a rate for a date or there is not
    if there is not a rate the calling class should be notified
    =======================================================================================*/
    class RollingFixingRateSource : public FixingRateSource
    {
    public:
        virtual ~RollingFixingRateSource() {};


        virtual boost::shared_ptr<RollingFixingRateSource> rollForward(Date toDate, 
                                                                       vector<Date> newRateSetDates,
                                                                       vector<double> newRateSets) = 0;


        // Calendar month average fixes may only store fixing rates on a subset of the all
        // the input dates
        virtual vector<Date> getFixingDatesFor(vector<Date> inputDates);
        void captureOriginalState();
        // Ensure that appended fixing rates are after the last observation date first
        virtual void appendRateSets(vector<Date> fixingDates, vector<double> fixingRates);
        virtual void updateAppedededRates(vector<double> fixingRates);
        void reset();

    protected:

        Date appendedLastDate;
        vector<Date> originalDates;
        vector<double> originalFixes;
        Date originalFinalDate;

    private:
    };

}

#endif