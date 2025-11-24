#pragma once

#include "ELanguages.h"

#include <iostream>
#include <sstream>

#include <CRuntime.h>


class NullRuntime : public IRuntime
{
public:
	NullRuntime(std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : IRuntime(rOutputStream, rExecutionhistoryStream) {};

	virtual ELanguages::Enum GetRuntimeLanguage() const override { return ELanguages::COUNT; }
	virtual std::vector<std::string> GetSupportedFileTypes() const override { return {}; }

	virtual void SetSourceCode(std::string str) {};

	virtual bool StepExecution() { return false;  };

	virtual void RenderWindows(RuntimeSyncronisationStruct& rSync) override {};
	virtual void CacheState() override {};

private:


	void RenderImageDisplay(RuntimeSyncronisationStruct& rSync);

	virtual void OnInput(int val) override {};

	virtual void ResetTokenisers() override {}
};