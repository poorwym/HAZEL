#pragma once
#include "Hazel/Core.h"
#include "Layer.h"
#include <vector>

namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		// Layer是普通层,从前往后渲染,用于游戏主要内容
		// Overlay是覆盖层,永远在最上层渲染,用于UI等需要显示在最上层的内容
		// Layer通过PushLayer添加到LayerStack中,会插入到普通层的最后
		// Overlay通过PushOverlay添加到LayerStack中,会插入到整个LayerStack的最后
		// Overlay永远在Layer的上面
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer); 
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
		unsigned int m_LayerInsertIndex = 0;
	};
};
