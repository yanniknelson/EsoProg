#pragma once

#include <IAST.h>

#include <memory>

template<typename OperationType>
class IASTVisitor
{
public:
	void Traverse(std::shared_ptr<IOperation<OperationType>> pOperation)
	{

	};
};