# network_project
这是我们ACM班计算机网络课程设计程序

分的四个组，DLL大概要实现的是协议的类，然后调用获得html（js目前管不了），然后显示出来
所以DLL的筒子们加油咯 @Ails

每个组都创建一个属于自己小组的分支
命令举例 `checkout -b group_windows`
先每个组完成自己的任务，等确保自己组内的程序没问题后，我们再把各个分支合并到master上。

由于时间紧迫，乃们UI又不给我，DLL也不给我，我只好做了个暴力的browser，又丑又直接

留言几个问题
1、VS版本问题和架构问题
2、整个项目赶快合并起来，直接在上面该就可以了，记得加注释你在哪里改了，不加注释就不要改



----------

上次课上对不住大家了，一起加油吧

大家可以在该项目中测试自己的代码，有问题参考示例或联系（李贤亮）
注意：工程默认为Unicode编码涉及字符串处理的请注意>_<#已疯,哪位童鞋对编码了解的请告知O(∩_∩)O
1.常量字符串 使用_T()宏
   例：平时习惯 char s[]="abc"; 此处为TCHAR s[] =_T("abc");
2.定义字符串类型为TCHAR型
3.字符串函数itoa,atoi等等啥的使用对应的兼容版本(TCHAR.H中定义的)
  例：不使用atoi，使用_ttoi，大多数函数名都是加一个_，把a(ASCII)改为t(TCHAR),查MSDN

五层协议分别封装在五个类中，暂时不用考虑（T_T）
APPlayer
Trslayer
Netlayer
Linklayer
Phylayer

动态链接库封装在Dll类
触发加载*.Dll函数为	Dll::OnBnClickedLoad()中，
触发使用*Dll导出函数的函数为Dll::OnBnClickedButton1()

获取html文本数据封装在Gethtml类
类中有一个浏览器控件myweb
获取网页数据显示网页函数为 GetHtml::getURLContext(url);
//PS:程序访问外网且数据较大时会卡顿无响应，大神多线程来解救吧⊙﹏⊙!
//浏览器控件还是直接根据url获取的网页，没找到根据CHttpFile获取网页的方法
22222
