#ifndef STRUCTURE_INCLUDED
#define STRUCTURE_INCLUDED
#pragma once

#include <list>

#include <sjd\Contracts\Contracts.h>
#include <sjd\Contracts\Cashflow.h>
#include <sjd\Contracts\Portfolio\RegCap\RegCapUnderlyingFactors.h>
#include <sjd\Contracts\Portfolio\RegCap\RegCapCreditParameters.h>


using namespace QuantLib;

/*======================================================================================
Factors for add-ons: From Annex 4: Treatment of Counterparty Credit Risk

            Int Rates   FX&XAU  Equities    Precious    Other 
                                            Metals      Commodities
                                            (ex xau)
    <= 1y       0.0%    1.0%    6.0%        7.0%        10.0%
    (1y,5y]     0.5%    5.0%    8.0%        7.0%        12.0%
    >=5y        1.5%    7.5%    10.0%       8.0%        15.0%

=======================================================================================*/

namespace sjd 
{

    /*======================================================================================
    Structure
    Structures consist of one or more Single Instrument Contracts naturally grouped for 
    regulatory capital purposes. This code only works for a final payoff profile that is 
    piecewise linear i.e. it only works if the input contracts are forwards, regular or 
    digital options.
    
    The two most important logical decisions this class must make regard the volume of the
    structure and the cap (if any). Determining the level of the cap, if it exists, is
    easy but determining the effective volume of a structure is not. Consider the example 
    of a payoff structure _____/\_____ (buy one call at X1, sell 2 calls at X2 and buy 
    one call at X3 with X1 < X2 < X3). What is the underlying volume of this structure
    1 or 2? This problem is potentailly compounded when this structure is in the 
    "interior" of an uncapped structure. Consider the payoff \____/\____/ where the 
    "volume" for the interior capped strucutre may be very large but the volume for the 
    uncapped "wings" could be very low. Since the Addon is Spot x Volume x f(t) , getting 
    the volume correct is important and failing to get it correct would incentivise 
    economic arbitrage.

    The approach adoped here is to seperate the input contracts *assumed to have the 
    same mautiry profile and rates* into at most two structures, one for the capped and
    one for the uncapped exposure. The volume of each structure will be determined 
    by the gradient of the payoff profile over various ranges    
    =======================================================================================*/
    class Structure : public MultipleInstrumentContract
    {
    public:
        Structure(boost::shared_ptr<SingleInstrumentContract> underlyingContract, 
                  boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors);

        Structure(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts, 
                  boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors);

        Structure(boost::shared_ptr<Contract> underlyingContract, 
                  boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors);

        Structure(Structure &structure);
        virtual Structure* clone();
        virtual ~Structure() {};

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);

        virtual Date getSettlementDate();
        virtual bool isExposureCapped() const;
        virtual bool getHasPriceExposure() const;
        virtual double getCap() const;
        virtual double getPayoff(double forSpot);

        // To get the structure's reg cap, multiply the value returned from this function my
        // The couterparty weight and the capital charge
        virtual double getMtMPlusAddOn(boost::shared_ptr<RatesEnvironment> re, double netGrossRatio = 1);
        // Often we already have the MtM so it's much more efficient just to use it than re-calcuate it
        virtual double getMtMPlusAddOnForGivenMtM(boost::shared_ptr<RatesEnvironment> re, 
                                                  double mtm, 
                                                  double netGrossRatio = 1);
        
        // The following functions are static becuase they will be used both in the class 
        // and also when manipulating a general collection of Single Instrument Contracts which may 
        // need to be manipulated before it qualifies as a Structure
        static set<double> getAllStrikes(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts);
        static vector<boost::shared_ptr<SingleInstrumentContract>> getAllContractsWithStrike(
                                            vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                            double strike);
        // returns the signed gradient of the line passing though the three mtms corresponding to three input spot values. If these do
        // not fall on the same line then a NAN is returned
        static double gradient(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts, double x1, double x2);
        // Piecewise linear is a fundamental assumption here so I need a specific test for it.
        static bool isPiecewiseLinear(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts);
        // There is a issue to do with precision, especially when dealing with the gradients of the payoff
        // functions. The extent of the precision issues depends on the volume of the payoff function but calculating this
        // every time is painfull so this shortcut just finds the the maximum volume of each of the contracts as
        // a simple estimate of the volume of the collection.
        static double getMaximumNominalVolume(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts);
        // Returns the largest absolute value gradient of the payoff of all the contracts at maturity, or the 
        // Volume of the underlying collection of contracts
        static double getEffectiveVolume(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts);
        static bool isMaximumVolumeInUncappedWing(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts);

        static bool isUncappedOnTheLeft(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                        double &effectiveVolumeAtLowestStrike);
        static bool isUncappedOnTheRight(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                        double &effectiveVolumeAtHighestStrike);
        static bool isCapped(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts, 
                             double &cap, 
                             bool &hasPriceExpsoure);

        // Groups all contracts by strike and finds the first strike where the contracts have have uncapped
        // exposure to the left. Returns all contracts with this strike
        static vector<boost::shared_ptr<SingleInstrumentContract> > getContractsWithUncappedExposureOnTheLeft(
                                            vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                            double &commonStrike);
        static vector<boost::shared_ptr<SingleInstrumentContract> > getContractsWithUncappedExposureOnTheRight(
                                            vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                            double &commonStrike);


        static bool isContactADerivative(boost::shared_ptr<SingleInstrumentContract> underlyingContract);        
        static bool isAnAsianDerivative(boost::shared_ptr<SingleDerivativeContract> underlyingContract);        
        // true if the underlying settlement / maturty profiles are the same and the rates environement is compatable
        static bool qualifyForRegulatoryCapitalOffset(boost::shared_ptr<SingleInstrumentContract> contract1,
                                                      boost::shared_ptr<SingleInstrumentContract> contract2,
                                                      string &reasonIfNot = string(""));
        static bool moveContracts(vector<boost::shared_ptr<SingleInstrumentContract> > contractsToMove,
                                  vector<boost::shared_ptr<SingleInstrumentContract> > &fromContracts,
                                  vector<boost::shared_ptr<SingleInstrumentContract> > &toContracts);

        // Takes contacts which are *assumed* to qualify for full or partial reg cap offest and builds *optimal* structures
        // By grouping according to volume
        static vector<boost::shared_ptr<Structure>> createStructures(vector<boost::shared_ptr<SingleInstrumentContract>> contracts,
                                                                     boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors,
                                                                     bool &successful);

        // Assumes the input cappedContracts and unCappedContracts are empty to start with, copies allContracts and
        // distributes the inputs so that *only* the wings will be moved to the unCappedContracts and then only in 
        // the *minimum* volume necessary to leave the remaing contracts uncapped.
        static void decomposeIntoCappedAndUncapped(vector<boost::shared_ptr<SingleInstrumentContract>> &allContracts,
                                                   vector<boost::shared_ptr<SingleInstrumentContract>> &cappedContracts,
                                                   vector<boost::shared_ptr<SingleInstrumentContract>> &unCappedContracts,
                                                   bool &successful);

        // Once we have capped and uncapped strucutres with the same maturity profile, this method moves any deferred 
        // premiums from one to the other to ensure the cappedContracts have a maximum MtM and the uncapped structures
        // have a minimum MtM
        static void optimiseCashflow(vector<boost::shared_ptr<SingleInstrumentContract>> &cappedContracts,
                                     vector<boost::shared_ptr<SingleInstrumentContract>> &unCappedContracts,
                                     bool &successful);

        virtual string toString(boost::shared_ptr<RatesEnvironment> re);


    protected:
        void commonConstructor(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts, 
                               boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors);

        boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors;
        vector<double> regCapFactorsFromForwardDates;
        bool cappedExposure, hasPriceExposure;
        double cap;
        double volume;

    };

}

#endif

/*
        virtual vector<double> getMtMPlusAddOnAlongPath(const vector<double> spotValues, 
                                                        vector<double> netGrossRatios);
        virtual vector<double> getMtMPlusAddOnAlongPathForGivenMtM(const vector<double> mtm, 
                                                                   const vector<double> spotValues, 
                                                                   vector<double> netGrossRatios);
*/