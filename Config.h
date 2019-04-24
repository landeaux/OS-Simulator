/**
 * @file Config.h
 * 
 * @brief Definition file for Config class
 * 
 * @details Specifies all member methods of the Config class
 * 
 * @version 1.00
 *          Adam Landis (7 April 2019)
 *          Initial development and testing of Config class
 * 
 * @note None
 */
//
// Precompiler Directives //////////////////////////////////////////////////////
//
#ifndef CONFIG_H
#define CONFIG_H
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include <string>
#include <fstream>      // ifstream
#include <map>          // for the config map
#include <iostream>
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
//
// Class Definition ////////////////////////////////////////////////////////////
// 
class Config
{
public:
    Config(const std::string& filename);
    void validateConfigFile(std::ifstream& configFile);
    void parseConfigFile(std::ifstream& configFile);
    configSetting parseConfigLine(const std::string& configLine);
    void logConfigFileData();
    std::string genConfigLogData();
    void logData(std::string data);

    std::string getShortConfigSettingName(const std::string& longStr);
    const configSetting getConfigSetting(const std::string& descriptor);
    
    /**
     * @brief      Gets the configuration map.
     *
     * @return     The configuration map.
     */
    inline const configMap getConfigMap() const { return this->config; }

    /**
     * @brief      Gets the value of a setting given the setting key.
     *
     * @param[in]  key   The setting key
     *
     * @return     The setting value.
     */
    std::string getSettingVal(std::string key) { return this->config[key]; }
private:
    std::string filename;
    configMap config;
    const std::string CONFIG_HEADER = "Start Simulator Configuration File";
    const std::string CONFIG_FOOTER = "End Simulator Configuration File";
    const std::string CONFIG_SETTING_NAMES[17] = {
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
            "Log File Path",
            "Processor Quantum Number",
            "CPU Scheduling Code"
    };
};
//
// Terminating Precompiler Directives
// 
#endif  // CONFIG_H
//