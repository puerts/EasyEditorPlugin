#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "LevelEditor.h"
#include "JsEnv.h"
#include "SourceFileWatcher.h"
#include <functional>

class FEasyEditorPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	std::function<void()> OnJsEnvPreReload;
	
	std::function<void(const FString&)> Eval;

	TSparseArray<TUniquePtr<FAutoConsoleCommand>> TsConsoleCommands;

	FSimpleMulticastDelegate OnJsEnvCleanup;

private:
	TSharedPtr<puerts::FJsEnv> JsEnv;
	TSharedPtr<puerts::FSourceFileWatcher> SourceFileWatcher;

	TUniquePtr<FAutoConsoleCommand> ConsoleCommand;

	void OnPostEngineInit();

	void InitJsEnv();

	void UnInitJsEnv();

	bool Tick(float);

	bool StartupScriptCalled = false;

	void HandleMapChanged(UWorld* InWorld, EMapChangeType InMapChangeType);
};
