// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionGAS/ActionGameTypes.h"
#include "Engine/DataAsset.h"
#include "CharacterDataAsset.generated.h"

/**
 * 
 */
//1.BlueprintType是用于声明C++的结构体、枚举、类等是允许在蓝图中使用，并且可以直接在蓝图中实例化
//2.Blueprintable是用于声明C++的类是允许在被蓝图继承 
UCLASS(BlueprintType, Blueprintable)
class ACTIONGAS_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FCharacterData CharacterData;
	
	
};
