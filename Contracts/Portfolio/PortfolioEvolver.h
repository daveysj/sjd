#ifndef PORTFOLIOEVOLVER_INCLUDED
#define PORTFOLIOEVOLVER_INCLUDED

#include <list>
#include <algorithm>

#include "Portfolio.h"

namespace sjd 
{
    class PortfolioEvolver 
    {
    public:
        // Use this for unmargined portfolios or portfolios that do not require Holding Period calculations
        PortfolioEvolver(boost::shared_ptr<Portfolio> portfolio, 
                         boost::shared_ptr<SingleFactorEconomicScenarioGenerator> pathGenerator,
                         set<Date> forwardDates);

        // Use this for portfolios that require Holding Period calculations
        PortfolioEvolver(boost::shared_ptr<Portfolio> portfolio, 
                         boost::shared_ptr<SingleFactorEconomicScenarioGenerator> pathGenerator,
                         set<Date> forwardDates,
                         boost::shared_ptr<Calendar> calendarForHoldingPeriodCalcuation, 
                         size_t holdingPeriodInBusinessDays);

        bool isOK();
        vector<string> getErrorMessages() const;
        string getErrorMessagesAsString() const;

        vector<Date> getSimulationDates();
        vector<Date> getExposureDates();

        void runMonteCarlo(unsigned long numberOfPaths);
        void getMonteCarloDetail(vector<Date>& simulationDates,
                                 vector<string>& columnHeadings, 
                                 vector<vector<double>>& data);
        void getMonteCarloAnalysis(vector<string> &columnHeadings, 
                                   vector<double> &data,
                                   boost::shared_ptr<DiscountRateSource> zeroCurve, 
                                   boost::shared_ptr<DiscountRateSource> spreadCurve,
                                   boost::shared_ptr<CVA> cva,
                                   boost::shared_ptr<CVA> dva);


        void getCIDetail(double confidenceInterval,
                         vector<Date>& simulationDates,
                         vector<string>& columnHeadings, 
                         vector<vector<double>>& data);

        void getCIAnalysis(double confidenceInterval,
                           vector<string> &columnHeadings, 
                           std::vector<double> &data,
                            boost::shared_ptr<DiscountRateSource> zeroCurve, 
                            boost::shared_ptr<DiscountRateSource> spreadCurve,
                           boost::shared_ptr<CVA> cva,
                           boost::shared_ptr<CVA> dva);

        void getSpotPathDetail(vector<double> spotPath,
                               vector<Date>& simulationDates,
                               vector<string>& columnHeadings, 
                               vector<vector<double>>& data);

        void getSpotPathAnalysis(vector<double> spotPath,
                                 vector<string> &columnHeadings, 
                                 std::vector<double> &data,
                                   boost::shared_ptr<DiscountRateSource> zeroCurve, 
                                  boost::shared_ptr<DiscountRateSource> spreadCurve,
                                 boost::shared_ptr<CVA> cva,
                                 boost::shared_ptr<CVA> dva);



    protected:
        void getPathDetail(boost::shared_ptr<Path> path,
                           vector<Date>& simulationDates,
                           vector<string>& columnHeadings, 
                           vector<vector<double>>& data);
        void getPathAnalysis(boost::shared_ptr<Path> path,
                             vector<string> &columnHeadings, 
                             std::vector<double> &data,
                               boost::shared_ptr<DiscountRateSource> zeroCurve, 
                              boost::shared_ptr<DiscountRateSource> spreadCurve,
                             boost::shared_ptr<CVA> cva,
                             boost::shared_ptr<CVA> dva);


        vector<double> getSpotAlongAPath(boost::shared_ptr<Path> path);
        vector<double> getMtMAlongAPath(boost::shared_ptr<Path> path);
        void getRegulatoryCapitalAlongAPath(boost::shared_ptr<Path> path, 
                                            vector<double>& regCap, 
                                            vector<double>& mtm, 
                                            vector<double>& margin);
        vector<double> margingBalanceForMtM(const vector<double> mtm);
        
        virtual void evolvePortfolioAlongPath(boost::shared_ptr<Path> path);
        virtual void collectStatsFromPath();
        virtual void accumulateStatistics(size_t i);
        virtual void calculateSummaryStats();

        void setSimulationDates(set<Date> dates); 
        virtual void initialiseAccumulators(size_t paths);

        double getXVAForExposurePath(vector<double> exposurePath, 
                                     boost::shared_ptr<CVA> cva, 
                                     boost::shared_ptr<DiscountRateSource> drs); 

        //dates MUST be sorted
        double getExpectedCostOfFundingPosition(vector<double> cashProfileToFund, 
                                                vector<Date> dates, 
                                                    boost::shared_ptr<DiscountRateSource> zeroCurve,
                                                boost::shared_ptr<DiscountRateSource> spreadCurve);

        boost::shared_ptr<sjdTools::ErrorTracking> errorTracking;

        bool portfolioIsMargined;
        bool portfolioAttractsCapital;

        boost::shared_ptr<Portfolio> portfolio;
        boost::shared_ptr<PortfolioWithRegulatoryCapital> regCapPortfolio;

        boost::shared_ptr<SingleFactorEconomicScenarioGenerator> pathGenerator;
        boost::shared_ptr<Calendar> calendarForHoldingPeriod;
        size_t holdingPeriod;

        vector<Date> simulationDates;
        vector<size_t> holdingPeriodIndex; // those simulation dates needed only for holding period calculations
        vector<size_t> exposureDatesIndex; // dates on which exposures will be calculated
        size_t simulationSteps;
        size_t exposureSteps;


      // Tmp variables used when evaluating the portfolio data on a path. 
        vector<double> spotPath, 
                       mtmOnPath, 
                       marginOnPath, 
                       exposurePathNoOffset, 
                       negativeExposurePathNoOffset,
                       exposurePathWithOffset, 
                       negativeExposurePathWithOffset,
                       regCap;
      
        vector<vector<double> > pathAccumulator,
                                aveMtMAccumulator, 
                                marginAccumulator,
                                eeNoHoldingPeriodAccumulator, 
                                neeNoHoldingPeriodAccumulator,
                                eeWithHoldingPeriodAccumulator, 
                                neeWithHoldingPeriodAccumulator,
                                regCapAccumulator;      

      // Summary stats for MC output
        vector<double> averageSimulatedPath,
                       averageMtM, 
                       expectedMargin,
                       eeNoHoldingPeriod, 
                       neeNoHoldingPeriod,  
                       eeWithHoldingPeriod, 
                       neeWithHoldingPeriod,
                       eePFENoHoldingPeriod,
                       neePFENoHoldingPeriod, 
                       eePFEWithHoldingPeriod,
                       neePFEWithHoldingPeriod;

        //vector<double> riskWeightedAssets; 
    };

}

#endif
