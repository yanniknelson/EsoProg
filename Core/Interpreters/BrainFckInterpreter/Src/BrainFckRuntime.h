#pragma once

#include "BrainFckAST.h"
#include "BrainFckAstVisitor.h"
#include "BrainFckParser.h"
#include "BrainFckTokeniser.h"

#include <CRuntime.h>
#include <ELanguages.h>
#include <IMemoryArray.h>
#include <IRuntime.h>

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class BrainFckRuntime : public CRuntime<BrainFckOperationTypes::Enum>
{
  public:
    BrainFckRuntime(RuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream)
        : CRuntime(rSync, rOutputStream, rExecutionhistoryStream), m_parser(&m_tokeniser)
    {
        m_tokeniser.SetTextStream(m_code);
    };

    virtual ELanguages::Enum GetRuntimeLanguage() const override
    {
        return ELanguages::Brainfck;
    }
    virtual std::vector<std::string> GetSupportedFileTypes() const override
    {
        return { ".txt" };
    }

    virtual void ResetImplementation() override
    {
        m_array.Clear();
    }

    virtual void RenderWindows() override;
    virtual void CacheState() override;

  private:
    BrainFckTokeniser m_tokeniser;
    BrainFckParser m_parser;
    std::shared_ptr<BrainFckProgram> m_pProgramAST{ nullptr };
    BrainFckRuntimeVisitor m_runtimeVisitor;

    int m_currentIndex{ 0 };
    IMemoryArray<uint8_t> m_array;
    int m_cachedIndex{ 0 };
    IMemoryArray<uint8_t> m_cachedArray;

    virtual void OnSourceSet() override;

    virtual void OnInput(int val) override;

    virtual bool ShouldEnd(const BrainFckOperationTypes::Enum& token) override;

    virtual BrainFckOperationTypes::Enum StepExecution_Internal() override;
};
