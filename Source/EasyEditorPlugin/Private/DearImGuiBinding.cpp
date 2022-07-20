#include "Binding.hpp"
#include "Object.hpp"
#include <string>
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#include <imgui.h>

#define MakeFunctionWithScriptTypePtr(M, ...)                                                   \
    [](::puerts::CallbackInfoType info)                                                                          \
    { ::puerts::FuncCallWrapper<decltype(M), M, false, false>::callWithDefaultValues(info, ##__VA_ARGS__); },    \
        ::puerts::FuncCallWrapper<decltype(M), M, false, false>::info(puerts::Count(__VA_ARGS__))

#define MakeFunction_PtrRet(M, ...)                                                    \
    [](::puerts::CallbackInfoType info)                                                             \
    { ::puerts::FuncCallWrapper<decltype(M), M, true>::callWithDefaultValues(info, ##__VA_ARGS__); }, \
        ::puerts::FuncCallWrapper<decltype(M), M, true>::info(puerts::Count(__VA_ARGS__))

#define MakeOverloadWithScriptTypePtr(SIGNATURE, M) puerts::FuncCallWrapper<SIGNATURE, M, false, false>

struct DearImGui
{
};

UsingNamedCppType(DearImGui, ImGui);
UsingCppType(ImGuiContext);
UsingCppType(ImFontAtlas);
UsingCppType(ImGuiIO);
UsingCppType(ImGuiStyle);
UsingCppType(ImDrawData);
UsingCppType(ImVec2);
UsingCppType(ImDrawList);
UsingCppType(ImFont);
UsingCppType(ImVec4);
UsingCppType(ImGuiPayload);
UsingCppType(ImDrawListSharedData);
UsingCppType(ImGuiStorage);
UsingCppType(ImFontConfig);
UsingCppType(ImFontGlyph);
UsingCppType(ImDrawListSplitter);
UsingCppType(ImDrawVert);
UsingCppType(ImGuiInputTextCallbackData);

namespace ImGuiWrapped
{
    static void Text(const std::string& info)
    {
        const char* text = info.c_str();
        ImGui::TextUnformatted(text, text + info.size());
    }

    static void TextColored(const ImVec4& col, const std::string& info)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        Text(info);
        ImGui::PopStyleColor();
    }

    static void TextDisabled(const std::string& info)
    {
        ImGui::TextDisabled("%s", info.c_str());
    }

    static void TextWrapped(const std::string& info)
    {
        ImGui::TextWrapped("%s", info.c_str());
    }

    
    static void LabelText(const char* label, const std::string& info)
    {
        ImGui::TextWrapped(label, "%s", info.c_str());
    }
    
    static void BulletText(const std::string& info)
    {
        ImGui::BulletText("%s", info.c_str());
    }
    static bool TreeNode(const char* str_id, const std::string& info)
    {
        return ImGui::TreeNode(str_id, "%s", info.c_str());
    }
    static bool TreeNode(const void* ptr_id, const std::string& info)
    {
        return ImGui::TreeNode(ptr_id, "%s", info.c_str());
    }
    static bool TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const std::string& info)
    {
        return ImGui::TreeNodeEx(str_id, flags, "%s", info.c_str());
    }
    static bool TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const std::string& info)
    {
        return ImGui::TreeNodeEx(ptr_id, flags, "%s", info.c_str());
    }
    static void SetTooltip(const std::string& info)
    {
        ImGui::SetTooltip("%s", info.c_str());
    }
    static void LogText(const std::string& info)
    {
        ImGui::LogText("%s", info.c_str());
    }

    static bool ListBox(const char* label, int* current_item, void* items, int items_count, int height_in_items)
    {
        return ImGui::ListBox(label, current_item, (const char* const *)items, items_count, height_in_items);
    }

    struct InputTextCallback_UserData
    {
        std::string* Str;
        std::function<int(ImGuiInputTextCallbackData *data)> Callback;
    };

    static int InputTextCallback(ImGuiInputTextCallbackData* data)
    {
        InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            std::string* str = user_data->Str;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->c_str();
        }
        else if (user_data->Callback)
        {
            return user_data->Callback(data);
        }
        return 0;
    }
    
    bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, std::function<int(ImGuiInputTextCallbackData *data)> callback = nullptr)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.Callback = callback;
        return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, std::function<int(ImGuiInputTextCallbackData *data)> callback = nullptr)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.Callback = callback;
        return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
    }

    bool InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, std::function<int(ImGuiInputTextCallbackData *data)> callback = nullptr)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.Callback = callback;
        return ImGui::InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }
}

// overload with default values
DeclOverloads(ImGui_Selectable);
DeclOverload(ImGui_Selectable, bool (*)(const char*, bool, ImGuiSelectableFlags, const ImVec2&), &ImGui::Selectable, false, 0, ImVec2(0,0));
DeclOverload(ImGui_Selectable, bool (*)(const char*, bool*, ImGuiSelectableFlags, const ImVec2&), &ImGui::Selectable, 0, ImVec2(0,0));

DeclOverloads(ImDrawList_AddText);
DeclOverload(ImDrawList_AddText, void(ImDrawList::*)(const ImVec2&, ImU32, const char*, const char*), &ImDrawList::AddText, nullptr);
DeclOverload(ImDrawList_AddText, void(ImDrawList::*)(const ImFont*, float, const ImVec2&, ImU32, const char*, const char*, float, const ImVec4*), &ImDrawList::AddText, nullptr, 0.0f, nullptr);

struct AutoRegisterForDearImGui
{
    AutoRegisterForDearImGui()
    {
        puerts::DefineClass<ImGuiContext>()
            .Register(nullptr); // 避免这类错误 type_traits(731): [C2139] “ImDrawListSharedData”: 未定义的类不允许作为编译器内部类型特征“__is_destructible”的参数

        puerts::DefineClass<ImDrawListSharedData>()
            .Register(nullptr);

        puerts::DefineClass<ImFontConfig>()
            .Register(nullptr);

        puerts::DefineClass<ImFontGlyph>()
            .Register(nullptr);

        puerts::DefineClass<ImDrawListSplitter>()
            .Register(nullptr);

        puerts::DefineClass<ImDrawVert>()
            .Register(nullptr);

        puerts::DefineClass<ImGuiInputTextCallbackData>()
            .Property("EventFlag", MakeProperty(&ImGuiInputTextCallbackData::EventFlag))
            .Property("Flags", MakeProperty(&ImGuiInputTextCallbackData::Flags))
            .Property("EventChar", MakeProperty(&ImGuiInputTextCallbackData::EventChar))
            .Property("EventKey", MakeProperty(&ImGuiInputTextCallbackData::EventKey))
            .Property("Buf", MakeProperty(&ImGuiInputTextCallbackData::Buf))
            .Property("BufTextLen", MakeProperty(&ImGuiInputTextCallbackData::BufTextLen))
            .Property("BufSize", MakeProperty(&ImGuiInputTextCallbackData::BufSize))
            .Property("BufDirty", MakeProperty(&ImGuiInputTextCallbackData::BufDirty))
            .Property("CursorPos", MakeProperty(&ImGuiInputTextCallbackData::CursorPos))
            .Property("SelectionStart", MakeProperty(&ImGuiInputTextCallbackData::SelectionStart))
            .Property("SelectionEnd", MakeProperty(&ImGuiInputTextCallbackData::SelectionEnd))
            .Register();
        
    	puerts::DefineClass<ImFontAtlas>()
            .Property("Locked", MakeProperty(&ImFontAtlas::Locked))
            .Property("Flags", MakeProperty(&ImFontAtlas::Flags))
            .Property("TexID", MakeProperty(&ImFontAtlas::TexID))
            .Property("TexDesiredWidth", MakeProperty(&ImFontAtlas::TexDesiredWidth))
            .Property("TexGlyphPadding", MakeProperty(&ImFontAtlas::TexGlyphPadding))
            .Property("TexPixelsAlpha8", MakeProperty(&ImFontAtlas::TexPixelsAlpha8))
            .Property("TexPixelsRGBA32", MakeProperty(&ImFontAtlas::TexPixelsRGBA32))
            .Property("TexWidth", MakeProperty(&ImFontAtlas::TexWidth))
            .Property("TexHeight", MakeProperty(&ImFontAtlas::TexHeight))
            .Property("TexUvScale", MakeProperty(&ImFontAtlas::TexUvScale))
            .Property("TexUvWhitePixel", MakeProperty(&ImFontAtlas::TexUvWhitePixel))
            //.Property("Fonts", MakeProperty(&ImFontAtlas::Fonts))  // ImVector
            //.Property("CustomRects", MakeProperty(&ImFontAtlas::CustomRects))  // ImVector
            //.Property("ConfigData", MakeProperty(&ImFontAtlas::ConfigData)) // ImVector
            //.Property("CustomRectIds", MakeProperty(&ImFontAtlas::CustomRectIds)) //1.81没有
            .Register();

        puerts::DefineClass<ImGuiIO>()
            .Property("ConfigFlags", MakeProperty(&ImGuiIO::ConfigFlags))
            .Property("BackendFlags", MakeProperty(&ImGuiIO::BackendFlags))
            .Property("DisplaySize", MakeProperty(&ImGuiIO::DisplaySize))
            .Property("DeltaTime", MakeProperty(&ImGuiIO::DeltaTime))
            .Property("IniSavingRate", MakeProperty(&ImGuiIO::IniSavingRate))
            .Property("IniFilename", MakeReadonlyProperty(&ImGuiIO::IniFilename))
            .Property("LogFilename", MakeReadonlyProperty(&ImGuiIO::LogFilename))
            .Property("MouseDoubleClickTime", MakeProperty(&ImGuiIO::MouseDoubleClickTime))
            .Property("MouseDoubleClickMaxDist", MakeProperty(&ImGuiIO::MouseDoubleClickMaxDist))
            .Property("MouseDragThreshold", MakeProperty(&ImGuiIO::MouseDragThreshold))
            .Property("KeyMap", MakeProperty(&ImGuiIO::KeyMap))
            .Property("KeyRepeatDelay", MakeProperty(&ImGuiIO::KeyRepeatDelay))
            .Property("KeyRepeatRate", MakeProperty(&ImGuiIO::KeyRepeatRate))
            .Property("UserData", MakeProperty(&ImGuiIO::UserData))
            .Property("Fonts", MakeProperty(&ImGuiIO::Fonts))
            .Property("FontGlobalScale", MakeProperty(&ImGuiIO::FontGlobalScale))
            .Property("FontAllowUserScaling", MakeProperty(&ImGuiIO::FontAllowUserScaling))
            .Property("FontDefault", MakeProperty(&ImGuiIO::FontDefault))
            .Property("DisplayFramebufferScale", MakeProperty(&ImGuiIO::DisplayFramebufferScale))
            .Property("MouseDrawCursor", MakeProperty(&ImGuiIO::MouseDrawCursor))
            .Property("ConfigMacOSXBehaviors", MakeProperty(&ImGuiIO::ConfigMacOSXBehaviors))
            .Property("ConfigInputTextCursorBlink", MakeProperty(&ImGuiIO::ConfigInputTextCursorBlink))
            .Property("ConfigWindowsResizeFromEdges", MakeProperty(&ImGuiIO::ConfigWindowsResizeFromEdges))
            .Property("ConfigWindowsMoveFromTitleBarOnly", MakeProperty(&ImGuiIO::ConfigWindowsMoveFromTitleBarOnly))
            //.Property("ConfigWindowsMemoryCompactTimer", MakeProperty(&ImGuiIO::ConfigWindowsMemoryCompactTimer)) //1.81没有
            .Property("BackendPlatformName", MakeReadonlyProperty(&ImGuiIO::BackendPlatformName))
            .Property("BackendRendererName", MakeReadonlyProperty(&ImGuiIO::BackendRendererName))
            .Property("BackendPlatformUserData", MakeProperty(&ImGuiIO::BackendPlatformUserData))
            .Property("BackendRendererUserData", MakeProperty(&ImGuiIO::BackendRendererUserData))
            .Property("BackendLanguageUserData", MakeProperty(&ImGuiIO::BackendLanguageUserData))
            .Property("ClipboardUserData", MakeProperty(&ImGuiIO::ClipboardUserData))
            //.Property("ImeWindowHandle", MakeProperty(&ImGuiIO::ImeWindowHandle)) //1.81没有
            .Property("MousePos", MakeProperty(&ImGuiIO::MousePos))
            .Property("MouseDown", MakeProperty(&ImGuiIO::MouseDown))
            .Property("MouseWheel", MakeProperty(&ImGuiIO::MouseWheel))
            .Property("MouseWheelH", MakeProperty(&ImGuiIO::MouseWheelH))
            .Property("KeyCtrl", MakeProperty(&ImGuiIO::KeyCtrl))
            .Property("KeyShift", MakeProperty(&ImGuiIO::KeyShift))
            .Property("KeyAlt", MakeProperty(&ImGuiIO::KeyAlt))
            .Property("KeySuper", MakeProperty(&ImGuiIO::KeySuper))
            .Property("KeysDown", MakeProperty(&ImGuiIO::KeysDown))
            .Property("NavInputs", MakeProperty(&ImGuiIO::NavInputs))
            .Property("WantCaptureMouse", MakeProperty(&ImGuiIO::WantCaptureMouse))
            .Property("WantCaptureKeyboard", MakeProperty(&ImGuiIO::WantCaptureKeyboard))
            .Property("WantTextInput", MakeProperty(&ImGuiIO::WantTextInput))
            .Property("WantSetMousePos", MakeProperty(&ImGuiIO::WantSetMousePos))
            .Property("WantSaveIniSettings", MakeProperty(&ImGuiIO::WantSaveIniSettings))
            .Property("NavActive", MakeProperty(&ImGuiIO::NavActive))
            .Property("NavVisible", MakeProperty(&ImGuiIO::NavVisible))
            .Property("Framerate", MakeProperty(&ImGuiIO::Framerate))
            .Property("MetricsRenderVertices", MakeProperty(&ImGuiIO::MetricsRenderVertices))
            .Property("MetricsRenderIndices", MakeProperty(&ImGuiIO::MetricsRenderIndices))
            .Property("MetricsRenderWindows", MakeProperty(&ImGuiIO::MetricsRenderWindows))
            .Property("MetricsActiveWindows", MakeProperty(&ImGuiIO::MetricsActiveWindows))
            .Property("MetricsActiveAllocations", MakeProperty(&ImGuiIO::MetricsActiveAllocations))
            .Property("MouseDelta", MakeProperty(&ImGuiIO::MouseDelta))
            .Property("MousePosPrev", MakeProperty(&ImGuiIO::MousePosPrev))
            //.Property("MouseClickedPos", MakeProperty(&ImGuiIO::MouseClickedPos)) //ImVec2 MouseClickedPos[5];
            .Property("MouseClickedTime", MakeProperty(&ImGuiIO::MouseClickedTime))
            .Property("MouseClicked", MakeProperty(&ImGuiIO::MouseClicked))
            .Property("MouseDoubleClicked", MakeProperty(&ImGuiIO::MouseDoubleClicked))
            .Property("MouseReleased", MakeProperty(&ImGuiIO::MouseReleased))
            .Property("MouseDownOwned", MakeProperty(&ImGuiIO::MouseDownOwned))
            .Property("MouseDownWasDoubleClick", MakeProperty(&ImGuiIO::MouseDownWasDoubleClick))
            .Property("MouseDownDuration", MakeProperty(&ImGuiIO::MouseDownDuration))
            .Property("MouseDownDurationPrev", MakeProperty(&ImGuiIO::MouseDownDurationPrev))
            //.Property("MouseDragMaxDistanceAbs", MakeProperty(&ImGuiIO::MouseDragMaxDistanceAbs)) //ImVec2 MouseDragMaxDistanceAbs[5]
            .Property("MouseDragMaxDistanceSqr", MakeProperty(&ImGuiIO::MouseDragMaxDistanceSqr))
            .Property("KeysDownDuration", MakeProperty(&ImGuiIO::KeysDownDuration))
            .Property("KeysDownDurationPrev", MakeProperty(&ImGuiIO::KeysDownDurationPrev))
            .Property("NavInputsDownDuration", MakeProperty(&ImGuiIO::NavInputsDownDuration))
            .Property("NavInputsDownDurationPrev", MakeProperty(&ImGuiIO::NavInputsDownDurationPrev))
            //.Property("InputQueueCharacters", MakeProperty(&ImGuiIO::InputQueueCharacters)) // ImVector
            .Register();

        puerts::DefineClass<ImGuiStyle>()
            .Property("Alpha", MakeProperty(&ImGuiStyle::Alpha))
            .Property("WindowPadding", MakeProperty(&ImGuiStyle::WindowPadding))
            .Property("WindowRounding", MakeProperty(&ImGuiStyle::WindowRounding))
            .Property("WindowBorderSize", MakeProperty(&ImGuiStyle::WindowBorderSize))
            .Property("WindowMinSize", MakeProperty(&ImGuiStyle::WindowMinSize))
            .Property("WindowTitleAlign", MakeProperty(&ImGuiStyle::WindowTitleAlign))
            .Property("WindowMenuButtonPosition", MakeProperty(&ImGuiStyle::WindowMenuButtonPosition))
            .Property("ChildRounding", MakeProperty(&ImGuiStyle::ChildRounding))
            .Property("ChildBorderSize", MakeProperty(&ImGuiStyle::ChildBorderSize))
            .Property("PopupRounding", MakeProperty(&ImGuiStyle::PopupRounding))
            .Property("PopupBorderSize", MakeProperty(&ImGuiStyle::PopupBorderSize))
            .Property("FramePadding", MakeProperty(&ImGuiStyle::FramePadding))
            .Property("FrameRounding", MakeProperty(&ImGuiStyle::FrameRounding))
            .Property("FrameBorderSize", MakeProperty(&ImGuiStyle::FrameBorderSize))
            .Property("ItemSpacing", MakeProperty(&ImGuiStyle::ItemSpacing))
            .Property("ItemInnerSpacing", MakeProperty(&ImGuiStyle::ItemInnerSpacing))
            .Property("TouchExtraPadding", MakeProperty(&ImGuiStyle::TouchExtraPadding))
            .Property("IndentSpacing", MakeProperty(&ImGuiStyle::IndentSpacing))
            .Property("ColumnsMinSpacing", MakeProperty(&ImGuiStyle::ColumnsMinSpacing))
            .Property("ScrollbarSize", MakeProperty(&ImGuiStyle::ScrollbarSize))
            .Property("ScrollbarRounding", MakeProperty(&ImGuiStyle::ScrollbarRounding))
            .Property("GrabMinSize", MakeProperty(&ImGuiStyle::GrabMinSize))
            .Property("GrabRounding", MakeProperty(&ImGuiStyle::GrabRounding))
            .Property("TabRounding", MakeProperty(&ImGuiStyle::TabRounding))
            .Property("TabBorderSize", MakeProperty(&ImGuiStyle::TabBorderSize))
            .Property("ColorButtonPosition", MakeProperty(&ImGuiStyle::ColorButtonPosition))
            .Property("ButtonTextAlign", MakeProperty(&ImGuiStyle::ButtonTextAlign))
            .Property("SelectableTextAlign", MakeProperty(&ImGuiStyle::SelectableTextAlign))
            .Property("DisplayWindowPadding", MakeProperty(&ImGuiStyle::DisplayWindowPadding))
            .Property("DisplaySafeAreaPadding", MakeProperty(&ImGuiStyle::DisplaySafeAreaPadding))
            .Property("MouseCursorScale", MakeProperty(&ImGuiStyle::MouseCursorScale))
            .Property("AntiAliasedLines", MakeProperty(&ImGuiStyle::AntiAliasedLines))
            .Property("AntiAliasedFill", MakeProperty(&ImGuiStyle::AntiAliasedFill))
            .Property("CurveTessellationTol", MakeProperty(&ImGuiStyle::CurveTessellationTol))
            //.Property("Colors", MakeProperty(&ImGuiStyle::Colors)) //ImVec4      Colors[ImGuiCol_COUNT]
            .Register();

        puerts::DefineClass<ImDrawData>()
            .Constructor()
            .Property("Valid", MakeProperty(&ImDrawData::Valid))
            //.Property("CmdLists", MakeProperty(&ImDrawData::CmdLists)) //ImDrawList**    CmdLists;
            .Property("CmdListsCount", MakeProperty(&ImDrawData::CmdListsCount))
            .Property("TotalIdxCount", MakeProperty(&ImDrawData::TotalIdxCount))
            .Property("TotalVtxCount", MakeProperty(&ImDrawData::TotalVtxCount))
            .Property("DisplayPos", MakeProperty(&ImDrawData::DisplayPos))
            .Property("DisplaySize", MakeProperty(&ImDrawData::DisplaySize))
            .Property("FramebufferScale", MakeProperty(&ImDrawData::FramebufferScale))
            .Register();

        puerts::DefineClass<ImVec2>()
            .Constructor()
            .Constructor<float, float>()
            .Property("x", MakeProperty(&ImVec2::x))
            .Property("y", MakeProperty(&ImVec2::y))
            .Register();

        puerts::DefineClass<ImDrawList>()
            //.Constructor<ImDrawListSharedData>()
            //.Property("CmdBuffer", MakeProperty(&ImDrawList::CmdBuffer)) //ImVector
            //.Property("IdxBuffer", MakeProperty(&ImDrawList::IdxBuffer)) //ImVector
            //.Property("VtxBuffer", MakeProperty(&ImDrawList::VtxBuffer)) //ImVector
            .Property("Flags", MakeProperty(&ImDrawList::Flags))
            //.Property("_Data", MakeProperty(&ImDrawList::_Data))
            .Property("_OwnerName", MakeReadonlyProperty(&ImDrawList::_OwnerName))
            //.Property("_VtxCurrentOffset", MakeProperty(&ImDrawList::_VtxCurrentOffset)) //1.81没有
            .Property("_VtxCurrentIdx", MakeProperty(&ImDrawList::_VtxCurrentIdx))
            .Property("_VtxWritePtr", MakeProperty(&ImDrawList::_VtxWritePtr))
            .Property("_IdxWritePtr", MakeProperty(&ImDrawList::_IdxWritePtr))
            //.Property("_ClipRectStack", MakeProperty(&ImDrawList::_ClipRectStack)) //ImVector
            //.Property("_TextureIdStack", MakeProperty(&ImDrawList::_TextureIdStack)) //ImVector
            //.Property("_Path", MakeProperty(&ImDrawList::_Path)) //ImVector
            .Property("_Splitter", MakeProperty(&ImDrawList::_Splitter))
            .Method("AddText", CombineOverloads(
                SelectOverload(ImDrawList_AddText, void(ImDrawList::*)(const ImVec2&, ImU32, const char*, const char*)),
                SelectOverload(ImDrawList_AddText, void(ImDrawList::*)(const ImFont*, float, const ImVec2&, ImU32, const char*, const char*, float, const ImVec4*))
                ))
            .Register();

        puerts::DefineClass<ImFont>()
            //.Property("IndexAdvanceX", MakeProperty(&ImFont::IndexAdvanceX)) //ImVector
            .Property("FallbackAdvanceX", MakeProperty(&ImFont::FallbackAdvanceX))
            .Property("FontSize", MakeProperty(&ImFont::FontSize))
            //.Property("IndexLookup", MakeProperty(&ImFont::IndexLookup)) //ImVector
            //.Property("Glyphs", MakeProperty(&ImFont::Glyphs)) //ImVector
            .Property("FallbackGlyph", MakeProperty(&ImFont::FallbackGlyph))
            //.Property("DisplayOffset", MakeProperty(&ImFont::DisplayOffset)) //1.81没有
            .Property("ContainerAtlas", MakeProperty(&ImFont::ContainerAtlas))
            .Property("ConfigData", MakeProperty(&ImFont::ConfigData))
            .Property("ConfigDataCount", MakeProperty(&ImFont::ConfigDataCount))
            .Property("FallbackChar", MakeProperty(&ImFont::FallbackChar))
            .Property("EllipsisChar", MakeProperty(&ImFont::EllipsisChar))
            .Property("Scale", MakeProperty(&ImFont::Scale))
            .Property("Ascent", MakeProperty(&ImFont::Ascent))
            .Property("Descent", MakeProperty(&ImFont::Descent))
            .Property("MetricsTotalSurface", MakeProperty(&ImFont::MetricsTotalSurface))
            .Property("DirtyLookupTables", MakeProperty(&ImFont::DirtyLookupTables))
            .Register();

        puerts::DefineClass<ImVec4>()
            .Constructor()
            .Constructor<float, float, float, float>()
            .Property("x", MakeProperty(&ImVec4::x))
            .Property("y", MakeProperty(&ImVec4::y))
            .Property("z", MakeProperty(&ImVec4::z))
            .Property("w", MakeProperty(&ImVec4::w))
            .Register();

        puerts::DefineClass<ImGuiPayload>()
            .Constructor()
            .Property("Data", MakeProperty(&ImGuiPayload::Data))
            .Property("DataSize", MakeProperty(&ImGuiPayload::DataSize))
            .Property("SourceId", MakeProperty(&ImGuiPayload::SourceId))
            .Property("SourceParentId", MakeProperty(&ImGuiPayload::SourceParentId))
            .Property("DataFrameCount", MakeProperty(&ImGuiPayload::DataFrameCount))
            .Property("DataType", MakeProperty(&ImGuiPayload::DataType))
            .Property("Preview", MakeProperty(&ImGuiPayload::Preview))
            .Property("Delivery", MakeProperty(&ImGuiPayload::Delivery))
            .Register();

        puerts::DefineClass<ImGuiStorage>()
            //.Property("Data", MakeProperty(&ImGuiStorage::Data)) //ImVector
            .Register();
    	
        puerts::DefineClass<DearImGui>()
            .Function("Text", MakeFunction(&ImGuiWrapped::Text))
            .Function("TextColored", MakeFunction(&ImGuiWrapped::TextColored))
            .Function("TextDisabled", MakeFunction(&ImGuiWrapped::TextDisabled))
            .Function("TextWrapped", MakeFunction(&ImGuiWrapped::TextWrapped))
            .Function("LabelText", MakeFunction(&ImGuiWrapped::LabelText))
            .Function("BulletText", MakeFunction(&ImGuiWrapped::BulletText))
            .Function("TreeNode", CombineOverloads(
                    MakeOverload(bool (*)(const char*, const std::string&), &ImGuiWrapped::TreeNode),
                    MakeOverload(bool (*)(const void*, const std::string&), &ImGuiWrapped::TreeNode)
                    ))
            .Function("TreeNodeEx", CombineOverloads(
                    MakeOverload(bool (*)(const char*, ImGuiTreeNodeFlags, const std::string&), &ImGuiWrapped::TreeNodeEx),
                    MakeOverload(bool (*)(const void*, ImGuiTreeNodeFlags, const std::string&), &ImGuiWrapped::TreeNodeEx)
                    ))
            .Function("SetTooltip", MakeFunction(&ImGuiWrapped::SetTooltip))
            .Function("LogText", MakeFunction(&ImGuiWrapped::LogText))
    	    .Function("CreateContext", MakeFunction(&ImGui::CreateContext, nullptr))
            .Function("DestroyContext", MakeFunction(&ImGui::DestroyContext, nullptr))
            .Function("GetCurrentContext", MakeFunction(&ImGui::GetCurrentContext))
            .Function("SetCurrentContext", MakeFunction(&ImGui::SetCurrentContext))
            .Function("DebugCheckVersionAndDataLayout", MakeFunction(&ImGui::DebugCheckVersionAndDataLayout))
            .Function("GetIO", MakeFunction_PtrRet(&ImGui::GetIO))
            .Function("GetStyle", MakeFunction(&ImGui::GetStyle))
            .Function("NewFrame", MakeFunction(&ImGui::NewFrame))
            .Function("EndFrame", MakeFunction(&ImGui::EndFrame))
            .Function("Render", MakeFunction(&ImGui::Render))
            .Function("GetDrawData", MakeFunction(&ImGui::GetDrawData))
            .Function("ShowDemoWindow", MakeFunction(&ImGui::ShowDemoWindow, nullptr))
            .Function("ShowAboutWindow", MakeFunction(&ImGui::ShowAboutWindow, nullptr))
            .Function("ShowMetricsWindow", MakeFunction(&ImGui::ShowMetricsWindow, nullptr))
            .Function("ShowStyleEditor", MakeFunction(&ImGui::ShowStyleEditor, nullptr))
            .Function("ShowStyleSelector", MakeFunction(&ImGui::ShowStyleSelector))
            .Function("ShowFontSelector", MakeFunction(&ImGui::ShowFontSelector))
            .Function("ShowUserGuide", MakeFunction(&ImGui::ShowUserGuide))
            .Function("GetVersion", MakeFunction(&ImGui::GetVersion))
            .Function("StyleColorsDark", MakeFunction(&ImGui::StyleColorsDark, nullptr))
            .Function("StyleColorsClassic", MakeFunction(&ImGui::StyleColorsClassic, nullptr))
            .Function("StyleColorsLight", MakeFunction(&ImGui::StyleColorsLight, nullptr))
            .Function("Begin", MakeFunction(&ImGui::Begin, nullptr, 0))
            .Function("End", MakeFunction(&ImGui::End))
            .Function("BeginChild", CombineOverloads( // TODO: default value with overload
                MakeOverload(bool (*)(const char*, const ImVec2&, bool, ImGuiWindowFlags), &ImGui::BeginChild), //.Function("BeginChild", MakeFunction(&ImGui::BeginChild, ImVec2(0,0), false, 0))
                MakeOverload(bool (*)(ImGuiID, const ImVec2&, bool, ImGuiWindowFlags), &ImGui::BeginChild) //.Function("BeginChild", MakeFunction(&ImGui::BeginChild, ImVec2(0,0), false, 0))
                ))
            .Function("EndChild", MakeFunction(&ImGui::EndChild))
            .Function("IsWindowAppearing", MakeFunction(&ImGui::IsWindowAppearing))
            .Function("IsWindowCollapsed", MakeFunction(&ImGui::IsWindowCollapsed))
            .Function("IsWindowFocused", MakeFunction(&ImGui::IsWindowFocused, 0))
            .Function("IsWindowHovered", MakeFunction(&ImGui::IsWindowHovered, 0))
            .Function("GetWindowDrawList", MakeFunction(&ImGui::GetWindowDrawList))
            .Function("GetWindowPos", MakeFunction(&ImGui::GetWindowPos))
            .Function("GetWindowSize", MakeFunction(&ImGui::GetWindowSize))
            .Function("GetWindowWidth", MakeFunction(&ImGui::GetWindowWidth))
            .Function("GetWindowHeight", MakeFunction(&ImGui::GetWindowHeight))
            .Function("SetNextWindowPos", MakeFunction(&ImGui::SetNextWindowPos, 0, ImVec2(0,0)))
            .Function("SetNextWindowSize", MakeFunction(&ImGui::SetNextWindowSize, 0))
            //.Function("SetNextWindowSizeConstraints", MakeFunction(&ImGui::SetNextWindowSizeConstraints, nullptr, nullptr)) // callback
            .Function("SetNextWindowContentSize", MakeFunction(&ImGui::SetNextWindowContentSize))
            .Function("SetNextWindowCollapsed", MakeFunction(&ImGui::SetNextWindowCollapsed, 0))
            .Function("SetNextWindowFocus", MakeFunction(&ImGui::SetNextWindowFocus))
            .Function("SetNextWindowBgAlpha", MakeFunction(&ImGui::SetNextWindowBgAlpha))
            .Function("SetWindowPos", CombineOverloads(
                MakeOverload(void (*)(const ImVec2&, ImGuiCond), &ImGui::SetWindowPos), //  TODO:default value  .Function("SetWindowPos", MakeFunction(&ImGui::SetWindowPos, 0))
                MakeOverload(void (*)(const char*, const ImVec2&, ImGuiCond), &ImGui::SetWindowPos) // TODO:default value .Function("SetWindowPos", MakeFunction(&ImGui::SetWindowPos, 0))
                ))
            .Function("SetWindowSize", CombineOverloads(
                MakeOverload(void (*)(const ImVec2&, ImGuiCond), &ImGui::SetWindowSize), //  TODO:default value  .Function("SetWindowSize", MakeFunction(&ImGui::SetWindowSize, 0))
                MakeOverload(void (*)(const char*, const ImVec2&, ImGuiCond), &ImGui::SetWindowSize) // TODO:default value .Function("SetWindowSize", MakeFunction(&ImGui::SetWindowSize, 0))
                ))
            .Function("SetWindowCollapsed", CombineOverloads(
                MakeOverload(void (*)(bool, ImGuiCond), &ImGui::SetWindowCollapsed), //  TODO:default value  .Function("SetWindowCollapsed", MakeFunction(&ImGui::SetWindowCollapsed, 0))
                MakeOverload(void (*)(const char*, bool, ImGuiCond), &ImGui::SetWindowCollapsed) // TODO:default value .Function("SetWindowCollapsed", MakeFunction(&ImGui::SetWindowCollapsed, 0))
                ))
            .Function("SetWindowFocus", CombineOverloads(
                MakeOverload(void (*)(), &ImGui::SetWindowFocus),
                MakeOverload(void (*)(const char*), &ImGui::SetWindowFocus)
                ))
            .Function("SetWindowFontScale", MakeFunction(&ImGui::SetWindowFontScale))
            .Function("GetContentRegionMax", MakeFunction(&ImGui::GetContentRegionMax))
            .Function("GetContentRegionAvail", MakeFunction(&ImGui::GetContentRegionAvail))
            .Function("GetWindowContentRegionMin", MakeFunction(&ImGui::GetWindowContentRegionMin))
            .Function("GetWindowContentRegionMax", MakeFunction(&ImGui::GetWindowContentRegionMax))
            .Function("GetWindowContentRegionWidth", MakeFunction(&ImGui::GetWindowContentRegionWidth))
            .Function("GetScrollX", MakeFunction(&ImGui::GetScrollX))
            .Function("GetScrollY", MakeFunction(&ImGui::GetScrollY))
            .Function("GetScrollMaxX", MakeFunction(&ImGui::GetScrollMaxX))
            .Function("GetScrollMaxY", MakeFunction(&ImGui::GetScrollMaxY))
            .Function("SetScrollX", MakeFunction(&ImGui::SetScrollX))
            .Function("SetScrollY", MakeFunction(&ImGui::SetScrollY))
            .Function("SetScrollHereX", MakeFunction(&ImGui::SetScrollHereX, 0.5f))
            .Function("SetScrollHereY", MakeFunction(&ImGui::SetScrollHereY, 0.5f))
            .Function("SetScrollFromPosX", MakeFunction(&ImGui::SetScrollFromPosX, 0.5f))
            .Function("SetScrollFromPosY", MakeFunction(&ImGui::SetScrollFromPosY, 0.5f))
            .Function("PushFont", MakeFunction(&ImGui::PushFont))
            .Function("PopFont", MakeFunction(&ImGui::PopFont))
            .Function("PushStyleColor", CombineOverloads(
                MakeOverload(void (*)(ImGuiCol, ImU32), &ImGui::PushStyleColor),
                MakeOverload(void (*)(ImGuiCol, const ImVec4&), &ImGui::PushStyleColor)
                ))
            .Function("PopStyleColor", MakeFunction(&ImGui::PopStyleColor, 1))
            .Function("PushStyleVar", CombineOverloads(
                MakeOverload(void (*)(ImGuiStyleVar, float), &ImGui::PushStyleVar),
                MakeOverload(void (*)(ImGuiStyleVar, const ImVec2&), &ImGui::PushStyleVar)
                ))
            .Function("PopStyleVar", MakeFunction(&ImGui::PopStyleVar, 1))
            .Function("GetStyleColorVec4", MakeFunction(&ImGui::GetStyleColorVec4))
            .Function("GetFont", MakeFunction(&ImGui::GetFont))
            .Function("GetFontSize", MakeFunction(&ImGui::GetFontSize))
            .Function("GetFontTexUvWhitePixel", MakeFunction(&ImGui::GetFontTexUvWhitePixel))
            .Function("GetColorU32", CombineOverloads(
                MakeOverload(ImU32 (*)(ImGuiCol, float), &ImGui::GetColorU32), //TODO: default value  .Function("GetColorU32", MakeFunction(&ImGui::GetColorU32, 1.0f))
                MakeOverload(ImU32 (*)(const ImVec4&), &ImGui::GetColorU32),
                MakeOverload(ImU32 (*)(ImU32), &ImGui::GetColorU32)
                ))
            .Function("PushItemWidth", MakeFunction(&ImGui::PushItemWidth))
            .Function("PopItemWidth", MakeFunction(&ImGui::PopItemWidth))
            .Function("SetNextItemWidth", MakeFunction(&ImGui::SetNextItemWidth))
            .Function("CalcItemWidth", MakeFunction(&ImGui::CalcItemWidth))
            .Function("PushTextWrapPos", MakeFunction(&ImGui::PushTextWrapPos, 0.0f))
            .Function("PopTextWrapPos", MakeFunction(&ImGui::PopTextWrapPos))
            .Function("PushAllowKeyboardFocus", MakeFunction(&ImGui::PushAllowKeyboardFocus))
            .Function("PopAllowKeyboardFocus", MakeFunction(&ImGui::PopAllowKeyboardFocus))
            .Function("PushButtonRepeat", MakeFunction(&ImGui::PushButtonRepeat))
            .Function("PopButtonRepeat", MakeFunction(&ImGui::PopButtonRepeat))
            .Function("Separator", MakeFunction(&ImGui::Separator))
            .Function("SameLine", MakeFunction(&ImGui::SameLine, 0.0f, -1.0f))
            .Function("NewLine", MakeFunction(&ImGui::NewLine))
            .Function("Spacing", MakeFunction(&ImGui::Spacing))
            .Function("Dummy", MakeFunction(&ImGui::Dummy))
            .Function("Indent", MakeFunction(&ImGui::Indent, 0.0f))
            .Function("Unindent", MakeFunction(&ImGui::Unindent, 0.0f))
            .Function("BeginGroup", MakeFunction(&ImGui::BeginGroup))
            .Function("EndGroup", MakeFunction(&ImGui::EndGroup))
            .Function("GetCursorPos", MakeFunction(&ImGui::GetCursorPos))
            .Function("GetCursorPosX", MakeFunction(&ImGui::GetCursorPosX))
            .Function("GetCursorPosY", MakeFunction(&ImGui::GetCursorPosY))
            .Function("SetCursorPos", MakeFunction(&ImGui::SetCursorPos))
            .Function("SetCursorPosX", MakeFunction(&ImGui::SetCursorPosX))
            .Function("SetCursorPosY", MakeFunction(&ImGui::SetCursorPosY))
            .Function("GetCursorStartPos", MakeFunction(&ImGui::GetCursorStartPos))
            .Function("GetCursorScreenPos", MakeFunction(&ImGui::GetCursorScreenPos))
            .Function("SetCursorScreenPos", MakeFunction(&ImGui::SetCursorScreenPos))
            .Function("AlignTextToFramePadding", MakeFunction(&ImGui::AlignTextToFramePadding))
            .Function("GetTextLineHeight", MakeFunction(&ImGui::GetTextLineHeight))
            .Function("GetTextLineHeightWithSpacing", MakeFunction(&ImGui::GetTextLineHeightWithSpacing))
            .Function("GetFrameHeight", MakeFunction(&ImGui::GetFrameHeight))
            .Function("GetFrameHeightWithSpacing", MakeFunction(&ImGui::GetFrameHeightWithSpacing))
            .Function("PushID", CombineOverloads(
                MakeOverload(void (*)(const char*), &ImGui::PushID),
                MakeOverload(void (*)(const char*, const char*), &ImGui::PushID),
                MakeOverload(void (*)(const void*), &ImGui::PushID),
                MakeOverload(void (*)(int), &ImGui::PushID)
                ))
            .Function("PopID", MakeFunction(&ImGui::PopID))
            .Function("GetID", CombineOverloads(
                MakeOverload(ImGuiID (*)(const char*), &ImGui::GetID),
                MakeOverload(ImGuiID (*)(const char*, const char*), &ImGui::GetID),
                MakeOverload(ImGuiID (*)(const void*), &ImGui::GetID)
                ))
            .Function("TextUnformatted", MakeFunction(&ImGui::TextUnformatted, nullptr))
            .Function("Button", MakeFunction(&ImGui::Button, ImVec2(0,0)))
            .Function("SmallButton", MakeFunction(&ImGui::SmallButton))
            .Function("InvisibleButton", MakeFunction(&ImGui::InvisibleButton))
            .Function("ArrowButton", MakeFunction(&ImGui::ArrowButton))
            .Function("Image", MakeFunction(&ImGui::Image, ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0)))
            .Function("ImageButton", MakeFunction(&ImGui::ImageButton, ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1,1,1,1)))
            .Function("Checkbox", MakeFunction(&ImGui::Checkbox))
            //.Function("CheckboxFlags", MakeFunction(&ImGui::CheckboxFlags)) //1.81有两个重载，后面再处理
            .Function("RadioButton", CombineOverloads(
                MakeOverload(bool (*)(const char*, bool), &ImGui::RadioButton),
                MakeOverload(bool (*)(const char*, int*, int), &ImGui::RadioButton)
                ))
            .Function("ProgressBar", MakeFunction(&ImGui::ProgressBar, ImVec2(-1,0), nullptr))
            .Function("Bullet", MakeFunction(&ImGui::Bullet))
            .Function("BeginCombo", MakeFunction(&ImGui::BeginCombo, 0))
            .Function("EndCombo", MakeFunction(&ImGui::EndCombo))
            //.Function("Combo", CombineOverloads( // TODO: fault value
                //MakeOverload(bool (*)(const char*, int*, const char* const[], int, int), &ImGui::Combo), //   Binding.hpp(271): [C2027] 使用了未定义类型“puerts::converter::Converter<const char *const *,void>” //.Function("Combo", MakeFunction(&ImGui::Combo, -1))
                //MakeOverload(bool (*)(const char*, int*, const char*, int), &ImGui::Combo) //.Function("Combo", MakeFunction(&ImGui::Combo, -1))
                //MakeOverload(bool (*)(const char*, int*, bool(*)(void* data, int idx, const char** out_text), void*, int, int), &ImGui::Combo) // callback
            //    ))
            .Function("Combo", SelectFunction(bool (*)(const char*, int*, const char*, int), &ImGui::Combo)) //.Function("Combo", MakeFunction(&ImGui::Combo, -1))
            .Function("DragFloat", MakeFunction(&ImGui::DragFloat, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f))
            .Function("DragFloat2", MakeFunction(&ImGui::DragFloat2, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f))
            .Function("DragFloat3", MakeFunction(&ImGui::DragFloat3, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f))
            .Function("DragFloat4", MakeFunction(&ImGui::DragFloat4, 1.0f, 0.0f, 0.0f, "%.3f", 1.0f))
            .Function("DragFloatRange2", MakeFunction(&ImGui::DragFloatRange2, 1.0f, 0.0f, 0.0f, "%.3f", nullptr, 1.0f))
            .Function("DragInt", MakeFunction(&ImGui::DragInt, 1.0f, 0, 0, "%d", 0)) //1.81增加了参数和默认值
            .Function("DragInt2", MakeFunction(&ImGui::DragInt2, 1.0f, 0, 0, "%d", 0)) //1.81增加了参数和默认值
            .Function("DragInt3", MakeFunction(&ImGui::DragInt3, 1.0f, 0, 0, "%d", 0)) //1.81增加了参数和默认值
            .Function("DragInt4", MakeFunction(&ImGui::DragInt4, 1.0f, 0, 0, "%d", 0)) //1.81增加了参数和默认值
            .Function("DragIntRange2", MakeFunction(&ImGui::DragIntRange2, 1.0f, 0, 0, "%d", nullptr, 0)) //1.81增加了参数和默认值
            .Function("DragScalar", MakeFunction(&ImGui::DragScalar, 1.0f, nullptr, nullptr, nullptr, 0)) //1.81增加了参数和默认值
            .Function("DragScalarN", MakeFunction(&ImGui::DragScalarN, 1.0f, nullptr, nullptr, nullptr, 0)) //1.81增加了参数和默认值
            .Function("SliderFloat", MakeFunction(&ImGui::SliderFloat, "%.3f", 1.0f))
            .Function("SliderFloat2", MakeFunction(&ImGui::SliderFloat2, "%.3f", 1.0f))
            .Function("SliderFloat3", MakeFunction(&ImGui::SliderFloat3, "%.3f", 1.0f))
            .Function("SliderFloat4", MakeFunction(&ImGui::SliderFloat4, "%.3f", 1.0f))
            .Function("SliderAngle", MakeFunction(&ImGui::SliderAngle, -360.0f, +360.0f, "%.0f deg", 0)) //1.81增加了参数和默认值
            .Function("SliderInt", MakeFunction(&ImGui::SliderInt, "%d", 0)) //1.81增加了参数和默认值
            .Function("SliderInt2", MakeFunction(&ImGui::SliderInt2, "%d", 0))
            .Function("SliderInt3", MakeFunction(&ImGui::SliderInt3, "%d", 0))
            .Function("SliderInt4", MakeFunction(&ImGui::SliderInt4, "%d", 0))
            .Function("SliderScalar", MakeFunction(&ImGui::SliderScalar, nullptr, 1.0f)) 
            .Function("SliderScalarN", MakeFunction(&ImGui::SliderScalarN, nullptr, 1.0f)) 
            .Function("VSliderFloat", MakeFunction(&ImGui::VSliderFloat, "%.3f", 1.0f))
            .Function("VSliderInt", MakeFunction(&ImGui::VSliderInt, "%d", 0)) //1.81增加了参数和默认值
            .Function("VSliderScalar", MakeFunction(&ImGui::VSliderScalar, nullptr, 1.0f))
            .Function("InputText", MakeFunction(&ImGuiWrapped::InputText, 0, nullptr))
            .Function("InputTextMultiline", MakeFunction(&ImGuiWrapped::InputTextMultiline, ImVec2(0,0), 0, nullptr))  //callback
            .Function("InputTextWithHint", MakeFunction(&ImGuiWrapped::InputTextWithHint, 0, nullptr))  //callback
            .Function("InputFloat", MakeFunctionWithScriptTypePtr(&ImGui::InputFloat, 0.0f, 0.0f, "%.3f", 0))
            .Function("InputFloat2", MakeFunctionWithScriptTypePtr(&ImGui::InputFloat2, "%.3f", 0))
            .Function("InputFloat3", MakeFunctionWithScriptTypePtr(&ImGui::InputFloat3, "%.3f", 0))
            .Function("InputFloat4", MakeFunctionWithScriptTypePtr(&ImGui::InputFloat4, "%.3f", 0))
            .Function("InputInt", MakeFunction(&ImGui::InputInt, 1, 100, 0))
            .Function("InputInt2", MakeFunction(&ImGui::InputInt2, 0))
            .Function("InputInt3", MakeFunction(&ImGui::InputInt3, 0))
            .Function("InputInt4", MakeFunction(&ImGui::InputInt4, 0))
            .Function("InputDouble", MakeFunction(&ImGui::InputDouble, 0.0, 0.0, "%.6f", 0))
            .Function("InputScalar", MakeFunction(&ImGui::InputScalar, nullptr, nullptr, nullptr, 0))
            .Function("InputScalarN", MakeFunction(&ImGui::InputScalarN, nullptr, nullptr, nullptr, 0))
            .Function("ColorEdit3", MakeFunctionWithScriptTypePtr(&ImGui::ColorEdit3, 0))
            .Function("ColorEdit4", MakeFunctionWithScriptTypePtr(&ImGui::ColorEdit4, 0))
            .Function("ColorPicker3", MakeFunctionWithScriptTypePtr(&ImGui::ColorPicker3, 0))
            .Function("ColorPicker4", MakeFunctionWithScriptTypePtr(&ImGui::ColorPicker4, 0, nullptr))
            .Function("ColorButton", MakeFunction(&ImGui::ColorButton, 0, ImVec2(0,0)))
            .Function("SetColorEditOptions", MakeFunction(&ImGui::SetColorEditOptions))
            .Function("TreePush", CombineOverloads(
                MakeOverload(void (*)(const char*), &ImGui::TreePush),
                MakeOverload(void (*)(const void* ptr_id), &ImGui::TreePush) //TODO:default value  .Function("TreePush", MakeFunction(&ImGui::TreePush, nullptr))
                ))
            .Function("TreePop", MakeFunction(&ImGui::TreePop))
            .Function("GetTreeNodeToLabelSpacing", MakeFunction(&ImGui::GetTreeNodeToLabelSpacing))
            .Function("CollapsingHeader", CombineOverloads(
                MakeOverload(bool (*)(const char*, ImGuiTreeNodeFlags), &ImGui::CollapsingHeader), //TODO:default value .Function("CollapsingHeader", MakeFunction(&ImGui::CollapsingHeader, 0))
                MakeOverload(bool (*)(const char*, bool*, ImGuiTreeNodeFlags), &ImGui::CollapsingHeader) //TODO:default value  .Function("CollapsingHeader", MakeFunction(&ImGui::CollapsingHeader, 0))
                ))
            .Function("SetNextItemOpen", MakeFunction(&ImGui::SetNextItemOpen, 0))
            .Function("Selectable", CombineOverloads(
                SelectOverload(ImGui_Selectable, bool (*)(const char*, bool, ImGuiSelectableFlags, const ImVec2&)), 
                SelectOverload(ImGui_Selectable, bool (*)(const char*, bool*, ImGuiSelectableFlags, const ImVec2&)) 
                ))
            .Function("ListBox", MakeFunction(&ImGuiWrapped::ListBox, -1))
            //.Function("ListBox", MakeFunction(&ImGui::ListBox, -1)) //bool ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
            //.Function("ListBox", MakeFunction(&ImGui::ListBox, -1)) //bool ImGui::ListBox(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
            //.Function("ListBoxHeader", CombineOverloads(  // OBSOLETED in 1.81 (from February 2021)
            //    MakeOverload(bool (*)(const char*, const ImVec2&), &ImGui::ListBoxHeader), //TODO:default value .Function("ListBoxHeader", MakeFunction(&ImGui::ListBoxHeader, ImVec2(0,0)))
            //    MakeOverload(bool (*)(const char*, int, int), &ImGui::ListBoxHeader) //TODO:default value  .Function("ListBoxHeader", MakeFunction(&ImGui::ListBoxHeader, -1))
            //    ))
            //.Function("ListBoxFooter", MakeFunction(&ImGui::ListBoxFooter)) OBSOLETED in 1.81 (from February 2021)
            .Function("PlotLines", SelectFunction(void (*)(const char*, const float*, int, int, const char*, float, float, ImVec2, int), &ImGui::PlotLines)) //TODO:default value .Function("PlotLines", MakeFunction(&ImGui::PlotLines, 0, nullptr, 3.402823466e+38F, 3.402823466e+38F, ImVec2(0, 0), sizeof(float)))
            //.Function("PlotLines", MakeFunction(&ImGui::PlotLines, 0, nullptr, 3.402823466e+38F, 3.402823466e+38F, ImVec2(0, 0))) // callback
            .Function("PlotHistogram", SelectFunction(void (*)(const char*, const float*, int, int, const char*, float, float, ImVec2, int), &ImGui::PlotHistogram)) //TODO:default value .Function("PlotHistogram", MakeFunction(&ImGui::PlotHistogram, 0, nullptr, 3.402823466e+38F, 3.402823466e+38F, ImVec2(0, 0), sizeof(float)))
            //.Function("PlotHistogram", MakeFunction(&ImGui::PlotHistogram, 0, nullptr, 3.402823466e+38F, 3.402823466e+38F, ImVec2(0, 0))) // callback
            .Function("Value", CombineOverloads(
                MakeOverload(void (*)(const char*, bool), &ImGui::Value),
                MakeOverload(void (*)(const char*, int), &ImGui::Value),
                MakeOverload(void (*)(const char*, unsigned int), &ImGui::Value),
                MakeOverload(void (*)(const char*, float, const char*), &ImGui::Value) // TODO:default value  .Function("Value", MakeFunction(&ImGui::Value, nullptr))
                ))
            .Function("BeginMenuBar", MakeFunction(&ImGui::BeginMenuBar))
            .Function("EndMenuBar", MakeFunction(&ImGui::EndMenuBar))
            .Function("BeginMainMenuBar", MakeFunction(&ImGui::BeginMainMenuBar))
            .Function("EndMainMenuBar", MakeFunction(&ImGui::EndMainMenuBar))
            .Function("BeginMenu", MakeFunction(&ImGui::BeginMenu, true))
            .Function("EndMenu", MakeFunction(&ImGui::EndMenu))
            .Function("MenuItem", CombineOverloads(
                MakeOverload(bool (*)(const char*, const char*, bool, bool), &ImGui::MenuItem), //TODO:default value .Function("MenuItem", MakeFunction(&ImGui::MenuItem, nullptr, false, true))
                MakeOverload(bool (*)(const char*, const char*, bool*, bool), &ImGui::MenuItem) //TODO:default value  .Function("MenuItem", MakeFunction(&ImGui::MenuItem, true))
                ))
            .Function("BeginTooltip", MakeFunction(&ImGui::BeginTooltip))
            .Function("EndTooltip", MakeFunction(&ImGui::EndTooltip))
            //.Function("OpenPopup", MakeFunction(&ImGui::OpenPopup)) // 1.81增加了重载
            .Function("BeginPopup", MakeFunction(&ImGui::BeginPopup, 0))
            .Function("BeginPopupContextItem", MakeFunction(&ImGui::BeginPopupContextItem, nullptr, 1))
            .Function("BeginPopupContextWindow", MakeFunction(&ImGui::BeginPopupContextWindow, nullptr, 1)) //1.81也有区别
            .Function("BeginPopupContextVoid", MakeFunction(&ImGui::BeginPopupContextVoid, nullptr, 1))
            .Function("BeginPopupModal", MakeFunction(&ImGui::BeginPopupModal, nullptr, 0))
            .Function("EndPopup", MakeFunction(&ImGui::EndPopup))
            .Function("OpenPopupOnItemClick", MakeFunction(&ImGui::OpenPopupOnItemClick, nullptr, 1))
            .Function("IsPopupOpen", MakeFunction(&ImGui::IsPopupOpen))
            .Function("CloseCurrentPopup", MakeFunction(&ImGui::CloseCurrentPopup))
            .Function("Columns", MakeFunction(&ImGui::Columns, 1, nullptr, true))
            .Function("NextColumn", MakeFunction(&ImGui::NextColumn))
            .Function("GetColumnIndex", MakeFunction(&ImGui::GetColumnIndex))
            .Function("GetColumnWidth", MakeFunction(&ImGui::GetColumnWidth, -1))
            .Function("SetColumnWidth", MakeFunction(&ImGui::SetColumnWidth))
            .Function("GetColumnOffset", MakeFunction(&ImGui::GetColumnOffset, -1))
            .Function("SetColumnOffset", MakeFunction(&ImGui::SetColumnOffset))
            .Function("GetColumnsCount", MakeFunction(&ImGui::GetColumnsCount))
            .Function("BeginTabBar", MakeFunction(&ImGui::BeginTabBar, 0))
            .Function("EndTabBar", MakeFunction(&ImGui::EndTabBar))
            .Function("BeginTabItem", MakeFunction(&ImGui::BeginTabItem, nullptr, 0))
            .Function("EndTabItem", MakeFunction(&ImGui::EndTabItem))
            .Function("SetTabItemClosed", MakeFunction(&ImGui::SetTabItemClosed))
            .Function("LogToTTY", MakeFunction(&ImGui::LogToTTY, -1))
            .Function("LogToFile", MakeFunction(&ImGui::LogToFile, -1, nullptr))
            .Function("LogToClipboard", MakeFunction(&ImGui::LogToClipboard, -1))
            .Function("LogFinish", MakeFunction(&ImGui::LogFinish))
            .Function("LogButtons", MakeFunction(&ImGui::LogButtons))
            .Function("BeginDragDropSource", MakeFunction(&ImGui::BeginDragDropSource, 0))
            .Function("SetDragDropPayload", MakeFunction(&ImGui::SetDragDropPayload, 0)) 
            .Function("EndDragDropSource", MakeFunction(&ImGui::EndDragDropSource))
            .Function("BeginDragDropTarget", MakeFunction(&ImGui::BeginDragDropTarget))
            .Function("AcceptDragDropPayload", MakeFunction(&ImGui::AcceptDragDropPayload, 0)) 
            .Function("EndDragDropTarget", MakeFunction(&ImGui::EndDragDropTarget))
            .Function("GetDragDropPayload", MakeFunction(&ImGui::GetDragDropPayload)) 
            .Function("PushClipRect", MakeFunction(&ImGui::PushClipRect))
            .Function("PopClipRect", MakeFunction(&ImGui::PopClipRect))
            .Function("SetItemDefaultFocus", MakeFunction(&ImGui::SetItemDefaultFocus))
            .Function("SetKeyboardFocusHere", MakeFunction(&ImGui::SetKeyboardFocusHere, 0))
            .Function("IsItemHovered", MakeFunction(&ImGui::IsItemHovered, 0))
            .Function("IsItemActive", MakeFunction(&ImGui::IsItemActive))
            .Function("IsItemFocused", MakeFunction(&ImGui::IsItemFocused))
            .Function("IsItemClicked", MakeFunction(&ImGui::IsItemClicked, 0))
            .Function("IsItemVisible", MakeFunction(&ImGui::IsItemVisible))
            .Function("IsItemEdited", MakeFunction(&ImGui::IsItemEdited))
            .Function("IsItemActivated", MakeFunction(&ImGui::IsItemActivated))
            .Function("IsItemDeactivated", MakeFunction(&ImGui::IsItemDeactivated))
            .Function("IsItemDeactivatedAfterEdit", MakeFunction(&ImGui::IsItemDeactivatedAfterEdit))
            .Function("IsItemToggledOpen", MakeFunction(&ImGui::IsItemToggledOpen))
            .Function("IsAnyItemHovered", MakeFunction(&ImGui::IsAnyItemHovered))
            .Function("IsAnyItemActive", MakeFunction(&ImGui::IsAnyItemActive))
            .Function("IsAnyItemFocused", MakeFunction(&ImGui::IsAnyItemFocused))
            .Function("GetItemRectMin", MakeFunction(&ImGui::GetItemRectMin))
            .Function("GetItemRectMax", MakeFunction(&ImGui::GetItemRectMax))
            .Function("GetItemRectSize", MakeFunction(&ImGui::GetItemRectSize))
            .Function("SetItemAllowOverlap", MakeFunction(&ImGui::SetItemAllowOverlap))
            .Function("IsRectVisible", CombineOverloads(
                MakeOverload(bool (*)(const ImVec2&), &ImGui::IsRectVisible),
                MakeOverload(bool (*)(const ImVec2&, const ImVec2&), &ImGui::IsRectVisible)
                ))
            .Function("GetTime", MakeFunction(&ImGui::GetTime))
            .Function("GetFrameCount", MakeFunction(&ImGui::GetFrameCount))
            //.Function("GetBackgroundDrawList", MakeFunction(&ImGui::GetBackgroundDrawList)) //1.81增加了重载
            //.Function("GetForegroundDrawList", MakeFunction(&ImGui::GetForegroundDrawList)) //1.81增加了重载
            .Function("GetDrawListSharedData", MakeFunction(&ImGui::GetDrawListSharedData)) 
            .Function("GetStyleColorName", MakeFunction(&ImGui::GetStyleColorName))
            .Function("SetStateStorage", MakeFunction(&ImGui::SetStateStorage))
            .Function("GetStateStorage", MakeFunction(&ImGui::GetStateStorage))
            .Function("CalcTextSize", MakeFunction(&ImGui::CalcTextSize, nullptr, false, -1.0f))
            .Function("CalcListClipping", MakeFunction(&ImGui::CalcListClipping))
            .Function("BeginChildFrame", MakeFunction(&ImGui::BeginChildFrame, 0))
            .Function("EndChildFrame", MakeFunction(&ImGui::EndChildFrame))
            .Function("ColorConvertU32ToFloat4", MakeFunction(&ImGui::ColorConvertU32ToFloat4))
            .Function("ColorConvertFloat4ToU32", MakeFunction(&ImGui::ColorConvertFloat4ToU32))
            .Function("ColorConvertRGBtoHSV", MakeFunction(&ImGui::ColorConvertRGBtoHSV))
            .Function("ColorConvertHSVtoRGB", MakeFunction(&ImGui::ColorConvertHSVtoRGB))
            .Function("GetKeyIndex", MakeFunction(&ImGui::GetKeyIndex))
            .Function("IsKeyDown", MakeFunction(&ImGui::IsKeyDown))
            .Function("IsKeyPressed", MakeFunction(&ImGui::IsKeyPressed, true))
            .Function("IsKeyReleased", MakeFunction(&ImGui::IsKeyReleased))
            .Function("GetKeyPressedAmount", MakeFunction(&ImGui::GetKeyPressedAmount))
            .Function("IsMouseDown", MakeFunction(&ImGui::IsMouseDown))
            .Function("IsAnyMouseDown", MakeFunction(&ImGui::IsAnyMouseDown))
            .Function("IsMouseClicked", MakeFunction(&ImGui::IsMouseClicked, false))
            .Function("IsMouseDoubleClicked", MakeFunction(&ImGui::IsMouseDoubleClicked))
            .Function("IsMouseReleased", MakeFunction(&ImGui::IsMouseReleased))
            .Function("IsMouseDragging", MakeFunction(&ImGui::IsMouseDragging, 0, -1.0f))
            .Function("IsMouseHoveringRect", MakeFunction(&ImGui::IsMouseHoveringRect, true))
            .Function("IsMousePosValid", MakeFunction(&ImGui::IsMousePosValid, nullptr))
            .Function("GetMousePos", MakeFunction(&ImGui::GetMousePos))
            .Function("GetMousePosOnOpeningCurrentPopup", MakeFunction(&ImGui::GetMousePosOnOpeningCurrentPopup))
            .Function("GetMouseDragDelta", MakeFunction(&ImGui::GetMouseDragDelta, 0, -1.0f))
            .Function("ResetMouseDragDelta", MakeFunction(&ImGui::ResetMouseDragDelta, 0))
            .Function("GetMouseCursor", MakeFunction(&ImGui::GetMouseCursor))
            .Function("SetMouseCursor", MakeFunction(&ImGui::SetMouseCursor))
            .Function("CaptureKeyboardFromApp", MakeFunction(&ImGui::CaptureKeyboardFromApp, true))
            .Function("CaptureMouseFromApp", MakeFunction(&ImGui::CaptureMouseFromApp, true))
            .Function("GetClipboardText", MakeFunction(&ImGui::GetClipboardText))
            .Function("SetClipboardText", MakeFunction(&ImGui::SetClipboardText))
            .Function("LoadIniSettingsFromDisk", MakeFunction(&ImGui::LoadIniSettingsFromDisk))
            .Function("LoadIniSettingsFromMemory", MakeFunction(&ImGui::LoadIniSettingsFromMemory, 0))
            .Function("SaveIniSettingsToDisk", MakeFunction(&ImGui::SaveIniSettingsToDisk))
            .Function("SaveIniSettingsToMemory", MakeFunction(&ImGui::SaveIniSettingsToMemory, nullptr))
            //.Function("SetAllocatorFunctions", MakeFunction(&ImGui::SetAllocatorFunctions, nullptr))   // callback
            .Function("MemAlloc", MakeFunction(&ImGui::MemAlloc))
            .Function("MemFree", MakeFunction(&ImGui::MemFree))
            .Register();
    }
};

AutoRegisterForDearImGui _AutoRegisterForImGui__;