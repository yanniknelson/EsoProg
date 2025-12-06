#pragma once

#include <vector>

template<typename OperationTypes> class IRegion;

template<typename OperationTypes>
class IOperation
{
public:
	IOperation(typename IOperation<OperationTypes>* pParent) : m_pParent(pParent) {};
	virtual OperationTypes::Enum GetType() const = 0;
	
protected:
	typename IOperation<OperationTypes>* m_pParent{ nullptr };
};

template<typename OperationTypes>
class IRegion
{
public:
	IRegion() {};
	virtual ~IRegion()
	{
		for (IOperation<OperationTypes>* pOperation : m_contents)
		{
			delete pOperation;
		}
	}
	
	void AddOperation(typename IOperation<OperationTypes>* pOperation) { m_contents.push_back(pOperation); }

protected:
	std::vector<typename IOperation<OperationTypes>*> m_contents;
};

template<typename OperationTypes>
class IError : public IOperation<OperationTypes>
{
public:
	IError(IOperation<OperationTypes>* pParent) : IOperation<OperationTypes>(pParent) {};
	virtual OperationTypes::Enum GetType() const { return OperationTypes::Error; };
};