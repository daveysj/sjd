#include "ForwardRateSource.h"


namespace sjdObjects {
    /*======================================================================================
    ForwardRateSource

    =======================================================================================*/
    ForwardRateSource::ForwardRateSource(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                         boost::shared_ptr<sjd::ForwardRateSource> frs,
                                         bool permanent) :
        ObjectHandler::LibraryObject<sjd::ForwardRateSource>(properties, permanent) 
    {
        libraryObject_ = frs;
    }
    
    vector<double> ForwardRateSource::getForward(const vector<QuantLib::Date>& dates) const
    {
        return libraryObject_->getForward(dates);
    }

    bool ForwardRateSource::isOK() const
    {
        return libraryObject_->isOK();
    }

    string ForwardRateSource::getErrorMessages() const
    {
      return libraryObject_->getErrorMessagesAsString();
    }

    bool ForwardRateSource::isInRange(QuantLib::Date date) const
    {
        return libraryObject_->isInRange(date);
    }

   /*======================================================================================
    ForwardRateSourceDeterministic

    =======================================================================================*/
    boost::shared_ptr<ForwardRateSource> ForwardRateSourceDeterministic::parallelBump(
                        const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                        double spread,
                        bool permanent)
    {
        boost::shared_ptr<sjd::ForwardRateSourceDeterministic> dc =             
            boost::dynamic_pointer_cast<sjd::ForwardRateSourceDeterministic>(libraryObject_); 
        boost::shared_ptr<sjd::ForwardRateSource> bumpedSource = dc->parallelBump(spread);
        //boost::shared_ptr<sjd::ForwardRateSourceDeterministic> bumpedCurve = 
        //    boost::dynamic_pointer_cast<sjd::ForwardRateSourceDeterministic>(bumpedSource);             
        return boost::shared_ptr<ForwardRateSource>(new ForwardRateSource(properties, bumpedSource, permanent));
    }


    boost::shared_ptr<ForwardRateSource> ForwardRateSourceDeterministic::rollForward(
                        const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                        Date toDate,
                        bool permanent)
    {
        boost::shared_ptr<sjd::ForwardRateSourceDeterministic> lo = 
            boost::dynamic_pointer_cast<sjd::ForwardRateSourceDeterministic>(libraryObject_); 
        boost::shared_ptr<sjd::ForwardRateSource> rolledLO = lo->rollForward(toDate);
        return boost::shared_ptr<ForwardRateSource>(new ForwardRateSource(properties, rolledLO, permanent));
    }


   /*======================================================================================
    ForwardRateSourceStochastic

    =======================================================================================*/
    vector<boost::shared_ptr<ForwardRateSource> > ForwardRateSourceStochastic::rollForward(
                                            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                            set<Date> toDates,
                                            vector<double> normalRVs,
                                            bool permanent)
    {
        boost::shared_ptr<sjd::ForwardRateSourceStochastic> lo = 
            boost::dynamic_pointer_cast<sjd::ForwardRateSourceStochastic>(libraryObject_); 
        lo->setForwardValuationDates(toDates);
        lo->setRandomVariables(normalRVs);
        vector<boost::shared_ptr<sjd::ForwardRateSource> > rolledLOs = lo->rollForward();
        vector<boost::shared_ptr<ForwardRateSource> > rolledRates;
        for (size_t i = 0; i < rolledLOs.size(); ++i)
        {
            rolledRates.push_back(boost::shared_ptr<ForwardRateSource>(new ForwardRateSource(properties, rolledLOs[i], permanent)));
        }
        return rolledRates;
    }
}