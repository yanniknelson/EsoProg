#pragma once

#include "ELanguages.h"  // for ELanguages::Enum

#include <IRuntime.h>    // for IRuntime, SRuntimeSyncronisationStruct

#include <sstream>       // for std::ostringstream
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////
class NullRuntime : public IRuntime
{
  public:
    NullRuntime(SRuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream)
        : IRuntime(rSync, rOutputStream, rExecutionhistoryStream)
    {
    };

// clang-format off
    // IRuntime
    virtual ELanguages::Enum GetRuntimeLanguage() const override { return ELanguages::None; }
    virtual std::vector<std::string> GetSupportedFileTypes() const override { return {}; }
    virtual void SetSourceCode(std::string str) override {}
    virtual bool StepExecution() override { return false; }
    virtual void ResetImplementation() override {}
    virtual void ResetCodeStream() override {}
    virtual void RenderWindows() override {}
    virtual void CacheState() override {}
    // ~IRuntime

  private:
    // IRuntime
    virtual void OnInput(int val) override {};
    // ~IRuntime
// clang-format on
};
