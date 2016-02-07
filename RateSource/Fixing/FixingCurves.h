#ifndef SJD_FIXINGCURVE_INCLUDED
#define SJD_FIXINGCURVE_INCLUDED

#include <boost/math/special_functions/round.hpp>
#include <sjd\RateSource\Fixing\RollingFixingRateSource.h>

using namespace QuantLib;

namespace sjd 
{

    /*======================================================================================
    DailyFixingCurve
    
    The simplest fixing curve which contains one fixing rate per date
    =======================================================================================*/
    class DailyFixingCurve : public RollingFixingRateSource
    {
    public:
        DailyFixingCurve(vector<Date> fixingDates, vector<double> fixingRates);

        virtual boost::shared_ptr<RollingFixingRateSource> rollForward(Date toDate, 
                                                                       vector<Date> newRateSetDates,
                                                                       vector<double> newRateSets);

    };


   /*======================================================================================
   MonthlyFixingCurve 

    A Fixing Curve where there is only one point per month but this point represents the 
    (cumulative) month average
    =======================================================================================*/
    class MonthlyFixingCurve : public RollingFixingRateSource
    {
    public:
        
        // The dates in the observationDates vector are only used for their month / year data.
        // If you input two dates in the same month the the class will register an error
        MonthlyFixingCurve(vector<Date> fixingDates, vector<double> fixingRates);    
        
        ~MonthlyFixingCurve();

        virtual boost::shared_ptr<RollingFixingRateSource> rollForward(Date toDate, 
                                                                       vector<Date> newRateSetDates,
                                                                       vector<double> newRateSets);

        virtual vector<Date> getFixingDatesFor(vector<Date> inputDates);
        // fixing dates are the 1st of the month. There is a chance that the last forward
        // fixing curve will be the same as the first appened fixing date (since they are
        // all set to the 1st of the month)
        virtual void appendRateSets(vector<Date> fixingDates, vector<double> fixingRates);

        virtual bool containsARateSetOn(QuantLib::Date date) const;
        virtual double getRateSetOn(QuantLib::Date toDate) const;
        virtual vector<double> getRateSetOn(vector<Date> toDates) const;

    };



   /*======================================================================================
    HistoricFedFundsCurve
    
    Used to determine the interest payable on a margin account
    =======================================================================================*/
    class HistoricFedFundsCurve : public DailyFixingCurve
    {
    public:
        HistoricFedFundsCurve(vector<Date> fixingDates, vector<double> fixingRates);
        virtual ~HistoricFedFundsCurve();

        virtual bool isOK();

        bool canCalculateInterestFor(Date startDate,
                                     Date endDate,
                                     vector<Date> marginCallDates,                             
                                     vector<double> marginCallAmounts);

        double calculateNonCompoundedInterestOnMargin(double openingBalance, 
                                                      Date startDate,
                                                      Date endDate,
                                                      vector<Date> marginCallDates, 
                                                      vector<double> marginCallAmounts);
    };

}

#endif