#ifndef SJD_CALENDARMONTHAVERAGECURVE_INCLUDED
#define SJD_CALENDARMONTHAVERAGECURVE_INCLUDED

#include <sjd\maths\maths.h>
#include <ql\time\date.hpp>
#include <boost\shared_ptr.hpp>
#include <boost/math/special_functions/round.hpp>

#include <ql\time\daycounters\simpledaycounter.hpp>
#include <ql\time\daycounters\actual365fixed.hpp>
#include <ql\time\calendar.hpp>
#include <ql\time\calendars\nullcalendar.hpp>

using namespace QuantLib;

namespace sjd 
{
    /*======================================================================================
    CalendarMonthAverageCurve

    =======================================================================================*/
    class CalendarMonthAverageCurve
    {
    public:
        CalendarMonthAverageCurve();

        CalendarMonthAverageCurve(Date anchorDate, 
                                  vector<Date> observationDates, 
                                  vector<double> rates, 
                                  ArrayInterpolatorType type = LINEAR,
                                  bool allowExtrapolation = false);    
        
        void setParameters(Date anchorDate, 
                           vector<Date> observationDates, 
                           vector<double> rates, 
                           ArrayInterpolatorType type = LINEAR,
                           bool allowExtrapolation = false);    

        bool isOK(string &errorMessage);

        Date getAnchorDate();
        Date getBeginningOfAnchorDateMonth();
        Date getFinalDate();
        ArrayInterpolatorType getType();
        bool getAllowsExtrapolation();

        vector<double> getRates();
        void setRates(vector<double> updatedRates);

        bool containsRateOnAnchorDate();
        void addPointOnAnchorDate(double valueToAdd);

        double getRate(Date toDate) const;
        vector<double> getRate(vector<QuantLib::Date> toDate) const;

        vector<double> getObservationPoints() const;
        vector<Date> getObservationDates() const;

        static int getNumberOfNumberOfMonthsBetween(Date fromDate, Date toDate);


    protected:
        int getNumberOfMonthsAheadOfAnchorDate(Date date) const;

        Date anchorDate;
        Date beginningOfAnchorDateMonth;
        Date finalDate;
        vector<Date> observationDates;
        vector<double> observationPoints;
        vector<double> observedRates;
        ArrayInterpolatorType type;
        boost::shared_ptr<ArrayInterpolator> interpolator;
        bool allowExtrapolation;
        boost::shared_ptr<DayCounter> dayCounter;
        boost::shared_ptr<DayCounter> a365DayCounter;
    };
}

#endif