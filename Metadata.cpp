/**
 * @file Metadata.cpp
 * 
 * @brief Implementation file for Metadata class
 * 
 * @details Implements all member methods of Metadata class
 * 
 * @version 1.00
 *          Adam Landis (8 March 2019)
 *          Initial development and testing of Metadata class
 * 
 * @note Requries Metadata.h
 */ 
//
// Header Files ////////////////////////////////////////////////////////////////
//
#include "Metadata.h"
#include "helpers.h"
//
// Class Member Implementation /////////////////////////////////////////////////
//

/**
 * @brief      Constructs the object.
 *
 * @param[in]  filename  The metadata filename
 */
Metadata::Metadata(const std::string& filename, Config *config): filename(filename), config(config)
{
    std::ifstream metadataFile(filename.c_str(), std::ios::in);

    validateMetadataFile(metadataFile);
    parseMetadataFile(metadataFile);

    metadataFile.close();
}

/**
 * Validates a metadata file (i.e. checks for correct extension, not empty, etc)
 *
 * @param   metadataFile
 *          The metadata file to parse (ifstream)
 *
 * @return  None
 */
void Metadata::validateMetadataFile(std::ifstream& metadataFile)
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
        throw std::string("Error: invalid metadata file header");
    }
}

/**
 * Parses the contents of a metadata file
 *
 * @param   metadataFile
 *          The metadata file to parse
 *
 * @return  a queue containing metadata instructions
 */
void Metadata::parseMetadataFile(std::ifstream& metadataFile)
{
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

                std::string descriptor = instr.getDescriptor();
                configSetting setting = this->config->getConfigSetting(descriptor);
                unsigned long cycleTime = strToUnsignedLong(setting.value);
                instr.setWaitTime(cycleTime);         

                if (descriptor == "hard drive")
                {
                    // instr.setSemPtr(&semHD);
                    instr.setSemPtr(NULL);
                }
                else if (descriptor == "projector")
                {
                    // instr.setSemPtr(&semProj);
                    instr.setSemPtr(NULL);
                }
                else if (descriptor == "keyboard")
                {
                    // instr.setSemPtr(&semKB);
                    instr.setSemPtr(NULL);
                }
                else if (descriptor == "monitor")
                {
                    // instr.setSemPtr(&semMon);
                    instr.setSemPtr(NULL);
                }
                else if (descriptor == "scanner")
                {
                    // instr.setSemPtr(&semScan);
                    instr.setSemPtr(NULL);
                }

                mdQueue.push(instr);
                start = end + 1;
            } while (end != std::string::npos && start < tempLine.length());
        }
    }
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
MetadataInstruction Metadata::parseMetadataInstruction(std::string& instructionStr)
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
void Metadata::validateMetadataCode(const char& code)
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
void Metadata::validateMetadataDescriptor(const std::string& descriptor)
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
void Metadata::validateMetadataCycles(const std::string& numCycles)
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
 * @return  None
 */
void Metadata::logMetadataFileData(const MetadataInstruction& instr)
{
    static bool beginLog = false;
    std::string logData;
    std::string logType = this->config->getSettingVal("Log");
    std::string logFilePath = this->config->getSettingVal("Log File Path");

    if (!beginLog)
    {
        logData = "Meta-Data Metrics\n";
        beginLog = true;
    }

    logData += generateMetadataLogData(instr);

    if (logType == "Log to Monitor")
    {
        logToMonitor(logData);
    }
    else if (logType == "Log to File")
    {
        logToFile(logData, logFilePath, std::ios_base::app);
    }
    else if (logType == "Log to Both")
    {
        logToMonitor(logData);
        logToFile(logData, logFilePath, std::ios_base::app);
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
 * @return  the generated metadata log string
 */
std::string Metadata::generateMetadataLogData(MetadataInstruction instr)
{
    std::string result;

    if (instr.getNumCycles() > 0)
    {
        configSetting setting = this->config->getConfigSetting(instr.getDescriptor());

        unsigned long cycleTime = strToUnsignedLong(setting.value);
        std::string totalTime = std::to_string(instr.getNumCycles() * cycleTime);

        result = instr.toString() + " - " + totalTime + " ms\n";
    }

    return result;
}