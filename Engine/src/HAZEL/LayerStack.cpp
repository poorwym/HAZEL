﻿#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
        for (auto layer : m_Layers)
        {
            // layer->OnDetach();
            delete layer;
        }
	}

	void LayerStack::PushLayer(Layer* layer)
	{
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer); // 在m_LayerInsert位置插入layer
		m_LayerInsertIndex++;
        layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
        m_Layers.emplace_back(overlay); // 在m_Layers的最后插入overlay
        overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer); // 在m_Layers中找到layer
        if (it != m_Layers.end())
        {
            m_Layers.erase(it); // 删除layer
            m_LayerInsertIndex--; // 将m_LayerInsert向前移动
        }
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay); // 在m_Layers中找到overlay
        if (it != m_Layers.end())
        {
            m_Layers.erase(it); // 删除overlay
        }
	}
}