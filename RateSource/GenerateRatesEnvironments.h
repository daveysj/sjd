#ifndef generateratesenvironments_test
#define generateratesenvironments_test

#include <iostream>

#include <sjd\RateSource\RatesEnvironment.h>

#include <sjd\RateSource\Discount\DiscountCurves.h>
#include <sjd\RateSource\Forward\Deterministic\ForwardCurveDeterministic.h>
#include <sjd\RateSource\Forward\Stochastic\ForwardCurveCMAStochasticFwdVol.h>
#include <sjd\RateSource\Volatility\VolatilityCurve.h>
#include <sjd\RateSource\Volatility\VolatilitySurfaces.h>
#include <sjd\RateSource\Volatility\VolatilitySurfaceMoneyness.h>
#include <sjd\RateSource\Volatility\VolatilitySurfaceMoneynessCMA.h>
#include <sjd\RateSource\Volatility\VolatilitySurfaceDelta.h>

#include <sjd\RateSource\Fixing\FixingCurves.h>

#include <ql\time\daycounter.hpp>
#include <ql\time\daycounters\all.hpp>
#include <ql\time\calendar.hpp>
#include <ql\time\calendars\southafrica.hpp>


#include <boost\assign\list_of.hpp> // To create static vectors with values

using namespace sjd;

/*======================================================================================
GenerateRates
    
Factory functions that generate generic rates sources and environments intended to make 
the testing of objects that require rate sources easier.
=======================================================================================*/
class GenerateRates 
{
  public:

    GenerateRates();

    boost::shared_ptr<DiscountCurve> getDiscountCurve(boost::shared_ptr<DayCounter> dayCounter,
                                                      boost::shared_ptr<Calendar> calendar,
                                                      Date anchorDate, 
                                                      size_t years, 
                                                      double flatNACCRate, 
                                                      vector<Date> manditoryDates = vector<Date>());

    boost::shared_ptr<DiscountCurve> getDefaultDiscountCurve(Date anchorDate);


    boost::shared_ptr<ForwardCurveDeterministic> getSimpleForwardCurve(boost::shared_ptr<DayCounter> dayCounter,
                                                                       boost::shared_ptr<Calendar> calendar,
                                                                       Date anchorDate, 
                                                                       size_t years, 
                                                                       double spot, 
                                                                       double flatNACCRateDifferential, 
                                                                       vector<Date> manditoryDates = vector<Date>());

    boost::shared_ptr<ForwardCurveCMAStochasticFwdVol> getCalendarMonthAverageCurve(boost::shared_ptr<DayCounter> dayCounter,
                                                                                    boost::shared_ptr<Calendar> calendar,
                                                                                    Date anchorDate, 
                                                                                    size_t years, 
                                                                                    double spot, 
                                                                                    double flatNACCRateDifferential, 
                                                                                    vector<Date> manditoryDates = vector<Date>());

    boost::shared_ptr<VolatilityCurve> getVolatilityCurve(boost::shared_ptr<DayCounter> dayCounter,
                                                          boost::shared_ptr<Calendar> calendar,
                                                          Date anchorDate, 
                                                          size_t years, 
                                                          double flatNACCVolaitlity, 
                                                          vector<Date> manditoryDates = vector<Date>());

    boost::shared_ptr<VolatilityCurve> getDefaultVolatiltiyCurve(Date anchorDate);
    boost::shared_ptr<VolatilitySurfaceMoneyness> getDefaultMoneynessSurface(Date anchorDate);
    boost::shared_ptr<VolatilitySurfaceMoneynessCMA> getDefaultCalendarMonthAverageMoneynessSurface(Date anchorDate);


    // Rates Environements
    boost::shared_ptr<RatesEnvironment> getRatesEnvironmentForCashflow(Date anchorDate, 
                                                                       vector<Date> manditoryDates);

    boost::shared_ptr<RatesEnvironment> getRatesEnvironmentForEuropeanForward(Date anchorDate,
                                                                              double spot, double 
                                                                              naccRFR, 
                                                                              double naccIRDiff,
                                                                              vector<Date> manditoryDates);

    boost::shared_ptr<RatesEnvironment> getRatesEnvironmentForEuropeanForward(Date anchorDate,
                                                                              vector<Date> manditoryDates);

    boost::shared_ptr<RatesEnvironment> getRatesEnvironmentForAsianForward(Date anchorDate,
                                                                           vector<Date> manditoryDates);

    boost::shared_ptr<RatesEnvironment> getRatesEnvironmentForEuropeanOption(Date anchorDate,
                                                                             vector<Date> manditoryDates);

    boost::shared_ptr<RatesEnvironment> getRatesEnvironmentForAsianOption(Date anchorDate,
                                                                          vector<Date> manditoryDates);






    // Generates fixing points using the forward curve
    boost::shared_ptr<DailyFixingCurve> buildFixingCurve(boost::shared_ptr<ForwardCurveDeterministic> forwardCurve,
                                                               vector<Date> fixingDates);

    boost::shared_ptr<DailyFixingCurve> buildFixingCurve(double fixedRates,
                                                               vector<Date> fixingDates);

    vector<Date> getDatesVector(boost::shared_ptr<Calendar> calendar, Date anchorDate, size_t years, vector<Date> manditoryDates = vector<Date>());


    boost::shared_ptr<DayCounter> defaultDayCounter;
    boost::shared_ptr<Calendar> defaultCalendar;
    Date defaultAnchorDate;
    size_t defaultYears;
    double defaultNACCRate;
    double defaultNACCIRDiff;
    double defaultSpot;
    double defaultVolatiltiy;
    vector<Date> defaultCurveDates, defaultSurfaceDates;
    vector<double> defaultMoneyness;
    vector<vector<double>> defaultVolatilitySurface;


};

#endif

