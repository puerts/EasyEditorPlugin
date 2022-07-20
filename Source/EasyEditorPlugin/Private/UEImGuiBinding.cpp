#include "Binding.hpp"
#include "Object.hpp"
#include "UEDataBinding.hpp"
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#include <imgui.h>
#include <Services/ImguiGlobalContextService.h>
#include <Extension/SmallWidgets.h>

struct UEImGuiWrapped
{
	static int32 AddGlobalWindow(std::function<bool()> InOnDraw, UObject* WorldContextObject = nullptr)
	{
		return UEImGui::AddGlobalWindow(FDrawGlobalImgui::CreateLambda(
		    [InOnDraw]() -> bool
		    {
		    	return InOnDraw();
		    }), WorldContextObject);
	}

	static void RemoveGlobalWindow(int32 InIndex, UObject* WorldContextObject = nullptr)
	{
		UEImGui::RemoveGlobalWindow(InIndex, WorldContextObject);
	}
};

UsingNamedCppType(UEImGuiWrapped, UEImGui);
UsingUClass(UObject);

struct AutoRegisterForUEImGui
{
	AutoRegisterForUEImGui()
	{
		puerts::DefineClass<UEImGuiWrapped>()
		    .Function("AddGlobalWindow", MakeFunction(&UEImGuiWrapped::AddGlobalWindow, nullptr))
		    .Function("RemoveGlobalWindow", MakeFunction(&UEImGuiWrapped::RemoveGlobalWindow, nullptr))
		    .Function("DrawSmallWidgetDemo", MakeFunction(&ImGui::DrawSmallWidgetDemo))
			.Register();
	}
};

AutoRegisterForUEImGui __AutoRegisterForUEImGui_;
