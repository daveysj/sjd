#ifndef CONTRACTS_INCLUDED
#define CONTRACTS_INCLUDED

#pragma once

#include <ql\time\calendar.hpp>
#include <set>

#include <sjd\RateSource\RatesEnvironment.h>

#include <Tools\Tools.h>
#include <sjd\Tools\Common.h>

#include <sstream>
#include <iomanip> 

using namespace QuantLib;

namespace sjd 
{

   /*======================================================================================
   Contract

    The base class is the base for all Financial Instruments AND groups of financial 
    instruments (e.g. structures and portfolios). 

    ****************************************************************************************
    DO NOT INHERIT FROM THIS CLASS, INHERIT FROM SINGLE OR MULTIPLE INSTRUMENT CONTRACTS
    =======================================================================================*/
    class Contract 
    {
    public:
        Contract() {valueCalcuated = false;};
        virtual ~Contract() {};
        virtual Contract* clone() = 0;

        // Call validate before checking for errors
        virtual bool isOK();
        vector<string> getErrorMessages() const; 
        string getErrorMessagesAsString() const; 

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);
        virtual double getValue(boost::shared_ptr<RatesEnvironment> re) = 0;        

        virtual string toString();
        virtual string toString(boost::shared_ptr<RatesEnvironment> re);

        // These to be completed on the assumption that validate has been run and there 
        // are no errors
        virtual string toStringContract() = 0;
        virtual string toStringRates(boost::shared_ptr<RatesEnvironment> re) = 0;

    protected:
        boost::shared_ptr<sjdTools::ErrorTracking> errorTracking;

        // When we get into a MC simulaiton, especially with Regulatory capital, we will
        // often call the same getValue(re) multiple times; The following variables allow
        // a slow class to cache the contract value for a given rates environement
        bool valueCalcuated;
        boost::shared_ptr<RatesEnvironment> cachedRE;
        double cachedContractValue;
    };

    /*======================================================================================
    SingleInstrumentContract

    The base class for all Contracts that do not consist of a collection of other contracts, 
    i.e. these are the smallest building blocks for a Portfolio or a Structure.
    =======================================================================================*/
    class SingleInstrumentContract : public Contract 
    {
    public:
        virtual ~SingleInstrumentContract() {};
        virtual SingleInstrumentContract* clone() = 0;

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);
        
        bool getHasPremium() const;
        virtual void setPremiumPaymentDate(Date premiumPaymentDate);
        Date getPremiumPaymentDate() const;
        virtual void setPremiumAmount(double premium);
        double getPremiumAmount() const;

        virtual void setMaturityDate(Date maturityDate);
        Date getMaturityDate() const;

        virtual void setSettlementDate(Date settlementDate);
        Date getSettlementDate() const;

        void setBuySell(BuySell bs);
        BuySell getBuySell() const;                
        double getBuySellAsDouble() const;        

        double getVolume() const;
        virtual void setVolume(double volume);

        // Used to decide if a particular structure’s MtM is capped. This theoretical method 
        // should be implemented to be independent of a rates environment 
        virtual double getPayoff(double forSpot) = 0;

        virtual string toStringContract();
        virtual string toStringRates(boost::shared_ptr<RatesEnvironment> re);
    protected:

        double volume;
        double premiumAmount;
        double buySellAsDouble; // = +1 for buy / -1 for sell

        Date maturityDate;
        Date settlementDate;
        Date premiumPaymentDate;

        BuySell buySell;
        
        bool hasPremium; 
   };

    
    /*======================================================================================
    SingleDerivativeContract

    A Derivative has a Forward Rate Source which impacts the contract’s valuation 
    =======================================================================================*/
    class SingleDerivativeContract : public SingleInstrumentContract 
    {
    public:
        virtual SingleDerivativeContract* clone() = 0;
        virtual ~SingleDerivativeContract() {};

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);

        virtual void setStrike(double strike);
        double getStrike() const;

        virtual string toStringContract();
        virtual string toStringRates(boost::shared_ptr<RatesEnvironment> re);
    protected:
        double strike;
    };

    /*======================================================================================
   SingleOptionContract

    Base class for all Derivatives that have a volatiltiy rate source
    =======================================================================================*/
    class SingleOptionContract : public SingleDerivativeContract 
    {
    public:
        virtual ~SingleOptionContract() {};

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);

        virtual double getPayoff(double forSpot);
        virtual double getOptionValue(boost::shared_ptr<RatesEnvironment> re) = 0;
        
        virtual double getValue(boost::shared_ptr<RatesEnvironment> re);
        
        virtual void setType(PutCall pc);
        PutCall getType() const;

        virtual string toStringContract();
        virtual string toStringRates(boost::shared_ptr<RatesEnvironment> re);
    protected:
        PutCall type;

    };

   /*======================================================================================
   AsianContract

    Base class for derivatives that rate set over a number of days
    =======================================================================================*/
    class AsianContract 
    {
    public:
        virtual ~AsianContract() {};

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);

        vector<string> getAsianContractErrorMessages();

        virtual void setAveragingDates(set<Date> averagingDates); 
        vector<Date> getAveragingDates() const; 

        set<Date> getAllBusinessDaysBetween(Date startDate, Date endDate, Calendar calendar);
        // Takes all averaging dates before or on the valuationDate and writes them to the 
        // fixingDates and all averaging dates after the valuationDate and writes them to the 
        // forwardDates
        virtual void getFixingAndForwardDates(Date valuationDate, 
                                              vector<Date> &fixingDates, 
                                              vector<Date> &forwardDates);

        virtual void getFixingAndForwardRates(Date valuationDate, 
                                              vector<double> &fixingRates, 
                                              vector<double> &forwardRates,
                                              boost::shared_ptr<RatesEnvironment> re);


    protected:
        boost::shared_ptr<sjdTools::ErrorTracking> errorTracking;
        vector<Date> orderedAveragingDates;

    };


   /*======================================================================================
    MultipleInstrumentContract

    Base class for derivatives may contain more than one underlying contract
    =======================================================================================*/
    class MultipleInstrumentContract : public Contract 
    {
    public:
        MultipleInstrumentContract() {};
        MultipleInstrumentContract(vector<boost::shared_ptr<SingleInstrumentContract>> input);
        MultipleInstrumentContract(vector<boost::shared_ptr<Contract>> input);
        MultipleInstrumentContract(MultipleInstrumentContract &multipleInstrumentContract);
        // Keeps a reference the the same underlying contracts
        virtual MultipleInstrumentContract* clone();
        virtual ~MultipleInstrumentContract() {};
        
        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);

        virtual double getValue(boost::shared_ptr<RatesEnvironment> re);
        

        virtual string toString(boost::shared_ptr<RatesEnvironment> re);
        virtual string toStringContract();
        virtual string toStringRates(boost::shared_ptr<RatesEnvironment> re);

        virtual vector<boost::shared_ptr<SingleInstrumentContract>> getUnderlyingSingleInstrumentContracts();

    protected:
        virtual void setInputVariables();
        virtual vector<boost::shared_ptr<SingleInstrumentContract>> getSingleInstrumentContracts(vector<boost::shared_ptr<Contract>> input);


        vector<boost::shared_ptr<SingleInstrumentContract>> contracts;
    };

}

#endif

