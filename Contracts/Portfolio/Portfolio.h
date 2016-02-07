#ifndef PORTFOLIO_INCLUDED
#define PORTFOLIO_INCLUDED
// #pragma once



#include <sjd\Contracts\Portfolio\Margin\Margin.h>
#include <sjd\Contracts\Portfolio\CVA\cva.h>
#include <sjd\Contracts\Portfolio\Structures\Structure.h>
#include <sjd\RateSource\PathGenerators\SingleFactorGMB.h>
#include <ql\time\daycounters\actual365fixed.hpp>


namespace sjd 
{

    /*======================================================================================
   Portfolio
    
    A portfolio consists of one or more single instrument contracts. A Portfolio only 
   addresses issues of forward valuation of the instruments. Regulatory Capital is a 
   concept that is added later
    =======================================================================================*/
    class Portfolio : public MultipleInstrumentContract
    {
    public:
        Portfolio() {};
        Portfolio(vector<boost::shared_ptr<SingleInstrumentContract>> input);
        Portfolio(vector<boost::shared_ptr<SingleInstrumentContract>> input, 
                  boost::shared_ptr<Margin> margin);
        Portfolio(vector<boost::shared_ptr<Contract>> input);
        Portfolio(vector<boost::shared_ptr<Contract>> input,
                  boost::shared_ptr<Margin> margin);
        Portfolio(Portfolio &Portfolio);
        // Keeps a reference the the same underlying contracts
        virtual Portfolio* clone();
        virtual ~Portfolio() {};

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);

        bool isMargined();
        boost::shared_ptr<Margin> getMargin();
        void setMarginBalance(double balance);
        double getMarginBalance();
        double getMarginCall(boost::shared_ptr<RatesEnvironment> re);
        void updateMarginBalanceWithCall(double marginCall);
                                         
        vector<boost::shared_ptr<SingleInstrumentContract>> getUnderlyingSingleInstrumentContracts();

    protected:
        virtual void setInputVariables();
        
        bool hasMargin;
        boost::shared_ptr<Margin> margin;

        size_t simulationSteps, exposureSteps;
        boost::shared_ptr<Calendar> regCapHoldingPeriodCalendar;
        size_t regCapMarginHoldingPeriodDays;
   };

   /*======================================================================================
    PortfolioWithRegulatoryCapital
   
    Interface for Portfolios that will have regulatory capital
    =======================================================================================*/
    class PortfolioWithRegulatoryCapital : public Portfolio
    {
    public:

        void setRegulatoryCapitalInputs(boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors,
                                        boost::shared_ptr<RegCapCreditParameters> regCapCreditParameters);

        boost::shared_ptr<RegCapUnderlyingFactors> getRegCapAssetFactors();
        boost::shared_ptr<RegCapCreditParameters> getRegCapCreditParameters();
        
        virtual double getRegulatoryCapital(boost::shared_ptr<RatesEnvironment> re) = 0;

    protected:

        boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors;
        boost::shared_ptr<RegCapCreditParameters> regCapCreditParameters;
    };

    /*======================================================================================
    PortfolioCEM - Current Exposure Method
    
    Using the Current Exposure Method requires the calculation of (MtM + Add-on) for a 
    group of transactions that are governed by a single ISDA agreement. There are many 
    interpretations of how to do this but fundamentally they all revolve around the 
    interpretation of what constitutes a single trade?

    This implementation allows for the user to manually specify how SingleInstrumentContracts 
    should be grouped to define a “transaction” (called a Structure here) and it also 
    supplies an automated way to “optimise” this calculation. Note however that to truly 
    optimise the allocation of options, forwards and cash flows for the CEM methodology 
    would require knowledge of the rates environment. The true optimal allocation could 
    change over time in a Monte Carlo Simulation. From a Regulatory Capital perspective 
    however, allocations need to be sensible so the optimisation routine implemented here 
    does not take the specific rates environment into account and instead simply tries to 
    do something efficient. 
    =======================================================================================*/
    class PortfolioCEM : public PortfolioWithRegulatoryCapital
    {
    public:

        virtual ~PortfolioCEM() {};

        virtual void construct(vector<boost::shared_ptr<Contract>> input,
                                boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors,
                                boost::shared_ptr<RegCapCreditParameters> regCapCreditParameters,
                                bool optimise,
                                boost::shared_ptr<Margin> margin);

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);

        virtual double getNGR(boost::shared_ptr<RatesEnvironment> re, double &mtm);


        static vector<boost::shared_ptr<Structure>> optimiseStructuresFromContracts(vector<boost::shared_ptr<SingleInstrumentContract>> contracts,
                                                                                  boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors,
                                                                                  bool &successful,
                                                                                  string &errorMessage);
        virtual string toString(boost::shared_ptr<RatesEnvironment> re);

    protected:
        virtual void setInputVariables();

        vector<boost::shared_ptr<Structure>> structures;

        bool structureConversionError;
        string structureConversionErrorMessage;
    };


        
    /*======================================================================================
   Portfolio - Basel II, Current Exposure Method
    
    A portfolio consists of one or more structures
    =======================================================================================*/
    class PortfolioB2CEM : public PortfolioCEM
    {
    public:
        PortfolioB2CEM(vector<boost::shared_ptr<Contract>> input,
                        boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors,
                        boost::shared_ptr<RegCapCreditParameters> regCapCreditParameters,
                        bool optimise = false);

        PortfolioB2CEM(vector<boost::shared_ptr<Contract>> input,
                        boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors,
                        boost::shared_ptr<RegCapCreditParameters> regCapCreditParameters,
                        bool optimise,
                        boost::shared_ptr<Margin> margin);

        PortfolioB2CEM(PortfolioB2CEM &portfolio);
        virtual PortfolioB2CEM* clone();
        virtual ~PortfolioB2CEM() {};

        virtual double getRegulatoryCapital(boost::shared_ptr<RatesEnvironment> re);

    protected:
        virtual void setInputVariables();
    };

    /*======================================================================================
   Portfolio - Basel III, Current Exposure Method
    
    A portfolio consists of one or more structures
    =======================================================================================*
    class B3CreditRating
    {
    public:
        B3CreditRating(string rating);
        double getRating();
        bool isOK();

    private:
        double rating;
        bool hasErrors;
    };

    /*======================================================================================
   Portfolio - Basel III, Current Exposure Method
    
    A portfolio consists of one or more structures
    =======================================================================================*
    class PortfolioB3CEM : public PortfolioCEM
    {
    public:
        PortfolioB3CEM(vector<boost::shared_ptr<Contract>> input,
                        boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors,
                        boost::shared_ptr<RegCapCreditParameters> regCapCreditParameters,
                        boost::shared_ptr<B3CreditRating> creditGrade,
                        bool optimise = false);

        PortfolioB3CEM(PortfolioB3CEM &portfolio);
        virtual PortfolioB3CEM* clone();
        virtual ~PortfolioB3CEM() {};

        virtual void setMarginParameters(boost::shared_ptr<Margin> margin);
        virtual void setMarginParameters(boost::shared_ptr<Margin> margin, 
                                         boost::shared_ptr<Calendar> calendar, 
                                         size_t holdingPeriodInBusinessDays);

        
        virtual double getRegulatoryCapital(Date valueDate);
        // I'm assuming you have called "areValuationDatesOK()" before you get here
        virtual vector<double> getRegulatoryCapitalAlongPath(const vector<double> spotValues);


    protected:
        virtual void setInputVariables();
        double getEffectiveMaturity(Date fromDate);
        boost::shared_ptr<B3CreditRating> creditRating;
    };
    */
}

#endif
