# 网络协议项目
> 该项目实现了应用层的HTTP协议，传输层的FTP，UDP协议，以及网络层协议和链路层协议。
该项目配置方法如下

## 1. 配置winpcap环境
### a. 下载winpcap开发包
下载地址请[点击这里](http://www.winpcap.org/install/bin/WpdPack_4_1_2.zip)。
下载好以后解压缩放到C盘目录下，名称定义为`WpdPack`即可。
### b. 配置vs
进入VS后进行如下操作
1. 点击 项目  ---> 属性 
2. 选择c/c++--->:常规--->附加包含目录，加入 **C:\WpdPack\Include**
3. 选择c/c++--->预处理器--->预处理器定义，加入
```	
WIN32
_DEBUG
_CONSOLE
_LIB
HAVE_REMOTE
```
4. 选择链接器--->常规--->附加库目录，加入`C:\WpdPack\Lib`
5. 选择链接器--->输入--->附加依赖项，加入
```
wpcap.lib;packet.lib;ws2_32.lib
```
> 这样winpcap的环境就配置好了。
