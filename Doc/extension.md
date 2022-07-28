# 第三方库调用

主要是两步，以[UEImgui](https://github.com/ZhuRong-HomoStation/UEImgui)为例说明。

这里的做法是新建了另外一个plugin对UEImgui进行封装。该新建的plugin是[EasyEditor_ImGui](https://github.com/puerts/EasyEditor_ImGui)。

## C++ api绑定声明

puerts对于UE能反射调用的api可以直接使用（标注了UCLASS，UPPROPERTY，UFUNCTION，USTRUCT，UENUM的C++类，以及所有的蓝图）。这已经能满足不少的编辑器扩展需求。

但UEImgui还有大量的普通C/C++ api，这时可以用puerts模板绑定功能，对需要在ts中调用的api用puerts的声明语法简单声明一下即可。

对UEImgui的绑定声明在这两个文件：[DearImGuiBinding.cpp](https://github.com/puerts/EasyEditor_ImGui/blob/master/Source/EasyEditor_ImGui/Private/DearImGuiBinding.cpp)、[UEImGuiBinding.cpp](https://github.com/puerts/EasyEditor_ImGui/blob/master/Source/EasyEditor_ImGui/Private/UEImGuiBinding.cpp)

## 虚拟机关闭清理

如果你有些工作需要在虚拟机关闭前清理一下，可以注册到FEasyEditorPluginModule::OnJsEnvCleanup

比如EasyEditor_ImGui就注册了清理回调，见[EasyEditor_ImGui.cpp](https://github.com/puerts/EasyEditor_ImGui/blob/master/Source/EasyEditor_ImGui/Private/EasyEditor_ImGui.cpp)

~~~c++
FModuleManager::LoadModuleChecked<FEasyEditorPluginModule>("EasyEditorPlugin").OnJsEnvCleanup.AddLambda([]()
{
    UEasyEditorDetailCustomization* CDO = Cast<UEasyEditorDetailCustomization>(UEasyEditorDetailCustomization::StaticClass()->GetDefaultObject(false));
    if (CDO)
    {
        CDO->ClearOnDetailDraw();
    }
});
~~~

