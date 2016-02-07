#ifndef SJD_FIXINGRATESOURCE_INCLUDED
#define SJD_FIXINGRATESOURCE_INCLUDED
#pragma once

#include <vector>
#include <ql\time\date.hpp>
#include <Tools\Tools.h>
#include <sjd\maths\maths.h>
#include <boost\shared_ptr.hpp>

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
    class FixingRateSource
    {
    public:
        virtual ~FixingRateSource() {};

        void setDatesAndRates(vector<Date> fixingDates, vector<double> fixingRates);

        virtual bool isOK();
        vector<string> getErrorMessages() const;

        // The earliest date which this Rate Source can return a rate
        Date getEarliestDate() const;
        Date getMostRecentDate() const;

        virtual bool containsARateSetOn(Date date) const;
        virtual double getRateSetOn(Date toDate) const;

        // toDates must be strictly increasing
        virtual vector<double> getRateSetOn(vector<Date> toDates) const;

        boost::shared_ptr<sjdTools::ErrorTracking> errorTracking;

    protected:
        Date firstDate;
        Date lastDate;
        vector<Date> fixingDates;
        vector<double> fixingRates;
    };

}

#endif