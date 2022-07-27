#ifdef UEIMGUI_API
#include "Binding.hpp"
#include "Object.hpp"
#include "UEDataBinding.hpp"
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#include <imgui.h>
#include <Services/ImguiGlobalContextService.h>
#include <Extension/SmallWidgets.h>
#include "EasyEditorDetailCustomization.h"
#include "ImguiWrap/ImguiUEWrap.h"

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

	static void AddDetailCustomization(UClass* InClass, std::function<void(UObject* InObject)> InOnDraw)
	{
		UEasyEditorDetailCustomization* CDO = Cast<UEasyEditorDetailCustomization>(UEasyEditorDetailCustomization::StaticClass()->GetDefaultObject(true));
		CDO->AddOnDetailDraw(InClass, InOnDraw);
	}

	static void RemoveDetailCustomization(UClass* InClass)
	{
		UEasyEditorDetailCustomization* CDO = Cast<UEasyEditorDetailCustomization>(UEasyEditorDetailCustomization::StaticClass()->GetDefaultObject(true));
		CDO->RemoveOnDetailDraw(InClass);
	}
};

UsingNamedCppType(UEImGuiWrapped, UEImGui);
UsingUClass(UObject);
UsingUClass(UClass);

struct AutoRegisterForUEImGui
{
	AutoRegisterForUEImGui()
	{
		puerts::DefineClass<UEImGuiWrapped>()
		    .Function("AddGlobalWindow", MakeFunction(&UEImGuiWrapped::AddGlobalWindow, nullptr))
		    .Function("RemoveGlobalWindow", MakeFunction(&UEImGuiWrapped::RemoveGlobalWindow, nullptr))
		    .Function("DrawSmallWidgetDemo", MakeFunction(&ImGui::DrawSmallWidgetDemo))
		    .Function("AddDetailCustomization", MakeFunction(&UEImGuiWrapped::AddDetailCustomization))
		    .Function("RemoveDetailCustomization", MakeFunction(&UEImGuiWrapped::RemoveDetailCustomization))
		    .Function("BeginDetail", MakeFunction(&ImGui::BeginDetail))
		    .Function("EndDetail", MakeFunction(&ImGui::EndDetail))
			.Register();
	}
};

AutoRegisterForUEImGui __AutoRegisterForUEImGui_;
#endif