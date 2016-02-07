#include "Portfolio.h"


namespace sjdObjects 
{

        /*======================================================================================
        Portfolio
    
        =======================================================================================*/
        Portfolio::Portfolio(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                             vector<boost::shared_ptr<Contract>> inputs,
                             bool permanent) 
                      : Contract(properties, permanent) 
        {
            vector<boost::shared_ptr<sjd::Contract>> sjdInputs;
            for (size_t i = 0; i < inputs.size(); ++i) 
            {
                boost::shared_ptr<sjd::Contract> ec;
                inputs[i]->getLibraryObject(ec);
                sjdInputs.push_back(ec);
            }
            libraryObject_ = boost::shared_ptr<sjd::Contract>(new sjd::Portfolio(sjdInputs));
        }

        Portfolio::Portfolio(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                             vector<boost::shared_ptr<Contract>> inputs,
                             boost::shared_ptr<Margin> margin,
                             bool permanent) 
                      : Contract(properties, permanent) 
        {
            vector<boost::shared_ptr<sjd::Contract>> sjdInputs;
            for (size_t i = 0; i < inputs.size(); ++i) 
            {
                boost::shared_ptr<sjd::Contract> ec;
                inputs[i]->getLibraryObject(ec);
                sjdInputs.push_back(ec);
            }
            boost::shared_ptr<sjd::Margin> sjdMargin;
            margin->getLibraryObject(sjdMargin);
            libraryObject_ = boost::shared_ptr<sjd::Contract>(new sjd::Portfolio(sjdInputs, sjdMargin));
        }

        bool Portfolio::isMargined()
        {
            boost::shared_ptr<sjd::Portfolio> sjdlo = boost::static_pointer_cast<sjd::Portfolio>(libraryObject_);
            return (sjdlo->isMargined());
        }

        double Portfolio::getMarginBalance()
        {
            boost::shared_ptr<sjd::Portfolio> sjdlo = boost::static_pointer_cast<sjd::Portfolio>(libraryObject_);
            return (sjdlo->getMarginBalance());
        }

        double Portfolio::getMarginCall(boost::shared_ptr<sjdObjects::RatesEnvironment> re)
        {
            boost::shared_ptr<sjd::Portfolio> sjdlo = boost::static_pointer_cast<sjd::Portfolio>(libraryObject_);
            boost::shared_ptr<sjd::RatesEnvironment> sjdRe;
            re->getLibraryObject(sjdRe);
            return (sjdlo->getMarginCall(sjdRe));
        }


        /*
        void Portfolio::runPortfolioSimulations(boost::shared_ptr<SingleFactorGBMPathGenerator> pathGen, 
                                                vector<Date> dates, 
                                                unsigned int numSims)
        {
            mcHasBeenRun = false;
            boost::shared_ptr<sjd::Portfolio> sjdlo = boost::static_pointer_cast<sjd::Portfolio>(libraryObject_);

            boost::shared_ptr<sjd::SingleFactorGBMPathGenerator> sjdpg;
            pathGen->getLibraryObject(sjdpg);

            sjdlo->setPathGenerator(sjdpg);
            set<Date> datesSet(dates.begin(), dates.end());
            sjdlo->setForwardValuationDates(datesSet);
             if (sjdlo->areValuationDatesOK()) 
             {
                sjdlo->calculateMonteCarloStatistics(numSims);
                mcHasBeenRun = true;

                columnHeadings.clear();
                data.clear();

                {
                    columnHeadings.push_back("Ave Path");
                    columnHeadings.push_back("Ave MtM");
                if (sjdlo->getMarginParameters() != NULL)
                    {
                        columnHeadings.push_back("Margin");
                    }

                    columnHeadings.push_back("EE");
                columnHeadings.push_back("NEE");
                    columnHeadings.push_back("EE with HP");
                columnHeadings.push_back("NEE with HP");
                    columnHeadings.push_back("PFE");
                    columnHeadings.push_back("Negative PFE");
                    columnHeadings.push_back("PFE with HP");
                    columnHeadings.push_back("Negative PFE with HP");            
                }

                set<Date> simDates = sjdlo->getSimulationDates();
                {
                    for (std::set<Date>::iterator it = simDates.begin(); it != simDates.end(); ++it) 
                    {
                        stringstream s;
                        s << QuantLib::io::iso_date(*it);
                        rowHeadings.push_back(s.str());
                    }
                }

                data = sjdlo->getMonteCarloResults();
             }
        }

      vector<Date> Portfolio::getSimDatesForDates(vector<Date> exposureDates)
      {
            boost::shared_ptr<sjd::Portfolio> sjdlo = boost::static_pointer_cast<sjd::Portfolio>(libraryObject_);
         set<Date> exposureDatesSet = set<Date>(exposureDates.begin(), exposureDates.end());
         sjdlo->setForwardValuationDates(exposureDatesSet);
         set<Date> simDatesSet = sjdlo->getSimulationDates();
         return vector<Date>(simDatesSet.begin(), simDatesSet.end());
      }

        vector<double> Portfolio::getMarginBalanceAlongPath(vector<QuantLib::Date> dates, vector<double> spotRates)
        {
            vector<double> marginBalance = vector<double>(0);
            if (dates.size() != spotRates.size())
            {
                return marginBalance;
            }
            boost::shared_ptr<sjd::Portfolio> sjdlo = boost::static_pointer_cast<sjd::Portfolio>(libraryObject_);
            set<Date> datesSet(dates.begin(), dates.end());
            setForwardValuationDates(datesSet);
            if (areValuationDatesOK())
            {
                marginBalance = sjdlo->margingBalanceAlongPath(spotRates);
            }
            return marginBalance;
        }

        bool Portfolio::getPortfolioMCAnalysis(Date valueDate,  
                                    vector<string> &columnHeadings, 
                                    std::vector<double> &data,
                                    boost::shared_ptr<DiscountRateSource> zeroCurve, 
                                    boost::shared_ptr<DiscountRateSource> spreadCurve,
                                               boost::shared_ptr<CVA> cva,
                                    boost::shared_ptr<CVA> dva)
        {
            if (!mcHasBeenRun || 
            !zeroCurve->isOK() || 
            !spreadCurve->isOK() ||
            (cva != NULL && (!cva->isOK())) ||
            (dva != NULL && (!dva->isOK())) )
            {
            return false;
            }

            columnHeadings.clear();
            data.clear();

            boost::shared_ptr<sjd::DiscountRateSource> sjdZeroCurve, sjdSpreadCurve;
         zeroCurve->getLibraryObject(sjdZeroCurve); 
         spreadCurve->getLibraryObject(sjdSpreadCurve); 

            boost::shared_ptr<sjd::Portfolio> sjdloPortfolio = 
            boost::static_pointer_cast<sjd::Portfolio>(libraryObject_);

            bool hasHoldingPeriod = false;
            if (hasMarginBeenSet()) 
            {
                if (sjdloPortfolio->getSimulationDates().size() != sjdloPortfolio->getExposureDates().size())
                {
                    hasHoldingPeriod = true;
                }
                columnHeadings.push_back("CoF Margin");
                data.push_back(sjdloPortfolio->getExpectedCostOfFundingMargin(valueDate, sjdZeroCurve, sjdSpreadCurve));
            }
            if (cva != NULL) 
            {
                boost::shared_ptr<sjd::CVA> sjdloCVA;
             cva->getLibraryObject(sjdloCVA);

                columnHeadings.push_back("CVA");
                data.push_back(sjdloPortfolio->getCVA(valueDate, sjdloCVA, sjdZeroCurve, false));

                if (hasHoldingPeriod) 
                {
                    columnHeadings.push_back("CVA (with HP)");
                    data.push_back(sjdloPortfolio->getCVA(valueDate, sjdloCVA, sjdZeroCurve, true));
                }
         }
            if (dva != NULL) 
            {
                boost::shared_ptr<sjd::CVA> sjdloDVA;
             dva->getLibraryObject(sjdloDVA);

                columnHeadings.push_back("DVA");
                data.push_back(sjdloPortfolio->getDVA(valueDate, sjdloDVA, sjdZeroCurve, false));

                if (hasHoldingPeriod) 
                {
                columnHeadings.push_back("DVA (with HP)");
                    data.push_back(sjdloPortfolio->getDVA(valueDate, sjdloDVA, sjdZeroCurve, true));
                }
            }
            return true;
      }

        bool Portfolio::getPortfolioMCStatistics(bool returnColumnHeadings, 
                                                 bool returnDates, 
                                                 vector<string> &rowHeadingsInput, 
                                                 vector<string> &columnHeadingsInput, 
                                                 std::vector<std::vector<double> > &dataInput)
        {
            if (!mcHasBeenRun) 
            {
                return false;
            }

         if (returnColumnHeadings == true) 
            {
                columnHeadingsInput = columnHeadings;
            }

            if (returnDates == true) 
            {
                rowHeadingsInput = rowHeadings;
            }
            dataInput = data;
            return true;
        }

        bool Portfolio::getPortfolioPathAnalysis(Date valueDate, 
                                     vector<QuantLib::Date> dates, 
                                     vector<double> spotRates, 
                                     vector<string> &columnHeadings, 
                                     std::vector<double> &data,
                                     boost::shared_ptr<DiscountRateSource> zeroCurve, 
                                     boost::shared_ptr<DiscountRateSource> spreadCurve,
                                                 boost::shared_ptr<CVA> cva,
                                                 boost::shared_ptr<CVA> dva)
        {
            if (//dates.size() != spotRates.size() || 
            !zeroCurve->isOK() ||
            !spreadCurve->isOK() ||
            (cva != NULL && (!cva->isOK())) ||
            (dva != NULL && (!dva->isOK()))   ) 
            {
               return false;
            }

            set<Date> datesSet(dates.begin(), dates.end());

            columnHeadings.clear();
            data.clear();
            boost::shared_ptr<sjd::Portfolio> sjdlo = boost::static_pointer_cast<sjd::Portfolio>(libraryObject_);
            sjdlo->setForwardValuationDates(datesSet);
            if (!sjdlo->areValuationDatesOK() ||
                sjdlo->getSimulationDates().size() != spotRates.size())
            {
                return false;
            }

            boost::shared_ptr<sjd::DiscountRateSource> sjdZeroCurve, sjdSpreadCurve;
         zeroCurve->getLibraryObject(sjdZeroCurve); 
         spreadCurve->getLibraryObject(sjdSpreadCurve); 

            if (hasMarginBeenSet()) 
            {
                columnHeadings.push_back("CoF Margin");
                data.push_back(sjdlo->getExpectedCostOfFundingMargin(valueDate, spotRates, sjdZeroCurve, sjdSpreadCurve));
            }            
            if (cva != NULL) 
            {
                boost::shared_ptr<sjd::CVA> sjdloCVA;
             cva->getLibraryObject(sjdloCVA);
         
             columnHeadings.push_back("EL");
                data.push_back(sjdlo->getExpectedLossFromPath(valueDate, spotRates, sjdloCVA, sjdZeroCurve, false));
            columnHeadings.push_back("EL (with HP)");
                data.push_back(sjdlo->getExpectedLossFromPath(valueDate, spotRates, sjdloCVA, sjdZeroCurve, true));
            }
            if (dva != NULL) 
            {
                boost::shared_ptr<sjd::CVA> sjdloDVA;
             dva->getLibraryObject(sjdloDVA);
         
             columnHeadings.push_back("Neg EL");
                data.push_back(sjdlo->getNegativeExpectedLossFromPath(valueDate, spotRates, sjdloDVA, sjdZeroCurve, false));
             columnHeadings.push_back("Neg EL (with HP)");
                data.push_back(sjdlo->getNegativeExpectedLossFromPath(valueDate, spotRates, sjdloDVA, sjdZeroCurve, true));
            }
            
            return true;
        }

        bool Portfolio::getPortfolioPathStatistics(bool returnColumnHeadings, 
                                                   bool returnDates, 
                                                   vector<QuantLib::Date> dates, 
                                                   vector<double> spotRates, 
                                                   vector<string> &rowHeadings, 
                                                   vector<string> &columnHeadings, 
                                                   std::vector<std::vector<double> > &data)
        {
            columnHeadings.clear();
            rowHeadings.clear();
            data.clear();
            set<Date> datesSet(dates.begin(), dates.end());
            boost::shared_ptr<sjd::Portfolio> sjdlo = boost::static_pointer_cast<sjd::Portfolio>(libraryObject_);

            sjdlo->setForwardValuationDates(datesSet);
            if (!sjdlo->areValuationDatesOK())
            {
                return false;
            }

            if (dates.size() != sjdlo->getExposureDates().size()) 
            {
                //string errorMsg = "The number of input spot dates does not correspond to the number of forward valuation dates";
                return false;
            }
            if (spotRates.size() != sjdlo->getSimulationDates().size()) 
            {
                //string errorMsg = "The number of input spot dates does not correspond to the number of forward valuation dates";
                return false;
            }


            std::vector<std::vector<double>> pathResults = sjdlo->getPathResults(spotRates);

            if (returnColumnHeadings == true) 
            {
                columnHeadings.push_back("Path Used");
                columnHeadings.push_back("MtM");
                if (sjdlo->getMarginParameters() != NULL) 
                {
                    columnHeadings.push_back("Margin");
                }
                columnHeadings.push_back("EE");
                columnHeadings.push_back("NEE");
                columnHeadings.push_back("EE with HP");
                columnHeadings.push_back("NEE with HP");
            }

            std::set<Date> simDates = sjdlo->getSimulationDates();
            if (returnDates == true) 
            {
                for (std::set<Date>::iterator it = simDates.begin(); it != simDates.end(); ++it) 
                {
                    stringstream s;
                    s << QuantLib::io::iso_date(*it);
                    rowHeadings.push_back(s.str());
                }
            }

            std::vector<double> dataRow;
            size_t i = 0;
            for (std::vector<std::vector<double>>::iterator it = pathResults.begin(); it != pathResults.end(); ++it) 
            {
                data.push_back(*it);
            }
            return true;
        }

        bool Portfolio::getMCHasBeenRun()
        {
            return mcHasBeenRun;
        }
        */
       /*======================================================================================
       PortfolioWithRegulatoryCapital
    

        =======================================================================================*/
        double PortfolioWithRegulatoryCapital::getRegulatoryCapital(boost::shared_ptr<sjdObjects::RatesEnvironment> re) 
        {
            boost::shared_ptr<sjd::PortfolioWithRegulatoryCapital> sjdlo = 
                boost::static_pointer_cast<sjd::PortfolioWithRegulatoryCapital>(libraryObject_);

            boost::shared_ptr<sjd::RatesEnvironment> sjdRe;
            re->getLibraryObject(sjdRe);

            return sjdlo->getRegulatoryCapital(sjdRe);
        }
        /*
        vector<double> PortfolioWithRegulatoryCapital::getRegulatoryCapitalAlongPath(const vector<double> spotValues) 
        {
            boost::shared_ptr<sjd::PortfolioWithRegulatoryCapital> sjdlo = 
                boost::static_pointer_cast<sjd::PortfolioWithRegulatoryCapital>(libraryObject_);
            return sjdlo->getRegulatoryCapitalAlongPath(spotValues);
        }

        bool PortfolioWithRegulatoryCapital::getPortfolioMCAnalysis(Date valueDate, 
                                                                    vector<string> &columnHeadings, 
                                                                    std::vector<double> &data,
                                                         boost::shared_ptr<DiscountRateSource> zeroCurve, 
                                                         boost::shared_ptr<DiscountRateSource> spreadCurve,
                                                                    boost::shared_ptr<CVA> cva,
                                                                    boost::shared_ptr<CVA> dva)
        {
            if (!Portfolio::getPortfolioMCAnalysis(valueDate, columnHeadings, data, zeroCurve, spreadCurve, cva, dva))
            {
                return false;
            }

            boost::shared_ptr<sjd::PortfolioWithRegulatoryCapital> sjdlo = 
                boost::static_pointer_cast<sjd::PortfolioWithRegulatoryCapital>(libraryObject_);

            boost::shared_ptr<sjd::DiscountRateSource> sjdZeroCurve, sjdSpreadCurve;
         zeroCurve->getLibraryObject(sjdZeroCurve); 
         spreadCurve->getLibraryObject(sjdSpreadCurve); 
            columnHeadings.push_back("CoF RegCap");
            data.push_back(sjdlo->getExpectedCostsOfFundingRegCap(valueDate, sjdZeroCurve, sjdSpreadCurve));
            return true;
        }


        void PortfolioWithRegulatoryCapital::runPortfolioSimulations(boost::shared_ptr<SingleFactorGBMPathGenerator> pathGen, 
                                                                     vector<Date> dates, 
                                                                     unsigned int numSims)
        {
            Portfolio::runPortfolioSimulations(pathGen, dates, numSims);
            columnHeadings.push_back("Reg Cap");
        }

        bool PortfolioWithRegulatoryCapital::getPortfolioPathAnalysis(Date valueDate, 
                                                                      vector<QuantLib::Date> dates, 
                                                                      vector<double> spotRates, 
                                                                      vector<string> &columnHeadings, 
                                                                      std::vector<double> &data,
                                                           boost::shared_ptr<DiscountRateSource> zeroCurve, 
                                                           boost::shared_ptr<DiscountRateSource> spreadCurve,
                                                                      boost::shared_ptr<CVA> cva,
                                                                      boost::shared_ptr<CVA> dva)
        {
            if (!Portfolio::getPortfolioPathAnalysis(valueDate, dates, spotRates, columnHeadings, data, zeroCurve, spreadCurve, cva, dva))
            {
                return false;
            }

            boost::shared_ptr<sjd::PortfolioWithRegulatoryCapital> sjdlo = 
                boost::static_pointer_cast<sjd::PortfolioWithRegulatoryCapital>(libraryObject_);
            if (!sjdlo->areValuationDatesOK())
            {
                return false;
            }

            boost::shared_ptr<sjd::DiscountRateSource> sjdZeroCurve, sjdSpreadCurve;
         zeroCurve->getLibraryObject(sjdZeroCurve); 
         spreadCurve->getLibraryObject(sjdSpreadCurve); 
            columnHeadings.push_back("CoF RegCap");

            data.push_back(sjdlo->getCostsOfFundingRegCap(valueDate, spotRates, sjdZeroCurve, sjdSpreadCurve));

            return true;
        }

        bool PortfolioWithRegulatoryCapital::getPortfolioPathStatistics(bool returnColumnHeadings, 
                                                                        bool returnDates, 
                                                                        vector<QuantLib::Date> dates, 
                                                                        vector<double> spotRates, 
                                                                        vector<string> &rowHeadings, 
                                                                        vector<string> &columnHeadings, 
                                                                        std::vector<std::vector<double> > &data)
        {
            if (!Portfolio::getPortfolioPathStatistics(returnColumnHeadings, 
                                                       returnDates, 
                                                       dates, 
                                                       spotRates, 
                                                       rowHeadings, 
                                                       columnHeadings, 
                                                       data))
            {
                return false;
            }

            boost::shared_ptr<sjd::PortfolioWithRegulatoryCapital> sjdlo = 
                boost::static_pointer_cast<sjd::PortfolioWithRegulatoryCapital>(libraryObject_);


            if (returnColumnHeadings == true) 
            {
                columnHeadings.push_back("Reg Cap");
            }
            return true;
        }


        /*======================================================================================
        Portfolio - Basel II, Current Exposure Method
    
        =======================================================================================*/
        PortfolioB2CEM::PortfolioB2CEM(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                        vector<boost::shared_ptr<Contract>> inputs,
                                        boost::shared_ptr<sjd::RegCapUnderlyingFactors> regCapFactors,
                                        boost::shared_ptr<sjd::RegCapCreditParameters> param,
                                        bool optimise,
                                        bool permanent) : PortfolioWithRegulatoryCapital(properties, permanent)
        {
            vector<boost::shared_ptr<sjd::Contract>> sjdInputs;
            for (size_t i = 0; i < inputs.size(); ++i) 
            {
                boost::shared_ptr<sjd::Contract> ec;
                inputs[i]->getLibraryObject(ec);
                sjdInputs.push_back(ec);
            }
            libraryObject_ = boost::shared_ptr<sjd::Contract>(
                                new sjd::PortfolioB2CEM(sjdInputs, regCapFactors, param, optimise));
        }

        PortfolioB2CEM::PortfolioB2CEM(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                        vector<boost::shared_ptr<Contract>> inputs,
                                        boost::shared_ptr<sjd::RegCapUnderlyingFactors> regCapFactors,
                                        boost::shared_ptr<sjd::RegCapCreditParameters> param,
                                        bool optimise,
                                        boost::shared_ptr<Margin> margin,
                                        bool permanent) : PortfolioWithRegulatoryCapital(properties, permanent)
        {
            vector<boost::shared_ptr<sjd::Contract>> sjdInputs;
            for (size_t i = 0; i < inputs.size(); ++i) 
            {
                boost::shared_ptr<sjd::Contract> ec;
                inputs[i]->getLibraryObject(ec);
                sjdInputs.push_back(ec);
            }
            boost::shared_ptr<sjd::Margin> sjdMargin;
            margin->getLibraryObject(sjdMargin);
            libraryObject_ = boost::shared_ptr<sjd::Contract>(
                                new sjd::PortfolioB2CEM(sjdInputs, regCapFactors, param, optimise, sjdMargin));
        }
        /*======================================================================================
       Portfolio - Basel III, Current Exposure Method
    
        =======================================================================================*
        PortfolioB3CEM::PortfolioB3CEM(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                        vector<boost::shared_ptr<Contract>> inputs,
                                        boost::shared_ptr<sjd::RegCapUnderlyingFactors> regCapFactors,
                                        boost::shared_ptr<sjd::RegCapCreditParameters> param,
                                        boost::shared_ptr<sjd::B3CreditRating> b3CreditRating,
                                        bool optimise,
                                        bool permanent) : PortfolioWithRegulatoryCapital(properties, permanent)
        {
            vector<boost::shared_ptr<sjd::Contract>> sjdInputs;
            for (size_t i = 0; i < inputs.size(); ++i) 
            {
                boost::shared_ptr<sjd::Contract> ec;
                inputs[i]->getLibraryObject(ec);
                sjdInputs.push_back(ec);
            }
            libraryObject_ = boost::shared_ptr<sjd::Contract>(
                                new sjd::PortfolioB3CEM(sjdInputs, regCapFactors, param, b3CreditRating, optimise));
            mcHasBeenRun = false;
        }
        */

}