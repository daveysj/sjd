#ifndef SJD_FORWARDCURVECMADETERMINISTIC_INCLUDED
#define SJD_FORWARDCURVECMADETERMINISTIC_INCLUDED

#include <boost/math/special_functions/round.hpp>
#include <sjd\RateSource\Forward\ForwardRateSourceDeterministic.h>
//#include <sjd\maths\maths.h>
#include <sjd\Tools\CalendarMonthAverageCurve.h>
#include <ql\time\daycounters\simpledaycounter.hpp>
#include <ql\time\calendar.hpp>
#include <ql\time\calendars\nullcalendar.hpp>

using namespace QuantLib;

namespace sjd 
{


    /*======================================================================================
    ForwardCurveCMADeterministic

    Calendar Month Average curve that rolls stochastically
    =======================================================================================*/
    class ForwardCurveCMADeterministic : public ForwardRateSourceDeterministic
    {
    public:
        
        // The dates in the observationDates vector are only used for their month / year data.
        // If you input two dates in the same month the the class will register an error
        ForwardCurveCMADeterministic(Date anchorDate, 
                                     vector<Date> observationDates, 
                                     vector<double> forwards, 
                                     ArrayInterpolatorType type = LINEAR,
                                     bool allowExtrapolation = false);    
        
        bool isOK();

        virtual bool isInRange(Date date) const;
        virtual bool extendsTo(Date date) const;        

        vector<Date> getObservationDates();
        virtual double getForward(Date toDate) const;
        double getForward(Month month, Year year) const;
        virtual vector<double> getForward(vector<QuantLib::Date> toDate) const;
        void setForwards(vector<double> updatedForwards);


        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate);
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate, 
                                                                 vector<Date> rateSetDates,
                                                                 vector<double> &rateSetsFromAnchorToDate);
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward();
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward(vector<Date> rateSetDates,
                                                              vector<double> &rateSetsFromAnchorToFinalRollDate);


        virtual boost::shared_ptr<ForwardRateSource> parallelBump(double spread);

    protected:
        boost::shared_ptr<CalendarMonthAverageCurve> curve;

    };
}

#endif