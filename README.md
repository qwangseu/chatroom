# chatroom

* 一个简单的 socket-server lib
    *  封装socket 的使用。
    *  该库不打算上升实现到 封包，解包，粘包等 层次。
    *  解决 c10k-problem。

*  思路（参考网上一些文章和某些前辈的源码）
    *  使用io多路复用技术实现（select ， epoll 结合使用）。


# simple benchmark

* 测试
    * 笔记本一台：i3（2核4线程） ，linux-64-bit ， 带宽（本地局域网）。
    * 模拟场景：one pub/sub
    * 测试结果：online-socket 达 30k，all users 都可以在 0.7s内响应。
