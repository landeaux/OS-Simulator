/**
 * @file Metadata.h
 * 
 * @brief Definition file for Metadata class
 * 
 * @details Specifies all member methods of the Metadata class
 * 
 * @version 1.01
 *          Adam Landis (24 April 2019)
 *          Change signature of parseMetadataFile() to have no parameters
 * 
 * @version 1.00
 *          Adam Landis (8 April 2019)
 *          Initial development and testing of Metadata class
 * 
 * @note None
 */
//
// Precompiler Directives //////////////////////////////////////////////////////
//
#ifndef METADATA_H
#define METADATA_H
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include <string>
#include <queue>        // to hold metadata instructions
#include <fstream>      // ifstream

#include "Config.h"
#include "MetadataInstruction.h"
//
// Typedefs ////////////////////////////////////////////////////////////////////
//
typedef std::queue<MetadataInstruction> metadataQueue;
//
// Class Definition ////////////////////////////////////////////////////////////
// 
class Metadata
{
public:
    Metadata(const std::string& filename, Config *config);

    void validateMetadataFile(std::ifstream& metadataFile);
    void parseMetadataFile();
    MetadataInstruction parseMetadataInstruction(std::string& instructionStr);
    void validateMetadataCode(const char& code);
    void validateMetadataDescriptor(const std::string& descriptor);
    void validateMetadataCycles(const std::string& numCycles);
    void logMetadataFileData(const MetadataInstruction& instr);
    std::string generateMetadataLogData(MetadataInstruction instr);

    void setSemPtrs(sem_t *semHD, sem_t *semProj, sem_t *semKB, sem_t *semMon, sem_t *semScan);

    /**
     * @brief      Gets the metadata instruction queue.
     *
     * @return     The metadata instruction queue.
     */
    inline metadataQueue getMetadataQueue() const { return this->mdQueue; }
private:
    std::string filename;
    metadataQueue mdQueue;
    Config *config;
    const std::string METADATA_HEADER = "Start Program Meta-Data Code:";
    const std::string METADATA_FOOTER = "End Program Meta-Data Code.";
    const std::string METADATA_CODES = "SAPIOM";
    const std::string METADATA_DESCRIPTORS[10] = {
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
    sem_t *semHD, *semProj, *semKB, *semMon, *semScan;
};
//
// Terminating Precompiler Directives
// 
#endif  // METADATA_H
//