#pragma once

#include "ELanguages.h"  // for ELanguages::Enum

#include <IRuntime.h>    // for IRuntime

#include <sstream>       // for std::ostringstream
#include <string>
#include <vector>

class NullRuntime : public IRuntime
{
  public:
    NullRuntime(RuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : IRuntime(rSync, rOutputStream, rExecutionhistoryStream) {};

    virtual ELanguages::Enum GetRuntimeLanguage() const override
    {
        return ELanguages::COUNT;
    }
    virtual std::vector<std::string> GetSupportedFileTypes() const override
    {
        return {};
    }

    virtual void SetSourceCode(std::string str) {};

    virtual bool StepExecution()
    {
        return false;
    };

    virtual void ResetImplementation() override {};
    virtual void ResetCodeStream() override {};

    virtual void RenderWindows() override {};
    virtual void CacheState() override {};

  private:
    void RenderImageDisplay(RuntimeSyncronisationStruct& rSync);

    virtual void OnInput(int val) override {};
};
