#pragma once
/**
* @note:    这里非常有趣一个点是dll会删去静态库中没有被使用的函数，所以如果sandbox要使用没有被使用的函数，会出现链接错误。
*           解决方案是module definition文件，里面定义了需要使用的函数  。 
*           当然也可以用静态库。 
*/
#include "Hazel/Layer.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {
    class HAZEL_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;

    };

}