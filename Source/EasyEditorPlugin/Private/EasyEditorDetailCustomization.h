// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "Services/ImguiDetailCustomization.h"
#include "EasyEditorDetailCustomization.generated.h"

/**
 * 
 */
UCLASS()
class UEasyEditorDetailCustomization : public UImguiDetailCustomization
{
	GENERATED_BODY()
	
public:
	virtual bool SupportSingleObjectEditing() override { return true; }

	virtual void OnEditSingleObject(UObject* InObject) override;

	virtual bool IsSupportClass(UClass* InClass) override;
	
	void AddOnDetailDraw(UClass* InClass, std::function<void(UObject* InObject)> InOnDraw)
	{
		DrawCallbacks.Add(InClass, InOnDraw);
	}

	void RemoveOnDetailDraw(UClass* InClass)
	{
		DrawCallbacks.Remove(InClass);
	}

	void ClearOnDetailDraw()
	{
		DrawCallbacks.Empty();
	}

private:
	TMap<UClass*, std::function<void(UObject* InObject)>> DrawCallbacks;
	
};
