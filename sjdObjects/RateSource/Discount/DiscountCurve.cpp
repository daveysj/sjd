#include "DiscountCurve.h"


namespace sjdObjects 
{
   
   /*======================================================================================
   DiscountCurve

    =======================================================================================*/
   DiscountCurve::DiscountCurve(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                Date anchorDate, 
                                vector<Date> observationDates, 
                                vector<double> discountFactors, 
                                sjd::ArrayInterpolatorType type,
                                bool allowExtrapolation,
                                bool permanent) : DeterministicDiscountRateSource(properties, permanent) 
    {
        libraryObject_ = boost::shared_ptr<sjd::DiscountRateSource>(new
                                    sjd::DiscountCurve(anchorDate, 
                                                       observationDates, 
                                                       discountFactors, 
                                                       type, 
                                                       allowExtrapolation));
    }

    boost::shared_ptr<DiscountRateSource> DiscountCurve::parallelBump(
                                 const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                 double spread, 
                                 boost::shared_ptr<QuantLib::DayCounter> dayCounter, 
                                 Compounding compounding, 
                                 Frequency frequency,
                                 bool permanent) 
    {        
        boost::shared_ptr<sjd::DiscountCurve> dc =             
            boost::dynamic_pointer_cast<sjd::DiscountCurve>(libraryObject_); 

        boost::shared_ptr<sjd::InterestRateConvention> irc = boost::shared_ptr<sjd::InterestRateConvention>(
            new sjd::InterestRateConvention(dayCounter, compounding, frequency));

        boost::shared_ptr<sjd::DiscountRateSource> bumpedSource = dc->parallelBump(spread, irc);
        boost::shared_ptr<sjd::DiscountCurve> bumpedCurve = 
            boost::dynamic_pointer_cast<sjd::DiscountCurve>(bumpedSource); 
            
        return boost::shared_ptr<DiscountRateSource>(new DiscountRateSource(properties, bumpedCurve, permanent));
    }


    boost::shared_ptr<DiscountRateSource> DiscountCurve::rollForward(
                                                    const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                    Date toDate,
                                                    bool permanent)
    {
        boost::shared_ptr<sjd::DiscountCurve> lo = 
            boost::dynamic_pointer_cast<sjd::DiscountCurve>(libraryObject_); 

        boost::shared_ptr<sjd::DiscountRateSource> rolledLO = lo->rollForward(toDate);

        return boost::shared_ptr<DiscountRateSource>(new DiscountRateSource(properties, rolledLO, permanent));
    }

}