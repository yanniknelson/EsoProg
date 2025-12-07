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
	using TOperation = typename IOperation<OperationTypes>;
	using TOperationPtr = std::shared_ptr<TOperation>;
	using TRegion = std::vector<TOperationPtr>;
public:
	IRegion() {};
	virtual ~IRegion() = default;
	
	void AddOperation(typename TOperationPtr pOperation) { m_contents.push_back(pOperation); }

	TRegion::iterator begin() { return m_contents.begin(); }
	TRegion::iterator end() { return m_contents.end(); }

protected:
	TRegion m_contents;
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