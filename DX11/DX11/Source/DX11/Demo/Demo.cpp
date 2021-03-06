﻿#include "Demo.h"
#include "DX11/External/ImGui/ImGui_DX11.h"



DemoSelector::DemoSelector(const std::vector<std::string>& demos)
    : m_DemoName(nullptr)
    , m_DemoNames(demos)
    , m_Demo()
{

}


DemoSelector::~DemoSelector()
{

}


void DemoSelector::UpdateSelector(const DemoCreateFunc& createFunc)
{
    bool changed = false;
    
    if (ImGui::BeginCombo("demo", m_DemoName != nullptr ? m_DemoName : ""))
    {
        for (auto& demoName : m_DemoNames)
        {
            bool selected = m_DemoName == demoName.c_str();

            if (ImGui::Selectable(demoName.c_str(), &selected))
            {
                m_DemoName = demoName.c_str();
                changed = true;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    if (changed)
    {
        m_Demo = createFunc(m_DemoName);
    }
}


void DemoSelector::UpdateDemo(float deltaTime)
{
    if (m_Demo)
    {
        m_Demo->Update(deltaTime);
    }
}


void DemoSelector::RenderDemo()
{
    if (m_Demo)
    {
        m_Demo->Render();
    }
}


void DemoSelector::OnResizedBuffer(const Size2D& size)
{
    if (m_Demo)
    {
        m_Demo->OnResizedBuffer(size);
    }
}

