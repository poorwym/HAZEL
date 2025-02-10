# Hazel 里关于 EventCallback 的实现

## 1. 回调函数的注册

### a. 接口定义与类型别名
- 在抽象窗口接口 `Window` 中，定义了一个类型别名：
  ```cpp Window.h
  using EventCallbackFn = std::function<void(Event&)>;
  ```
  这意味着所有事件回调函数都必须符合“接收一个 `Event` 的引用，返回 `void`”的接口。


### b. 具体实现
- 在 `WindowsWindow`（Windows平台具体实现）中，`SetEventCallback` 的实现非常简单，直接将传入的回调函数保存到窗口内部数据 `m_Data` 中：

  ```cpp WindowsWindow.cpp
  inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
  ```
- 应用程序（在 `Application` 类中）在构造函数中创建窗口后，通过如下调用注册回调：
  ```cpp
  m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
  ```
  其中 `BIND_EVENT_FN(OnEvent)` 实际上是将 `Application::OnEvent` 成员函数绑定到当前应用实例上，并转换为 `std::function<void(Event&)>` 类型。

**总结**：注册过程主要在应用层将自己的 `OnEvent` 函数设置为窗口内部的事件回调函数，而这个回调函数被存储在 `WindowData`（`m_Data`）中。

---

## 2. 事件的触发过程

### a. GLFW 与窗口用户指针 
- 在 `WindowsWindow::Init` 函数中，创建 GLFW 窗口后，调用：
  ```cpp WindowsWindow.cpp
  glfwSetWindowUserPointer(m_Window, &m_Data);
  ```
  将 `m_Data` 的地址传递给 GLFW，这样在各个回调中可以通过 `glfwGetWindowUserPointer(window)` 获取到 `m_Data`，进而访问到之前注册的回调函数 `EventCallback`。

### b. 注册 GLFW 事件回调
- 接下来，针对各种事件（如窗口大小改变、关闭、键盘、鼠标等），都设置了对应的 GLFW 回调函数，形式大多为 Lambda 表达式。例如，窗口大小改变回调：
  ```cpp WindowsWindow.cpp
  glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
  {
      // 从 GLFW 获取自定义的窗口数据
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.Width = width;
      data.Height = height;
  
      // 构造对应的事件对象
      WindowResizeEvent event(width, height);
      // 触发应用层注册的事件回调函数
      data.EventCallback(event);
  });
  ```
- 类似地，键盘、鼠标等回调中，首先通过 `glfwGetWindowUserPointer` 获取到 `WindowData`，然后根据 GLFW 回调参数构造出对应的事件（例如 `KeyPressedEvent`、`MouseMovedEvent` 等），并最终调用 `data.EventCallback(event)`。

**总结**：当 GLFW 检测到某个用户交互（如窗口尺寸变化、键盘输入等），相应的 GLFW 回调函数就会被调用，而这些回调函数内部都会构造一个具体的事件对象，并通过事先设置好的 `EventCallback`（实际上指向 Application::OnEvent）来传递该事件。

---

## 3. 事件回调函数的执行

### a. Application 中的事件入口
- 通过注册机制，所有事件最终都会调用 `Application::OnEvent(Event& e)` 函数。这个函数既是事件分发的入口，也是事件进一步传递给层（Layer）的桥梁。

### b. 事件分发器（EventDispatcher）
- 在 `OnEvent` 内部，首先创建了一个 `EventDispatcher` 对象：
  ```cpp
  EventDispatcher dispatcher(e);
  ```
- 接着，通过模板方法 `Dispatch<T>` 将事件分发到具体的处理函数上。例如：
  ```cpp
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
  dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
  ```
  这部分代码会判断事件对象的类型是否与模板参数 `T`（例如 `WindowCloseEvent`）匹配，如果匹配，就调用对应的处理函数（例如 `OnWindowClose`）。

### c. 传递给 LayerStack 中的各个 Layer
- 除了 Application 自身的处理之外，事件还会从顶层开始传递给各个注册的层。代码中倒序遍历 `m_LayerStack`：
  ```cpp
  for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
  {
      (*--it)->OnEvent(e);
      if (e.IsHandled())
      {
          break;
      }
  }
  ```
  这样设计的目的是先让最上层（例如覆盖层或 UI 层）优先处理事件，如果某个层标记了事件已处理（`e.m_Handled = true`），则停止继续传递。

**总结**：事件回调函数（`OnEvent`）首先利用 `EventDispatcher` 对事件进行初步的分类分发，然后再将事件传递到各个 Layer 中，由具体的层根据需要做进一步处理。这样形成了一个从底层触发、应用层分发、层级响应的完整事件处理流程。

---

## 4. 事件处理流程的关键步骤

1. **注册回调**  
   - 应用在创建窗口后，通过调用 `Window::SetEventCallback` 注册了一个事件回调函数，该回调函数通常是 `Application::OnEvent`，并存储在 `WindowData::EventCallback` 中。

2. **设置 GLFW 用户指针**  
   - 将 `WindowData` 作为 GLFW 的用户指针设置到 GLFW 窗口中，方便在后续的 GLFW 回调中访问注册的事件回调函数及其他窗口数据。

3. **注册 GLFW 事件回调**  
   - 针对不同类型的事件（窗口大小、关闭、键盘、鼠标等），注册相应的 GLFW 回调函数。在每个回调中：
     - 通过 `glfwGetWindowUserPointer` 获取到 `WindowData`。
     - 根据回调参数构造引擎内的具体事件对象（例如 `WindowResizeEvent`、`KeyPressedEvent` 等）。
     - 通过 `WindowData::EventCallback` 调用事件回调函数，将事件对象传递出去。

4. **事件回调执行与分发**  
   - 当事件触发时，调用的 `EventCallback` 实际上指向的是 `Application::OnEvent`。在这个函数中：
     - 使用 `EventDispatcher` 根据事件类型调用相应的处理函数（如 `OnWindowClose`、`OnWindowResize`）。
     - 遍历应用层的 LayerStack，将事件依次传递给各个层，直到某个层将事件标记为已处理。

---

## 5. 回调函数与事件的关联

- **关联方式**：在 GLFW 回调中，会根据具体的用户操作创建相应的事件对象，并直接调用之前注册的 `EventCallback`，传入该事件对象。这样，回调函数与事件之间形成了一种“一对一”或“一对多”的调用关系，即一个具体的 GLFW 回调对应生成一种或多种内部事件，再由统一的回调函数统一入口进行处理和分发。
  
- **解耦设计**：这种设计使得底层平台的事件（GLFW 回调）与应用层的事件处理逻辑（Application 和 Layer 中的 OnEvent 处理）解耦。窗口内部只负责捕获和转换事件，而实际的业务逻辑则由应用程序及其层负责，从而提高了代码的灵活性和可维护性。

---

## 总结

整个事件回调机制的流程可以总结为：

1. **注册阶段**：应用程序将自己的 `OnEvent` 函数（通过 `std::bind`）注册为窗口的事件回调函数，存储在窗口的 `WindowData` 中。

2. **触发阶段**：当 GLFW 检测到用户操作（如窗口大小改变、按键、鼠标移动等）时，触发相应的 GLFW 回调函数，这些回调通过获取 `WindowData`，构造对应的事件对象，并调用 `EventCallback`。

3. **执行阶段**：`EventCallback`（即 `Application::OnEvent`）收到事件后，先利用 `EventDispatcher`进行初步分发（比如窗口关闭、窗口尺寸变化等），随后再遍历各个 Layer 依次调用它们的 `OnEvent` 方法，完成事件的最终处理。

