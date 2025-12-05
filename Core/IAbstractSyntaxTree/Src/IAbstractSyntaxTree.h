#pragma once

#include <vector>

class IRegion;

class IOperation
{
public:
	IOperation(IRegion* pRegion) : m_pRegion(pRegion) {};
	
protected:
	IRegion* m_pRegion{ nullptr };
};

class IRegion : public IOperation
{
public:
	IRegion(IRegion* pRegion) : IOperation(pRegion) {};
	~IRegion()
	{
		for (IOperation* pOperation : m_contents)
		{
			delete pOperation;
		}
	}
	
	void AddOperation(IOperation* pOperation) { m_contents.push_back(pOperation); }

protected:
	std::vector<IOperation*> m_contents;
};
