
#include "./xll/xllAddIn.h"


//========================================================================
char *AddinVersionStr = "SJD add ins for Excel";
char *AddinName = "SJD";
char *DevAddinName = "SJD - DEV";

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
		"interpolateXLL",			            
		"RCKKKA",					
		"interpolate",			        
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
};
