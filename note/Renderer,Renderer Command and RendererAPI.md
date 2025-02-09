# Renderer,Renderer Command and RendererAPI
实际上上完ep33后，出现了大量新的文件和概念。
## 四个概念

### Render Queue(渲染队列)
- 用于存储和管理渲染命令的队列结构
- 按照特定顺序(如从前到后、从不透明到半透明等)组织渲染命令
- 优化渲染性能,减少状态切换
- 实现渲染排序和批处理

### Renderer(渲染器)
- 引擎渲染系统的高层抽象
- 管理场景的整体渲染流程
- 负责渲染队列的调度和执行
- 提供渲染API的统一接口
- 与具体图形API无关

### RenderCommand(渲染命令)
- 封装单个渲染操作(如清屏、绘制等)
- 作为Renderer和RendererAPI之间的桥梁
- 将渲染指令转换为具体API调用
- 提供线程安全的命令缓冲机制

### RendererAPI(渲染API)
- 底层图形API的抽象接口
- 定义了与具体图形API交互的方法
- 可以有多个实现(OpenGL、Vulkan等)
- 处理平台相关的渲染细节


## 包含关系
Renderer include RenderCommand include RendererAPI.

## 自己的解释
Renderer作为**最上层的结构**（这点非常重要）主要负责管理渲染队列，并根据不同的API进行调用。

RenderCommand只是一个中间结构，负责将Renderer的指令转换为具体API的调用。或者就是单纯的简单封装。比如下面的代码：
```c
inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
	s_RendererAPI->DrawIndexed(vertexArray);
}
```
单纯把DrawIndexed这个命令用RendererAPI去实现。

我觉得这样的设计主要是为了中间多一层缓冲，方便进行不同平台的维护。因为不同平台可能会有不同的RendererAPI，相同的接口方便进行不同的实现，同时又让Renderer不必接触到具体的API。（再次强调Renderer是上层的结构）

RendererAPI则是一个**纯虚类**或者叫**抽象接口**，类似buffer或者vertexarray，在Platform中存在不同的实现。

这样总体的设计之后,Hazel内部已经全是自己的内部代码（或者像spdlog和glm这种跨平台的代码），具有高度可拓展和可移植性。

