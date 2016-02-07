#include <xlsdk/xlsdkdefines.hpp>


void registerPortfolioEvolver(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x16""createPortfolioEvolver"),
            TempStrNoSize("\x08""CCCCPIL#"),
            TempStrNoSize("\x16""createPortfolioEvolver"),
            TempStrNoSize("\x3A""ObjectID,PortfolioID,PathGenID,Dates,HoldingPeriod,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x2E""Creates a portfolio evolver and returns its ID"),
            TempStrNoSize("\x13""ID of the Generator"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x18""ID of the Path Generator"),
            TempStrNoSize("\x0D""Forward Dates"),
            TempStrNoSize("\x3C""Holding period in days (use 0 if there is no holding period)"),
            TempStrNoSize("\x7""Trigger"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x1E""runPortfolioEvolverSimulations"),
            TempStrNoSize("\x05""PCIP#"),
            TempStrNoSize("\x1E""runPortfolioEvolverSimulations"),
            TempStrNoSize("\x1B""PortEvolver,NumSims,Trigger"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""), 
            TempStrNoSize("\x00"""), 
            TempStrNoSize("\x50""Runs a monte carlo simulation and collects statistics - nothing is returned here"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x15""Number of simulations"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x0B""getMCDetail"),
            TempStrNoSize("\x06""PCP#"),
            TempStrNoSize("\x0B""getMCDetail"),
            TempStrNoSize("\x15""PortEvolverID,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x73""Returns the intermediate calculations (EE, EPE, Margin, Reg Cap etc) from a Monte Carlo Simulation of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x0D""getMCAnalysis"),
            TempStrNoSize("\x08""PCCCCCP#"),
            TempStrNoSize("\xD""getMCAnalysis"),
            TempStrNoSize("\x2F""PortEvolverID,ZeroDRS,SpreadDRS,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x6A""Returns summary analysis of the Portfolio (CVA, FVA etc) from the Monte Carlo Simulations of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\xE""CVA - optional"),
            TempStrNoSize("\xE""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x0B""getCIDetail"),
            TempStrNoSize("\x07""PCBP#"),
            TempStrNoSize("\x0B""getCIDetail"),
            TempStrNoSize("\x18""PortEvolverID,CI,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x87""Returns the intermediate calculations (EE, EPE, Margin, Reg Cap etc) for a path corresponding to a confidence interval of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x13""Confidence Interval"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 17, &xDll,
            TempStrNoSize("\x0D""getCIAnalysis"),
            TempStrNoSize("\x09""PCBCCCCP#"),
            TempStrNoSize("\xD""getCIAnalysis"),
            TempStrNoSize("\x32""PortEvolverID,CI,ZeroDRS,SpreadDRS,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x7B""Returns summary analysis of the Portfolio (CVA, FVA etc) for a path corresponding to a confidence interval of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x13""Confidence Interval"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\x0E""CVA - optional"),
            TempStrNoSize("\x0E""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x11""getSpotPathDetail"),
            TempStrNoSize("\x07""PCPP#"),
            TempStrNoSize("\x11""getSpotPathDetail"),
            TempStrNoSize("\x1A""PortEvolverID,Spot,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x7D""Returns the intermediate calculations (EE, EPE, Margin, Reg Cap etc) for a path corresponding to a spot path of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x09""Spot path"),
            TempStrNoSize("\x08""Trigger"));

        Excel(xlfRegister, 0, 17, &xDll,
            TempStrNoSize("\x13""getSpotPathAnalysis"),
            TempStrNoSize("\x09""PCPCCCCP#"),
            TempStrNoSize("\x13""getSpotPathAnalysis"),
            TempStrNoSize("\x34""PortEvolverID,Spot,ZeroDRS,SpreadDRS,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x7B""Returns summary analysis of the Portfolio (CVA, FVA etc) for a path corresponding to a confidence interval of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x09""Spot path"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\x0E""CVA - optional"),
            TempStrNoSize("\x0E""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x1C""getSimulationDatesForEvolver"),
            TempStrNoSize("\x04""PCP#"),
            TempStrNoSize("\x1C""getSimulationDatesForEvolver"),
            TempStrNoSize("\x13""PortfolioID,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x09""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x4B""Returns the simulation dates (incl. dates withholding dates) for an evolver"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x7""Trigger"));
}

void unregisterPortfolioEvolver(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x16""createPortfolioEvolver"),
            TempStrNoSize("\x08""CCCCPIL#"),
            TempStrNoSize("\x16""createPortfolioEvolver"),
            TempStrNoSize("\x3A""ObjectID,PortfolioID,PathGenID,Dates,HoldingPeriod,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x2E""Creates a portfolio evolver and returns its ID"),
            TempStrNoSize("\x13""ID of the Generator"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x18""ID of the Path Generator"),
            TempStrNoSize("\x0D""Forward Dates"),
            TempStrNoSize("\x3C""Holding period in days (use 0 if there is no holding period)"),
            TempStrNoSize("\x7""Trigger"));


        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""createPortfolioEvolver"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x1E""runPortfolioEvolverSimulations"),
            TempStrNoSize("\x05""PCIP#"),
            TempStrNoSize("\x1E""runPortfolioEvolverSimulations"),
            TempStrNoSize("\x1B""PortEvolver,NumSims,Trigger"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""), 
            TempStrNoSize("\x00"""), 
            TempStrNoSize("\x50""Runs a monte carlo simulation and collects statistics - nothing is returned here"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x15""Number of simulations"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x1E""runPortfolioEvolverSimulations"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x0B""getMCDetail"),
            TempStrNoSize("\x06""PCP#"),
            TempStrNoSize("\x0B""getMCDetail"),
            TempStrNoSize("\x15""PortEvolverID,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x73""Returns the intermediate calculations (EE, EPE, Margin, Reg Cap etc) from a Monte Carlo Simulation of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x0B""getMCDetail"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x0D""getMCAnalysis"),
            TempStrNoSize("\x08""PCCCCCP#"),
            TempStrNoSize("\xD""getMCAnalysis"),
            TempStrNoSize("\x2F""PortEvolverID,ZeroDRS,SpreadDRS,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x6A""Returns summary analysis of the Portfolio (CVA, FVA etc) from the Monte Carlo Simulations of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\xE""CVA - optional"),
            TempStrNoSize("\xE""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x0D""getMCAnalysis"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);






        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x0B""getCIDetail"),
            TempStrNoSize("\x07""PCBP#"),
            TempStrNoSize("\x0B""getCIDetail"),
            TempStrNoSize("\x18""PortEvolverID,CI,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x87""Returns the intermediate calculations (EE, EPE, Margin, Reg Cap etc) for a path corresponding to a confidence interval of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x13""Confidence Interval"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x0B""getCIDetail"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 17, &xDll,
            TempStrNoSize("\x0D""getCIAnalysis"),
            TempStrNoSize("\x09""PCBCCCCP#"),
            TempStrNoSize("\xD""getCIAnalysis"),
            TempStrNoSize("\x32""PortEvolverID,CI,ZeroDRS,SpreadDRS,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x7B""Returns summary analysis of the Portfolio (CVA, FVA etc) for a path corresponding to a confidence interval of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x13""Confidence Interval"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\x0E""CVA - optional"),
            TempStrNoSize("\x0E""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x0D""getCIAnalysis"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x11""getSpotPathDetail"),
            TempStrNoSize("\x07""PCPP#"),
            TempStrNoSize("\x11""getSpotPathDetail"),
            TempStrNoSize("\x1A""PortEvolverID,Spot,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x7D""Returns the intermediate calculations (EE, EPE, Margin, Reg Cap etc) for a path corresponding to a spot path of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x09""Spot path"),
            TempStrNoSize("\x08""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x11""getSpotPathDetail"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 17, &xDll,
            TempStrNoSize("\x13""getSpotPathAnalysis"),
            TempStrNoSize("\x09""PCPCCCCP#"),
            TempStrNoSize("\x13""getSpotPathAnalysis"),
            TempStrNoSize("\x34""PortEvolverID,Spot,ZeroDRS,SpreadDRS,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x7B""Returns summary analysis of the Portfolio (CVA, FVA etc) for a path corresponding to a confidence interval of the portfolio"),
            TempStrNoSize("\x1B""ID of the Portfolio Evolver"),
            TempStrNoSize("\x09""Spot path"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\x0E""CVA - optional"),
            TempStrNoSize("\x0E""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x13""getSpotPathAnalysis"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x1C""getSimulationDatesForEvolver"),
            TempStrNoSize("\x04""PCP#"),
            TempStrNoSize("\x1C""getSimulationDatesForEvolver"),
            TempStrNoSize("\x13""PortfolioID,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x09""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x4B""Returns the simulation dates (incl. dates withholding dates) for an evolver"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x7""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x1C""getSimulationDatesForEvolver"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
