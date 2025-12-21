#pragma once

#include "IRuntime.h"    // for IRuntime, SRuntimeSyncronisationStruct

#include <ELanguages.h>  // for ELanguages::Enum

#include <iostream>
#include <sstream>       // for std::ostringstream

#include <string>
#include <vector>

//////////////////////////////////////////////////////////////
template<typename TokenClass>
class CRuntime : public IRuntime
{
  public:
    //////////////////////////////////////////////////////////////
    CRuntime(SRuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : IRuntime(rSync, rOutputStream, rExecutionhistoryStream)
    {
    }

    virtual ELanguages::Enum GetRuntimeLanguage() const = 0;
    virtual std::vector<std::string> GetSupportedFileTypes() const = 0;

    //////////////////////////////////////////////////////////////
    virtual void SetSourceCode(std::string str) override
    {
        m_codeStr = str;
        m_rSync.m_bResetCodeSource = true;
        RequestReset();
    }

    //////////////////////////////////////////////////////////////
    virtual bool StepExecution() override
    {
        if (m_waitingForCharInput || m_waitingForValInput)
        {
            return false;
        }

        const TokenClass token = StepExecution_Internal();
        m_rExecutionHistoryStream << token << std::endl;

        return !ShouldEnd(token);
    }

    virtual void ResetImplementation() = 0;

    //////////////////////////////////////////////////////////////
    virtual void ResetCodeStream() override
    {
        std::stringstream tmp(m_codeStr.c_str());
        m_code.swap(tmp);
        OnSourceSet();
    }

    virtual void RenderWindows() = 0;
    virtual void CacheState() = 0;

  protected:
    std::string m_codeStr = "";
    std::stringstream m_code;

    virtual void OnSourceSet() = 0;
    virtual void OnInput(int val) = 0;
    virtual bool ShouldEnd(const TokenClass& token) = 0;

    virtual TokenClass StepExecution_Internal() = 0;
};
