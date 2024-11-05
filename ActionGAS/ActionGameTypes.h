// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionGameTypes.generated.h"

//1.BlueprintType是用于声明C++的结构体、枚举、类等是允许在蓝图中使用，并且可以直接在蓝图中实例化

USTRUCT(BlueprintType)
struct FCharacterData
{
	//1.GENERATED_BODY()和GENERATED_UCLASS_BODY()宏定义在类声明中使用
	//2.GENERATED_USTRUCT_BODY()宏定义在结构体声明中使用

	//用于生成类的一些标准方法(例如Construct、Tick、BeginPlay等)
	GENERATED_BODY()
	// GENERATED_UCLASS_BODY()
	// GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> Effects;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;
};
