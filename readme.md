# dwlib使用说明 #

当前dwlib库暂时只支持rtthread(简称rtt)系统，整个库里面直接使用的是rtt操作系统系统资源，例如list内核链表、基础变量命名，底层读写操作使用的是rtt的IO设备驱动框架、finsh和msh组件。因此在其他平台使用dwlib修改代码量比较大，且不方便。

## 1. 添加库源码到rtt code中 ##
~~~
1. 我们首先在github将dwlib的全部工程代码打包下载。
2. 将dwlin库解压到rt-thread\components下，这样dwlib就成为了rtt的组件一部分。
3. 在你自己的工程中找到rtconfig.h在末尾添加RT_USING_DWIN和对应屏幕的宏。
   (目前支持RT_USING_DMT48270M043_02W好RT_USING_DMT48270M050_02W，其他也应该能支持，
   只是有些功能多或者少的问题，没有对应宏的屏幕可以尝试使用上面已经有的2个宏)。
4. 使用scons重新生成工程，这样dwlib的basic代码就添加到你的工程中了。
~~~

##  2. dwlib插件使用 ##
在dwlib库中有很多很好用的插件。目前支持以下插件：
1. 系统插件：查看屏幕配置和配置屏幕参数。
2. 登陆插件：实现1个超级密码和n个用户密码(动态内存和链表实现，只要内存够，就可以无限多的密码)，支持将迪文屏幕中的按键注册为登陆和退出按键。
3. 变量图标插件：支持修改迪文屏幕变量空间来改标屏幕中对于图标的图片。
4. 命令插件：支持将使用插件功能导出到rtt的finsh和msh中，使用超级终端进行调试。

以下是开启这些插件的宏开关：
~~~
1. 系统插件			RT_USING_DW_SYSTEM
2. 登陆插件			RT_USING_DW_LOGIN
3. 变量图标插件		RT_USING_DW_VICON
4. 命令插件			RT_USING_DW_CMD
~~~


## 3. dwlib调试宏 ## 
在开发阶段往往我们需要有打印函数调用状态的函数， 但是在发布时又不想有这些信息打印，所以我们添加了调试宏：
~~~
RT_USING_DW_DEBUG
~~~

