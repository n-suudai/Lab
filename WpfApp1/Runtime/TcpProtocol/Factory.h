#pragma once


#include <memory>
#include <map>
#include <string>


template<typename BaseClass>
class CreatorBase
{
public:
    virtual ~CreatorBase() {}
    virtual BaseClass* Create() const = 0;
    virtual std::shared_ptr<BaseClass> CreateShared() const = 0;
    virtual std::unique_ptr<BaseClass> CreateUnique() const = 0;
};


template<typename ProductClass, typename BaseClass>
class Creator final : public CreatorBase<BaseClass>
{
public:
    BaseClass* Create() const override
    {
        return new ProductClass();
    }

    std::shared_ptr<BaseClass> CreateShared() const override
    {
        return std::make_shared<ProductClass>();
    }

    std::unique_ptr<BaseClass> CreateUnique() const override
    {
        return std::make_unique<ProductClass>();
    }
};


template<typename BaseClass>
class Factory
{
public:
    typedef std::string ObjectType;
    typedef std::unique_ptr<CreatorBase<BaseClass>> CreatorBasePtr;

    BaseClass* Create(const ObjectType& type);
    std::shared_ptr<BaseClass> CreateShared(const ObjectType& type);
    std::unique_ptr<BaseClass> CreateUnique(const ObjectType& type);

    bool Register(const ObjectType& type, CreatorBasePtr&& creator);

private:
    CreatorBase<BaseClass>* FindCreator(const ObjectType& type);

    typedef std::map<ObjectType, CreatorBasePtr> CreatorMap;
    CreatorMap m_creatorMap;
};

template<typename BaseClass>
BaseClass* Factory<BaseClass>::Create(const ObjectType& type)
{
    CreatorBase<BaseClass>* creator = FindCreator(type);
    if (creator)
    {
        return creator->Create();
    }
    return nullptr;
}

template<typename BaseClass>
std::shared_ptr<BaseClass> Factory<BaseClass>::CreateShared(const ObjectType& type)
{
    CreatorBase<BaseClass>* creator = FindCreator(type);
    if (creator)
    {
        return creator->CreateShared();
    }
    return nullptr;
}

template<typename BaseClass>
std::unique_ptr<BaseClass> Factory<BaseClass>::CreateUnique(const ObjectType& type)
{
    CreatorBase<BaseClass>* creator = FindCreator(type);
    if (creator)
    {
        return creator->CreateUnique();
    }
    return nullptr;
}

template<typename BaseClass>
bool Factory<BaseClass>::Register(const ObjectType& type, CreatorBasePtr&& creator)
{
    if (FindCreator(type) == nullptr)
    {
        m_creatorMap[type] = std::move(creator);
        return true;
    }

    return false;
}

template<typename BaseClass>
CreatorBase<BaseClass>* Factory<BaseClass>::FindCreator(const ObjectType& type)
{
    typename CreatorMap::iterator it = m_creatorMap.find(type);
    if (it == m_creatorMap.end())
    {
        return nullptr;
    }

    CreatorBase<BaseClass>* creator = (*it).second.get();
    return creator;
}

