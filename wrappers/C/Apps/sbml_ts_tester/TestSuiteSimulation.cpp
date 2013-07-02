#pragma hdrstop
#include "rrException.h"
#include "rrUtils.h"
#include "rrRoadRunnerData.h"
#include "TestSuiteSimulation.h"

extern string gTSModelsPath;
extern string gTempFolder;
extern bool gDebug;
using namespace rr;
RoadRunnerData convertCAPIResultData(RRDataPtr        resultsHandle);

TestSuiteSimulation::TestSuiteSimulation(const string& dataOutputFolder, const string& modelFilePath, const string& modelFileName)
:
        rr::TestSuiteModelSimulation(dataOutputFolder, modelFilePath, modelFileName),
        mRRHandle(0),
        mResultHandle(0)
{
}

TestSuiteSimulation::~TestSuiteSimulation()
{
    freeRRData(mResultHandle);
}

void TestSuiteSimulation::UseHandle(RRHandle handle)
{
    mRRHandle = handle;
    if(mRRHandle)
    {
        this->UseEngine((RoadRunner*) mRRHandle);
    }
}


bool TestSuiteSimulation::LoadSBMLFromFile()
{
    if(!mRRHandle)
    {
        return false;
    }

    return loadSBMLFromFileE(mRRHandle, GetModelsFullFilePath().c_str(), true);
}

bool TestSuiteSimulation::LoadSettings(const string& settingsFName)
{
    mModelSettingsFileName = (settingsFName);

    if(!mModelSettingsFileName.size())
    {
        mModelSettingsFileName = joinPath(mModelFilePath, GetSettingsFileNameForCase(mCurrentCaseNumber));
    }
    SBMLModelSimulation::LoadSettings(mModelSettingsFileName);

    return loadSimulationSettings(mRRHandle, mModelSettingsFileName.c_str());
}

bool TestSuiteSimulation::Simulate()
{
    if(!mRRHandle)
    {
        return false;
    }
    mResultHandle = simulate(mRRHandle);

    if(mResultHandle)
    {
        mResultData = convertCAPIResultData(mResultHandle);
    }
    return mResultHandle ? true : false;
}

RoadRunnerData TestSuiteSimulation::GetResult()
{
    return mResultData; //Not that pretty.
}

bool TestSuiteSimulation::SaveResult()
{
    string resultFileName(joinPath(mDataOutputFolder, "rrCAPI_" + mModelFileName));
    resultFileName = changeFileExtensionTo(resultFileName, ".csv");

    if(!mResultHandle)
    {
        return false;
    }

    ofstream fs(resultFileName.c_str());
    fs << mResultData;
    fs.close();
    return true;
}

RoadRunnerData convertCAPIResultData(RRDataPtr    result)
{
    RoadRunnerData resultData;

    StringList colNames;
    //Copy column names
    for(int i = 0; i < result->CSize; i++)
    {
        colNames.add(result->ColumnHeaders[i]);
    }

    resultData.setColumnNames(colNames);

    //Then the data
    int index = 0;
    resultData.allocate(result->RSize, result->CSize);

    for(int j = 0; j < result->RSize; j++)
    {
        for(int i = 0; i < result->CSize; i++)
        {
            resultData(j,i) = result->Data[index++];
        }
    }

    return resultData;
}


bool RunTest(const string& version, int caseNumber)
{
    bool result = false;
    RRHandle gRR = 0;

    //Create instance..
    gRR = createRRInstanceEx(gTempFolder.c_str());

    if(gDebug && gRR)
    {
        enableLoggingToConsole();
        setLogLevel("Debug5");
    }
    else
    {
        setLogLevel("Error");
    }

    //Setup environment
    setTempFolder(gRR, gTempFolder.c_str());

    if(!gRR)
    {
        return false;
    }

    try
    {
        clog<<"Running Test: "<<caseNumber<<endl;
        string dataOutputFolder(gTempFolder);
        string dummy;
        string logFileName;
        string settingsFileName;

        //Create a log file name
        createTestSuiteFileNameParts(caseNumber, ".log", dummy, logFileName, settingsFileName);

        //Create subfolder for data output
        dataOutputFolder = joinPath(dataOutputFolder, getTestSuiteSubFolderName(caseNumber));

        if(!createFolder(dataOutputFolder))
        {
            string msg("Failed creating output folder for data output: " + dataOutputFolder);
            throw(rr::Exception(msg));
        }

        TestSuiteSimulation simulation(dataOutputFolder);

        simulation.UseHandle(gRR);

        //Read SBML models.....
        string modelFilePath(gTSModelsPath);
        string modelFileName;

        simulation.SetCaseNumber(caseNumber);
        createTestSuiteFileNameParts(caseNumber, "-sbml-" + version + ".xml", modelFilePath, modelFileName, settingsFileName);

        //The following will load and compile and simulate the sbml model in the file
        simulation.SetModelFilePath(modelFilePath);
        simulation.SetModelFileName(modelFileName);
        simulation.ReCompileIfDllExists(true);
        simulation.CopyFilesToOutputFolder();
        setTempFolder(gRR, simulation.GetDataOutputFolder().c_str());
        setComputeAndAssignConservationLaws(gRR, false);

        if(!simulation.LoadSBMLFromFile())
        {
            throw("Failed loading sbml from file");
        }

        //Then read settings file if it exists..
        string settingsOveride("");
        if(!simulation.LoadSettings(settingsOveride))
        {
            throw("Failed loading simulation settings");
        }

        //Then Simulate model
        if(!simulation.Simulate())
        {
            throw("Failed running simulation");
        }

        //Write result
        if(!simulation.SaveResult())
        {
            //Failed to save data
            throw("Failed saving result");
        }

        if(!simulation.LoadReferenceData())
        {
            throw("Failed Loading reference data");
        }

        simulation.CreateErrorData();
        result = simulation.Pass();
        simulation.SaveAllData();
        simulation.SaveModelAsXML(dataOutputFolder);
        if(!result)
        {
            clog<<"\t\tTest failed..\n";
        }
        else
        {
            clog<<"\t\tTest passed..\n";
        }
    }
    catch(rr::Exception& ex)
    {
        string error = ex.what();
        cerr<<"Case "<<caseNumber<<": Exception: "<<error<<endl;
        result = false;;
    }

    // done with rr
    freeRRInstance(gRR);
    return result;
}
