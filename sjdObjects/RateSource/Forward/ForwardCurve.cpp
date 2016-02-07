#include "ForwardCurve.h"


namespace sjdObjects 
{
   /*======================================================================================
   ForwardCurveDeterministic 

    =======================================================================================*/
    ForwardCurveDeterministic::ForwardCurveDeterministic(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                   Date anchorDate, vector<Date> observationDates, 
                                                   vector<double> forwards, 
                                                   sjd::ArrayInterpolatorType type,
                                                   bool allowExtrapolation,
                                                   bool permanent) : ForwardRateSourceDeterministic(properties, permanent) 
    {
        libraryObject_ = boost::shared_ptr<sjd::ForwardRateSource>(new
                            sjd::ForwardCurveDeterministic(anchorDate, 
                                                        observationDates, 
                                                        forwards, 
                                                        type, 
                                                        allowExtrapolation));
    }

    /*======================================================================================
    ForwardCurveStochasticFwdVol 

    =======================================================================================*/
    ForwardCurveStochasticFwdVol::ForwardCurveStochasticFwdVol(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                            Date anchorDate, vector<Date> observationDates, 
                                            vector<double> forwards, 
                                            sjd::ArrayInterpolatorType type,
                                            bool allowExtrapolation,
                                            boost::shared_ptr<VolatilityRateSource> vrs,
                                            bool permanent) : ForwardRateSourceStochastic(properties, permanent) 
    {
        boost::shared_ptr<sjd::VolatilityRateSource> sjdVrs;
        vrs->getLibraryObject(sjdVrs);
        libraryObject_ = boost::shared_ptr<sjd::ForwardRateSource>(new
                            sjd::ForwardCurveStochasticFwdVol(anchorDate, 
                                                              observationDates, 
                                                              forwards, 
                                                              type, 
                                                              allowExtrapolation,
                                                              sjdVrs));
    }              

    /*======================================================================================
    ForwardCurveStochasticSpotVol 

    =======================================================================================*/
    ForwardCurveStochasticSpotVol::ForwardCurveStochasticSpotVol(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                            Date anchorDate, vector<Date> observationDates, 
                                            vector<double> forwards, 
                                            sjd::ArrayInterpolatorType type,
                                            bool allowExtrapolation,
                                            boost::shared_ptr<VolatilityRateSource> vrs,
                                            bool permanent) : ForwardRateSourceStochastic(properties, permanent) 
    {
        boost::shared_ptr<sjd::VolatilityRateSource> sjdVrs;
        vrs->getLibraryObject(sjdVrs);
        libraryObject_ = boost::shared_ptr<sjd::ForwardRateSource>(new
                            sjd::ForwardCurveStochasticFwdVol(anchorDate, 
                                                              observationDates, 
                                                              forwards, 
                                                              type, 
                                                              allowExtrapolation,
                                                              sjdVrs));
    }
              
    /*======================================================================================
    ForwardCurveCMADeterministic 

    =======================================================================================*/
    ForwardCurveCMADeterministic::ForwardCurveCMADeterministic(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                       Date anchorDate, 
                                       vector<Date> observationDates, 
                                       vector<double> forwards, 
                                       sjd::ArrayInterpolatorType type,
                                       bool allowExtrapolation,
                                       bool permanent) : ForwardRateSourceDeterministic(properties, permanent) 
    {
        libraryObject_ = boost::shared_ptr<sjd::ForwardRateSource>(new
                            sjd::ForwardCurveCMADeterministic(anchorDate, 
                                                              observationDates, 
                                                              forwards, 
                                                              type, 
                                                              allowExtrapolation));
    }

    /*======================================================================================
    ForwardCurveCMADeterministic 

    =======================================================================================*/
    ForwardCurveCMAStochastic::ForwardCurveCMAStochastic(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                         Date anchorDate, 
                                                         vector<Date> observationDates, 
                                                         vector<double> forwards, 
                                                         sjd::ArrayInterpolatorType type,
                                                         bool allowExtrapolation,
                                                         boost::shared_ptr<VolatilityRateSource> vrs,
                                                         bool permanent) : ForwardRateSourceStochastic(properties, permanent) 
    {
        boost::shared_ptr<sjd::VolatilityRateSource> sjdVrs;
        vrs->getLibraryObject(sjdVrs);
        libraryObject_ = boost::shared_ptr<sjd::ForwardRateSource>(new
                            sjd::ForwardCurveCMAStochasticFwdVol(anchorDate, 
                                                                 observationDates, 
                                                                 forwards, 
                                                                 type, 
                                                                 allowExtrapolation,
                                                                 sjdVrs));
    }

    /*=====================================================================================
    ForwardCurveDeterministicFX

    =======================================================================================*/
    ForwardCurveDeterministicFX::ForwardCurveDeterministicFX(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                Date anchorDate, 
                                double spot,
                                Date spotDate,
                                boost::shared_ptr<DiscountRateSource> domesticDRS,
                                boost::shared_ptr<DiscountRateSource> foreignDRS,
                                bool permanent) : ForwardRateSourceDeterministic(properties, permanent) 
    {
        boost::shared_ptr<sjd::DiscountRateSource> sjdDomesticDRS, sjdForeignDRS;
        domesticDRS->getLibraryObject(sjdDomesticDRS);
        foreignDRS->getLibraryObject(sjdForeignDRS);
        libraryObject_ = boost::shared_ptr<sjd::ForwardRateSource>(new
                            sjd::ForwardCurveDeterministicFX(anchorDate, 
                                                             spot, 
                                                             spotDate, 
                                                             sjdDomesticDRS, 
                                                             sjdForeignDRS));
    }
              

    /*======================================================================================
    ForwardCurveStochasticFX 

    =======================================================================================*/
    ForwardCurveStochasticFX::ForwardCurveStochasticFX(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                    Date anchorDate, 
                                    double spot,
                                    Date spotDate,
                                    boost::shared_ptr<DiscountRateSource> domesticDRS,
                                    boost::shared_ptr<DiscountRateSource> foreignDRS,
                                    boost::shared_ptr<VolatilityRateSource> vrs,
                                    bool permanent) : ForwardRateSourceStochastic(properties, permanent) 
    {
        boost::shared_ptr<sjd::VolatilityRateSource> sjdVrs;
        vrs->getLibraryObject(sjdVrs);
        boost::shared_ptr<sjd::DiscountRateSource> sjdDomesticDRS, sjdForeignDRS;
        domesticDRS->getLibraryObject(sjdDomesticDRS);
        foreignDRS->getLibraryObject(sjdForeignDRS);
        libraryObject_ = boost::shared_ptr<sjd::ForwardRateSource>(new
                            sjd::ForwardCurveStochasticFX(anchorDate, 
                                                          spot, 
                                                          spotDate, 
                                                          sjdDomesticDRS, 
                                                          sjdForeignDRS,
                                                          sjdVrs));

    }
              

}