// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionGASCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AActionGASCharacter

void AActionGASCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AActionGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AActionGASCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AActionGASCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AActionGASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AActionGASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

AActionGASCharacter::AActionGASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)



	//初始化能力和属性集
	AbilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));
	
	AbilitySystemComponent->SetIsReplicated(true);
	//以下时GE网络同步的前提
	/*
	 * GAS组件网络模式必须是Mixed或Full，才会完整地同步在AbilitySystemComponent.h中有EGameplayEffectReplicationMode的定义，
	 * 可以通过UAbilitySystemComponentBase::SetReplicationMode()函数设置组件实力的网络模式
	 * 
	 * 根据以上三种模式的注释可以知道(minimal gameplay effect info 指的是GameplayTag和GameplayCue)
	 * Minimal不会完整地同步GE到客户端，只会同步GameplayTag和GameplayCue，
	 * Mixed只会完整地同步GE到客户端的owner代理（proxy），其他代理是不完整同步
	 * 而Full则完整地同步所有GameplayEffect，
	 * 
	 */

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAG_AttributeSetBase>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AActionGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//PostInitializeComponents()函数会在组件(如静态网格组件、动画组件、声音组件等)完成初始化之后立即被调用
void AActionGASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (IsValid(CharacterDataAsset))
	{
		SetCharacterData(CharacterDataAsset->CharacterData);
	}
}

void AActionGASCharacter::OnRep_CharacterData()
{
	InitFromCharacterData(CharacterData, true);
}

FCharacterData AActionGASCharacter::GetCharacterData() const
{
	return CharacterData;
}

void AActionGASCharacter::SetCharacterData(const FCharacterData& InCharacterData)
{
	CharacterData = InCharacterData;
	InitFromCharacterData(CharacterData, true);
}


void AActionGASCharacter::InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication) const
{
}


bool AActionGASCharacter::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect,
                                                    FGameplayEffectContextHandle InEffectContext)
{
	if (!Effect.Get())
		return false;

	//1. The GameplayEffectSpec(GESpc) 可以认为是GameplayEffects的实例化
	//UAbilitySystemComponentBase::MakeOutgoingSpec()专门用于从GameplayEffects创建GameplayEffectSpec
	//在应用一个GameplayEffect时，会先从GameplayEffect创建一个GameplayEffectSpec出来，然后实际上是把GameplayEffectSpec应用给目标
	//2. GameplayEffectSpec可以不是必须立即应用，通常是将GameplayEffectSpec传递给由技能创建的子弹，然后当子弹击中目标时将具体技能效果GameplayEffect应用给目标
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, InEffectContext);

	if (SpecHandle.IsValid())
	{
		//应用 FGameplayEffectSpec给目标 !spec !Handle
		FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		//返回应用效果的结果
		return  ActiveHandle.WasSuccessfullyApplied();
	}
	return false;
}

// //初始化属性集
// void AActionGASCharacter::InitializeAttributes()
// {
// 	if (GetLocalRole() == ROLE_Authority && DefaultAttributeSet && AttributeSet)
// 	{
// 		//创建GE效果上下文 实际就是FE的Params配置
// 		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
// 		// AddSourceObject()是EffectContext中的一个方法，用于向该效果上下文中添加一个源对象，源对象一般是指需要被处理的目标对象，暂时设置为de
// 		EffectContext.AddSourceObject(this);
// 		ApplyGameplayEffectToSelf(DefaultAttributeSet.Get(), EffectContext);
// 	}
// }

//授予->能力GA
void AActionGASCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (auto DefaultAbility : CharacterData.Abilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
	}
}

void AActionGASCharacter::ApplyStartuoEffects()
{
	//初始化属性集
	if (GetLocalRole() == ROLE_Authority )
	{
		//创建GE效果上下文 实际就是GE的Params配置
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		// AddSourceObject()是EffectContext中的一个方法，用于向该效果上下文中添加一个源对象，源对象一般是指需要被处理的目标对象，暂时设置为de
		EffectContext.AddSourceObject(this);

		//默认的GE列表
		for (auto DefaultEffect : CharacterData.Effects)
		{
			//应用GE
			ApplyGameplayEffectToSelf(DefaultEffect.Get(), EffectContext);
		}
	}
}

void AActionGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//只在服务器执行此函数

	//初始化服务器GAS
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GiveAbilities();

	// InitializeAttributes();
	
	ApplyStartuoEffects();
}

void AActionGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//初始化客户端GAS
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	//InitializeAttributes();
}

void AActionGASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActionGASCharacter, CharacterData);
}