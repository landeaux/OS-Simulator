/**
 * @file Config.cpp
 * 
 * @brief Implementation file for Config class
 * 
 * @details Implements all member methods of Config class
 * 
 * @version 1.01
 *          Adam Landis (24 April 2019)
 *          Add support for new config settings "Processor Quantum Number" and 
 *          "CPU Scheduling Code"
 *          
 * @version 1.00
 *          Adam Landis (8 March 2019)
 *          Initial development and testing of Config class
 * 
 * @note Requries Config.h
 */ 
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include "Config.h"
#include "helpers.h"
//
// Class Member Implementation /////////////////////////////////////////////////
//

/**
 * @brief      Constructs the object.
 *
 * @param[in]  filename  The config filename
 */
Config::Config(const std::string& filename): filename(filename)
{
    std::ifstream configFile(this->filename, std::ios::in);

    validateConfigFile(configFile);
    parseConfigFile(configFile);

    configFile.close();
}

/**
 * @brief      Validates the configuration file
 *
 * @param      configFile  The configuration file
 * @param[in]  filename    The filename
 */
void Config::validateConfigFile(std::ifstream& configFile)
{
    // make sure file exists
    if (!configFile.good())
    {
        throw std::string("Error: config file \"" + filename + "\" does not exist");
    }

    // make sure file has correct extension
    if (!isValidFileExtension(filename, "conf"))
    {
        throw std::string("Error: invalid extension for config file");
    }

    // make sure file is not empty
    if (isEmptyFile(configFile))
    {
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
 * @return  None
 */
void Config::parseConfigFile(std::ifstream& configFile)
{
    std::string tempLine;
    configSetting tempSetting;

    while (!configFile.eof() && tempLine != CONFIG_FOOTER)
    {
        getline(configFile, tempLine);
        if (tempLine != CONFIG_FOOTER)
        {
            tempSetting = parseConfigLine(tempLine);
            this->config[tempSetting.key] = tempSetting.value;
        }
    }
}

/**
 * Logs configuration file data - either to file, monitor, or both
 *
 * @return  None
 */
void Config::logConfigFileData()
{
    std::string data = genConfigLogData();

    logData(data);
}

/**
 * Generates a single string containing config log data for logging to monitor,
 * an output file, or both.
 *
 * @return  the generated string representing the log data
 */
std::string Config::genConfigLogData()
{
    std::string logData;

    logData += "Configuration File Data\n";
    logData += "Monitor = "       + this->config["Monitor"]       + " ms/cycle\n";
    logData += "Processor = "     + this->config["Processor"]     + " ms/cycle\n";
    logData += "Scanner = "       + this->config["Scanner"]       + " ms/cycle\n";
    logData += "Hard Drive = "    + this->config["Hard drive"]    + " ms/cycle\n";
    logData += "Keyboard = "      + this->config["Keyboard"]      + " ms/cycle\n";
    logData += "Memory = "        + this->config["Memory"]        + " ms/cycle\n";
    logData += "Projector = "     + this->config["Projector"]     + " ms/cycle\n";
    logData += "System memory = " + this->config["System memory"] + " kbytes\n";

    logData += "Logged to: ";
    if (this->config["Log"] == "Log to Both") logData += "monitor and ";
    logData += this->config["Log File Path"] + "\n\n";

    return logData;
}

/**
 * @brief      Logs a single string of data either to a file, monitor, or both
 *
 * @param[in]  data    The data
 */
void Config::logData(std::string data)
{
    std::string logType = this->config["Log"];
    std::string logFilePath = this->config["Log File Path"];

    if (logType == "Log to Monitor")
    {
        logToMonitor(data);
    }
    else if (logType == "Log to File")
    {
        logToFile(data, logFilePath);
    }
    else if (logType == "Log to Both")
    {
        logToMonitor(data);
        logToFile(data, logFilePath);
    }
    else
    {
        throw std::string("Error: cannot log data - invalid or missing log type");
    }
}

/**
 * Parses a given line in the config file and returns a key-value pair
 *
 * @param   configLine
 *          The current line in the config file to parse
 *
 * @return  key-value pair representing the config setting
 */
configSetting Config::parseConfigLine(const std::string& configLine)
{
    configSetting setting;
    std::string settingKey;
    std::string settingValue;
    unsigned long lineLen = configLine.length();
    unsigned long i = 0;

    // Find index of first ':' char
    while (configLine[++i] != ':' && i < lineLen);

    if (i >= lineLen) // if ':' char not found in configLine
    {
        throw std::string("Error: unable to parse config file");
    }

    // Find index of first non-space char before ':'
    int j = i;
    while (configLine[--j] == ' ' && j > 0);
    j++;

    settingKey = configLine.substr(0, j);

    // Check if the string is a valid config setting key
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
        setting.key != "Log File Path" &&
        setting.key != "CPU Scheduling Code")
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
 * Converts a long config setting name to a short one
 *
 * @param   longStr
 *          The longer string to shorten
 *
 * @return  The shortened string
 */
std::string Config::getShortConfigSettingName(const std::string& longStr)
{
    std::string result = longStr;

    if (longStr != "Version/Phase" &&
        longStr != "File Path" &&
        longStr != "Log" &&
        longStr != "Log File Path" &&
        longStr != "CPU Scheduling Code" &&
        longStr != "Processor Quantum Number" &&
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
const configSetting Config::getConfigSetting(const std::string& descriptor)
{
    configSetting setting;

    if (descriptor == "block" || descriptor == "allocate")
    {
        setting.key = "Memory";
        setting.value = this->config["Memory"];
    }
    else if (descriptor == "run")
    {
        setting.key = "Processor";
        setting.value = this->config["Processor"];
    }
    else
    {
        std::string key = descriptor;
        key[0] = (char) toupper(key[0]);
        setting.key = key;
        setting.value = this->config[key];
    }

    return setting;
}