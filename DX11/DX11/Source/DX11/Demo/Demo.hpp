#pragma once

#include <string>
#include <functional>
#include <vector>
#include <memory>


class Demo
{
public:
    Demo() {}
    virtual ~Demo() {}
    virtual void Update() {}
    virtual void Render() {}
};


class DemoSelector
{
public:
    typedef std::function<std::shared_ptr<Demo>(const std::string& name)> DemoCreateFunc;

public:
    DemoSelector(const std::vector<std::string>& demos);
    
    ~DemoSelector();

    void UpdateSelector(const DemoCreateFunc& createFunc);

    void UpdateDemo();

    void RenderDemo();

private:
    const char*              m_DemoName;
    std::vector<std::string> m_DemoNames;
    std::shared_ptr<Demo>    m_Demo;
};
