## GLFW 数据的传递与获取

相当于一个全局的,我们需要获取到的信息

![image-20240311091054979](image-20240311091054979.png)

## Event事件以及层的理解

对于Window，我们有一个WindowData结构体记录了全局所需的数据，其中包括了一个回调函数的指针
![image-20240311092654652](image-20240311092654652.png)

在进行Application初始化时，我们会绑定这个回调函数：
![image-20240311092823651](image-20240311092823651.png)

而所绑定的这个OnEvent所对应的实现为：
![image-20240311092942325](image-20240311092942325.png)
在这里我们会反向遍历所有的层，然后通过OnEvent方法去处理接下来传入的参数事件e

事件e是在回调函数时进行传入的
![image-20240311093151468](image-20240311093151468.png)

也就是说，在程序运行时触发回调函数后，我们首先会更新data的数据，然后生成相应的事件，并将这个事件传递给Application中的OnEvent方法，在Application的OnEvent方法中我们将反向遍历所有层级去处理这个由回调函数传回来的函数

层级的概念则指向的是我们在不同的层会进行不同的操作，相当于把逻辑分离开来。

![image-20240311093600305](image-20240311093600305.png)

## EventDispatcher

EventDispatcher类如下:

![image-20240311150223614](image-20240311150223614.png)

应用实例:
![image-20240311150545547](image-20240311150545547.png)

![image-20240311151010165](image-20240311151010165.png)
首先创建EventDispatcher，接下来调用Dispatch方法对事件进行判断，如果这个事件是我们想要处理的事件![image-20240311150757501](image-20240311150757501.png)，我们就处理![image-20240311150946358](image-20240311150946358.png)。

## Render

在编辑器层中进行渲染逻辑的书写。

在Engine中封装OpenGL的信息

### OpenGL中将要使用的功能

- 各种全局配置
  ![image-20240312095705301](image-20240312095705301.png)
- 帧缓冲
- 渲染缓冲
- 着色器资源
- 纹理
- Shader数据传递

## Render逻辑

- 在初始化Application同时初始化Renderer![image-20240312145909530](image-20240312145909530.png)
- 在进入到Run中的渲染循环中，通过![image-20240312150006535](image-20240312150006535.png)
  进行渲染

### Renderer::Init

![image-20240312150059928](image-20240312150059928.png)
在此初始化上述二者

#### RenderCommand::Init()

![image-20240312150158540](image-20240312150158540.png)
初始化了API，s_RendererAPI实例OpenGL对象![image-20240312150250581](image-20240312150250581.png)
随后对OpenGL进行一个初始化，在![image-20240312150427072](image-20240312150427072.png)中
![image-20240312150406669](image-20240312150406669.png)

#### Renderer2D::Init()

在此首先初始化Renderer2D内部数据s_Data![image-20240312150944324](image-20240312150944324.png)。
![image-20240312151004963](image-20240312151004963.png)

##### 初始化数据并创建VAO

随后创建s_Data->QuadVertexArray![image-20240312151209572](image-20240312151209572.png)。
同样，在创建VertexArray时我们会根据API返回不同的对象
![image-20240312151340340](image-20240312151340340.png)
OpenGLVertexArray::OpenGLVertexArray()构造函数如下：
![image-20240312151420755](image-20240312151420755.png)
在此，我们创建了个VertexArray

##### 创建VBO

![image-20240312154521148](image-20240312154521148.png)
跟VAO相同，VBO依然时通过Create创建，并在此选择API去创建API对应的VBO
![image-20240312154622536](image-20240312154622536.png)
OpenGL下真正创建VBO，在![image-20240312154839082](image-20240312154839082.png)里实现
![image-20240312154828588](image-20240312154828588.png)

##### 绑定VAO&VBO

设置数据的布局
![image-20240312155611882](image-20240312155611882.png)
这里在进行创建时需要传递顶点数据![image-20240312154954024](image-20240312154954024.png)
接下来为绑定VAO，VBO![image-20240312155153221](image-20240312155153221.png)
具体实现如下
![image-20240312155430732](image-20240312155430732.png)

##### 创建IBO索引

![image-20240312160021655](image-20240312160021655.png)
与VAO，VBO相同，依然是先Create一个实例来
![image-20240312160213170](image-20240312160213170.png)
直接进入OpenGL实现方法中，方法如下：
![image-20240312160255001](image-20240312160255001.png)

##### 绑定IBO

![image-20240312160402915](image-20240312160402915.png)

##### 创建默认白色Texture

代码如下，具体OpenGL层面不在扩展
![image-20240312160633053](image-20240312160633053.png)

##### 绑定Shader

![image-20240312160651463](image-20240312160651463.png)
Shader创建
![image-20240312160731600](image-20240312160731600.png)
绑定
![image-20240312160822022](image-20240312160822022.png)
设置Shader数据
![image-20240312160902854](image-20240312160902854.png)

## 在Editor层中的渲染逻辑

在OnAttach该层时，首先会进行如下的初始化设置
![image-20240313213416153](image-20240313213416153.png)

1. 为m_CheckerboardTexture加载实际的纹理
2. 填充FramebufferSpecification信息
3. 创建Framebuffer

## FrameBuffer的封装

### 成员：

![image-20240313214909378](image-20240313214909378.png)



### FramebufferSpecification结构体

首先解析一下`FramebufferSpecification`这个结构体
![image-20240313214207635](image-20240313214207635.png)
以及内部`FramebufferAttachmentSpecification`结构体：
![image-20240313214253231](image-20240313214253231.png)

`FramebufferSpecification`记录了Framebuffer的大小，多重采样，以及附件的格式
附件格式则是由一个vector存储，内部结构体为`FramebufferTextureSpecification`：
![image-20240313214549153](image-20240313214549153.png)
而FramebufferTextureFormat 实际上是一个枚举类，内容如下：
![image-20240313214616604](image-20240313214616604.png)
上面就是所涉及到的附件格式

### 构造函数：

![image-20240313213843888](image-20240313213843888.png)

首先遍历附件格式，将其分为两种（颜色附件以及深度缓冲附件）
接下来调用Invalidate方法，该方法我们逐步分析

## Invalidate函数

### 清理已存在的framebuffer

![image-20240313215422620](image-20240313215422620.png)

当framebuffer存在时，首先释放这个framebuffer

### 创建新的Framebuffer

![image-20240313215524459](image-20240313215524459.png)

创建新的Framebuffer对象并标记一下是否开启多重采样

### 创建并绑定颜色附件的纹理

![image-20240313230924465](image-20240313230924465.png)

纹理创建实现如下：
![image-20240313231048987](image-20240313231048987.png)

接下来绑定纹理

![image-20240313231417020](image-20240313231417020.png)

![image-20240313231431312](image-20240313231431312.png)

随后根据类型的不同将纹理附加到framebuffer上

![image-20240313231551862](image-20240313231551862.png)
![image-20240313231609414](image-20240313231609414.png)

AttachColorTexture的实现如下：
![image-20240313231710707](image-20240313231710707.png)

### 创建并绑定深度缓冲

逻辑跟颜色附件相同

![image-20240313231831015](image-20240313231831015.png)

### 设置渲染目标

![image-20240313232151461](image-20240313232151461.png)

### 结束，绑定空的Framebuffer

![image-20240313232232268](image-20240313232232268.png)

## 老版OenGL顶点数据传输方式

![image-20240312152050020](image-20240312152050020.png)

## Editor渲染流程

### 更新相机

![image-20240313234424113](image-20240313234424113.png)





## OpenGL渲染最基本流程

- 设置glViewport
- 