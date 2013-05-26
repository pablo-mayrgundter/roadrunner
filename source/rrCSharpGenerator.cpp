#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "sbml/Model.h"
#include "sbml/SBMLDocument.h"
#include "rr-libstruct/lsLibStructural.h"

#include "rrCSharpGenerator.h"
#include "rr-libstruct/lsLibStructural.h"
#include "rrStringListContainer.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrRule.h"
#include "rrScanner.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
//---------------------------------------------------------------------------

using namespace std;
using namespace ls;
using namespace libsbml;
namespace rr
{

//CSharpGenerator::CSharpGenerator(RoadRunner* rr)
CSharpGenerator::CSharpGenerator(LibStructural& ls, NOMSupport& nom)
:
ModelGenerator(ls, nom)
{
}

CSharpGenerator::~CSharpGenerator(){}

string CSharpGenerator::getSourceCode()
{
    return mSource.ToString();
}

// Generates the Model Code from the SBML string
string CSharpGenerator::generateModelCode(const string& sbmlStr, const bool& computeAndAssignConsevationLaws)
{
    Log(lDebug4)<<"Entering CSharpGenerators generateModelCode(string) function";

    StringList      Warnings;

    mSource.Clear();
    CodeBuilder&     sb = mSource;

    mNOM.reset();
    string sASCII = mNOM.convertTime(sbmlStr, "time");

    Log(lDebug4)<<"Loading SBML into NOM";
    mNOM.loadSBML(sASCII.c_str(), "time");


    mModelName = mNOM.getModelName();
    if(!mModelName.size())
    {
        Log(lError)<<"Model name is empty! Exiting...";
        return "";
    }

    Log(lDebug3)<<"Model name is "<<mModelName;
    mNumReactions = mNOM.getNumReactions();

    Log(lDebug3)<<"Number of reactions:"<<mNumReactions;

    mGlobalParameterList.Clear();
    mModifiableSpeciesReferenceList.Clear();
    mLocalParameterList.reserve(mNumReactions);
    mReactionList.Clear();
    mBoundarySpeciesList.Clear();
    mFloatingSpeciesConcentrationList.Clear();
    mFloatingSpeciesAmountsList.Clear();
    mCompartmentList.Clear();
    mConservationList.Clear();
    mFunctionNames.empty();
    mFunctionParameters.empty();

//    LibStructural* instance = LibStructural::getInstance();
    string msg;
    try
    {
        Log(lDebug)<<"Loading sbml into StructAnalysis";
        msg = mLibStruct.loadSBML(sASCII);
        if(!msg.size())
        {
            Log(lError)<<"Failed loading sbml into StructAnalysis";
        }
    }
    catch(...)
    {
        Log(lError)<<"Failed loading sbml into StructAnalysis";
    }


    Log(lDebug3)<<"Message from StructAnalysis.LoadSBML function\n"<<msg;

//    if (mRR != NULL && mRR->computeAndAssignConservationLaws())
	if(computeAndAssignConsevationLaws)
    {
        mNumIndependentSpecies = mLibStruct.getNumIndSpecies();
        mIndependentSpeciesList = mLibStruct.getIndependentSpecies();
        mDependentSpeciesList   = mLibStruct.getDependentSpecies();
    }
    else
    {
        mNumIndependentSpecies = mLibStruct.getNumSpecies();
        mIndependentSpeciesList = mLibStruct.getSpecies();
    }

    sb<<append("//************************************************************************** " + NL());

    // Load the compartment array (name and value)
    mNumCompartments         		= readCompartments();

    // Read FloatingSpecies
    mNumFloatingSpecies     		= readFloatingSpecies();
    mNumDependentSpecies     		= mNumFloatingSpecies - mNumIndependentSpecies;

    // Load the boundary species array (name and value)
    mNumBoundarySpecies     		= readBoundarySpecies();

    // Get all the parameters into a list, global and local
    mNumGlobalParameters     		= readGlobalParameters();
    mNumModifiableSpeciesReferences = readModifiableSpeciesReferences();

    // Load up local parameters next
    readLocalParameters(mNumReactions, mLocalParameterDimensions, mTotalLocalParmeters);
    mNumEvents = mNOM.getNumEvents();

    writeClassHeader(sb);
    writeOutVariables(sb);
    writeOutSymbolTables(sb);
    writeResetEvents(sb, mNumEvents);
    writeSetConcentration(sb);
    writeGetConcentration(sb);
    writeConvertToAmounts(sb);
    writeConvertToConcentrations(sb);
    writeProperties(sb);
    writeAccessors(sb);
    writeUserDefinedFunctions(sb);
    writeSetInitialConditions(sb, mNumFloatingSpecies);
    writeSetBoundaryConditions(sb);
    writeSetCompartmentVolumes(sb);
    writeSetParameterValues(sb, mNumReactions);
    writeComputeConservedTotals(sb, mNumFloatingSpecies, mNumDependentSpecies);

    // Get the L0 matrix
    int nrRows;
    int nrCols;
    DoubleMatrix* aL0 = initializeL0(nrRows, nrCols);     //Todo: What is this doing? answer.. it is used below..
//    DoubleMatrix L0(aL0,nrRows, nrCols);         //How many rows and cols?? We need to know that in order to use the matrix properly!

    writeUpdateDependentSpecies(sb, mNumIndependentSpecies, mNumDependentSpecies, *aL0);
    int numOfRules = writeComputeRules(sb, mNumReactions);
    writeComputeAllRatesOfChange(sb, mNumIndependentSpecies, mNumDependentSpecies, *aL0);
    writeComputeReactionRates(sb, mNumReactions);
    writeEvalModel(sb, mNumReactions, mNumIndependentSpecies, mNumFloatingSpecies, numOfRules);
    writeEvalEvents(sb, mNumEvents, mNumFloatingSpecies);
    writeEventAssignments(sb, mNumReactions, mNumEvents);
    writeEvalInitialAssignments(sb, mNumReactions);
    writeTestConstraints(sb);
    sb<<format("}{0}{0}", NL());
    return sb.ToString();
}

bool CSharpGenerator::saveSourceCodeToFolder(const string& folder, const string& codeBaseName)
{
    mSourceCodeFileName = folder + string("\\") + extractFileName(codeBaseName);
    mSourceCodeFileName = changeFileExtensionTo(mSourceCodeFileName, ".cs");

    ofstream outFile;
    outFile.open(mSourceCodeFileName.c_str());

    //We don't know the name of the file until here..
    //Write an include statement to it..
    vector<string> fNameParts = splitString(mSourceCodeFileName,"\\");
    string headerFName = fNameParts[fNameParts.size() - 1];

    outFile<<"//\""<<headerFName<<"\"\n"<<endl;
    outFile<<mSource.ToString();
    outFile.close();
    Log(lDebug3)<<"Wrote source code to file: "<<mSourceCodeFileName;

    return true;
}

string CSharpGenerator::convertUserFunctionExpression(const string& equation)
{
    if(!equation.size())
    {
        Log(lError)<<"The equation string supplied to "<<__FUNCTION__<<" is empty";
        return "";
    }
    Scanner s;
    stringstream ss;
    ss<<equation;
    s.AssignStream(ss);
    s.startScanner();
    s.nextToken();
    CodeBuilder  sb;

    try
    {
        while (s.token() != CodeTypes::tEndOfStreamToken)
           {
            string theToken = s.tokenString;
               switch (s.token())
               {
                case CodeTypes::tWordToken:
                    if(theToken == "pow")
                    {
                        sb<<append("Math.Pow");
                    }
                    else if(theToken == "sqrt")
                    {
                        sb<<append("Math.Sqrt");
                      }
                    else if(theToken == "log")
                    {
                        sb<<append("supportFunctions._log");
                    }
                    else if(theToken == "log10")
                    {
                        sb<<append("Math.Log10");
                    }
                    else if(theToken == "floor")
                    {
                        sb<<append("Math.Floor");
                    }
                    else if(theToken == "ceil")
                    {
                        sb<<append("Math.Ceiling");
                    }
                    else if(theToken == "factorial")
                    {
                        sb<<append("supportFunctions._factorial");
                    }
                    else if(theToken == "exp")
                    {
                        sb<<append("Math.Exp");
                    }
                    else if(theToken == "sin")
                    {
                        sb<<append("Math.Sin");
                    }
                    else if(theToken == "cos")
                    {
                        sb<<append("Math.Cos");
                    }
                    else if(theToken == "tan")
                    {
                        sb<<append("Math.Tan");
                    }
                    else if(theToken == "abs")
                    {
                        sb<<append("Math.Abs");
                    }
                    else if(theToken == "asin")
                    {
                        sb<<append("Math.Asin");
                    }
                    else if(theToken == "acos")
                    {
                        sb<<append("Math.Acos");
                    }
                    else if(theToken == "atan")
                    {
                        sb<<append("Math.Atan");
                    }
                    else if(theToken == "sec")
                    {
                        sb<<append("MathKGI.Sec");
                    }
                    else if(theToken == "csc")
                    {
                        sb<<append("MathKGI.Csc");
                    }
                    else if(theToken == "cot")
                    {
                        sb<<append("MathKGI.Cot");
                    }
                    else if(theToken == "arcsec")
                    {
                        sb<<append("MathKGI.Asec");
                    }
                    else if(theToken == "arccsc")
                    {
                        sb<<append("MathKGI.Acsc");
                    }
                    else if(theToken == "arccot")
                    {
                        sb<<append("MathKGI.Acot");
                    }
                    else if(theToken == "sinh")
                    {
                        sb<<append("Math.Sinh");
                    }
                    else if(theToken == "cosh")
                    {
                        sb<<append("Math.Cosh");
                    }
                    else if(theToken == "tanh")
                    {
                        sb<<append("Math.Tanh");
                    }
                    else if(theToken == "arcsinh")
                    {
                        sb<<append("MathKGI.Asinh");
                    }
                    else if(theToken == "arccosh")
                    {
                        sb<<append("MathKGI.Acosh");
                    }
                    else if(theToken == "arctanh")
                    {
                        sb<<append("MathKGI.Atanh");
                    }
                    else if(theToken == "sech")
                    {
                        sb<<append("MathKGI.Sech");
                    }
                    else if(theToken == "csch")
                    {
                        sb<<append("MathKGI.Csch");
                    }
                    else if(theToken == "coth")
                    {
                        sb<<append("MathKGI.Coth");
                    }
                    else if(theToken == "arcsech")
                    {
                        sb<<append("MathKGI.Asech");
                    }
                    else if(theToken == "arccsch")
                    {
                        sb<<append("MathKGI.Acsch");
                    }
                    else if(theToken == "arccoth")
                    {
                               sb<<append("MathKGI.Acoth");
                    }
                    else if(theToken == "pi")
                    {
                        sb<<append("Math.PI");
                    }
                    else if(theToken == "exponentiale")
                    {
                        sb<<append("Math.E");
                    }
                    else if(theToken == "avogadro")
                    {
                        sb<<append("6.02214179e23");
                    }
                    else if(theToken == "true")
                    {
                               //sb<<append("true");
                        sb<<append("1.0");
                    }
                    else if(theToken == "false")
                    {
                               //sb<<append("false");
                        sb<<append("0.0");
                    }
                    else if(theToken == "gt")
                    {
                        sb<<append("supportFunctions._gt");
                    }
                    else if(theToken == "lt")
                    {
                        sb<<append("supportFunctions._lt");
                    }
                    else if(theToken == "eq")
                    {
                        sb<<append("supportFunctions._eq");
                    }
                    else if(theToken == "neq")
                    {
                        sb<<append("supportFunctions._neq");
                    }
                    else if(theToken == "geq")
                    {
                        sb<<append("supportFunctions._geq");
                    }
                    else if(theToken == "leq")
                    {
                        sb<<append("supportFunctions._leq");
                    }
                    else if(theToken == "and")
                    {
                        sb<<append("supportFunction._and");
                    }
                    else if(theToken == "or")
                    {
                        sb<<append("supportFunction._or");
                    }
                    else if(theToken == "not")
                    {
                        sb<<append("supportFunction._not");
                    }
                    else if(theToken == "xor")
                    {
                        sb<<append("supportFunction._xor");
                    }
                    else if(theToken == "root")
                    {
                        sb<<append("supportFunctions._root");
                    }
                   else if(theToken == "squarewave")
                    {
                        sb<<append("supportFunctions._squarewave");
                    }
                    else if(theToken == "piecewise")
                    {
                        sb<<append("supportFunctions._piecewise");
                    }
                    else if (!mFunctionParameters.Contains(s.tokenString))
                    {
                        throw Exception("Token '" + s.tokenString + "' not recognized.");
                    }
                    else
                    {
                        sb<<append(s.tokenString);
                    }

                break; //Word token

                   case CodeTypes::tDoubleToken:
                       sb<<append(writeDouble(s.tokenDouble));
                   break;
                   case CodeTypes::tIntToken:
                    sb<<append((int) s.tokenInteger);
                       break;
                   case CodeTypes::tPlusToken:
                   sb<<append("+");
                   break;
                   case CodeTypes::tMinusToken:
                   sb<<append("-");
                   break;
                   case CodeTypes::tDivToken:
                   sb<<append("/");
                   break;
                   case CodeTypes::tMultToken:
                   sb<<append(mFixAmountCompartments);
                   break;
                   case CodeTypes::tPowerToken:
                   sb<<append("^");
                   break;
                   case CodeTypes::tLParenToken:
                   sb<<append("(");
                   break;
                   case CodeTypes::tRParenToken:
                   sb<<append(")");
                   break;
                   case CodeTypes::tCommaToken:
                   sb<<append(",");
                   break;
                   case CodeTypes::tEqualsToken:
                   sb<<append(" = ");
                   break;
                   case CodeTypes::tTimeWord1:
                   sb<<append("time");
                   break;
                   case CodeTypes::tTimeWord2:
                   sb<<append("time");
                   break;
                   case CodeTypes::tTimeWord3:
                   sb<<append("time");
                   break;
                   case CodeTypes::tAndToken:
                   sb<<append("supportFunctions._and");
                   break;
                   case CodeTypes::tOrToken:
                   sb<<append("supportFunctions._or");
                   break;
                   case CodeTypes::tNotToken:
                   sb<<append("supportFunctions._not");
                   break;
                   case CodeTypes::tLessThanToken:
                   sb<<append("supportFunctions._lt");
                   break;
                   case CodeTypes::tLessThanOrEqualToken:
                   sb<<append("supportFunctions._leq");
                   break;
                   case CodeTypes::tMoreThanOrEqualToken:
                   sb<<append("supportFunctions._geq");
                   break;
                   case CodeTypes::tMoreThanToken:
                   sb<<append("supportFunctions._gt");
                   break;
                   case CodeTypes::tXorToken:
                   sb<<append("supportFunctions._xor");
                   break;
                   default:
                   stringstream msg;
                   msg<< "Unknown token in convertUserFunctionExpression: " << s.tokenToString(s.token()) <<
                           "Exception raised in Module:roadRunner, Method:convertUserFunctionExpression";
                   throw Exception(msg.str());
               }
               s.nextToken();
        }
    }
    catch (Exception e)
    {
       throw new CoreException(e.Message());
    }
    return sb.ToString();
}

void CSharpGenerator::substituteEquation(const string& reactionName, Scanner& s, CodeBuilder& sb)
{
    string theToken(s.tokenString);
    if(theToken == "pow")
    {
        sb<<append("Math.Pow");
    }
    else if(theToken == "sqrt")
    {
        sb<<append("Math.Sqrt");
    }
    else if(theToken == "log")
    {
        sb<<append("supportFunctions._log");
    }
    else if(theToken == "floor")
    {
        sb<<append("Math.Floor");
    }
    else if(theToken == "ceil")
    {
        sb<<append("Math.Ceiling");
    }
    else if(theToken == "factorial")
    {
        sb<<append("supportFunctions._factorial");
    }
    else if(theToken == "log10")
    {
        sb<<append("Math.Log10");
    }
    else if(theToken == "exp")
    {
        sb<<append("Math.Exp");
    }
    else if(theToken == "abs")
    {
        sb<<append("Math.Abs");
    }
    else if(theToken == "sin")
    {
        sb<<append("Math.Sin");
    }
    else if(theToken == "cos")
    {
        sb<<append("Math.Cos");
    }
    else if(theToken == "tan")
    {
        sb<<append("Math.Tan");
    }
    else if(theToken == "asin")
    {
        sb<<append("Math.Asin");
    }
    else if(theToken == "acos")
    {
        sb<<append("Math.Acos");
    }
    else if(theToken == "atan")
    {
        sb<<append("Math.Atan");
    }
    else if(theToken == "sec")
    {
        sb<<append("MathKGI.Sec");
    }
    else if(theToken == "csc")
    {
        sb<<append("MathKGI.Csc");
    }
    else if(theToken == "cot")
    {
        sb<<append("MathKGI.Cot");
    }
    else if(theToken == "arcsec")
    {
        sb<<append("MathKGI.Asec");
    }
    else if(theToken == "arccsc")
    {
        sb<<append("MathKGI.Acsc");
    }
    else if(theToken == "arccot")
    {
        sb<<append("MathKGI.Acot");
    }
    else if(theToken == "sinh")
    {
        sb<<append("Math.Sinh");
    }
    else if(theToken == "cosh")
    {
        sb<<append("Math.Cosh");
    }
    else if(theToken == "tanh")
    {
        sb<<append("Math.Tanh");
    }
    else if(theToken == "arcsinh")
    {
        sb<<append("MathKGI.Asinh");
    }
    else if(theToken == "arccosh")
    {
        sb<<append("MathKGI.Acosh");
    }
    else if(theToken == "arctanh")
    {
        sb<<append("MathKGI.Atanh");
    }
    else if(theToken == "sech")
    {
        sb<<append("MathKGI.Sech");
    }
    else if(theToken == "csch")
    {
        sb<<append("MathKGI.Csch");
    }
    else if(theToken == "coth")
    {
        sb<<append("MathKGI.Coth");
    }
    else if(theToken == "arcsech")
    {
        sb<<append("MathKGI.Asech");
    }
    else if(theToken == "arccsch")
    {
        sb<<append("MathKGI.Acsch");
    }
    else if(theToken == "arccoth")
    {
        sb<<append("MathKGI.Acoth");
    }
    else if(theToken == "pi")
    {
        sb<<append("Math.PI");
    }
    else if(theToken == "avogadro")
    {
        sb<<append("6.02214179e23");
    }
    else if(theToken == "exponentiale")
    {
        sb<<append("Math.E");
    }
    else if(theToken == "true")
    {
        //sb<<append("true");
        sb<<append("1.0");
    }
    else if(theToken == "false")
    {
        //sb<<append("false");
        sb<<append("0.0");
    }
    else if(theToken == "NaN")
    {
        sb<<append("double.NaN");
    }
    else if(theToken == "INF")
    {
        sb<<append("double.PositiveInfinity");
    }
    else if(theToken == "geq")
    {
        sb<<append("supportFunctions._geq");
    }
    else if(theToken == "leq")
    {
        sb<<append("supportFunctions._leq");
    }
    else if(theToken == "gt")
    {
        sb<<append("supportFunctions._gt");
    }
    else if(theToken == "lt")
    {
        sb<<append("supportFunctions._lt");
    }
    else if(theToken == "eq")
    {
        sb<<append("supportFunctions._eq");
    }
    else if(theToken == "neq")
    {
        sb<<append("supportFunctions._neq");
    }
    else if(theToken == "and")
    {
        sb<<append("supportFunction._and");
    }
    else if(theToken == "or")
    {
        sb<<append("supportFunction._or");
    }
    else if(theToken == "not")
    {
        sb<<append("supportFunction._not");
    }
    else if(theToken == "xor")
    {
        sb<<append("supportFunction._xor");
    }
    else if(theToken == "root")
    {
        sb<<append("supportFunctions._root");
    }
    else if(theToken == "squarewave")
    {
        sb<<append("supportFunctions._squarewave");
    }
    else if(theToken == "piecewise")
    {
        sb<<append("supportFunctions._piecewise");
    }
    else if(theToken == "delay")
    {
        sb<<append("supportFunctions._delay");
        mWarnings.Add("RoadRunner does not yet support delay differential equations in SBML, they will be ignored (i.e. treated as delay = 0).");
    }
    else
    {
        bool bReplaced = false;
        int index;
        if (mReactionList.find(reactionName, index))
        {
            int nParamIndex = 0;
            if (mLocalParameterList[index].find(s.tokenString, nParamIndex))
            {
                sb<<append("_lp[" + toString(index) + "][" + toString(nParamIndex) + "]");
                bReplaced = true;
            }
        }

        if (mBoundarySpeciesList.find(s.tokenString, index))
        {
            sb<<append("_bc[" + toString(index) + "]");
            bReplaced = true;
        }
        if (!bReplaced &&
            (mFunctionParameters.Count() != 0 && !mFunctionParameters.Contains(s.tokenString)))
        {
            throw Exception("Token '" + s.tokenString + "' not recognized.");
        }
    }
}

void CSharpGenerator::substituteWords(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb)
{
    // Global parameters have priority
    int index;
    if (mGlobalParameterList.find(s.tokenString, index))
    {
        sb<<format("_gp[{0}]", index);
    }
    else if (mBoundarySpeciesList.find(s.tokenString, index))
    {
        sb<<format("_bc[{0}]", index);

        Symbol symbol = mBoundarySpeciesList[index];
        if (symbol.hasOnlySubstance)
        {
            // we only store concentration for the boundary so we better
            // fix that.
            int nCompIndex = 0;
            if (mCompartmentList.find(symbol.compartmentName, nCompIndex))
            {
                sb<<format("{0}_c[{1}]", mFixAmountCompartments, nCompIndex);
            }
        }
    }
    else if (mFloatingSpeciesConcentrationList.find(s.tokenString, index))
    {
        Symbol floating1 = mFloatingSpeciesConcentrationList[index];
        if (floating1.hasOnlySubstance)
        {
            sb<<format("amounts[{0}]", index);
        }
        else
        {
            sb<<format("_y[{0}]", index);
        }
    }
    else if (mCompartmentList.find(s.tokenString, index))
    {
        sb<<format("_c[{0}]", index);
    }
    else if (mFunctionNames.Contains(s.tokenString))
    {
        sb<<format("{0} ", s.tokenString);
    }
    else if (mModifiableSpeciesReferenceList.find(s.tokenString, index))
    {
        sb<<format("_sr[{0}]", index);
    }
    else if (mReactionList.find(s.tokenString, index))
    {
        sb<<format("_rates[{0}]", index);
    }
    else
    {
        substituteEquation(reactionName, s, sb);
    }
}

void CSharpGenerator::substituteToken(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb)
{
    string aToken = s.tokenString;
    CodeTypes codeType = s.token();
    switch(codeType)
    {
        case CodeTypes::tWordToken:
        case CodeTypes::tExternalToken:
        case CodeTypes::tExtToken:
            substituteWords(reactionName, bFixAmounts, s, sb);
            break;

        case CodeTypes::tDoubleToken:
            sb<<append("(double)" + writeDouble(s.tokenDouble));
            break;
        case CodeTypes::tIntToken:
            sb<<append("(double)" + writeDouble((double)s.tokenInteger));
            break;
        case CodeTypes::tPlusToken:
            sb<<format("+{0}\t", NL());
            break;
        case CodeTypes::tMinusToken:
            sb<<format("-{0}\t", NL());
            break;
        case CodeTypes::tDivToken:
            sb<<format("/{0}\t", NL());
            break;
        case CodeTypes::tMultToken:
            sb<<format("*{0}\t", NL());
            break;
        case CodeTypes::tPowerToken:
            sb<<format("^{0}\t", NL());
            break;
        case CodeTypes::tLParenToken:
            sb<<append("(");
            break;
        case CodeTypes::tRParenToken:
            sb<<format("){0}\t", NL());
            break;
        case CodeTypes::tCommaToken:
            sb<<append(",");
            break;
        case CodeTypes::tEqualsToken:
            sb<<format(" = {0}\t", NL());
            break;
      case CodeTypes::tTimeWord1:
            sb<<append("time");
            break;
        case CodeTypes::tTimeWord2:
            sb<<append("time");
            break;
        case CodeTypes::tTimeWord3:
            sb<<append("time");
            break;
        case CodeTypes::tAndToken:
            sb<<format("{0}supportFunctions._and", NL());
            break;
        case CodeTypes::tOrToken:
            sb<<format("{0}supportFunctions._or", NL());
            break;
        case CodeTypes::tNotToken:
            sb<<format("{0}supportFunctions._not", NL());
            break;
        case CodeTypes::tLessThanToken:
            sb<<format("{0}supportFunctions._lt", NL());
            break;
        case CodeTypes::tLessThanOrEqualToken:
            sb<<format("{0}supportFunctions._leq", NL());
            break;
        case CodeTypes::tMoreThanOrEqualToken:
            sb<<format("{0}supportFunctions._geq", NL());
            break;
        case CodeTypes::tMoreThanToken:
            sb<<format("{0}supportFunctions._gt", NL());
            break;
        case CodeTypes::tXorToken:
            sb<<format("{0}supportFunctions._xor", NL());
            break;
        default:
        string aToken = s.tokenToString(s.token());
        Exception ae = Exception(
                 format("Unknown token in substituteTerms: {0}", aToken,
                 "Exception raised in Module:roadRunner, Method:substituteTerms"));
         throw ae;
    }
}

void CSharpGenerator::writeOutSymbolTables(CodeBuilder& sb)
{
    sb<<append("\tvoid loadSymbolTables() {" + NL());

    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        sb<<format("\t\tvariableTable[{0}] = \"{1}\";{2}", i, mFloatingSpeciesConcentrationList[i].name, NL());
    }

    for (int i = 0; i < mBoundarySpeciesList.size(); i++)
    {
        sb<<format("\t\tboundaryTable[{0}] = \"{1}\";{2}", i, mBoundarySpeciesList[i].name, NL());
    }

    for (int i = 0; i < mGlobalParameterList.size(); i++)
    {
        string name = mGlobalParameterList[i].name;
           sb<<format("\t\tglobalParameterTable[{0}] = \"{1}\";{2}", i, mGlobalParameterList[i].name, NL());
    }
    sb<<format("\t}{0}{0}", NL());
}

int CSharpGenerator::readFloatingSpecies()
{
    // Load a reordered list into the variable list.
    StringList reOrderedList;
//    if (mRR && mRR->mComputeAndAssignConservationLaws)
	if(mComputeAndAssignConsevationLaws)
    {
       reOrderedList = mLibStruct.getReorderedSpecies();
    }
    else
    {
        reOrderedList = mLibStruct.getSpecies();
    }

    StringListContainer oFloatingSpecies = mNOM.getListOfFloatingSpecies();

    for (int i = 0; i < reOrderedList.Count(); i++)
    {
        for (int j = 0; j < oFloatingSpecies.Count(); j++)
        {
            StringList oTempList = oFloatingSpecies[j];
              if(reOrderedList[i] != (const string&) oTempList[0])
              {
                  continue;
              }

            string compartmentName = mNOM.getNthFloatingSpeciesCompartmentName(j);
            bool bIsConcentration  = toBool(oTempList[2]);
            double dValue = toDouble(oTempList[1]);
            if (isNaN(dValue))
            {
                  dValue = 0;
            }

            Symbol *symbol = NULL;
            if (bIsConcentration)
            {
              symbol = new Symbol(reOrderedList[i], dValue, compartmentName);
            }
            else
            {
              int nCompartmentIndex;
              mCompartmentList.find(compartmentName, nCompartmentIndex);

              double dVolume = mCompartmentList[nCompartmentIndex].value;
              if (isNaN(dVolume))
              {
                  dVolume = 1;
              }

              stringstream formula;
              formula<<toString(dValue, mDoubleFormat)<<"/ _c["<<nCompartmentIndex<<"]";

              symbol = new Symbol(reOrderedList[i],
                  dValue / dVolume,
                  compartmentName,
                  formula.str());
            }

            if(mNOM.GetModel())
            {
                Species *aSpecies = mNOM.GetModel()->getSpecies(reOrderedList[i]);
                if(aSpecies)
                {
                    symbol->hasOnlySubstance = aSpecies->getHasOnlySubstanceUnits();
                    symbol->constant = aSpecies->getConstant();
                }
            }
            else
            {
                //TODO: How to report error...?
                //Log an error...
                symbol->hasOnlySubstance = false;
            }
            Log(lDebug5)<<"Adding symbol to mFloatingSpeciesConcentrationList:"<<(*symbol);
            mFloatingSpeciesConcentrationList.Add(*(symbol));
            break;
          }
          //throw RRException("Reordered Species " + reOrderedList[i] + " not found.");
      }
      return oFloatingSpecies.Count();
}

int CSharpGenerator::readBoundarySpecies()
{
    int numBoundarySpecies;
    StringListContainer oBoundarySpecies = mNOM.getListOfBoundarySpecies();
    numBoundarySpecies = oBoundarySpecies.Count(); // sp1.size();
    for (int i = 0; i < numBoundarySpecies; i++)
    {
        StringList oTempList     = oBoundarySpecies[i];
        string sName             = oTempList[0];
        string compartmentName     = mNOM.getNthBoundarySpeciesCompartmentName(i);
        bool bIsConcentration     = toBool(oTempList[2]);
        double dValue             = toDouble(oTempList[1]);
        if (isNaN(dValue))
        {
            dValue = 0;
        }

        Symbol *symbol = NULL;
        if (bIsConcentration)
        {
            symbol = new Symbol(sName, dValue, compartmentName);
        }
        else
        {
            int nCompartmentIndex;
            double dVolume;
            if(mCompartmentList.find(compartmentName, nCompartmentIndex))
            {
                dVolume = mCompartmentList[nCompartmentIndex].value;
            }
            else
            {
                if (isNaN(dVolume))
                {
                    dVolume = 1;
                }
            }
            stringstream formula;
            formula<<toString(dValue, mDoubleFormat)<<"/ _c["<<nCompartmentIndex<<"]";
            symbol = new Symbol(sName,
                                dValue / dVolume,
                                compartmentName,
                                formula.str());
        }

        if(mNOM.GetModel())
        {
            Species* species = mNOM.GetModel()->getSpecies(sName);
            if(species)
            {
                symbol->hasOnlySubstance = species->getHasOnlySubstanceUnits();
                symbol->constant = species->getConstant();
            }
        }
        else
        {
            //TODO: How to report error...?
            //Log an error...
            symbol->hasOnlySubstance = false;

        }
        mBoundarySpeciesList.Add(*symbol);
    }
    return numBoundarySpecies;
}

void CSharpGenerator::writeComputeAllRatesOfChange(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0)
{
    sb<<append("\t// Uses the equation: dSd/dt = L0 dSi/dt" + NL());
    sb<<append("\tpublic void computeAllRatesOfChange ()" + NL());
    sb<<append("\t{" + NL());
    sb<<append("\t\tdouble[] dTemp = new double[amounts.Length + rateRules.Length];" + NL());
    for (int i = 0; i < numAdditionalRates(); i++)
    {
        sb<<format("\t\tdTemp[{0}] = {1};{2}", i, mMapRateRule[i], NL());
    }
    //sb<<append("\t\trateRules.CopyTo(dTemp, 0);" + NL());
    sb<<append("\t\tamounts.CopyTo(dTemp, rateRules.Length);" + NL());
    sb<<append("\t\tevalModel (time, dTemp);" + NL());
    bool isThereAnEntry = false;
    for (int i = 0; i < numDependentSpecies; i++)
    {
        sb<<format("\t\t_dydt[{0}] = ", (numIndependentSpecies + i));
        isThereAnEntry = false;
        for (int j = 0; j < numIndependentSpecies; j++)
        {
            string dyName = format("_dydt[{0}]", j);

            if (L0(i,j) > 0)
            {
                isThereAnEntry = true;
                if (L0(i,j) == 1)
                {
                    sb<<format(" + {0}{1}", dyName, NL());
                }
                else
                {
                    sb<<format(" + (double){0}{1}{2}{3}", writeDouble(L0(i,j)), mFixAmountCompartments, dyName, NL());
                }
            }
            else if (L0(i,j) < 0)
            {
                isThereAnEntry = true;
                if (L0(i,j) == -1)
                {
                    sb<<format(" - {0}{1}", dyName, NL());
                }
                else
                {
                    sb<<format(" - (double){0}{1}{2}{3}", writeDouble(fabs(L0(i,j))), mFixAmountCompartments, dyName, NL());
                }
            }
        }
        if (!isThereAnEntry)
        {
            sb<<append("0");
        }
        sb<<format(";{0}", NL());
    }

    sb<<format("\t}{0}{0}", NL());
}

void CSharpGenerator::writeComputeConservedTotals(CodeBuilder& sb, const int& numFloatingSpecies, const int& numDependentSpecies)
{
    sb<<append("\t// Uses the equation: C = Sd - L0*Si" + NL());
    sb<<append("\tpublic void computeConservedTotals ()" + NL());
    sb<<append("\t{" + NL());
    if (numDependentSpecies > 0)
    {
        string factor;
        ls::DoubleMatrix* gamma = mLibStruct.getGammaMatrix();

//        DoubleMatrix gamma(matPtr, numDependentSpecies, numFloatingSpecies);
        for (int i = 0; i < numDependentSpecies; i++)
        {
            sb<<format("\t\t_ct[{0}] = ", i);
            for (int j = 0; j < numFloatingSpecies; j++)
            {
                double current = (gamma != NULL) ? (*gamma)(i,j) : 1.0;    //Todo: This is a bug? We should not be here if the matrix i NULL.. Triggered by model 00029

                if ( current != 0.0 )
                {
                    if (!gamma)//isNaN(current)) //C# code is doing one of these.. factor = "" .. ??
                    {
                        // TODO: fix this
                        factor = "";
                    }
                    else if (fabs(current) == 1.0)
                    {
                        factor = "";
                    }
                    else
                    {
                        factor = writeDouble(fabs(current)) +
                                 mFixAmountCompartments;
                    }

                    if (current > 0)
                    {
                        string cYY = convertSpeciesToY(mFloatingSpeciesConcentrationList[j].name);
                        string cTC = convertCompartmentToC(mFloatingSpeciesConcentrationList[j].compartmentName);
                        sb<<append(" + " + factor + convertSpeciesToY(mFloatingSpeciesConcentrationList[j].name) +
                                  mFixAmountCompartments +
                                  convertCompartmentToC(mFloatingSpeciesConcentrationList[j].compartmentName) +
                                  NL());
                    }
                    else
                    {
                        sb<<append(" - " + factor + convertSpeciesToY(mFloatingSpeciesConcentrationList[j].name) +
                                  mFixAmountCompartments +
                                  convertCompartmentToC(mFloatingSpeciesConcentrationList[j].compartmentName) +
                                  NL());
                    }
                }
            }
            sb<<append(";" + NL());
            mConservationList.Add(Symbol("CSUM" + toString(i))); //TODO: how to deal with this?
        }
    }
    sb<<append("    }" + NL() + NL());
}

void CSharpGenerator::writeUpdateDependentSpecies(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0)
{
    sb<<append("\t// Compute values of dependent species " + NL());
    sb<<append("\t// Uses the equation: Sd = C + L0*Si" + NL());
    sb<<append("\tpublic void updateDependentSpeciesValues (double[] y)" + NL());
    sb<<append("\t{" + NL());

    if (numDependentSpecies > 0)
    {
        // Use the equation: Sd = C + L0*Si to compute dependent concentrations
        if (numDependentSpecies > 0)
        {
            for (int i = 0; i < numDependentSpecies; i++)
            {
                sb<<format("\t\t_y[{0}] = {1}\t", (i + numIndependentSpecies), NL());
                sb<<format("(_ct[{0}]", i);
                string cLeftName =
                    convertCompartmentToC(
                        mFloatingSpeciesConcentrationList[i + numIndependentSpecies].compartmentName);

                for (int j = 0; j < numIndependentSpecies; j++)
                {
                    string yName = format("y[{0}]", j);
                    string cName = convertCompartmentToC(mFloatingSpeciesConcentrationList[j].compartmentName);
                    double* mat = L0.GetPointer();
                    double matElementValue = L0(i,j);

                    if (L0(i,j) > 0) // In C# code there is no checking for index out of bound..
                    {
                        if (L0(i,j) == 1)
                        {
                            sb<<format(" + {0}\t{1}{2}{3}{0}\t",
                                NL(),
                                yName,
                                mFixAmountCompartments,
                                cName);
                        }
                        else
                        {
                            sb<<format("{0}\t + (double){1}{2}{3}{2}{4}",
                                NL(),
                                writeDouble(L0(i,j)),
                                mFixAmountCompartments,
                                yName,
                                cName);
                        }
                    }
                    else if (L0(i,j) < 0)
                    {
                        if (L0(i,j) == -1)
                        {
                            sb<<format("{0}\t - {1}{2}{3}",
                                NL(),
                                yName,
                                mFixAmountCompartments,
                                cName);
                        }
                        else
                        {
                            sb<<format("{0}\t - (double){1}{2}{3}{2}{4}",
                                NL(),
                                writeDouble(fabs(L0(i,j))),
                                mFixAmountCompartments,
                                yName,
                                cName);
                        }
                    }
                }
                sb<<format(")/{0};{1}", cLeftName, NL());
            }
        }
    }
    sb<<format("\t}{0}{0}", NL());
}

void CSharpGenerator::writeUserDefinedFunctions(CodeBuilder& sb)
{
    for (int i = 0; i < mNOM.getNumFunctionDefinitions(); i++)
    {
        try
        {
            StringListContainer oList = mNOM.getNthFunctionDefinition(i);
            StringList aList = oList[0];

              string sName = aList[0];
              //sName.Trim();
            mFunctionNames.Add(sName);
            StringList oArguments = oList[1];
            StringList list2 = oList[2];
            string sBody = list2[0];

            sb<<format("\t// User defined function:  {0}{1}", sName, NL());
            sb<<format("\tpublic double {0} (", sName);

            for (int j = 0; j < oArguments.Count(); j++)
            {
                sb<<append("double " + (string)oArguments[j]);
                mFunctionParameters.Add((string)oArguments[j]);
                if (j < oArguments.Count() - 1)
                    sb<<append(", ");
            }
            sb<<append(")" + NL() + "\t{" + NL() + "\t\t return " +
                      convertUserFunctionExpression(sBody)
                      + ";" + NL() + "\t}" + NL() + NL());
        }
        catch (const Exception& ex)
        {
            CodeBuilder msg;
            msg<<"Error while trying to get Function Definition #" << i <<ex.what() << "\r\n\r\n";
            throw Exception(msg.ToString());
        }
    }
}

void CSharpGenerator::writeResetEvents(CodeBuilder& sb, const int& numEvents)
{
      sb<<format("{0}\tpublic void resetEvents() {{0}", NL());
      for (int i = 0; i < numEvents; i++)
      {
          sb<<format("\t\t_eventStatusArray[{0}] = false;{1}", i, NL());
          sb<<format("\t\t_previousEventStatusArray[{0}] = false;{1}", i, NL());
      }
      sb<<format("\t}{0}{0}", NL());
}

void CSharpGenerator::writeSetConcentration(CodeBuilder& sb)
{
    sb<<format("\tpublic void setConcentration(int index, double value) {{0}", NL());
    sb<<format("\t\tdouble volume = 0.0;{0}", NL());
    sb<<format("\t\t_y[index] = value;{0}", NL());
    sb<<format("\t\tswitch (index) {{0}", NL());
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        sb<<format("\t\t\tcase {0}: volume = {1};{2}",
          i,
          convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName),
          NL());
      sb<<format("\t\t\t\tbreak;{0}", NL());
    }
    sb<<format("\t\t}{0}", NL());
    sb<<format("\t\t_amounts[index] = _y[index]*volume;{0}", NL());
    sb<<format("\t}{0}{0}", NL());
}

void CSharpGenerator::writeGetConcentration(CodeBuilder& sb)
{
    sb<<format("\tpublic double getConcentration(int index) {{0}", NL());
    sb<<format("\t\treturn _y[index];{0}", NL());
    sb<<format("\t}{0}{0}", NL());
}

void CSharpGenerator::writeConvertToAmounts(CodeBuilder& sb)
{
    sb<<format("\tpublic void convertToAmounts() {{0}", NL());
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        sb<<format("\t\t_amounts[{0}] = _y[{0}]*{1};{2}",
            i,
            convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName),
            NL());
    }
    sb<<format("\t}{0}{0}", NL());
}

void CSharpGenerator::writeConvertToConcentrations(CodeBuilder& sb)
{
    sb<<append("\tpublic void convertToConcentrations() {" + NL());
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        sb<<"\t\t_y[" << i << "] = _amounts[" << i << "]/" <<
                  convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
    }
    sb<<append("\t}" + NL() + NL());
}

void CSharpGenerator::writeProperties(CodeBuilder& sb)
{
    sb<<append("\tpublic double[] y {" + NL());
    sb<<append("\t\tget { return _y; } " + NL());
    sb<<append("\t\tset { _y = value; } " + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] init_y {" + NL());
    sb<<append("\t\tget { return _init_y; } " + NL());
    sb<<append("\t\tset { _init_y = value; } " + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] amounts {" + NL());
    sb<<append("\t\tget { return _amounts; } " + NL());
    sb<<append("\t\tset { _amounts = value; } " + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] bc {" + NL());
    sb<<append("\t\tget { return _bc; } " + NL());
    sb<<append("\t\tset { _bc = value; } " + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] gp {" + NL());
    sb<<append("\t\tget { return _gp; } " + NL());
    sb<<append("\t\tset { _gp = value; } " + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] sr {" + NL());
    sb<<append("\t\tget { return _sr; } " + NL());
    sb<<append("\t\tset { _sr = value; } " + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[][] lp {" + NL());
    sb<<append("\t\tget { return _lp; } " + NL());
    sb<<append("\t\tset { _lp = value; } " + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] c {" + NL());
    sb<<append("\t\tget { return _c; } " + NL());
    sb<<append("\t\tset { _c = value; } " + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] dydt {" + NL());
    sb<<append("\t\tget { return _dydt; }" + NL());
    sb<<append("\t\tset { _dydt = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] rateRules {" + NL());
    sb<<append("\t\tget { return _rateRules; }" + NL());
    sb<<append("\t\tset { _rateRules = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] rates {" + NL());
    sb<<append("\t\tget { return _rates; }" + NL());
    sb<<append("\t\tset { _rates = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] ct {" + NL());
    sb<<append("\t\tget { return _ct; }" + NL());
    sb<<append("\t\tset { _ct = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] eventTests {" + NL());
    sb<<append("\t\tget { return _eventTests; }" + NL());
    sb<<append("\t\tset { _eventTests = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic TEventDelayDelegate[] eventDelay {" + NL());
    sb<<append("\t\tget { return _eventDelay; }" + NL());
    sb<<append("\t\tset { _eventDelay = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic bool[] eventType {" + NL());
    sb<<append("\t\tget { return _eventType; }" + NL());
    sb<<append("\t\tset { _eventType = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic bool[] eventPersistentType {" + NL());
    sb<<append("\t\tget { return _eventPersistentType; }" + NL());
    sb<<append("\t\tset { _eventPersistentType = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic bool[] eventStatusArray {" + NL());
    sb<<append("\t\tget { return _eventStatusArray; }" + NL());
    sb<<append("\t\tset { _eventStatusArray = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic bool[] previousEventStatusArray {" + NL());
    sb<<append("\t\tget { return _previousEventStatusArray; }" + NL());
    sb<<append("\t\tset { _previousEventStatusArray = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double[] eventPriorities {" + NL());
    sb<<append("\t\tget { return _eventPriorities; }" + NL());
    sb<<append("\t\tset { _eventPriorities = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic TEventAssignmentDelegate[] eventAssignments {" + NL());
    sb<<append("\t\tget { return _eventAssignments; }" + NL());
    sb<<append("\t\tset { _eventAssignments = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic TComputeEventAssignmentDelegate[] computeEventAssignments {" + NL());
    sb<<append("\t\tget { return _computeEventAssignments; }" + NL());
    sb<<append("\t\tset { _computeEventAssignments = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic TPerformEventAssignmentDelegate[] performEventAssignments {" + NL());
    sb<<append("\t\tget { return _performEventAssignments; }" + NL());
    sb<<append("\t\tset { _performEventAssignments = value; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic double time {" + NL());
    sb<<append("\t\tget { return _time; }" + NL());
    sb<<append("\t\tset { _time = value; }" + NL());
    sb<<append("\t}" + NL() + NL());
}

void CSharpGenerator::writeAccessors(CodeBuilder& sb)
{
    sb<<append("\tpublic int getNumIndependentVariables {" + NL());
    sb<<append("\t\tget { return numIndependentVariables; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic int getNumDependentVariables {" + NL());
    sb<<append("\t\tget { return numDependentVariables; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic int getNumTotalVariables {" + NL());
    sb<<append("\t\tget { return numTotalVariables; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic int getNumBoundarySpecies {" + NL());
    sb<<append("\t\tget { return numBoundaryVariables; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic int getNumGlobalParameters {" + NL());
    sb<<append("\t\tget { return numGlobalParameters; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic int getNumLocalParameters(int reactionId)" + NL());
    sb<<append("\t{" + NL());
    sb<<append("\t\treturn localParameterDimensions[reactionId];" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic int getNumCompartments {" + NL());
    sb<<append("\t\tget { return numCompartments; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic int getNumReactions {" + NL());
    sb<<append("\t\tget { return numReactions; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic int getNumEvents {" + NL());
    sb<<append("\t\tget { return numEvents; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic int getNumRules {" + NL());
    sb<<append("\t\tget { return numRules; }" + NL());
    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic List<string> mWarnings {" + NL());
    sb<<append("\t\tget { return _Warnings; }" + NL());
    sb<<append("\t\tset { _Warnings = value; }" + NL());
    sb<<append("\t}" + NL() + NL());
}

 void CSharpGenerator::writeOutVariables(CodeBuilder& sb)
{
      sb<<append("\tprivate List<string> _Warnings = new List<string>();" + NL());
      sb<<append("\tprivate double[] _gp = new double[" + toString(mNumGlobalParameters + mTotalLocalParmeters) +
                "];           // Vector containing all the global parameters in the System  " + NL());
      sb<<append("\tprivate double[] _sr = new double[" + toString(mNumModifiableSpeciesReferences) +
                "];           // Vector containing all the modifiable species references  " + NL());
      sb<<append("\tprivate double[][] _lp = new double[" + toString(mNumReactions) +
                "][];       // Vector containing all the local parameters in the System  " + NL());

      sb<<append("\tprivate double[] _y = new double[", mFloatingSpeciesConcentrationList.size(),
                "];            // Vector containing the concentrations of all floating species ",  NL());

      //sb<<append(String.format("\tprivate double[] _init_y = new double[{0}];            // Vector containing the initial concentrations of all floating species {1}", mFloatingSpeciesConcentrationList.Count, NL()));
      sb<<format("\tprivate double[] _init_y = new double[{0}];            // Vector containing the initial concentrations of all floating species {1}", mFloatingSpeciesConcentrationList.Count(), NL());

      sb<<append("\tprivate double[] _amounts = new double[", mFloatingSpeciesConcentrationList.size(),
                "];      // Vector containing the amounts of all floating species ", NL());

      sb<<append("\tprivate double[] _bc = new double[", mNumBoundarySpecies,
                "];           // Vector containing all the boundary species concentration values   " , NL());

      sb<<append("\tprivate double[] _c = new double[" , mNumCompartments ,
                "];            // Vector containing all the compartment values   " + NL());

      sb<<append("\tprivate double[] _dydt = new double[" , mFloatingSpeciesConcentrationList.size() ,
                "];         // Vector containing rates of changes of all species   " , NL());

      sb<<append("\tprivate double[] _rates = new double[" , mNumReactions ,
                "];        // Vector containing the rate laws of all reactions    " , NL());

      sb<<append("\tprivate double[] _ct = new double[" , mNumDependentSpecies ,
                "];           // Vector containing values of all conserved sums      " , NL());

      sb<<append("\tprivate double[] _eventTests = new double[" , mNumEvents ,
                "];   // Vector containing results of any event tests        " , NL());

      sb<<append("\tprivate TEventDelayDelegate[] _eventDelay = new TEventDelayDelegate[" , mNumEvents ,
                "]; // array of trigger function pointers" , NL());

      sb<<append("\tprivate bool[] _eventType = new bool[" , mNumEvents ,
                "]; // array holding the status whether events are useValuesFromTriggerTime or not" , NL());

      sb<<append("\tprivate bool[] _eventPersistentType = new bool[" , mNumEvents ,
                "]; // array holding the status whether events are persitstent or not" , NL());

      sb<<append("\tprivate double _time;" , NL());
      sb<<append("\tprivate int numIndependentVariables;" , NL());
      sb<<append("\tprivate int numDependentVariables;" , NL());
      sb<<append("\tprivate int numTotalVariables;" , NL());
      sb<<append("\tprivate int numBoundaryVariables;" , NL());
      sb<<append("\tprivate int numGlobalParameters;" , NL());
      sb<<append("\tprivate int numCompartments;" , NL());
      sb<<append("\tprivate int numReactions;" , NL());
      sb<<append("\tprivate int numRules;" , NL());
      sb<<append("\tprivate int numEvents;" , NL());
      sb<<append("\tstring[] variableTable = new string[" , mFloatingSpeciesConcentrationList.size() , "];" , NL());
      sb<<append("\tstring[] boundaryTable = new string[" , mBoundarySpeciesList.size() , "];" , NL());
      sb<<append("\tstring[] globalParameterTable = new string[" , mGlobalParameterList.size() , "];" , NL());
      sb<<append("\tint[] localParameterDimensions = new int[" , mNumReactions , "];" , NL());
      sb<<append("\tprivate TEventAssignmentDelegate[] _eventAssignments;" , NL());
      sb<<append("\tprivate double[] _eventPriorities;" , NL());
      sb<<append("\tprivate TComputeEventAssignmentDelegate[] _computeEventAssignments;" , NL());
      sb<<append("\tprivate TPerformEventAssignmentDelegate[] _performEventAssignments;" , NL());
      sb<<append("\tprivate bool[] _eventStatusArray = new bool[" , mNumEvents , "];" , NL());
      sb<<append("\tprivate bool[] _previousEventStatusArray = new bool[" , mNumEvents , "];" , NL());
      sb<<append(NL());
      sb<<append("\tpublic TModel ()  " , NL());
      sb<<append("\t{" , NL());

      sb<<append("\t\tnumIndependentVariables = " , mNumIndependentSpecies , ";" , NL());
      sb<<append("\t\tnumDependentVariables = " , mNumDependentSpecies , ";" , NL());
      sb<<append("\t\tnumTotalVariables = " , mNumFloatingSpecies , ";" , NL());
      sb<<append("\t\tnumBoundaryVariables = " , mNumBoundarySpecies , ";" , NL());
      sb<<append("\t\tnumGlobalParameters = " , mGlobalParameterList.size() , ";" , NL());
      sb<<append("\t\tnumCompartments = " , mCompartmentList.size() , ";" , NL());
      sb<<append("\t\tnumReactions = " , mReactionList.size() , ";" , NL());
      sb<<append("\t\tnumEvents = " , mNumEvents , ";" , NL());
      sb<<append("\t\tInitializeDelays();" , NL());

      // Declare any eventAssignment delegates
      if (mNumEvents > 0)
      {
          sb<<append("\t\t_eventAssignments = new TEventAssignmentDelegate[numEvents];" , NL());
          sb<<append("\t\t_eventPriorities = new double[numEvents];" , NL());
          sb<<append("\t\t_computeEventAssignments= new TComputeEventAssignmentDelegate[numEvents];" , NL());
          sb<<append("\t\t_performEventAssignments= new TPerformEventAssignmentDelegate[numEvents];" , NL());

          for (int i = 0; i < mNumEvents; i++)
          {
              string iStr = toString(i);
              sb<<append("\t\t_eventAssignments[" + iStr + "] = new TEventAssignmentDelegate (eventAssignment_" + iStr +
                        ");" + NL());
              sb<<append("\t\t_computeEventAssignments[" + iStr +
                        "] = new TComputeEventAssignmentDelegate (computeEventAssignment_" + iStr + ");" + NL());
              sb<<append("\t\t_performEventAssignments[" + iStr +
                        "] = new TPerformEventAssignmentDelegate (performEventAssignment_" + iStr + ");" + NL());
          }

          sb<<append("\t\tresetEvents();" + NL());
          sb<<append(NL());
      }

      if (mNumModifiableSpeciesReferences > 0)
      {
          for (int i = 0; i < mModifiableSpeciesReferenceList.size(); i++)
          {
              sb<<append("\t\t_sr[" + toString(i) + "]  = " + writeDouble(mModifiableSpeciesReferenceList[i].value) + ";" + NL());
          }
          sb<<append(NL());
      }

      // Declare space for local parameters
      for (int i = 0; i < mNumReactions; i++)
      {
          sb<<append("\t\tlocalParameterDimensions[" + toString(i) + "] = " , mLocalParameterDimensions[i] , ";" + NL());
          sb<<append("\t\t_lp[" + toString(i) + "] = new double[" , mLocalParameterDimensions[i] , "];" , NL());
      }

      sb<<append("\t}" + NL() + NL());
}

void CSharpGenerator::writeClassHeader(CodeBuilder& sb)
{
    sb<<append("using System;" + NL());
    sb<<append("using System.IO;" + NL());
    sb<<append("using System.Collections;" + NL());
    sb<<append("using System.Collections.Generic;" + NL());
    sb<<append("using LibRoadRunner;" + NL());

    sb<<append(" " + NL() + NL());
    sb<<append(NL());
    sb<<format("class TModel : IModel{0}", NL());
    sb<<append("{" + NL());
    sb<<format("\t// Symbol Mappings{0}{0}", NL());
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {

        sb<<"\t// y["<<i<<"] = "<<mFloatingSpeciesConcentrationList[i].name<<endl;//{2}", NL());
    }
    sb<<append(NL());
}

string CSharpGenerator::findSymbol(const string& varName)
{
      int index = 0;
      if (mFloatingSpeciesConcentrationList.find(varName, index))
      {
          return format("\t\t_y[{0}]", index);
      }
      else if (mGlobalParameterList.find(varName, index))
      {
          return format("\t\t_gp[{0}]", index);
      }
      else if (mBoundarySpeciesList.find(varName, index))
      {
          return format("\t\t_bc[{0}]", index);
      }
      else if (mCompartmentList.find(varName, index))
      {
          return format("\t\t_c[{0}]", index);
      }
      else if (mModifiableSpeciesReferenceList.find(varName, index))
          return format("\t\t_sr[{0}]", index);

      else
          throw Exception(format("Unable to locate lefthand side symbol in assignment[{0}]", varName));
}

void CSharpGenerator::writeTestConstraints(CodeBuilder& sb)
{
    sb<<append("\tpublic void testConstraints()" + NL());
    sb<<append("\t{" + NL());

    for (int i = 0; i < mNOM.getNumConstraints(); i++)
    {
        string sMessage;
        string sCheck = mNOM.getNthConstraint(i, sMessage);

        sb<<append("\t\tif (" + substituteTerms(mNOM.getNumReactions(), "", sCheck) + " == 0.0 )" + NL());
        sb<<append("\t\t\tthrow new Exception(\"" + sMessage + "\");" + NL());
    }

    sb<<append("\t}" + NL() + NL());
}

void CSharpGenerator::writeEvalInitialAssignments(CodeBuilder& sb, const int& numReactions)
{
    sb<<append("\tpublic void evalInitialAssignments()" + NL());
    sb<<append("\t{" + NL());

    int numInitialAssignments = mNOM.getNumInitialAssignments();

    if (numInitialAssignments > 0)
    {
        vector< pair<string, string> > oList;// = new List<Pair<string, string>>();
        for (int i = 0; i < numInitialAssignments; i++)
        {
            pair<string, string> pair = mNOM.getNthInitialAssignmentPair(i);
            oList.push_back(mNOM.getNthInitialAssignmentPair(i));
        }

        // sort them ...
        bool bChange = true;
        int nIndex = -1;
        while (bChange)
        {
            bChange = false;

            for (int i = 0; i < oList.size(); i++)
            {
                pair<string, string> current = oList[i];
                for (int j = i + 1; j < oList.size(); j++)
                {
                    if (expressionContainsSymbol(current.second, oList[j].first))
                    {
                        bChange = true;
                        nIndex = j;
                        break;
                    }
                }
                if (bChange)
                {
                    break;
                }
            }

            if (bChange)
            {
                pair<string, string> pairToMove = oList[nIndex];
                oList.erase(oList.begin() + nIndex);
                //oList.RemoveAt(nIndex);
                oList.insert(oList.begin(), pairToMove);    //Todo: check it this is correct...
            }
        }

        vector< pair<string, string> >::iterator iter;
        for(iter = oList.begin(); iter < oList.end(); iter++)
        {
            pair<string, string>& pair = (*iter);
            string leftSideRule = findSymbol(pair.first);
            string rightSideRule = pair.second;
            if (leftSideRule.size())
            {
                sb<<append(leftSideRule + " = ");
                sb<<append(substituteTerms(numReactions, "", rightSideRule) + ";" + NL());
            }
        }
    }
    for (int i = 0; i < mNOM.GetModel()->getNumEvents(); i++)
    {
        libsbml::Event *current = mNOM.GetModel()->getEvent(i);
        string initialTriggerValue = toString(current->getTrigger()->getInitialValue());//.toString().ToLowerInvariant();
        sb<<append("\t\t_eventStatusArray[" + toString(i) + "] = " + initialTriggerValue + ";" + NL());
        sb<<append("\t\t_previousEventStatusArray[" + toString(i) + "] = " + initialTriggerValue + ";" + NL());
    }
    sb<<append("\t}" + NL() + NL());
}

int CSharpGenerator::writeComputeRules(CodeBuilder& sb, const int& numReactions)
{
    IntStringHashTable mapVariables;
    int numRateRules = 0;
    int numOfRules = mNOM.getNumRules();

    sb<<append("\tpublic void computeRules(double[] y) {" + NL());
    // ------------------------------------------------------------------------------
    for (int i = 0; i < numOfRules; i++)
    {
        try
        {
            string leftSideRule = "";
            string rightSideRule = "";
            string ruleType = mNOM.getNthRuleType(i);

            // We only support assignment and ode rules at the moment
            string eqnRule = mNOM.getNthRule(i);
            RRRule aRule(eqnRule, ruleType);
            string varName =  trim(aRule.GetLHS());    //eqnRule.Substring(0, index).Trim();
            string rightSide = trim(aRule.GetRHS());    //eqnRule.Substring(index + 1).Trim();

            bool isRateRule = false;

            switch (aRule.GetType())
            {
                case rtAlgebraic:
                    mWarnings.Add("RoadRunner does not yet support algebraic rules in SBML, they will be ignored.");
                    leftSideRule = "";//NULL;
                break;

                case rtAssignment:
                    leftSideRule = findSymbol(varName);
                break;

                case rtRate:
                    isRateRule = true;
                    int index;
                    if (mFloatingSpeciesConcentrationList.find(varName,  index))
                    {
                        leftSideRule = format("\t\t_dydt[{0}]", index);
                        mFloatingSpeciesConcentrationList[index].rateRule = true;
                    }
                    else
                    {
                        leftSideRule = "\t\t_rateRules[" + toString(numRateRules) + "]";
                        mMapRateRule[numRateRules] = findSymbol(varName);
                        mapVariables[numRateRules] = varName;
                        numRateRules++;
                    }

                break;
            }

            // Run the equation through MathML to carry out any conversions (eg ^ to Pow)
            string rightSideMathml = mNOM.convertStringToMathML(rightSide);
            rightSideRule = mNOM.convertMathMLToString(rightSideMathml);
            if (leftSideRule.size())// != NULL)
            {
                sb<<append(leftSideRule + " = ");
                int speciesIndex;
                bool isSpecies = mFloatingSpeciesConcentrationList.find(varName, speciesIndex);

                Symbol* symbol = (speciesIndex != -1) ? &(mFloatingSpeciesConcentrationList[speciesIndex]) : NULL;
                string sCompartment;

                if(isRateRule && mNOM.MultiplyCompartment(varName, sCompartment) && (rightSide.find(sCompartment) == string::npos))
                {
                    sb<<format("({0}) * {1};{2}", substituteTerms(numReactions, "", rightSideRule), findSymbol(sCompartment), NL());
                }
                else
                {
                    if (isSpecies && !isRateRule && symbol != NULL && symbol->hasOnlySubstance && symbol->compartmentName.size() != 0)
                    {
                        sb<<format("({0}) / {1};{2}", substituteTerms(numReactions, "", rightSideRule), findSymbol(symbol->compartmentName), NL());
                    }
                    else
                    {
                        sb<<format("{0};{1}", substituteTerms(numReactions, "", rightSideRule), NL());
                    }
                }

                if (mNOM.IsCompartment(varName))
                {
                    sb<<append("\t\tconvertToConcentrations();");
                }
            }
        }
        catch (Exception& e)
        {
            throw CoreException("Error while trying to get Rule #" + toString(i) + e.Message());
        }
    }

    sb<<append("\t}" + NL() + NL());
    sb<<append("\tprivate double[] _rateRules = new double[" + toString(numRateRules) +
              "];           // Vector containing values of additional rate rules      " + NL());

    sb<<append("\tpublic void InitializeRates()" + NL() + "\t{" + NL());

    for (int i = 0; i < numRateRules; i++)
    {
        sb<<"\t\t_rateRules[" << i << "] = " << mMapRateRule[i] << ";" << NL();
    }

    sb<<append("\t}" + NL() + NL());
    sb<<append("\tpublic void AssignRates()" + NL() + "\t{" + NL());

    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        sb<<(string)mMapRateRule[i] << " = _rateRules[" << i << "];" << NL();
    }

    sb<<append("\t}" + NL() + NL());

    sb<<append("\tpublic void InitializeRateRuleSymbols()" + NL() + "\t{" + NL());
    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        string varName = (string)mapVariables[i];
        double value = mNOM.getValue(varName);
        if (!isNaN(value))
        {
            sb<< mMapRateRule[i] << " = " << toString(value, mDoubleFormat) << ";" << NL();
        }
    }

    sb<<append("\t}" + NL() + NL());
    sb<<append("\tpublic void AssignRates(double[] oRates)" + NL() + "\t{" + NL());

    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        sb<< mMapRateRule[i] << " = oRates[" << i << "];" << NL();
    }

    sb<<append("\t}" + NL() + NL());
    sb<<append("\tpublic double[] GetCurrentValues()" + NL() + "\t{" + NL());
    sb<<append("\t\tdouble[] dResult = new double[" + toString(numAdditionalRates()) + "];" + NL());

    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        sb<<"\t\tdResult[" << i << "] = " << mMapRateRule[i] << ";" << NL();
    }
    sb<<append("\t\treturn dResult;" + NL());

    sb<<append("\t}" + NL() + NL());
    return numOfRules;
}

void CSharpGenerator::writeComputeReactionRates(CodeBuilder& sb, const int& numReactions)
{
    sb<<append("\t// Compute the reaction rates" + NL());
    sb<<append("\tpublic void computeReactionRates (double time, double[] y)" + NL());
    sb<<append("\t{" + NL());

    for (int i = 0; i < numReactions; i++)
    {
        string kineticLaw = mNOM.getKineticLaw(i);

        // The following code is for the case when the kineticLaw contains a ^ in place
        // of pow for exponent handling. It would not be needed in the case when there is
        // no ^ in the kineticLaw.
        string subKineticLaw;
//        if (kineticLaw.IndexOf("^", System.StringComparison.Ordinal) > 0) //Todo: fix this...
//        {
//            string kineticLaw_mathml = mNOM.convertStringToMathML(kineticLaw);
//            subKineticLaw = mNOM.convertMathMLToString(kineticLaw_mathml);
//        }
//        else
        {
            subKineticLaw = kineticLaw;
        }

        string modKineticLaw = substituteTerms(mReactionList[i].name, subKineticLaw, true) + ";";

        // modify to use current y ...
        modKineticLaw = substitute(modKineticLaw, "_y[", "y[");
        sb<<format("\t\t_rates[{0}] = {1}{2}", i, modKineticLaw, NL());
    }
    sb<<format("\t}{0}{0}", NL());
}

void CSharpGenerator::writeEvalEvents(CodeBuilder& sb, const int& numEvents, const int& numFloatingSpecies)
{
    sb<<append("\t// Event handling function" + NL());
    sb<<append("\tpublic void evalEvents (double timeIn, double[] oAmounts)" + NL());
    sb<<append("\t{" + NL());

    if (numEvents > 0)
    {
        for (int i = 0; i < numAdditionalRates(); i++)
        {
            sb<<(string) mMapRateRule[i] << " = oAmounts[" << i << "];" << NL();
        }
        for (int i = 0; i < numFloatingSpecies; i++)
        {
            sb<<"\t\t_y[" << i << "] = oAmounts[" << (i + numAdditionalRates()) << "]/" <<
                      convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
        }
    }

    sb<<append("\t\t_time = timeIn;  // Don't remove" + NL());
    sb<<append("\t\tupdateDependentSpeciesValues(_y);" + NL());
    sb<<append("\t\tcomputeRules (_y);" + NL());

    for (int i = 0; i < numEvents; i++)
    {
        ArrayList ev = mNOM.getNthEvent(i);
        StringList tempList = ev[0];
        string eventString = tempList[0];

        eventString = substituteTerms(0, "", eventString);
        sb<<"\t\tpreviousEventStatusArray[" << i << "] = eventStatusArray[" << i << "];" << NL();
        sb<<append("\t\tif (" + eventString + " == 1.0) {" + NL());
        sb<<append("\t\t     eventStatusArray[" + toString(i) + "] = true;" + NL());
        sb<<append("\t\t     eventTests[" + toString(i) + "] = 1;" + NL());
        sb<<append("\t\t} else {" + NL());
        sb<<append("\t\t     eventStatusArray[" + toString(i) + "] = false;" + NL());
        sb<<append("\t\t     eventTests[" + toString(i) + "] = -1;" + NL());
        sb<<append("\t\t}" + NL());
    }
    sb<<append("\t}" + NL() + NL());
}

void CSharpGenerator::writeEvalModel(CodeBuilder& sb, const int& numReactions, const int& numIndependentSpecies, const int& numFloatingSpecies, const int& numOfRules)
{
    sb<<append("\t// Model Function" + NL());
    sb<<append("\tpublic void evalModel (double timein, double[] oAmounts)" + NL());
    sb<<append("\t{" + NL());

    for (int i = 0; i < numAdditionalRates(); i++)
    {
        sb<<(string)mMapRateRule[i] << " = oAmounts[" << i << "];" << NL();
    }

    for (int i = 0; i < numFloatingSpecies; i++)
    {
        sb<<"\t\t_y[" << i << "] = oAmounts[" << i + numAdditionalRates() << "]/" <<
                  convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
    }

    sb<<append(NL());
    sb<<append("\t\tconvertToAmounts();" + NL());
    sb<<append("\t\t_time = timein;  // Don't remove" + NL());
    sb<<append("\t\tupdateDependentSpeciesValues (_y);" + NL());

    if (numOfRules > 0)
    {
        sb<<append("\t\tcomputeRules (_y);" + NL());
    }

    sb<<append("\t\tcomputeReactionRates (time, _y);" + NL());

    // Write out the ODE equations
    string stoich;
    for (int i = 0; i < numIndependentSpecies; i++)
    {
        CodeBuilder eqnBuilder;// = new StringBuilder(" ");
        string floatingSpeciesName = mIndependentSpeciesList[i];
        for (int j = 0; j < numReactions; j++)
        {
            Reaction *oReaction = mNOM.GetModel()->getReaction(j);
            int numProducts = (int) oReaction->getNumProducts();
            double productStoichiometry;
            for (int k1 = 0; k1 < numProducts; k1++)
            {
                SpeciesReference* product = oReaction->getProduct(k1);

                string productName = product->getSpecies();
                if (floatingSpeciesName == productName)
                {
                    productStoichiometry = product->getStoichiometry();

                    if (product->isSetId() && product->getLevel() > 2)
                    {
                        stoich = "(" +
                             substituteTerms(numReactions, "",
                                product->getId()) +
                             ") * ";
                    }
                    else if (product->isSetStoichiometry())
                    {
                        if (productStoichiometry != 1)
                        {
                            int denom = product->getDenominator();
                            if (denom != 1)
                            {
                                stoich = format("((double){0}/(double){1})*", writeDouble(productStoichiometry), denom);
                            }
                            else
                            {
                                stoich = writeDouble(productStoichiometry) + '*';
                            }
                        }
                        else
                        {
                            stoich = "";
                        }
                    }
                    else
                    {
                        if (product->isSetStoichiometryMath() && product->getStoichiometryMath()->isSetMath())
                        {
                            stoich = "(" +
                                     substituteTerms(numReactions, "",
                                        SBML_formulaToString(product->getStoichiometryMath()->getMath())) +
                                     ") * ";
                        }
                        else
                        {
                            stoich = "";
                        }
                    }
                    eqnBuilder<<format(" + {0}_rates[{1}]", stoich, j);
                }
            }

            int numReactants = (int)oReaction->getNumReactants();
            double reactantStoichiometry;
            for (int k1 = 0; k1 < numReactants; k1++)
            {
                SpeciesReference *reactant = oReaction->getReactant(k1);
                string reactantName = reactant->getSpecies();
                if (floatingSpeciesName == reactantName)
                {
                    reactantStoichiometry = reactant->getStoichiometry();

                    if (reactant->isSetId() && reactant->getLevel() > 2)
                    {
                        stoich = format("({0}) * ", substituteTerms(numReactions, "", reactant->getId()));
                    }
                    else if (reactant->isSetStoichiometry())
                    {
                        if (reactantStoichiometry != 1)
                        {
                            int denom = reactant->getDenominator();
                            if (denom != 1)
                            {
                                stoich = format("((double){0}/(double){1})*", writeDouble(reactantStoichiometry), denom);
                            }
                            else
                            {
                                stoich = writeDouble(reactantStoichiometry) + "*";
                            }
                        }
                        else
                        {
                            stoich = "";
                        }
                    }

                    else
                    {
                        if (reactant->isSetStoichiometryMath() && reactant->getStoichiometryMath()->isSetMath())
                        {
                            stoich = "(" +
                                     substituteTerms(numReactions, "",
                                        SBML_formulaToString(reactant->getStoichiometryMath()->getMath())) +
                                     ") * ";
                        }
                        else
                        {
                            stoich = "";
                        }
                    }

                    eqnBuilder<<append(format(" - {0}_rates[{1}]", stoich, j));
                }
            }
        }

        string final = eqnBuilder.ToString();//.Trim();

        if (isNullOrEmpty(final))
        {
            final = "    0.0";
        }

        if (mNOM.GetSBMLDocument()->getLevel() > 2)
        {
            // remember to take the conversion factor into account
            string factor = "";
            Species* species = mNOM.GetModel()->getSpecies(floatingSpeciesName);
            if (species != NULL)
            {
                if (species->isSetConversionFactor())
                {
                    factor = species->getConversionFactor();
                }
                else if (mNOM.GetModel()->isSetConversionFactor())
                {
                    factor = mNOM.GetModel()->getConversionFactor();
                }
            }

            if (!isNullOrEmpty(factor))
            {
                final = findSymbol(factor) + " * (" + final + ")";
            }
        }

        // If the floating species has a raterule then prevent the dydt
        // in the model function from overriding it. I think this is expected behavior.
        if (!mFloatingSpeciesConcentrationList[i].rateRule)
        {
            sb<<"\t\t_dydt[" << i << "] =" << final << ";" << NL();
        }
    }

    sb<<append("\t\tconvertToAmounts ();" + NL());
    sb<<append("\t}" + NL() + NL());
}

void CSharpGenerator::writeEventAssignments(CodeBuilder& sb, const int& numReactions, const int& numEvents)
{
    StringList delays;
    vector<bool> eventType;
    vector<bool> eventPersistentType;
    if (numEvents > 0)
    {
        sb<<append("\t// Event assignments" + NL());
        for (int i = 0; i < numEvents; i++)
        {
            ArrayList ev = mNOM.getNthEvent(i);
            eventType.push_back(mNOM.getNthUseValuesFromTriggerTime(i));
            eventPersistentType.push_back(mNOM.GetModel()->getEvent(i)->getTrigger()->getPersistent());

            StringList event = ev[1];
            int numItems = event.Count();
            string str = substituteTerms(numReactions, "", event[0]);
            delays.Add(str);

            sb<<format("\tpublic void eventAssignment_{0} () {{1}", i, NL());
            sb<<format("\t\tperformEventAssignment_{0}( computeEventAssignment_{0}() );{1}", i, NL());
            sb<<append("\t}" + NL());
            sb<<format("\tpublic double[] computeEventAssignment_{0} () {{1}", i, NL());
            StringList oTemp;
            StringList oValue;
            int nCount = 0;
            int numAssignments = ev.Count() - 2;
            sb<<format("\t\tdouble[] values = new double[ {0}];{1}", numAssignments, NL());
            for (int j = 2; j < ev.Count(); j++)
            {
                StringList asgn = (StringList) ev[j];
                //string assignmentVar = substituteTerms(numReactions, "", (string)asgn[0]);
                string assignmentVar = findSymbol((string)asgn[0]);
                string str;
                Symbol *species = getSpecies(assignmentVar);


                if (species != NULL && species->hasOnlySubstance)
                {
                    str = format("{0} = ({1}) / {2}", assignmentVar, substituteTerms(numReactions, "", (string)asgn[1]), findSymbol(species->compartmentName));
                }
                else
                {
                    str = format("{0} = {1}", assignmentVar, substituteTerms(numReactions, "", (string) asgn[1]));
                }

                string sTempVar = format("values[{0}]", nCount);

                oTemp.Add(assignmentVar);
                oValue.Add(sTempVar);

                str = sTempVar+ str.substr(str.find(" ="));
                nCount++;
                sb<<format("\t\t{0};{1}", str, NL());
            }
            sb<<append("\t\treturn values;" + NL());
            sb<<append("\t}" + NL());
            sb<<format("\tpublic void performEventAssignment_{0} (double[] values) {{1}", i, NL());

            for (int j = 0; j < oTemp.Count(); j++)
            {
                sb<<format("\t\t{0} = values[{1}];{2}", oTemp[j], j, NL());
                string aStr = (string) oTemp[j];
                aStr = trim(aStr);

                if (startsWith(aStr, "_c[")) //Todo:May have to trim?
                {
                    sb<<append("\t\tconvertToConcentrations();" + NL());
                }
            }

            sb<<append("\t}" + NL());
        }
        sb<<append("\t" + NL());
    }

    sb<<format("{0}{0}\tprivate void InitializeDelays() { {0}", NL());
    for (int i = 0; i < delays.Count(); i++)
    {
        sb<<format("\t\t_eventDelay[{0}] = new TEventDelayDelegate(delegate { return {1}; } );{2}", i, delays[i], NL());
        sb<<format("\t\t_eventType[{0}] = {1};{2}", i, toString((eventType[i] ? true : false)), NL());
        sb<<format("\t\t_eventPersistentType[{0}] = {1};{2}", i, (eventPersistentType[i] ? "true" : "false"), NL());
    }
    sb<<format("\t}{0}{0}", NL());

    sb<<format("{0}{0}\tpublic void computeEventPriorites() { {0}", NL());
    for (int i = 0; i < numEvents; i++)
    {
        libsbml::Event* current = mNOM.GetModel()->getEvent(i);

        if (current->isSetPriority() && current->getPriority()->isSetMath())
        {
            string priority = SBML_formulaToString(current->getPriority()->getMath());
            sb<<format("\t\t_eventPriorities[{0}] = {1};{2}", i, substituteTerms(numReactions, "", priority), NL());
        }
        else
        {
            sb<<format("\t\t_eventPriorities[{0}] = 0f;{1}", i, NL());
        }
    }
    sb<<format("\t}{0}{0}", NL());
}

void CSharpGenerator::writeSetParameterValues(CodeBuilder& sb, const int& numReactions)
{
    sb<<append("\tpublic void setParameterValues ()" + NL());
    sb<<append("\t{" + NL());

    for (int i = 0; i < mGlobalParameterList.size(); i++)
    {
        sb<<format("\t\t{0} = (double){1};{2}",
                      convertSymbolToGP(mGlobalParameterList[i].name),
                      writeDouble(mGlobalParameterList[i].value),
                      NL());
    }

    // Initialize local parameter values
    for (int i = 0; i < numReactions; i++)
    {
        for (int j = 0; j < mLocalParameterList[i].size(); j++)
            sb<<format("\t\t_lp[{0}][{1}] = (double){2};{3}",
                          i,
                          j,
                          writeDouble(mLocalParameterList[i][j].value),
                          NL());
    }

    sb<<append("\t}" + NL() + NL());
}

void CSharpGenerator::writeSetCompartmentVolumes(CodeBuilder& sb)
{
    sb<<append("\tpublic void setCompartmentVolumes ()" + NL());
    sb<<append("\t{" + NL());
    for (int i = 0; i < mCompartmentList.size(); i++)
    {
        sb<<append("\t\t" + convertSymbolToC(mCompartmentList[i].name) + " = (double)" +
                  writeDouble(mCompartmentList[i].value) + ";" + NL());

        // at this point we also have to take care of all initial assignments for compartments as well as
        // the assignment rules on compartments ... otherwise we are in trouble :)
        stack<string> initializations = mNOM.GetMatchForSymbol(mCompartmentList[i].name);
        while (initializations.size() > 0)
        {
            string term(initializations.top());
            string sub = substituteTerms(mNumReactions, "", term);
            sb<<append("\t\t" + sub + ";" + NL());
            initializations.pop();
        }
    }

    sb<<append("\t}" + NL() + NL());
}

void CSharpGenerator::writeSetBoundaryConditions(CodeBuilder& sb)
{
    sb<<append("\tpublic void setBoundaryConditions ()" + NL());
    sb<<append("\t{" + NL());
    for (int i = 0; i < mBoundarySpeciesList.size(); i++)
    {
        if (isNullOrEmpty(mBoundarySpeciesList[i].formula))
        {
            sb<<append("\t\t" + convertSpeciesToBc(mBoundarySpeciesList[i].name) + " = (double)" +
                      writeDouble(mBoundarySpeciesList[i].value) + ";" + NL());
        }
        else
        {
            sb<<append("\t\t" + convertSpeciesToBc(mBoundarySpeciesList[i].name) + " = (double)" +
                      mBoundarySpeciesList[i].formula + ";" + NL());
        }
    }
    sb<<append("\t}" + NL() + NL());
}


void CSharpGenerator::writeSetInitialConditions(CodeBuilder& sb, const int& numFloatingSpecies)
{
    sb<<append("\tpublic void initializeInitialConditions ()" + NL());
    sb<<append("\t{" + NL());
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        if (isNullOrEmpty(mFloatingSpeciesConcentrationList[i].formula))
        {
            sb<<append("\t\t_init" + convertSpeciesToY(mFloatingSpeciesConcentrationList[i].name) + " = (double)" +
                      writeDouble(mFloatingSpeciesConcentrationList[i].value) + ";" + NL());
        }
        else
        {
            sb<<append("\t\t_init" + convertSpeciesToY(mFloatingSpeciesConcentrationList[i].name) + " = (double)" +
                      mFloatingSpeciesConcentrationList[i].formula + ";" + NL());
        }
    }
    sb<<append(NL());

    sb<<append("\t}" + NL() + NL());

    // ------------------------------------------------------------------------------
    sb<<append("\tpublic void setInitialConditions ()" + NL());
    sb<<append("\t{" + NL());

    for (int i = 0; i < numFloatingSpecies; i++)
    {
        sb<<"\t\t_y[" << i << "] =  _init_y[" << i << "];" << NL();
        sb<<"\t\t_amounts[" << i << "] = _y[" << i << "]*" <<
                  convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
    }

    sb<<append(NL());
    sb<<append("\t}" + NL() + NL());
}

string CSharpGenerator::convertSpeciesToY(const string& speciesName)
{
    int index;
    if (mFloatingSpeciesConcentrationList.find(speciesName, index))
    {
        return "_y[" + toString(index) + "]";
    }
    throw new CoreException("Internal Error: Unable to locate species: " + speciesName);
}

string CSharpGenerator::convertSpeciesToBc(const string& speciesName)
{
    int index;
    if (mBoundarySpeciesList.find(speciesName, index))
    {
        return "_bc[" + toString(index) + "]";
    }
    throw CoreException("Internal Error: Unable to locate species: " + speciesName);
}

string CSharpGenerator::convertCompartmentToC(const string& compartmentName)
{
    int index;
    if (mCompartmentList.find(compartmentName, index))
    {
        return "_c[" + toString(index) + "]";
    }

    throw CoreException("Internal Error: Unable to locate compartment: " + compartmentName);
}

string CSharpGenerator::convertSymbolToGP(const string& parameterName)
{
    int index;
    if (mGlobalParameterList.find(parameterName, index))
    {
        return "_gp[" + toString(index) + "]";
    }
      throw CoreException("Internal Error: Unable to locate parameter: " + parameterName);
}

string CSharpGenerator::convertSymbolToC(const string& compartmentName)
{
    int index;
    if (mCompartmentList.find(compartmentName, index))
    {
        return "_c[" + toString(index) + "]";
    }
      throw CoreException("Internal Error: Unable to locate compartment: " + compartmentName);
}



}//rr namespace
