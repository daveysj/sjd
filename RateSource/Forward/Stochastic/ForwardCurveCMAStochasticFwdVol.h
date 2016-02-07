#ifndef SJD_FUTURESCURVE_INCLUDED
#define SJD_FUTURESCURVE_INCLUDED

#include <sjd\Tools\Curve.h>
#include <boost/math/special_functions/round.hpp>
#include <sjd\RateSource\Forward\ForwardRateSourceStochastic.h>
#include <sjd\Tools\CalendarMonthAverageCurve.h>
#include <sjd\RateSource\Forward\Deterministic\ForwardCurveCMADeterministic.h>
#include <ql\time\daycounters\simpledaycounter.hpp>
#include <ql\time\calendar.hpp>
#include <ql\time\calendars\nullcalendar.hpp>

using namespace QuantLib;

namespace sjd 
{
    /*======================================================================================
    ForwardCurveCMAStochasticFwdVol 

    Calendar Month Average curve that rolls stochastically
    =======================================================================================*/
    class ForwardCurveCMAStochasticFwdVol : public ForwardRateSourceStochastic
    {
    public:
        
        // The dates in the observationDates vector are only used for their month / year data.
        // If you input two dates in the same month the the class will register an error
        ForwardCurveCMAStochasticFwdVol(Date anchorDate, 
                                         vector<Date> observationDates, 
                                         vector<double> forwards, 
                                         ArrayInterpolatorType type,
                                         bool allowExtrapolation,
                                         boost::shared_ptr<VolatilityRateSource> vrs);    

        bool isOK();

        virtual bool isInRange(Date date) const;
        virtual bool extendsTo(Date date) const;        

        vector<Date> getObservationDates();
        virtual double getForward(Date toDate) const;
        double getForward(Month month, Year year) const;
        virtual vector<double> getForward(vector<QuantLib::Date> toDate) const;

        vector<double> getForwards();
        void setForwards(vector<double> updatedForwards);

        virtual void setForwardValuationDates(set<Date> dates); 
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate, double normalRV);
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate, 
                                                                 double normalRV,
                                                                 vector<Date> rateSetDates,
                                                                 vector<double> &rateSetsFromAnchorToDate);

        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward();
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward(vector<Date> rateSetDates,
                                                              vector<double> &rateSetsFromAnchorToFinalRollDate);

        virtual vector<double> getImpliedRandomNumbers(vector<double> spot);

    protected:
        boost::shared_ptr<CalendarMonthAverageCurve> curve;

        vector<size_t> forwardIndexes;
        vector<vector<double> > standardDeviations;
        vector<boost::shared_ptr<ForwardCurveCMADeterministic> > frss;

        vector<Date> originalObservationDates;
        vector<double> originalObservationRates;
    };
}

#endif