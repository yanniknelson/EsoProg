#pragma once

#include "BrainFckAST.h"         // for BrainFckProgram
#include "BrainFckAstVisitor.h"  // for BrainFckRuntimeVisitor
#include "BrainFckParser.h"      // for BrainFckParser
#include "BrainFckTokeniser.h"   // for BrainFckTokeniser

#include <CRuntime.h>            // for CRuntime
#include <ELanguages.h>          // for ELanguages::Enum
#include <IMemoryArray.h>        // for IMemoryArray
#include <IRuntime.h>            // for SRuntimeSyncronisationStruct
#include <LogManager.h>          // for TLoggerPtr

#include <cstdint>               // for uint8_t
#include <memory>
#include <sstream>               // for std::ostringstream
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////
class BrainFckRuntime : public CRuntime<BrainFckOperationTypes::Enum>
{
  public:
    BrainFckRuntime(SRuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream);

    // IRuntime
    virtual ELanguages::Enum GetRuntimeLanguage() const override;
    virtual std::vector<std::string> GetSupportedFileTypes() const override;
    virtual void ResetImplementation() override;
    virtual void RenderWindows() override;
    virtual void CacheState() override;
    // ~IRuntime

  private:
    TLoggerPtr m_pLogger{ nullptr };

    BrainFckTokeniser m_tokeniser;
    BrainFckParser m_parser;
    std::shared_ptr<BrainFckProgram> m_pProgramAST{ nullptr };
    BrainFckRuntimeVisitor m_runtimeVisitor;

    int m_currentIndex{ 0 };
    IMemoryArray<uint8_t> m_array;
    int m_cachedIndex{ 0 };
    IMemoryArray<uint8_t> m_cachedArray;

    // IRuntime
    virtual void OnInput(int val) override;
    // ~IRuntime

    // CRuntime
    virtual void OnSourceSet() override;
    virtual bool ShouldEnd(const BrainFckOperationTypes::Enum& token) override;
    virtual BrainFckOperationTypes::Enum StepExecution_Internal() override;
    // ~CRuntime
};
