
#include "./xll/xllAddIn.h"


//========================================================================
char *AddinVersionStr = "Protorisk formula add ins for Excel";
char *AddinName = "Protorisk";
char *DevAddinName = "Protorisk - DEV";

/*
//========================================================================
// Strings used regularly in the dialog box
char *descriptionCorrel = "A column factorisation of the correlation matrix, i.e. a column matrix B s.t. rho = B * transpose(B). \
B determines the dimension of the LMM";
char* descriptionTime = "A column vector containing the time to maturity. This must be strictly increasing. \
The length of this vector (N+2) defines the number of stochastic factors (N)";
char* descriptionLMMDF = "A column vector with the same number of elements as the \"time\" vector. \
In the LMM model, rates are positive so these discount factors must be decreasing.";
char* descriptionCapletVol = "The input caplet volatilities. The number of stochastic factors (N) must be the \
same as in the \"time\" vector and the number of columns determines the number of time steps";
char* superCorrelationMatrix = "Super correlation matrix. The order of the inputs is assumed to be \
1) Asset, 2-(2+n) the n domestic LMM factors and then all the foreign LMM factors";
char* antithetic = "True or False. True to us antithetic random numbers (Num Sims / 2 unique numbers \
and their negatives). False for unique random numbers for each path";
*/

//---------------------------------------------------------
// Registered function argument and return types
// Data type					Pass by value		Pass by ref
// boolean						A					L
// double						B					E
// char*											C, F
// unsigned char*									D, G
// unsigned short int (DWORD)	H
// signed short int				I					M
// signed long int				J					N
// struct FP (xl_array)								K
// struct oper										P
// struct xloper									R
//---------------------------------------------------------
char *FunctionExports[NUM_FUNCTIONS][MAX_EXCEL4_ARGS - 1] =
{
	{
		"prGetXllExpiryDate",			            
		"R",					
		"prGetXllExpiryDate",			        
		"",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the expiry date of the xll",
		// Help text line (optional)
		"",
	},
	{
		"prInterpolate",			            
		"RCKKKA",					
		"prInterpolate",			        
		"Type,x array, y array, point(s),Extrap",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Interpolates",
		// Help text line (optional)
		"Linear or Cubic",
        "Input x array",
        "Input y array",
        "Value(s) at which to interpolate",
        "Allow extrapolation (default = false)",
		"",
	},
	{
		"prVolFromDeltaSurface",			            
		"RCBKBBBA",					
		"prVolFromDeltaSurface",			        
		"Type,anchorDate,surface,toDate,strike,forward,Extrap",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the volatility to use for a Black-Scholes option with a strike from a delta based suface",
		// Help text line (optional)
		"BiLinear or BiCubic interpolation",
        "The anchor date for the volatility surface",
        "The entire volatility surface including the time / dates in the top row and the PUT deltas in the first column. Note that the first entry A1 is ignored",
        "The value to interpolate to in the time dimension",
        "The strike of the option",
        "The market forward for the option",
        "Allow extrapolation (default = false)",
		"",
	},
	{
		"prOptionValue",			            
		"RCBBBB",					
		"prOptionValue",			        
		"P/C,forward,strike,sd,df",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the PV premium of a Black 76 option on a future / forward",
		// Help text line (optional)
		"(P)ut or (C)all",
        "Forward",
        "Strike",
        "Standard Deviation (=vol*sqrt(time))",
        "Discount Factor",
		"",
	},
	{
		"prOptionDelta",			            
		"RCBBBB",					
		"prOptionDelta",			        
		"P/C,forward,strike,sd,df",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the PV premium of a Black 76 option on a future / forward",
		// Help text line (optional)
		"(P)ut or (C)all",
        "Forward",
        "Strike",
        "Standard Deviation (=vol*sqrt(time))",
        "Discount Factor",
		"",
	},
	{
		"black76PremiumAndAnalyticGreeks",			            
		"RCBBBBBBAA",					
		"prBlack76AndAnalyticGreeks",			        
		"P/C,forward,strike,sd,df,yte,yts,Output Row,Headings",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the PV premium of a Black 76 option on a future / forward and all the options Greeks",
		// Help text line (optional)
		"(P)ut or (C)all",
        "Forward",
        "Strike",
        "Standard Deviation (=vol*sqrt(time))",
        "Discount Factor",
        "Year fractions to option Expiry",
        "Year fractions to option Settlement",
        "Return the solution in a Row with TRUE and in a Column with FALSE",
        "Retrun the greeks with headings - default is to return values only",
		"",
	},
	{
		"black76PremiumAndNumericGreeks",			            
		"RCBBBBBBAABBBB",					
		"prBlack76AndNumericGreeks",			        
		"P/C,forward,strike,sd,df,yte,yts,Output Row,Headings, dF, dV, dr, dT",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the PV premium of a Black 76 option on a future / forward and all the options Greeks",
		// Help text line (optional)
		"(P)ut or (C)all",
        "Forward",
        "Strike",
        "Standard Deviation (=vol*sqrt(time))",
        "Discount Factor",
        "Year fractions to option Expiry",
        "Year fractions to option Settlement",
        "Return the solution in a Row with TRUE and in a Column with FALSE",
        "Retrun the greeks with headings - default is to return values only",
        "Absolute change in forward for numeric delta and gamma, default = 0.0001",
        "Absolute change in volatility for numeric vega, default = 0.0001",
        "Absolute change in NACC interest rate for numeric rho, default = 0.0001",
        "Absolute change in time for numeric theta, default = 0.0001",
        "",
	},
    {
		"asianPremium",			            
		"RCKIBKB",					
		"prAsianOptionValue",			        
		"P/C,forwards,M,strike,sds,df",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the PV premium of an Asian or Average Rate Option",
		// Help text line (optional)
		"(P)ut or (C)all",
        "Rate sets and Forwards in one vector",
        "Num of Fwds that have ALREADY rate set",
        "Strike",
        "Standard Deviation (=vol*sqrt(time)) - please make sure this is the same dim. as the fwds. Entries corresponding to rate set fwds are ignored",
        "Discount Factor",
		"",
	},
	{
		"asianPremiumAndAnalyticGreeks",			            
		"RCKKBKBBBAA",					
		"prAsianAndAnalyticGreeks",			        
		"P/C,fwds,time,strike,sds,df,yte,yts,R/C,Headings",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the PV premium of an Asian or Average Rate and all the options Greeks",
		// Help text line (optional)
		"(P)ut or (C)all",
        "Rate sets and Forwards in one vector",
        "Year fractions to rate set (negative for points that have already set)",
        "Strike",
        "Standard Deviation (=vol*sqrt(time)) - please make sure this is the same dim. as the fwds. Entries corresponding to rate set fwds are ignored",
        "Discount Factor",
        "Year fractions to option Expiry",
        "Year fractions to option Settlement",
        "Return the solution in a Row with TRUE and in a Column with FALSE",
        "Retrun the greeks with headings - default is to return values only",
		"",
	},
	{
		"asianPremiumAndNumericGreeks",			            
		"RCKKBKBBBAABBBB",					
		"prAsianAndNumericGreeks",			        
		"P/C,fwds,time,strike,sds,df,yte,yts,R/C,Headings, dF, dV, dr, dT",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the PV premium of an Asian or Average Rate and all the options Greeks",
		// Help text line (optional)
		"(P)ut or (C)all",
        "Rate sets and Forwards in one vector",
        "Year fractions to rate set (negative for points that have already set)",
        "Strike",
        "Standard Deviation (=vol*sqrt(time)) - please make sure this is the same dim. as the fwds. Entries corresponding to rate set fwds are ignored",
        "Discount Factor",
        "Year fractions to option Expiry",
        "Year fractions to option Settlement",
        "Return the solution in a Row with TRUE and in a Column with FALSE",
        "Retrun the greeks with headings - default is to return values only",
        "Absolute change in forward for numeric delta and gamma, default = 0.0001",
        "Absolute change in volatility for numeric vega, default = 0.0001",
        "Absolute change in NACC interest rate for numeric rho, default = 0.0001",
        "Absolute change in time for numeric theta, default = 0.0001",
		"",
	},
	{
		"getIntersection",			            
		"RKK",					
		"getIntersection",			        
		"array 1,array 2",	    
		"1",						
		AddinName,			    
		"",							
		"",							
		"Returns the common dates from two dated time series along with their data",
		// Help text line (optional)
		"Time series 1",
      "Time series 2",
		"",
	},

};
