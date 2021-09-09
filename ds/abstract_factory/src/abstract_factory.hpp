#pragma once

#include <memory>
#include <unordered_map>

namespace obps
{

template<typename Key, typename T, typename ...Args>
class AbstractFactory final
{
public:
    using FactoryMethod = std::function<std::unique_ptr<T>()>;

    void Add(Key key, FactoryMethod factory_method);
    std::unique_ptr<T> Create(Key key);
    
private:
    std::unordered_map<Key, FactoryMethod> m_factory_methods;
};

template<typename Key, typename T, typename ...Args>
void AbstractFactory<Key, T, Args...>::Add(Key key, FactoryMethod factory_method)
{
    m_factory_methods.emplace(key, factory_method);
}

template<typename Key, typename T, typename ...Args>
std::unique_ptr<T> AbstractFactory<Key, T, Args...>::Create(Key key)
{
    try
    {
        auto&& factory_method = m_factory_methods.at(key); 
        return factory_method();
    }
    catch(const std::out_of_range& e)
    {
        throw e;
    }
}

} // namespace obps
