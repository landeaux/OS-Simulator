// Program Information /////////////////////////////////////////////////////////

/**
 * @file main.cpp
 *
 * @brief Driver program for OS Simulator
 *
 * @details This program is the driver program for OS Simulator classes
 * 
 * @version 3.04
 *          Adam Landis (7 April 2019)
 *          - Add semaphores for all I/O resources
 * 
 * @version 3.03
 *          Adam Landis (7 April 2019)
 *          - Add support for semaphores
 *          - Change implementation of startSimulation() to initialize 
 *            semaphores for resource management
 *          - Add threaded function executeIOInstruction() to execute IO 
 *            instructions and utilize semaphores for resource managemnent and 
 *            mutual exclusion
 * 
 * @version 3.02
 *          Adam Landis (6 April 2019)
 *          - Add executeMemInstruction() for memory allocation (and soon,  
 *            blocking) functionality
 *          - Add uintToHex() function to convert an unsigned to a 
 *            hexidecimal string
 *          - Add mutex locks for memory instructions
 * 
 * @version 3.01
 *          Adam Landis (6 April 2019)
 *          - Change implementation of parseMetadataFile() to calc and set 
 *            waitTime for each metadata instruction before pushing it onto 
 *            queue.
 *          - Change implementation of startSimulation() to get wait_time from 
 *            each metadata instruction instead of calculating it every time in
 *            the loop.
 * 
 * @version 3.00
 *          Adam Landis (5 April 2019)
 *          Add support for configuration settings.
 * 
 * @version 2.05
 *          Adam Landis (15 March 2019)
 *          Change implementation of startSimulation() to change PCB state at
 *          appropriate moments
 * 
 * @version 2.04
 *          Adam Landis (14 March 2019)
 *          - Add non-threaded version of wait() function for non I/O operations
 *          - Add genRandNum() function to generate random numbers for memory 
 *            allocation purposes
 *          - Change implementation of startSimulation to properly log all 
 *            timestamped operations
 * 
 * @version 2.03
 *          Adam Landis (13 March 2019)
 *          - Add Timer class
 *          - Include pthread.h
 *          - Add threaded wait() function
 *          - Change implementation of startSimulation() to test these new 
 *            additions
 * 
 * @version 2.02
 *          Adam Landis (9 March 2019)
 *          Add MetadataInstruction Class and remove metadataInstruction struct.
 *          Modify implementation of functions which used struct to use class 
 *          instead.
 *
 * @version 2.01
 *          Adam Landis (6 March 2019)
 *          Add PCB class            
 *
 * @version 1.05
 *          Adam Landis (18 February 2019)
 *          Add logging functionality for config data and metadata and
 *          implemented helper functions related to logging
 *
 * @version 1.04
 *          Adam Landis (17 February 2019)
 *          Remove ability to take in an arbitrary number of config files - not
 *          required
 *
 * @version 1.03
 *          Adam Landis (17 February 2019)
 *          Add functionality to parse metadata files and add helper functions
 *
 * @version 1.02
 *          Adam Landis (16 February 2019)
 *          Add ability to take in an arbitrary number of config files
 *
 * @version 1.01
 *          Adam Landis (15 February 2019)
 *          Implement parsing of config file and error checking
 *
 * @version 1.00
 *          Adam Landis (14 February 2019)
 *          Initial development
 *
 * @note None
 */ 

// Program Description/Support /////////////////////////////////////////////////
/*
 * This program runs from the terminal and takes one argument for the filename
 * of the configuration file to be used to initialize the OS simulator. This
 * config file contains the filename of a meta data file for use in loading the
 * operations to perform in the OS simulator.
 *
 */
// Header Files ////////////////////////////////////////////////////////////////
//
#include <iostream>     // cout
#include <iomanip>      // setprecision, fixed
#include <fstream>      // ofstream
#include <string>       // string, to_string
#include <sstream>      // stringstream
#include <map>          // for the configutation settings
#include <queue>        // for the metadata
#include <pthread.h>    // for threads
#include <semaphore.h>  // for semaphores
#include <random>       // for generating random numbers
#include <limits>       // for generating random numbers

#include "PCB.h"
#include "MetadataInstruction.h"
#include "Timer.h"
//
// Global Constant Definitions /////////////////////////////////////////////////
//
const std::string CONFIG_HEADER = "Start Simulator Configuration File";
const std::string CONFIG_FOOTER = "End Simulator Configuration File";
const std::string CONFIG_SETTING_NAMES[] = {
        "Version/Phase",
        "File Path",
        "Monitor display time {msec}",
        "Processor cycle time {msec}",
        "Scanner cycle time {msec}",
        "Hard drive cycle time {msec}",
        "Keyboard cycle time {msec}",
        "Memory cycle time {msec}",
        "Projector cycle time {msec}",
        "System memory {kbytes}",
        "Memory block size {kbytes}",
        "Projector quantity",
        "Hard drive quantity",
        "Log",
        "Log File Path"
};
const std::string METADATA_HEADER = "Start Program Meta-Data Code:";
const std::string METADATA_FOOTER = "End Program Meta-Data Code.";
const std::string METADATA_CODES = "SAPIOM";
const std::string METADATA_DESCRIPTORS[] = {
        "begin",
        "finish",
        "hard drive",
        "keyboard",
        "scanner",
        "monitor",
        "run",
        "allocate",
        "projector",
        "block",
};
//
// Global Variable Definitions /////////////////////////////////////////////////
//
pthread_mutex_t mutex;                          // mutex for memory mgmt
sem_t semHD, semProj, semKB, semMon, semScan;   // semaphores for resource mgmt
//
// Class/Struct Definitions ////////////////////////////////////////////////////
//
/**
 * Struct to hold a single key-value pair for a config setting
 */
struct configSetting
{
    std::string key;
    std::string value;
};
//
// Typedefs ////////////////////////////////////////////////////////////////////
//
typedef std::map<std::string, std::string> configMap;
typedef std::queue<MetadataInstruction> metadataQueue;
//
// Free Function Prototypes ////////////////////////////////////////////////////
//
bool isValidFileExtension(const std::string& filename, const std::string& ext);
bool isEmptyFile(std::ifstream& file);
bool isPositiveInteger(const std::string& str);
bool isNonNegativeInteger(const std::string& str);
std::string stripSpaces(const std::string& s);
unsigned long strToUnsignedLong(const std::string& str);
void logToMonitor(std::string& logData);
void logToFile(std::string& logData, std::string& logFilename,
               std::ios_base::openmode mode = std::ios_base::app);
void logData(configMap config, std::string data);

configMap initializeConfig(const std::string& filename);
void validateConfigFile(std::ifstream& configFile, const std::string& filename);
configMap parseConfigFile(std::ifstream& configFile);
configSetting parseConfigLine(const std::string& configLine);
void logConfigFileData(configMap config);
std::string generateConfigLogData(configMap config);
std::string getShortConfigSettingName(const std::string& longStr);
configSetting getConfigSetting(const std::string& descriptor, configMap config);

metadataQueue initializeMetadata(const std::string& filename, const configMap& config);
void validateMetadataFile(std::ifstream& metadataFile, const std::string& filename);
metadataQueue parseMetadataFile(std::ifstream& metadataFile, const configMap& config);
MetadataInstruction parseMetadataInstruction(std::string& instructionStr);
void validateMetadataCode(const char& code);
void validateMetadataDescriptor(const std::string& descriptor);
void validateMetadataCycles(const std::string& numCycles);
void logMetadataFileData(const MetadataInstruction& instr, configMap config);
std::string generateMetadataLogData(MetadataInstruction instr, const configMap& config);

void startSimulation(configMap config, metadataQueue mdQueue);
void* wait(void* param);
void wait(float duration);
unsigned genRandNum();
void executeMemInstruction(
    MetadataInstruction instr, 
    unsigned &nextBlockPtr, 
    unsigned blockSize, 
    unsigned &memAddr,
    unsigned sysMem);
void* executeIOInstruction(void* param);
std::string uintToHexStr(unsigned num);
//
// Main Function Implementation ////////////////////////////////////////////////
//
int main(int argc, char *argv[])
{
    srand(time(NULL));

    try
    {
        if (argc == 1)
        {
            throw std::string("Error: missing argument for configuration file");
        }

        configMap config;
        std::string configFilename = argv[1];

        metadataQueue metaQueue;
        std::string metadataFilename;

        std::ofstream logFile;
        std::string logFilename;

        config = initializeConfig(configFilename);

        metadataFilename = config["File Path"];
        if (metadataFilename.empty())
        {
            throw std::string("Error: 'File Path' missing from config file");
        }
        metaQueue = initializeMetadata(metadataFilename, config);

        startSimulation(config, metaQueue);
    }
    catch (std::string& e)
    {
        std::cout << e << std::endl;
        exit(1);
    }

    return 0;
}
//
// Free Function Implementation ////////////////////////////////////////////////
//
/**
 * Checks if a filename contains a specified extension
 *
 * @param   filename
 *          The filename to validate
 *
 * @param   extension
 *          The extension to check for
 *
 * @return  success of validation (bool)
 */
bool isValidFileExtension(const std::string& filename, const std::string& ext)
{
    unsigned long flnLen = filename.length();
    unsigned long extLen = ext.length();
    size_t extLoc = filename.rfind('.' + ext);

    // if extension found in string and extension is at the end, return true
    return ((extLoc != std::string::npos) && (extLen + extLoc + 1 == flnLen));
}

/**
 * Check if a file is empty
 *
 * @param   file
 *          The file to check
 *
 * @return  true if file is empty, false otherwise
 */
bool isEmptyFile(std::ifstream& file)
{
    return file.peek() == std::ifstream::traits_type::eof();
}

/**
 * Checks if a given string is a positive integer or not.
 *
 * Credit: Code adapted from https://stackoverflow.com/a/2845275
 *
 * @param   str
 *          The string to check
 *
 * @return  true if string is a positive integer, false otherwise
 */
bool isPositiveInteger(const std::string& str)
{
    if(str.empty() || (!isdigit(str[0])))
    {
        return false;
    }

    char * p;
    long int num = strtol(str.c_str(), &p, 10);

    return (*p == 0 && num != 0);
}

/**
 * Checks if a given string is a non-negative integer or not.
 *
 * Credit: Code adapted from https://stackoverflow.com/a/2845275
 *
 * @param   str
 *          The string to check
 *
 * @return  true if string is a positive integer, false otherwise
 */
bool isNonNegativeInteger(const std::string& str)
{
    if(str.empty() || (!isdigit(str[0])))
    {
        return false;
    }

    char * p;
    strtol(str.c_str(), &p, 10);

    return (*p == 0);
}

/**
 * Takes a given string and strips ' ' characters from it
 *
 * @param   str
 *          The string to strip ' ' characters from
 *
 * @return  the input string without ' ' characters in it
 */
std::string stripSpaces(const std::string& str)
{
    std::string result;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ')
        {
            result += str[i];
        }
    }

    return result;
}

/**
 * Wrapper function for strtol which takes a std::string and converts it to an
 * unsigned long.
 *
 * @param   str
 *          The string to convert
 *
 * @return
 */
unsigned long strToUnsignedLong(const std::string& str)
{
    return (unsigned long) strtol(str.c_str(), nullptr, 10);
}

/**
 * Logs data to monitor (terminal)
 *
 * @param   logData
 *          The data to log to monitor (string)
 */
void logToMonitor(std::string& logData)
{
    std::cout << logData;
}

/**
 * Logs data to an output file
 *
 * @param   logData
 *          The data to log to file (string)
 *
 * @param   logFile
 *          The filename of the file to log the output to (string)
 *
 * @param   mode
 *          The openmode for the file. Default: ios_base::out
 *
 * @return  None
 */
void logToFile(std::string& logData, std::string& logFilename,
               std::ios_base::openmode mode)
{
    std::ofstream logFile;

    if (logFilename.empty())
    {
        throw std::string("Error: cannot log to file - filename missing");
    }

    logFile.open(logFilename, mode);

    if (!logFile)
    {
        throw std::string("Error: cannot log to file - unable to open log file");
    }

    logFile << logData;

    logFile.close();
}

/**
 * @brief      Logs a single string of data either to a file, monitor, or both
 *
 * @param[in]  config  The configuration
 * @param[in]  data    The data
 */
void logData(configMap config, std::string data)
{
    if (config["Log"] == "Log to Monitor")
    {
        logToMonitor(data);
    }
    else if (config["Log"] == "Log to File")
    {
        logToFile(data, config["Log File Path"]);
    }
    else if (config["Log"] == "Log to Both")
    {
        logToMonitor(data);
        logToFile(data, config["Log File Path"]);
    }
    else
    {
        throw std::string("Error: cannot log data - invalid or missing log type");
    }
}

/**
 * @brief   Function to initialize OS Configuration
 *
 * @detail  Opens specified config file, validates the file, and parses the file
 *
 * @param   filename
 *          The config file to initialize the configuration from
 *
 * @return  Associative array (map) containing the key-value pairs for
 *          configuration settings
 */
configMap initializeConfig(const std::string& filename)
{
    configMap config;
    std::ifstream configFile(filename.c_str(), std::ios::in);

    validateConfigFile(configFile, filename);
    config = parseConfigFile(configFile);

    configFile.close();

    return config;
}

/**
 * Checks the validity of a configuration file (correct extension, non-empty)
 *
 * @param   configFile
 *          The config file to validate (ifstream)
 *
 * @param   filename
 *          The filename of the config file (string)
 *
 * @return  None
 */
void validateConfigFile(std::ifstream& configFile, const std::string& filename)
{
    // make sure file exists
    if (!configFile.good())
    {
        throw std::string("Error: config file \"" + filename + "\" does not exist");
    }

    // make sure file has correct extension
    if (!isValidFileExtension(filename, "conf")) {
        throw std::string("Error: invalid extension for config file");
    }

    // make sure file is not empty
    if (isEmptyFile(configFile)) {
        configFile.close();
        throw std::string("Error: config file empty");
    }

    std::string header;
    getline(configFile, header);

    // make sure the 1st line in file matches correct config file header
    if (header != CONFIG_HEADER)
    {
        throw std::string("Error: invalid config file header");
    }
}

/**
 * Parses the config file and reads in the config values to an associative array
 *
 * @param   configFile
 *          The config file to parse
 *
 * @return  The associative array containing the config key-value pairs (map)
 */
configMap parseConfigFile(std::ifstream& configFile)
{
    configMap config;
    std::string tempLine;
    configSetting tempSetting;

    while (!configFile.eof() && tempLine != CONFIG_FOOTER)
    {
        getline(configFile, tempLine);
        if (tempLine != CONFIG_FOOTER)
        {
            tempSetting = parseConfigLine(tempLine);
            config[tempSetting.key] = tempSetting.value;
        }
    }

    return config;
}

/**
 * Parses a given line in the config file and returns a key-value pair
 *
 * @param   configLine
 *          The current line in the config file to parse
 *
 * @return  key-value pair representing the config setting
 */
configSetting parseConfigLine(const std::string& configLine)
{
    configSetting setting;
    std::string settingKey;
    std::string settingValue;
    unsigned long lineLen = configLine.length();
    unsigned long i = 0;

    while (configLine[++i] != ':' && i < lineLen);

    if (i >= lineLen)
    {
        throw std::string("Error: unable to parse config file");
    }

    settingKey = configLine.substr(0, i);
    bool settingFound = false;

    for (const std::string& name: CONFIG_SETTING_NAMES)
    {
        if (settingKey == name)
        {
            settingFound = true;
        }
    }

    if (!settingFound)
    {
        throw std::string("Error: invalid setting \"" + settingKey + "\"");
    }

    setting.key = getShortConfigSettingName(settingKey);

    if (configLine[++i] != ' ')
    {
        throw std::string("Error: unable to parse config file");
    }

    while (configLine[++i] == ' ' && i < lineLen);

    if (i >= lineLen)
    {
        throw std::string("Error: unable to parse config file");
    }

    settingValue = configLine.substr(i, lineLen - i);

    if (setting.key != "Version/Phase" &&
        setting.key != "File Path" &&
        setting.key != "Log" &&
        setting.key != "Log File Path")
    {
        if (!isPositiveInteger(settingValue))
        {
            throw std::string("Error: invalid cycle/display time \"" + settingValue + "\"");
        }
    }

    setting.value = settingValue;

    return setting;
}

/**
 * Logs configuration file data - either to file, monitor, or both
 *
 * @param   config
 *          The config map holding the configuration data to log
 *
 * @return  None
 */
void logConfigFileData(configMap config)
{
    std::string data = generateConfigLogData(config);

    logData(config, data);
}

/**
 * Generates a single string containing config log data for logging to monitor,
 * an output file, or both.
 *
 * @param   config
 *          The configMap object holding the configuration data to log
 *
 * @return  the generated string representing the log data
 */
std::string generateConfigLogData(configMap config)
{
    std::string logData;

    logData += "Configuration File Data\n";
    logData += "Monitor = "       + config["Monitor"]       + " ms/cycle\n";
    logData += "Processor = "     + config["Processor"]     + " ms/cycle\n";
    logData += "Scanner = "       + config["Scanner"]       + " ms/cycle\n";
    logData += "Hard Drive = "    + config["Hard drive"]    + " ms/cycle\n";
    logData += "Keyboard = "      + config["Keyboard"]      + " ms/cycle\n";
    logData += "Memory = "        + config["Memory"]        + " ms/cycle\n";
    logData += "Projector = "     + config["Projector"]     + " ms/cycle\n";
    logData += "System memory = " + config["System memory"] + " kbytes\n";

    logData += "Logged to: ";
    if (config["Log"] == "Log to Both") logData += "monitor and ";
    logData += config["Log File Path"] + "\n\n";

    return logData;
}

/**
 * Converts a long config setting name to a short one
 *
 * @param   longStr
 *          The longer string to shorten
 *
 * @return  The shortened string
 */
std::string getShortConfigSettingName(const std::string& longStr)
{
    std::string result = longStr;

    if (longStr != "Version/Phase" &&
        longStr != "File Path" &&
        longStr != "Log" &&
        longStr != "Log File Path" &&
        longStr.find("quantity") == std::string::npos)
    {
        if (longStr.find("Memory block size") != std::string::npos)
        {
            result = "Memory block size";
        }
        else
        {
            size_t end = longStr.find(' ');
            result = longStr.substr(0, end);
            
            if (result == "Hard")
            {
                result += " drive";
            }
            else if (result == "System")
            {
                result += " memory";
            }
        }
    }
    
    return result;
}

/**
 * Gets the config setting for a given metadata instruction descriptor
 *
 * @param   descriptor
 *          The given metadata instruction descriptor to find the config setting
 *          for
 *
 * @param   config
 *          configMap object which holds all of the configuration settings
 *
 * @return  The config setting which corresponds to the given metadata
 *          instruction descriptor (configSetting)
 */
configSetting getConfigSetting(const std::string& descriptor, configMap config)
{
    configSetting setting;

    if (descriptor == "block" || descriptor == "allocate")
    {
        setting.key = "Memory";
        setting.value = config["Memory"];
    }
    else if (descriptor == "run")
    {
        setting.key = "Processor";
        setting.value = config["Processor"];
    }
    else
    {
        std::string key = descriptor;
        key[0] = (char) toupper(key[0]);
        setting.key = key;
        setting.value = config[key];
    }

    return setting;
}

/**
 * Initializes metadata (i.e. opens, validates, parses, and closes a metadata
 * file)
 *
 * @param   filename
 *          The filename of the metadata file to initialize
 *
 * @param   config
 *          The config map object containing all configuration settings
 *
 * @return  a queue containing metadata instructions
 */
metadataQueue initializeMetadata(const std::string& filename, const configMap& config)
{
    metadataQueue metaQueue;
    std::ifstream metadataFile(filename.c_str(), std::ios::in);

    validateMetadataFile(metadataFile, filename);
    metaQueue = parseMetadataFile(metadataFile, config);

    metadataFile.close();

    return metaQueue;
}

/**
 * Validates a metadata file (i.e. checks for correct extension, not empty, etc)
 *
 * @param   metadataFile
 *          The metadata file to parse (ifstream)
 *
 * @param   filename
 *          The filename of the metadata file to validate
 *
 * @return  None
 */
void validateMetadataFile(std::ifstream& metadataFile, const std::string& filename)
{
    if (!metadataFile.good())
    {
        throw std::string("Error: metadata file \"" + filename + "\" does not exist");
    }

    if (!isValidFileExtension(filename, "mdf")) {
        throw std::string("Error: invalid extension for metadata file");
    }

    if (isEmptyFile(metadataFile)) {
        metadataFile.close();
        throw std::string("Error: metadata file empty");
    }

    std::string header;
    getline(metadataFile, header);

    if (header != METADATA_HEADER)
    {
        throw std::string("Error: invalid config file header");
    }
}

/**
 * Parses the contents of a metadata file
 *
 * @param   metadataFile
 *          The metadata file to parse
 *
 * @param   config
 *          The config map object containing all configuration settings
 *
 * @return  a queue containing metadata instructions
 */
metadataQueue parseMetadataFile(std::ifstream& metadataFile, const configMap& config)
{
    metadataQueue metaQueue;
    std::string tempLine;
    std::string tempInstr;
    bool isLastLine = false;

    while (!metadataFile.eof() && tempLine != METADATA_FOOTER && ! isLastLine)
    {
        getline(metadataFile, tempLine);

        if (tempLine != METADATA_FOOTER)
        {
            tempLine = stripSpaces(tempLine);
            size_t start = 0;
            size_t end = 0;

            do
            {
                end = tempLine.find(';', start);

                // if ';' not found but there are more chars left
                if ((end == std::string::npos) && (start < tempLine.length()))
                {
                    // look for '.'
                    end = tempLine.find('.', start);

                    // if we didn't find '.' either, throw error
                    if (end == std::string::npos)
                    {
                        throw std::string("Error: cannot parse metadata - missing ';' or '.'");
                    }

                    isLastLine = true;
                }

                tempInstr = tempLine.substr(start, end - start);

                MetadataInstruction instr = parseMetadataInstruction(tempInstr);

                configSetting setting = getConfigSetting(instr.getDescriptor(), config);
                unsigned long cycleTime = strToUnsignedLong(setting.value);
                instr.setWaitTime(cycleTime);

                std::string descriptor = instr.getDescriptor();

                if (descriptor == "hard drive")
                {
                    instr.setSemPtr(&semHD);
                }
                else if (descriptor == "projector")
                {
                    instr.setSemPtr(&semProj);
                }
                else if (descriptor == "keyboard")
                {
                    instr.setSemPtr(&semKB);
                }
                else if (descriptor == "monitor")
                {
                    instr.setSemPtr(&semMon);
                }
                else if (descriptor == "scanner")
                {
                    instr.setSemPtr(&semScan);
                }

                metaQueue.push(instr);
                start = end + 1;
            } while (end != std::string::npos && start < tempLine.length());
        }
    }

    return metaQueue;
}

/**
 * Parses a single metadata instruction from a given string read from a metadata
 * file
 *
 * @param   instructionStr
 *          The string containing the instruction to be parsed
 *
 * @return  the parsed metadata instruction
 */
MetadataInstruction parseMetadataInstruction(std::string& instructionStr)
{
    unsigned long i = 0;
    unsigned long instrLen = instructionStr.length();

    char code;
    std::string descriptor;
    std::string numCycles;

    code = instructionStr[i];
    validateMetadataCode(code);

    while (instructionStr[++i] == ' ' && i < instrLen);

    if (instructionStr[i] != '{')
    {
        throw std::string("Error: cannot parse metadata - missing start '{'");
    }

    ++i;

    unsigned long j = instructionStr.find('}');

    if (j == std::string::npos)
    {
        throw std::string("Error: cannot parse metadata - missing end '}'");
    }

    descriptor = instructionStr.substr(i, j - i);
    if (descriptor == "harddrive") descriptor = "hard drive";
    validateMetadataDescriptor(descriptor);

    i = j + 1;

    numCycles = instructionStr.substr(i, instrLen - i);
    validateMetadataCycles(numCycles);

    return MetadataInstruction(code, descriptor, strToUnsignedLong(numCycles));
}

/**
 * Checks that a given character is a valid metadata code
 *
 * @param   metadataCode
 *          The char representing the metadata code to validate
 *
 * @return  None
 */
void validateMetadataCode(const char& code)
{
    if (code == '{')
    {
        throw std::string("Error: cannot parse metadata - missing metadata code");
    }

    if (METADATA_CODES.find(code) == std::string::npos)
    {
        throw std::string("Error: cannot parse metadata - invalid metadata code");
    }
}

/**
 * Checks that a given string is a valid metadata descriptor
 *
 * @param   descriptor
 *          The string representing the metadata descriptor to validate
 *
 * @return  None
 */
void validateMetadataDescriptor(const std::string& descriptor)
{
    bool descriptorFound = false;

    for (const std::string& desc: METADATA_DESCRIPTORS)
    {
        if (descriptor == desc)
        {
            descriptorFound = true;
        }
    }

    if (!descriptorFound)
    {
        throw std::string("Error: cannot parse metadata - invalid descriptor");
    }
}

/**
 * Checks that a given string represents a valid integer value for the number of
 * cycles
 *
 * @param   numCycles
 *          The string representing number of cycles for metadata instruction
 *
 * @return  None
 */
void validateMetadataCycles(const std::string& numCycles)
{
    if (numCycles.empty())
    {
        throw std::string("Error: cannot parse metadata - "
                          "missing number of cycles");
    }

    if (!isNonNegativeInteger(numCycles))
    {
        throw std::string("Error: cannot parse metadata - "
                          "invalid cycle number \"" + numCycles + "\"");
    }
}

/**
 * Logs metadata file data to a file, monitor, or both
 *
 * @param   instr
 *          The metadata instruction to be used in generating the log data
 *
 * @param   config
 *          The configMap object holding the configuration data which is used in
 *          processing the metadata file data to log
 *
 * @return  None
 */
void logMetadataFileData(const MetadataInstruction& instr, configMap config)
{
    static bool beginLog = false;
    std::string logData;

    if (!beginLog)
    {
        logData = "Meta-Data Metrics\n";
        beginLog = true;
    }

    logData += generateMetadataLogData(instr, config);

    if (config["Log"] == "Log to Monitor")
    {
        logToMonitor(logData);
    }
    else if (config["Log"] == "Log to File")
    {
        logToFile(logData, config["Log File Path"], std::ios_base::app);
    }
    else if (config["Log"] == "Log to Both")
    {
        logToMonitor(logData);
        logToFile(logData, config["Log File Path"], std::ios_base::app);
    }
    else
    {
        throw std::string("Error: cannot log data - invalid or missing log type");
    }
}

/**
 * Generates a single string containing metadata log data for logging to monitor,
 * an output file, or both.
 *
 * @param   instr
 *          The metadata instruction to be used in generating the log data
 *
 * @param   config
 *          The configMap object holding the configuration data which is used in
 *          processing the metadata file data to log
 *
 * @return  the generated metadata log string
 */
std::string generateMetadataLogData(MetadataInstruction instr, const configMap& config)
{
    std::string result;

    if (instr.getNumCycles() > 0)
    {
        configSetting setting = getConfigSetting(instr.getDescriptor(), config);

        unsigned long cycleTime = strToUnsignedLong(setting.value);
        std::string totalTime = std::to_string(instr.getNumCycles() * cycleTime);

        result = instr.toString() + " - " + totalTime + " ms\n";
    }

    return result;
}

/**
 * @brief      Starts the OS simulation.
 *
 * @param[in]  config   The configuration map
 * @param[in]  mdQueue  The queue of metadata instructions
 */
void startSimulation(configMap config, metadataQueue mdQueue)
{
    Timer myTimer;
    float duration;
    PCB* pcb;
    unsigned numHD, countHD = 0,
             numProj, countProj = 0,
             sysMem, memBlockSize, nextBlockPtr = 0,
             pid = 0;

    numHD   = (unsigned) strToUnsignedLong(config["Hard drive quantity"]);
    numProj = (unsigned) strToUnsignedLong(config["Projector quantity"]);

    sysMem       = (unsigned) strToUnsignedLong(config["System memory"]);
    memBlockSize = (unsigned) strToUnsignedLong(config["Memory block size"]);

    pthread_mutex_init(&mutex, NULL);

    sem_init(&semHD,   0, numHD);
    sem_init(&semProj, 0, numProj);
    sem_init(&semKB,   0, 1);
    sem_init(&semMon,  0, 1);
    sem_init(&semScan, 0, 1);

    std::cout << std::setprecision(6) << std::fixed;

    myTimer.startTimer();

    while (!mdQueue.empty())
    {
        MetadataInstruction instr = mdQueue.front();
        char code = instr.getCode();
        std::string descriptor = instr.getDescriptor();
        float wait_time = instr.getWaitTime();
        unsigned memAddr;
        std::string data;
        
        if (code == 'A' && descriptor == "begin")
        {
            pcb = new PCB(++pid);
            pcb->setState(START);
        }
        else if (code == 'P')
        {
            pcb->setState(RUNNING);
        }

        duration = myTimer.getDuration() / 1000.0f;
        data = std::to_string(duration) + " - " + instr.genLogString(true, pid);

        if (descriptor == "hard drive")
        {
            data += std::to_string(countHD++ % numHD);
        }
        else if (descriptor == "projector")
        {
            data += std::to_string(countProj++ % numProj);
        }

        data += "\n";

        logData(config, data);

        if (code == 'I' || code == 'O')
        {
            pcb->setState(WAIT);

            pthread_t tid;
            // pthread_create(&tid, NULL, executeIOInstruction, (void*)&instr);
            pthread_create(&tid, NULL, wait, (void*)&wait_time);
            pthread_join(tid, NULL);
        }
        else if (code == 'M')
        {
            executeMemInstruction(instr, nextBlockPtr, memBlockSize, memAddr, sysMem);
        }
        else
        {
            wait(wait_time);
        }

        if (code == 'A' && descriptor == "finish")
        {
            pcb->setState(EXIT);
            delete pcb;
        }
        else if (code != 'S')
        {
            pcb->setState(READY);
        }

        if (   code != 'S' && 
             !(code == 'A' && descriptor == "finish") )
        {
            duration = myTimer.getDuration() / 1000.0f;
            data = std::to_string(duration) + " - " + instr.genLogString(false, pid);

            if (code == 'M' && descriptor == "allocate")
            {
                data += " ";
                data += uintToHexStr(memAddr);
            }

            data += "\n";
            
            logData(config, data);
        }

        mdQueue.pop();
    }

    logData(config, "\n");
}

/**
 * @brief      Thread that waits for a specified amount of time in milliseconds.
 *
 * @param      param  The duration parameter
 *
 * @return     None
 */
void* wait(void* param)
{
    float duration = *((float*)param);
    wait(duration);
    pthread_exit(0);
}

/**
 * @brief      Non-threaded version of wait() - waits for a specified amount of
 *             time in milliseconds.
 *
 * @param      duration  The duration to wait for
 *
 * @return     None
 */
void wait(float duration)
{
    Timer myTimer;

    myTimer.startTimer();
    while (myTimer.getDuration() < duration);
}

/**
 * @brief      Generates a random number between 0 and max value for unsigned
 *
 * @return     The generated random number (unsigned)
 */
unsigned genRandNum()
{
    // Get a random seed from the OS entropy device
    std::random_device rd;

    // Use the 64-bit Mersenne Twister 19937 generator and seed it with entropy
    std::mt19937_64 eng(rd());  
                                
    // Define the distribution
    std::uniform_int_distribution<unsigned> distr;

    return distr(eng);
}

/**
 * @brief      Executes a memory instruction
 *
 * @param[in]  instr         The instruction
 * @param      nextBlockPtr  The next block pointer
 * @param[in]  blockSize     The block size
 * @param      memAddr       The memory address
 * @param[in]  sysMem        The system memory
 */
void executeMemInstruction(
    MetadataInstruction instr, 
    unsigned &nextBlockPtr, 
    unsigned blockSize, 
    unsigned &memAddr,
    unsigned sysMem)
{
    std::string descriptor = instr.getDescriptor();

    pthread_mutex_lock(&mutex);

    if (descriptor == "allocate")
    {
        if (sysMem - nextBlockPtr >= blockSize)
        {
            memAddr = nextBlockPtr;
            nextBlockPtr += blockSize;
        }
        else
        {
            memAddr = 0;
            nextBlockPtr = blockSize;
        }
    }
    else if (descriptor == "block")
    {
        /* For now, do nothing */
    }

    wait(instr.getWaitTime());
    
    pthread_mutex_unlock(&mutex);
}

/**
 * @brief      Converts an unsigned integer to a string in hexidecimal format
 *
 * @param[in]  num   The unsigned integer to convert
 *
 * @return     The resultant hex string after conversion
 */
std::string uintToHexStr(unsigned num)
{
    std::stringstream stream;
    stream.width(8);
    stream.fill('0');
    stream << std::hex << num;
    return "0x" + std::string(stream.str());
}

/**
 * @brief      Executes an IO instruction
 *
 * @param      param  The parameter holding the instruction
 *
 * @return     None
 */
void* executeIOInstruction(void* param)
{
    MetadataInstruction instr = *((MetadataInstruction*)param);
    sem_t *semPtr;
    std::string descriptor = instr.getDescriptor();

    semPtr = instr.getSemPtr();

    sem_wait(semPtr);

    wait(instr.getWaitTime());

    sem_post(semPtr);

    return 0;
}
