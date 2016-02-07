#include "CVA.h"


namespace sjdObjects 
{

   /*======================================================================================
   CVA

    =======================================================================================*/
    bool CVA::extendsTo(Date endDate)
    {
        return libraryObject_->extendsTo(endDate);
    }

    double CVA::getLGD(Date t1)
    {
        return libraryObject_->getLGD(t1);
    }

    double CVA::getPD(Date t1, Date t2)
    {
        return libraryObject_->getPD(t1, t2);
    }

   bool CVA::isOK()
    {
        return libraryObject_->isOK();
    }

   string CVA::getErrorMessage()
    {
        return libraryObject_->errorTracking->getErrorMessagesAsString();
    }

   
   /*======================================================================================
   FlatPD

    =======================================================================================*/
   FlatPD::FlatPD(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                   Date anchorDate, double pd, double lgd,
                   bool permanent) : CVA(properties, permanent)
    {
        libraryObject_ = boost::shared_ptr<sjd::CVA>(new sjd::FlatPD(anchorDate, pd, lgd));
    }


    /*======================================================================================
   CumulativeDefaultProbability

   Class for CVA assuming that the cumulative default probability is input
    =======================================================================================*/
    CumulativeDefaultProbability::CumulativeDefaultProbability(
                                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                Date anchorDate, 
                                vector<Date> dates, 
                                vector<double> cdp, 
                                vector<double> lgd,
                                bool permanent) : CVA(properties, permanent)
    {
        libraryObject_ = boost::shared_ptr<sjd::CVA>(new sjd::CumulativeDefaultProbability(anchorDate, dates, cdp, lgd));
    }
}