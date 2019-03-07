// Program Information /////////////////////////////////////////////////////////
/**
 * @file main.cpp
 *
 * @brief Driver program for OS Simulator
 *
 * @details This program is the driver program for OS Simulator classes
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
// Precompiler Directives //////////////////////////////////////////////////////
//
// None
//
// Header Files ////////////////////////////////////////////////////////////////
//
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <vector>

#include "PCB.h"
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

/**
 * Struct to hold a single metadata instruction
 */
struct metadataInstruction
{
    char code;
    std::string descriptor;
    unsigned long numCycles;
};
//
// Typedefs ////////////////////////////////////////////////////////////////////
//
typedef std::map<std::string, std::string> configMap;
typedef std::queue<metadataInstruction> metadataQueue;
//
// Free Function Prototypes ////////////////////////////////////////////////////
//
bool isValidFileExtension(const std::string& filename, const std::string& ext);
bool isEmptyFile(std::ifstream& file);
bool isPositiveInteger(const std::string& str);
bool isNonNegativeInteger(const std::string& str);
std::string stripSpaces(const std::string& s);
unsigned long strToUnsignedLong(const std::string& str);

configMap initializeConfig(const std::string& filename);
void validateConfigFile(std::ifstream& configFile, const std::string& filename);
configMap parseConfigFile(std::ifstream& configFile);
configSetting parseConfigLine(const std::string& configLine);
void logConfigFileData(configMap config);
std::string generateConfigLogData(configMap config);
std::string getShortConfigSettingName(const std::string& longStr);

void logToMonitor(std::string& logData);
void logToFile(std::string& logData, std::string& logFilename,
               std::ios_base::openmode mode = std::ios_base::out);

metadataQueue initializeMetadata(const std::string& filename, const configMap& config);
void validateMetadataFile(std::ifstream& metadataFile, const std::string& filename);
metadataQueue parseMetadataFile(std::ifstream& metadataFile, const configMap& config);
metadataInstruction parseMetadataInstruction(std::string& instructionStr);
void validateMetadataCode(const char& code);
void validateMetadataDescriptor(const std::string& descriptor);
void validateMetadataCycles(const std::string& numCycles);
void logMetadataFileData(const metadataInstruction& instr, configMap config);
std::string generateMetadataLogData(metadataInstruction instr, const configMap& config);
configSetting getConfigSetting(const std::string& descriptor, configMap config);
//
// Main Function Implementation ////////////////////////////////////////////////
//
int main(int argc, char *argv[])
{
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
        logConfigFileData(config);

        metadataFilename = config["File Path"];
        if (metadataFilename.empty())
        {
            throw std::string("Error: 'File Path' missing from config file");
        }
        metaQueue = initializeMetadata(metadataFilename, config);
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
    std::string logData = generateConfigLogData(config);

    if (config["Log"] == "Log to Monitor")
    {
        logToMonitor(logData);
    }
    else if (config["Log"] == "Log to File")
    {
        logToFile(logData, config["Log File Path"]);
    }
    else if (config["Log"] == "Log to Both")
    {
        logToMonitor(logData);
        logToFile(logData, config["Log File Path"]);
    }
    else
    {
        throw std::string("Error: cannot log data - invalid or missing log type");
    }
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
    logData += "Monitor = "     + config["Monitor"]     + " ms/cycle\n";
    logData += "Processor = "   + config["Processor"]   + " ms/cycle\n";
    logData += "Scanner = "     + config["Scanner"]     + " ms/cycle\n";
    logData += "Hard Drive = "  + config["Hard drive"]  + " ms/cycle\n";
    logData += "Keyboard = "    + config["Keyboard"]    + " ms/cycle\n";
    logData += "Memory = "      + config["Memory"]      + " ms/cycle\n";
    logData += "Projector = "   + config["Projector"]   + " ms/cycle\n";

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
    if (longStr != "Version/Phase" &&
        longStr != "File Path" &&
        longStr != "Log" &&
        longStr != "Log File Path")
    {
        std::string shortStr;
        size_t end = longStr.find(' ');
        shortStr = longStr.substr(0, end);
        if (shortStr == "Hard")
        {
            shortStr += " drive";
        }
        return shortStr;
    }
    else
    {
        return longStr;
    }
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
    metadataInstruction instruction;
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

                instruction = parseMetadataInstruction(tempInstr);

                // logging happens here
                logMetadataFileData(instruction, config);

                metaQueue.push(instruction);
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
metadataInstruction parseMetadataInstruction(std::string& instructionStr)
{
    unsigned long i = 0;
    unsigned long instrLen = instructionStr.length();

    metadataInstruction instruction;
    char code;
    std::string descriptor;
    std::string numCycles;

    code = instructionStr[i];
    validateMetadataCode(code);
    instruction.code = code;

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
    instruction.descriptor = descriptor;

    i = j + 1;

    numCycles = instructionStr.substr(i, instrLen - i);
    validateMetadataCycles(numCycles);
    instruction.numCycles = strToUnsignedLong(numCycles);

    return instruction;
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
void logMetadataFileData(const metadataInstruction& instr, configMap config)
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
std::string generateMetadataLogData(metadataInstruction instr, const configMap& config)
{
    std::string result;

    if (instr.numCycles > 0)
    {
        std::string code = std::string(1, instr.code);
        std::string descriptor = instr.descriptor;
        std::string numCycles = std::to_string(instr.numCycles);

        configSetting setting = getConfigSetting(descriptor, config);

        unsigned long cycleTime = strToUnsignedLong(setting.value);
        std::string totalTime = std::to_string(instr.numCycles * cycleTime);

        result = code + "{" + descriptor + "}" + numCycles +
                " - " + totalTime + " ms\n";
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
