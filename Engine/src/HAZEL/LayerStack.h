#pragma once
/**
 * @note: 这个是Layer的管理器，提供一些layer的管理方法，
 *	overlay 和 layer 的区别是：overlay 永远在 layer 上面一层。
 * 渲染顺序是 先 layer 再 overlay
 * 而对事件的处理顺序是：先判断overlay，再判断layer
 */
#include "Hazel/Core.h"
#include "Layer.h"
#include <vector>

namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

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
