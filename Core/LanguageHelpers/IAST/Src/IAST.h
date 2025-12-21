#pragma once

#include <memory>
#include <vector>

template<typename OperationTypes>
class IRegion;

//////////////////////////////////////////////////////////////
template<typename OperationTypes>
class IOperation
{
  protected:
    using TOperationShrdPtr = typename std::shared_ptr<IOperation<OperationTypes>>;
    using TOperationWkPtr = typename std::weak_ptr<IOperation<OperationTypes>>;
    using TRegionShrdPtr = typename std::shared_ptr<IRegion<OperationTypes>>;
    using TRegionWkPtr = typename std::weak_ptr<IRegion<OperationTypes>>;

  public:
    //////////////////////////////////////////////////////////////
    IOperation(typename TOperationShrdPtr pParent, typename TRegionShrdPtr pRegion)
        : m_pParent(pParent)
        , m_pParentRegion(pRegion)
        {
        };

    virtual ~IOperation() = default;

    virtual OperationTypes::Enum GetType() const = 0;

    //////////////////////////////////////////////////////////////
    void SetIndex(size_t regionIndex)
    {
        m_regionIndex = regionIndex;
    };

    //////////////////////////////////////////////////////////////
    TOperationShrdPtr GetParent() const
    {
        return m_pParent.lock();
    }

    //////////////////////////////////////////////////////////////
    TOperationShrdPtr GetNextOperation() const
    {
        TRegionShrdPtr pParentRegion = m_pParentRegion.lock();
        if (!pParentRegion)
            return nullptr;

        // this iterator type is highly tied up in templates the cleanest way to use it is with auto
        auto nextOp = pParentRegion->begin() + m_regionIndex + 1;
        if (nextOp != pParentRegion->end())
        {
            return *nextOp;
        }
        return nullptr;
    }

  protected:
    typename TOperationWkPtr m_pParent;
    typename size_t m_regionIndex{ 0 };
    typename TRegionWkPtr m_pParentRegion;
};

//////////////////////////////////////////////////////////////
template<typename OperationTypes>
class IRegion
{
    using TOperation = typename IOperation<OperationTypes>;
    using TOperationPtr = std::shared_ptr<TOperation>;
    using TRegion = std::vector<TOperationPtr>;

  public:
    using TRegionIterator = TRegion::iterator;

    IRegion() {};
    virtual ~IRegion() = default;

    //////////////////////////////////////////////////////////////
    void AddOperation(typename TOperationPtr pOperation)
    {
        m_contents.push_back(pOperation);
        m_contents.back()->SetIndex(m_contents.size() - 1);
    }

    //////////////////////////////////////////////////////////////
    TOperationPtr front()
    {
        return m_contents.front();
    }

    //////////////////////////////////////////////////////////////
    TRegionIterator begin()
    {
        return m_contents.begin();
    }

    //////////////////////////////////////////////////////////////
    TRegionIterator end()
    {
        return m_contents.end();
    }

  protected:
    TRegion m_contents;
};

//////////////////////////////////////////////////////////////
template<typename OperationTypes>
class IError : public IOperation<OperationTypes>
{
  public:
    IError(std::shared_ptr<IOperation<OperationTypes>> pParent, std::shared_ptr<IRegion<OperationTypes>> pRegion) : IOperation<OperationTypes>(pParent, pRegion) {};

    // IOperation
    virtual typename OperationTypes::Enum GetType() const override
    {
        return OperationTypes::Error;
    };
    // ~IOperation
};

//////////////////////////////////////////////////////////////
template<typename OperationTypes>
class IProgram : public IOperation<OperationTypes>
{
  public:
    IProgram(std::shared_ptr<IOperation<OperationTypes>> pParent, std::shared_ptr<IRegion<OperationTypes>> pRegion) : IOperation<OperationTypes>(pParent, pRegion) {};

    // IOperation
    virtual typename OperationTypes::Enum GetType() const override
    {
        return OperationTypes::Program;
    };
    // ~IOperation

    std::shared_ptr<IRegion<OperationTypes>> m_pRegion = std::make_shared<IRegion<OperationTypes>>();
};
