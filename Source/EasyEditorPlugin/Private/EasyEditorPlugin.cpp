#include "EasyEditorPlugin.h"

#include "ToolMenuDelegates.h"
#include "Misc/FileHelper.h"
#include "Containers/Ticker.h"
#include "ContentBrowserItem.h"
#include "ToolMenus.h"
#include "Binding.hpp"
#include "UEDataBinding.hpp"
#include "Object.hpp"
#ifdef UEIMGUI_API
#include "EasyEditorDetailCustomization.h"
#endif

namespace puerts
{
template <typename T>
struct ScriptTypeName<TAttribute<T>>
{
    static constexpr auto value()
    {
        return ScriptTypeName<T>::value();
    }
};

namespace converter
{
template <typename T>
struct Converter<TAttribute<T>>
{
    static ValueType toScript(ContextType context, TAttribute<T> value)
    {
    	if (value.IsSet())
    	{
    		return Converter<T>::toScript(context, value.Get());
    	}
        return GetUndefined(context);
    }

    static TAttribute<T> toCpp(ContextType context, const ValueType value)
    {
        if (IsNullOrUndefined(context, value))
            return TAttribute<T>();
        return TAttribute<T>(Converter<T>::toCpp(context, value));
    }

    static bool accept(ContextType context, const ValueType value)
    {
        return IsNullOrUndefined(context, value) || Converter<T>::accept(context, value);
    }
};
}    // namespace converter
}    // namespace puerts


struct EasyEditorPlugin
{
	static void SetOnJsEnvPreReload(std::function<void()> Func)
	{
		FModuleManager::LoadModuleChecked<FEasyEditorPluginModule>("EasyEditorPlugin").OnJsEnvPreReload = Func;
	}

	static void SetEval(std::function<void(const FString&)> Func)
	{
		FModuleManager::LoadModuleChecked<FEasyEditorPluginModule>("EasyEditorPlugin").Eval = Func;
	}

	
	static int32 AddConsoleCommand(const TCHAR* Name, const TCHAR* Help, puerts::Function Command)
	{
		return FModuleManager::LoadModuleChecked<FEasyEditorPluginModule>("EasyEditorPlugin").TsConsoleCommands.Add(
            MakeUnique<FAutoConsoleCommand>(Name, Help,
			FConsoleCommandWithArgsDelegate::CreateLambda(
				[Command](const TArray<FString>& Args)
				{
					auto Isolate = Command.Isolate;
					v8::Isolate::Scope IsolateScope(Isolate);
					v8::HandleScope HandleScope(Isolate);
					auto Context = Command.GContext.Get(Isolate);
					v8::Context::Scope ContextScope(Context);

					auto JsFunction = Command.GObject.Get(Isolate).As<v8::Function>();

					v8::TryCatch TryCatch(Isolate);

					v8::Local<v8::Value>* JsArgs =
		                static_cast<v8::Local<v8::Value>*>(FMemory_Alloca(sizeof(v8::Local<v8::Value>) * Args.Num()));

					for(int i = 0; i < Args.Num(); i++)
					{
						JsArgs[i] = puerts::FV8Utils::ToV8String(Isolate, Args[i]);
					}

					auto Result = JsFunction->Call(Context, v8::Undefined(Isolate), Args.Num(), JsArgs);

					if (TryCatch.HasCaught())
					{
						UE_LOG(Puerts, Error, TEXT("call function throw: %s"), *puerts::FV8Utils::TryCatchToString(Isolate, &TryCatch));
					}
				})));
	}

	static void RemoveConsoleCommand(int32 Index)
	{
		FModuleManager::LoadModuleChecked<FEasyEditorPluginModule>("EasyEditorPlugin").TsConsoleCommands.RemoveAt(Index);
	}
};

UsingUStruct(FToolMenuEntry);
UsingUClass(UToolMenu);
UsingUStruct(FToolMenuContext);
UsingUStruct(FContentBrowserItem);
UsingCppType(FSlateIcon);
UsingCppType(EasyEditorPlugin);

static FToolMenuEntry FToolMenuEntry_InitMenuEntry(const FName InName, const FText& InLabel, const FText& InToolTip, std::function<void(const FToolMenuContext&)> InExecuteAction)
{
	return FToolMenuEntry::InitMenuEntry(InName, InLabel, InToolTip, TAttribute<FSlateIcon>(), FToolUIActionChoice(FToolMenuExecuteAction::CreateLambda([InExecuteAction](const FToolMenuContext& InContext)
		{
			if(InExecuteAction)
			{
				InExecuteAction(InContext);
			}
		})));
}

static FToolMenuEntry FToolMenuEntry_InitToolBarButton(const FName InName, const FText& InLabel, std::function<void(const FToolMenuContext&)> InExecuteAction)
{
	return FToolMenuEntry::InitToolBarButton(InName, FToolUIActionChoice(FToolMenuExecuteAction::CreateLambda([InExecuteAction](const FToolMenuContext& InContext)
		{
			if(InExecuteAction)
			{
				InExecuteAction(InContext);
			}
		})), InLabel);
}


static FToolMenuEntry FToolMenuEntry_InitComboButton(const FName InName, std::function<void(const FToolMenuContext&)> InExecuteAction, std::function<void(UToolMenu*)> InMenuContentGenerator,
	const TAttribute<FText>& InLabel = TAttribute<FText>(), const TAttribute<FText>& InToolTip = TAttribute<FText>(), const FSlateIcon& InIcon = FSlateIcon())
{
	return FToolMenuEntry::InitComboButton(
			InName,
			FToolUIActionChoice(FToolMenuExecuteAction::CreateLambda([InExecuteAction](const FToolMenuContext& InContext)
			{
				if(InExecuteAction)
				{
					InExecuteAction(InContext);
				}
			})),
			FNewToolMenuDelegate::CreateLambda([InMenuContentGenerator](UToolMenu* InSubMenu)
			{
				if (InMenuContentGenerator)
				{
					InMenuContentGenerator(InSubMenu);
				}
			}), InLabel, InToolTip, InIcon);
}

struct AutoRegisterForEEP
{
	AutoRegisterForEEP()
	{
		puerts::DefineClass<FSlateIcon>()
		    .Constructor(CombineConstructors(
		    	MakeConstructor(FSlateIcon),
                MakeConstructor(FSlateIcon, const FName&, const FName&),
                MakeConstructor(FSlateIcon, const FName&, const FName&, const FName&)
                ))
		    .Register();
		
		puerts::DefineClass<FToolMenuEntry>()
			.Function("InitMenuEntry", MakeFunction(&FToolMenuEntry_InitMenuEntry))
		    .Function("InitToolBarButton", MakeFunction(&FToolMenuEntry_InitToolBarButton))
		    .Function("InitComboButton", MakeFunction(&FToolMenuEntry_InitComboButton, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon()))
			.Register();

		puerts::DefineClass<EasyEditorPlugin>()
			.Function("SetOnJsEnvPreReload", MakeFunction(&EasyEditorPlugin::SetOnJsEnvPreReload))
		    .Function("SetEval", MakeFunction(&EasyEditorPlugin::SetEval))
		    .Function("AddConsoleCommand", MakeFunction(&EasyEditorPlugin::AddConsoleCommand))
		    .Function("RemoveConsoleCommand", MakeFunction(&EasyEditorPlugin::RemoveConsoleCommand))
			.Register();

		puerts::DefineClass<FContentBrowserItem>()
		    .Method("IsFolder", MakeFunction(&FContentBrowserItem::IsFolder))
		    .Method("IsFile", MakeFunction(&FContentBrowserItem::IsFile))
		    .Method("GetItemName", MakeFunction(&FContentBrowserItem::GetItemName))
		    .Method("GetItemPhysicalPath", MakeFunction(&FContentBrowserItem::GetItemPhysicalPath))
		    .Register();
		
	}
};

AutoRegisterForEEP _AutoRegisterForEEP__;

#define LOCTEXT_NAMESPACE "FEasyEditorPluginModule"

void FEasyEditorPluginModule::StartupModule()
{
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FEasyEditorPluginModule::OnPostEngineInit);
}

void FEasyEditorPluginModule::OnPostEngineInit()
{
	InitJsEnv();
	FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FEasyEditorPluginModule::Tick));
	
	ConsoleCommand = MakeUnique<FAutoConsoleCommand>(TEXT("EasyEditor.Restart"), TEXT("Start Script"),
	FConsoleCommandWithArgsDelegate::CreateLambda(
		[this](const TArray<FString>& Args)
		{
		    if (OnJsEnvPreReload)
		    {
	    		OnJsEnvPreReload();
		    	UnInitJsEnv();
		    	InitJsEnv();
		    	StartupScriptCalled = true;
		    	JsEnv->Start(TEXT("Main"));
		    }
			else
			{
				UE_LOG(Puerts, Warning, TEXT("OnJsEnvPreReload no seted!"));
			}
		}));
	
	UToolMenus::Get()->RegisterStringCommandHandler("TypeScript", FToolMenuExecuteString::CreateLambda([this](const FString& InString, const FToolMenuContext& InContext) {
		if(Eval)
		{
			Eval(InString);
		}
		else
		{
			UE_LOG(Puerts, Warning, TEXT("call EasyEditorPlugin.SetEval set eval before execute a StringCommand"));
		}
	}));
}

void FEasyEditorPluginModule::InitJsEnv()
{
	SourceFileWatcher = MakeShared<puerts::FSourceFileWatcher>(
		[this](const FString& InPath)
		{
			if (JsEnv.IsValid())
			{
				TArray<uint8> Source;
				if (FFileHelper::LoadFileToArray(Source, *InPath))
				{
					JsEnv->ReloadSource(InPath, std::string((const char*) Source.GetData(), Source.Num()));
				}
				else
				{
					UE_LOG(Puerts, Error, TEXT("read file fail for %s"), *InPath);
				}
			}
		});
	JsEnv = MakeShared<puerts::FJsEnv>(std::make_shared<puerts::DefaultJSModuleLoader>(TEXT("EasyEditorScripts")),
		std::make_shared<puerts::FDefaultLogger>(), -1,
		[this](const FString& InPath)
		{
			if (SourceFileWatcher.IsValid())
			{
				SourceFileWatcher->OnSourceLoaded(InPath);
			}
		});
}

void FEasyEditorPluginModule::UnInitJsEnv()
{
#ifdef UEIMGUI_API
	UEasyEditorDetailCustomization* CDO = Cast<UEasyEditorDetailCustomization>(UEasyEditorDetailCustomization::StaticClass()->GetDefaultObject(false));
	if (CDO)
	{
		CDO->ClearOnDetailDraw();
	}
#endif
	Eval = nullptr;
	OnJsEnvPreReload = nullptr;
	if (JsEnv.IsValid())
	{
		JsEnv.Reset();
	}
	if (SourceFileWatcher.IsValid())
	{
		SourceFileWatcher.Reset();
	}
}

bool FEasyEditorPluginModule::Tick(float)
{
	if (!StartupScriptCalled)
	{
		StartupScriptCalled = true;
		if (JsEnv.IsValid())
		{
			JsEnv->Start(TEXT("Main"));
		}
	}
	return !StartupScriptCalled;
}

void FEasyEditorPluginModule::ShutdownModule()
{
	UnInitJsEnv();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEasyEditorPluginModule, EasyEditorPlugin)