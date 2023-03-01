# 项目【gi_messager】

开源项目：https://girakoo.com/  
联系方式：[contact@girakoo.com](mailto:contact@girakoo.com)  

## 需求简述

1. 在多线程环境中，为每个线程提供独立的消息队列 MessageLoop。注：主线程默认自动创建消息队列。
2. MessageLoopCenter 提供 MessageLoop 的查询功能。能够获得指定 MessageLoop 的句柄。
	- 同一个 MessageLoop 可以绑定多个模块 ID，便于多个模块共享同一个线程 MessageLoop。
3. MessageLoop 提供线程安全的 postMessage，getMessage，peekMessage 方法。
	- Message 结构中需要包含模块 ID，消息 ID，参数类型 MsgType，参数 MessageParam。
	- 同时匹配模块 ID，消息 ID 时，能够获得相应的消息分发通知。
	- MessageParam 默认提供基础类型 int, float, double, char, std::string，如果需要其他数据，需要自行实现 MsgParam 的方法。实现内存的创建，拷贝，释放等过程，保证数据线程安全和合理的内存释放。
4. 提供 ExhaustMessage 消息。阻塞式消息，将队列中全部消息分发完毕后，解除阻塞。
5. 消息分发提供计时功能，超过100ms未返回的回调处理，将会显示警告日志。

## 模块设计

- GiMessage：消息结构体。可以通过继承该类，实现自定义类型的消息分发。
- GiMessageObserver：消息观察者。可以接受到符合自身ID的消息。可以接受GiMessageLoop的状态通知。
- GiMessageLoop：消息循环主题。

## 代码范例

```c++
// TODO
```

## 计划中

-

## 代码托管

https://gitee.com/girakoo/gi_messager

## SAST Tools

[PVS-Studio](https://pvs-studio.com/en/pvs-studio/?utm_source=github&utm_medium=organic&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.