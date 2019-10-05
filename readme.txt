fornjust branch
main函数中K0键按下 驱动器由当前位置运动到绝对位置 NEWZEROPOSITION，然后将新位置置为0，即重新设定0位
K1键按下，驱动器从当前位置运动到0位
KWU键按下，驱动器执行插值模式

NEWZEROPOSITION 的数据在epos2_thread.c文件中可修改
插值模式数据在 appdata.c appdata.h文件中可修改
可能使用硬件差别，按键IO可在bsp_key.c文件中修改(注意高低电平表示按下的选择)