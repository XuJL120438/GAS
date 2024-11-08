// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "Ability/Components/AGAbilitySystemComponentBase.h"
#include "AbilitySystem/AttributeSets/AG_AttributeSetBase.h"
#include "DataAssets/CharacterDataAsset.h"
#include "ActionGASCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAGAbilitySystemComponentBase;
class UAG_AttributeSetBase;
class UCharacterDataAsset;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AActionGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AActionGASCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual void PostInitializeComponents() override;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	//初始化技能组件和属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	UAGAbilitySystemComponentBase* AbilitySystemComponent;

	//说明该属性不会保存或者从磁盘加载，相当于该值是其他值通过计算而得，没有存储的必要，
	//网络传输序列化时，不希望这些字段被记录或传输，字段上面加transient关键字
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", Transient)
	UAG_AttributeSetBase* AttributeSet;

	// //属性 一个GE去初始化默认属性的东西
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	// TSubclassOf<UGameplayEffect> DefaultAttributeSet;
	//
	// //能力
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	// TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	//
	// //效果
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	// TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	//角色数据要开启网络复制
	UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
	FCharacterData CharacterData;

	UFUNCTION()
	void OnRep_CharacterData();
	
	UPROPERTY(EditDefaultsOnly)
	UCharacterDataAsset* CharacterDataAsset;
protected:
	// //初始化函数(Attribute,effects,abilities)
	// void InitializeAttributes();

	//授予 -> 能力 GA
	void GiveAbilities();

	//准备应用的GE列表
	void ApplyStartuoEffects();

	//角色切换
	//只发生服务器
	virtual void PossessedBy(AController* NewController) override;

	// 在客户端PlayerController的OnRep_PlayerState()函数中初始化
	virtual void OnRep_PlayerState() override;
public:

public:
	UFUNCTION(BlueprintCallable)
	FCharacterData GetCharacterData() const;

	UFUNCTION(BlueprintCallable)
	void SetCharacterData(const FCharacterData& InCharacterData);
	
	//对自己应用GE
	bool ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContext);

	//子类去实现的虚方法，方便之后重写此方法修改器
	virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false) const;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

