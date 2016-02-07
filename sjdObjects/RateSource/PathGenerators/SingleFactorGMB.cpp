#include "SingleFactorGMB.h"


namespace sjdObjects 
{
    SingleFactorEconomicScenarioGenerator::SingleFactorEconomicScenarioGenerator(
                            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                            const boost::shared_ptr<RatesEnvironment> re,
                            bool permanent) :
        ObjectHandler::LibraryObject<sjd::SingleFactorEconomicScenarioGenerator>(properties, permanent)
    {
        boost::shared_ptr<sjd::RatesEnvironment> sjdRE;
        re->getLibraryObject(sjdRE);

        libraryObject_ = boost::shared_ptr<sjd::SingleFactorEconomicScenarioGenerator>(
            new sjd::SingleFactorEconomicScenarioGenerator(sjdRE));
    }

   bool SingleFactorEconomicScenarioGenerator::isOK()
    {
        return libraryObject_->isOK();
    }

    string SingleFactorEconomicScenarioGenerator::getErrorMessage()
    {
        return libraryObject_->getErrorMessageAsString();
    }

    bool SingleFactorEconomicScenarioGenerator::setSimulationDates(set<Date> simulationDates)
    {
        return libraryObject_->setSimulationDates(simulationDates);
    }

    void SingleFactorEconomicScenarioGenerator::setRandomSeed(long seed)
    {
        libraryObject_->setRandomSeed(seed);
    }

    vector<boost::shared_ptr<sjd::RatesEnvironment>> SingleFactorEconomicScenarioGenerator::getPathForCI(double ci)
    {
        boost::shared_ptr<sjd::Path> path = libraryObject_->getConfidenceInterval(ci);
        vector<boost::shared_ptr<sjd::RatesEnvironment>> allRE;
        for (size_t i = 0; i < path->getNumberOfSteps(); ++i)
        {
            allRE.push_back(path->getRatesEnvironment(i));
        }
        return allRE;
    }

    vector<boost::shared_ptr<sjd::RatesEnvironment>> SingleFactorEconomicScenarioGenerator::getPathForSpot(vector<double> spotPath)
    {
        boost::shared_ptr<sjd::Path> path = libraryObject_->getPathForSpot(spotPath);
        vector<boost::shared_ptr<sjd::RatesEnvironment>> allRE;
        for (size_t i = 0; i < path->getNumberOfSteps(); ++i)
        {
            allRE.push_back(path->getRatesEnvironment(i));
        }
        return allRE;
    }

}