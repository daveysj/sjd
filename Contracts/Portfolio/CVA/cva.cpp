#include "cva.h"

namespace sjd {
   /*======================================================================================
   CVA

    =======================================================================================*/       
   bool CVA::isOK()
   {
        errorTracking->clearErrorMessages();
      return !errorTracking->getHasErrors();
   }

   vector<string> CVA::getErrorMessage() const
   {
      return errorTracking->getErrorMessages();
   }


    void CVA::setAnchorDate(Date date)
    {
        anchorDate = date;
    }

   double CVA::getPD(Date d1, Date d2)
   {
        if ((d2 < d1) || (d1 < anchorDate) || (!extendsTo(d2)))
        {
            return 0;
        }

        double pd = 0;
        if (d1 == anchorDate)
        {
            pd = getPD(d2);
        }
        else
        {
            pd = getPD(d2) - getPD(d1);
        }
        if (pd < 0)
        {
            return 0;
        }
      return pd;
   }

    /*======================================================================================
   Flat PD

    =======================================================================================*/
    FlatPD::FlatPD(Date anchorDateInput, double pdInput, double lgdInput)
   {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("FlatPD"));
      setLGD(lgdInput);
      setPD(pdInput);
        setAnchorDate(anchorDateInput);
   }

   bool FlatPD::isOK()
   {
      CVA::isOK();
      if ((pd < 0) || (pd > 1)) 
        {
         errorTracking->populateErrorMessage("PD is not in the range [0,1]");
      }
      if ((lgd < 0) || (lgd > 1)) 
        {
         errorTracking->populateErrorMessage("LDG is not in the range [0,1]");
      }
      return !errorTracking->getHasErrors();
   }

   void FlatPD::setLGD(double lgdInput) 
   {
      lgd = lgdInput;
   }

   double FlatPD::getLGD(Date d) 
   {
      return lgd;
   }

   void FlatPD::setPD(double pdInput)
   {
      pd = pdInput;
        hazardRate = -log(1-pd);
   }

    double FlatPD::getPD(Date d)
    {
        if (!extendsTo(d))
        {
            return 0;
        }

        if (d == anchorDate) 
        {
            return 0;
        }

        double t1 = (d - anchorDate) / 365.0;
        double pd1 = 1 - exp(-t1 * hazardRate);
      return pd1;
    }

   bool FlatPD::extendsTo(Date date)
   {
        return (date >= anchorDate);
   }


    /*======================================================================================
   CumulativeDefaultProbability

    =======================================================================================*/
    CumulativeDefaultProbability::CumulativeDefaultProbability(Date anchorDateInput, 
                                                               vector<Date> datesInput, 
                                                               vector<double> cdpInput, 
                                                               vector<double> lgdInput)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("CumulativeDefaultProbability"));
        setAnchorDate(anchorDateInput);
        observationDates = datesInput;
        vector<double> cdp = cdpInput;
        vector<double> lgd = lgdInput;

        if (observationDates[0] > anchorDate)
        {
            observationDates.insert(observationDates.begin(), anchorDate);
            cdp.insert(cdp.begin(), 0);
            lgd.insert(lgd.begin(), 0);
        }

        cumulativeDefaultProbability = cdp;
        lossGivenDefault = lgd;
        vector<double> time;
        for (size_t i = 0; i < observationDates.size(); ++i) 
        {
            time.push_back((observationDates[i] - anchorDate) / 365.0);
        }
        defaultInterpolator = boost::shared_ptr<ArrayInterpolator>(new CubicSplineInterpolator(time, cdp, false));
        lossInterpolator = boost::shared_ptr<ArrayInterpolator>(new CubicSplineInterpolator(time, lgd, false));
    }

   bool CumulativeDefaultProbability::isOK()
    {
        CVA::isOK();
        if (observationDates[0] < anchorDate) 
        {
            errorTracking->populateErrorMessage("Observation dates before anchor date");
        }
        if (!isStrictlyIncreasing<double>(cumulativeDefaultProbability))
        {
            errorTracking->populateErrorMessage("Cumulative default probabilities are not strictly increasing");
        }
        if ((cumulativeDefaultProbability.back() > 1) || (cumulativeDefaultProbability.front() < 0)) 
        {
            errorTracking->populateErrorMessage("Cumulative default probability not in the range [0, 1]");
        }
        if (abs(cumulativeDefaultProbability.front()) < -1e-14)
        {
            errorTracking->populateErrorMessage("Cumulative default probability on the anchor date is not 0");
        }
        if (!defaultInterpolator->isOk()) 
        {
            errorTracking->populateErrorMessage(defaultInterpolator->getErrorMessage());
        }
        for (size_t i = 0; i < lossGivenDefault.size(); ++i)
        {
            if ((lossGivenDefault[i] < 0) || (lossGivenDefault[i] > 1))
            {
                errorTracking->populateErrorMessage("Loss given default must be between 0 and 1");
            }
        }
        if (!lossInterpolator->isOk()) 
        {
            errorTracking->populateErrorMessage(lossInterpolator->getErrorMessage());
        }
        return !errorTracking->getHasErrors();
    }

   double CumulativeDefaultProbability::getPD(Date d)
    {
        if (!extendsTo(d))
        {
            return 0;
        }
        double t1 = (d - anchorDate) / 365.0;
        double cpd1 = defaultInterpolator->getRate(t1);
        return cpd1;
    }

    double CumulativeDefaultProbability::getLGD(Date d)
    {
        if (!extendsTo(d))
        {
            return 0;
        }
        double t1 = (d - anchorDate) / 365.0;
        double lgd1 = lossInterpolator->getRate(t1);
        return lgd1;
    }

   bool CumulativeDefaultProbability::extendsTo(Date date)
    {
        return ((date >= anchorDate) && (date <= observationDates.back()));
    }


}
