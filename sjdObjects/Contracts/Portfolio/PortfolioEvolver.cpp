#include "PortfolioEvolver.h"

namespace sjdObjects 
{
    PortfolioEvolver::PortfolioEvolver(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                       boost::shared_ptr<Portfolio> portfolio, 
                                       boost::shared_ptr<SingleFactorEconomicScenarioGenerator> pathGenerator,
                                       set<Date> forwardDates,
                                       bool permanent) 
        : ObjectHandler::LibraryObject<sjd::PortfolioEvolver>(properties, permanent)
    {
        boost::shared_ptr<sjd::Portfolio> sjdPortfolio;
        portfolio->getLibraryObject(sjdPortfolio);
        boost::shared_ptr<sjd::SingleFactorEconomicScenarioGenerator> sjdPathGenerator;
        pathGenerator->getLibraryObject(sjdPathGenerator);

        libraryObject_ = boost::shared_ptr<sjd::PortfolioEvolver>(
            new sjd::PortfolioEvolver(sjdPortfolio, sjdPathGenerator, forwardDates));
        mcHasBeenRun = false;
    }



    PortfolioEvolver::PortfolioEvolver(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                       boost::shared_ptr<Portfolio> portfolio, 
                                       boost::shared_ptr<SingleFactorEconomicScenarioGenerator> pathGenerator,
                                       set<Date> forwardDates,
                                       size_t holdingPeriodInBusinessDays,
                                       bool permanent)
        : ObjectHandler::LibraryObject<sjd::PortfolioEvolver>(properties, permanent)
    {
        boost::shared_ptr<sjd::Portfolio> sjdPortfolio;
        portfolio->getLibraryObject(sjdPortfolio);
        boost::shared_ptr<sjd::SingleFactorEconomicScenarioGenerator> sjdPathGenerator;
        pathGenerator->getLibraryObject(sjdPathGenerator);

        boost::shared_ptr<Calendar> calendar = boost::shared_ptr<Calendar>(new NullCalendar());

        libraryObject_ = boost::shared_ptr<sjd::PortfolioEvolver>(
            new sjd::PortfolioEvolver(sjdPortfolio, sjdPathGenerator, forwardDates, calendar, holdingPeriodInBusinessDays));
        mcHasBeenRun = false;

    }

    bool PortfolioEvolver::isOK()
    {
        return libraryObject_->isOK();
    }

    string PortfolioEvolver::getErrorMessages() const
    {
        return libraryObject_->getErrorMessagesAsString();
    }

    void PortfolioEvolver::runMonteCarlo(unsigned long numberOfPaths)
    {
        libraryObject_->runMonteCarlo(numberOfPaths);
        mcHasBeenRun = true;
    }

    vector<Date> PortfolioEvolver::getSimulationDates()
    {
        return libraryObject_->getSimulationDates();
    }

    bool PortfolioEvolver::hasMCBeenRun()
    {
        return mcHasBeenRun;
    }

    void PortfolioEvolver::getMonteCarloDetail(vector<Date>& simulationDates,
                                               vector<string>& columnHeadings, 
                                               vector<vector<double>>& data)
    {
        libraryObject_->getMonteCarloDetail(simulationDates, columnHeadings, data);
    }

    void PortfolioEvolver::getMonteCarloAnalysis(vector<string> &columnHeadings, 
                                                 vector<double> &data,
                                                  boost::shared_ptr<DiscountRateSource> zeroCurve, 
                                                 boost::shared_ptr<DiscountRateSource> spreadCurve,
                                                 boost::shared_ptr<CVA> cva,
                                                 boost::shared_ptr<CVA> dva)
    {
        boost::shared_ptr<sjd::DiscountRateSource> sjdZeroCurve, sjdSpreadCurve;
        zeroCurve->getLibraryObject(sjdZeroCurve);
        spreadCurve->getLibraryObject(sjdSpreadCurve);
        boost::shared_ptr<sjd::CVA> sjdCVA, sjdDVA;
        if (cva != 0)
        {
            cva->getLibraryObject(sjdCVA);
        }
        if (dva != 0)
        {
            dva->getLibraryObject(sjdDVA);
        }
        libraryObject_->getMonteCarloAnalysis(columnHeadings, data, sjdZeroCurve, sjdSpreadCurve, sjdCVA, sjdDVA);
    }


    void PortfolioEvolver::getCIDetail(double confidenceInterval,
                                       vector<Date>& simulationDates,
                                       vector<string>& columnHeadings, 
                                       vector<vector<double>>& data)
    {
        libraryObject_->getCIDetail(confidenceInterval, simulationDates, columnHeadings, data);
    }

    void PortfolioEvolver::getCIAnalysis(double confidenceInterval,
                            vector<string> &columnHeadings, 
                            std::vector<double> &data,
                              boost::shared_ptr<DiscountRateSource> zeroCurve, 
                             boost::shared_ptr<DiscountRateSource> spreadCurve,
                            boost::shared_ptr<CVA> cva,
                            boost::shared_ptr<CVA> dva)
    {
        boost::shared_ptr<sjd::DiscountRateSource> sjdZeroCurve, sjdSpreadCurve;
        zeroCurve->getLibraryObject(sjdZeroCurve);
        spreadCurve->getLibraryObject(sjdSpreadCurve);
        boost::shared_ptr<sjd::CVA> sjdCVA, sjdDVA;
        if (cva != 0)
        {
            cva->getLibraryObject(sjdCVA);
        }
        if (dva != 0)
        {
            dva->getLibraryObject(sjdDVA);
        }
        libraryObject_->getCIAnalysis(confidenceInterval, columnHeadings, data, sjdZeroCurve, sjdSpreadCurve, sjdCVA, sjdDVA);
    }

    void PortfolioEvolver::getSpotPathDetail(vector<double> spotPath,
                                             vector<Date>& simulationDates,
                                             vector<string>& columnHeadings, 
                                             vector<vector<double>>& data)
    {
        libraryObject_->getSpotPathDetail(spotPath, simulationDates, columnHeadings, data);
    }

    void PortfolioEvolver::getSpotPathAnalysis(vector<double> spotPath,
                                                vector<string> &columnHeadings, 
                                                std::vector<double> &data,
                                                  boost::shared_ptr<DiscountRateSource> zeroCurve, 
                                                 boost::shared_ptr<DiscountRateSource> spreadCurve,
                                                boost::shared_ptr<CVA> cva,
                                                boost::shared_ptr<CVA> dva)
    {
        boost::shared_ptr<sjd::DiscountRateSource> sjdZeroCurve, sjdSpreadCurve;
        zeroCurve->getLibraryObject(sjdZeroCurve);
        spreadCurve->getLibraryObject(sjdSpreadCurve);
        boost::shared_ptr<sjd::CVA> sjdCVA, sjdDVA;
        if (cva != 0)
        {
            cva->getLibraryObject(sjdCVA);
        }
        if (dva != 0)
        {
            dva->getLibraryObject(sjdDVA);
        }
        libraryObject_->getSpotPathAnalysis(spotPath, columnHeadings, data, sjdZeroCurve, sjdSpreadCurve, sjdCVA, sjdDVA);
    }

}