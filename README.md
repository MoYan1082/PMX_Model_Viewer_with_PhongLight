## PMX_Model_Viewer_with_PhongLight

<img src="./images/Figure1.jpg" alt="Figure2" style="zoom:60%;" />

1. 使用`Blinn-Phong`光照模型渲染`.pmx`格式的模型。

2. 点光源阴影：`Shadow Map`、`PCF`。
3. 屏幕空间的环境光遮蔽：`SSAO`。

### :bulb: TODO

1. 修复天空盒无法显示的问题。
2. 增加PBR材质和IBL。

### :computer:开发环境

- OS：Win10。

- IDE：Visual studio 2022。
- 项目中使用到的库在`Dependencies`和`PMX_Files_Viewer\external`中。

### :hammer_and_wrench:如何使用

1. 使用Visual studio打开 `PMX_Files_Viewer.sln` 文件，并运行。

3. 如果出现`由于无法找到 assimp-vc143-mt.dll...`的提示，需要将`Dependencies\dll`下面的`assimp-vc143-mt.dll`复制到项目中的`Debug`或`Release`中（即和`PMX_Files_Viewer.exe`放在同一目录下）。

4. 如果`.pmx`模型或贴图路径中存在中文可能会报错，我感觉这是assimp库中的一个遗留问题 [assimp/issues/1612#](https://github.com/assimp/assimp/issues/1612#)。我自己的解决方案是修改系统的编码方式：

   ![Figure3](./images/Figure3.jpg)

5. 如果无法运行，可以根据自己的操作系统使用`cmake`重新编译库文件并引入项目，具体步骤可以参考：[LearnOpenGL](https://learnopengl.com/Getting-started/Creating-a-window)、[LearnOpenGL-CN](https://learnopengl-cn.github.io/01%20Getting%20started/02%20Creating%20a%20window/)。
