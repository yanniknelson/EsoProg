#pragma once

#include <vector>
#include <memory>

template<typename OperationTypes> class IRegion;

template<typename OperationTypes>
class IOperation
{
public:
	IOperation(typename std::shared_ptr<IOperation<OperationTypes>> pParent) : m_pParent(pParent) {};
	virtual ~IOperation() = default;
	virtual OperationTypes::Enum GetType() const = 0;
	
protected:
	typename std::shared_ptr<IOperation<OperationTypes>> m_pParent{ nullptr };
};

template<typename OperationTypes>
class IRegion
{
public:
	IRegion() {};
	virtual ~IRegion() = default;
	
	void AddOperation(typename std::shared_ptr<IOperation<OperationTypes>> pOperation) { m_contents.push_back(pOperation); }

protected:
	std::vector<typename std::shared_ptr<IOperation<OperationTypes>>> m_contents;
};

template<typename OperationTypes>
class IError : public IOperation<OperationTypes>
{
public:
	IError(std::shared_ptr<IOperation<OperationTypes>> pParent) : IOperation<OperationTypes>(pParent) {};
	virtual OperationTypes::Enum GetType() const { return OperationTypes::Error; };
};

template<typename OperationTypes>
class IProgram : public IOperation<OperationTypes>
{
public:
	IProgram(std::shared_ptr<IOperation<OperationTypes>> pParent) : IOperation<OperationTypes>(pParent) {};
	virtual OperationTypes::Enum GetType() const { return OperationTypes::Program; };
	IRegion<OperationTypes> m_region;
};