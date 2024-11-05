// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGAS/Public/AbilitySystem/AttributeSets/AG_AttributeSetBase.h"


//1.当属性变化后在GameplayEffectExecute()对某个Attribute的BaseValue修改之后才会触发
//2.当postGameplayEffectExecute()调用的时候，只会发生在服务器上
/*
 * 例如，我们要求出收到伤害后的最终血量，生命值的Attribute - 实际对我的伤害Attribute，如果有护盾的Attribute的话，我们可以在这里先让护盾值Attribute - 实际对我的伤害Attribute
 */
void UAG_AttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	//传递GE处理属性范围
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth((FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth())));
	}
}

//1.属性变化前(PreAttributeChange)主要是响应Attribute中的CurrentValue的修改发生之前的调用
//2.这里最好就是去做一些对输入的限制和调整，限制到某个合理的区间范围
/*
 * 注意Epic的对PreAttributeChange()的注释提到，不要使用它来处理游玩相关的时间，而只是把它用作数值的修正和处理
 * 监听Attribute的变化而产生的和游玩相关的事件(译者注：比如生命值、弹药数等属性的、UI响事件)的推荐的处理方案是：
 * UAbilitySystemComponent::GetGameplayAttributeValueChangeDelegate(FGameplayAttribute Attribute)
 */
void UAG_AttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		//GetMaxHealth()是获取AttributeSetBase中定义的属性，GetMaxHealth()是AttributeSetBase中定义的函数
		// NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UAG_AttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	//可以在ReNotify函数中使用，以处理客户端进行预测性修改的属性
	//这个宏的作用是确保在服务器上，属性值发生变化时，客户端也会收到这个通知，从而更新UI
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAG_AttributeSetBase, Health,  OldHealth);
}

void UAG_AttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAG_AttributeSetBase, MaxHealth,  OldMaxHealth);
}

void UAG_AttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//1.DOREPLIFETIME_CONDITION_NOTIFY宏主要用于带有通知的属性同步
	//而DOREPLIFETIME_CONDITION宏主要用于不带通知的属性同步

	//2.COND_None表示属性同步条件为None，这意味着该属性始终被同步，而不开率某种特殊情况
	//除了COND_None，还有其他的同步方式，例如COND_InitialOnly，COND_OwnerOnly和COND_SkipOwner等，这些回根据不同的条件触发属性同步
	//COND_InitialOnly表示属性同步条件为InitialOnly，这意味着该属性仅在初始化时同步，而不在之后的任何时刻同步
	//COND_OwnerOnly表示属性同步条件为OwnerOnly，这意味着该属性仅在属性拥有者（即拥有者角色）时同步，而不在其他角色的同步中同步
	//COND_SkipOwner表示属性同步条件为SkipOwner，这意味着该属性仅在属性拥有者（即拥有者角色）以外的角色时同步，而不在属性拥有者（即拥有者角色）的同步中同步
	//REPNOTIFY_Always表示属性同步通知为Always

	//3.REPNOTIFY_Always是默认的属性同步通知，表示属性更改时始终触发属性同步，告诉OnRep函数当本地值和服务器的值相同的时候也去进行相应的触发。
	//默认情况下(即这里不用REPNOTIFY_Always的情况下)这两个值一样的时候不会触发OnRep函数，但是REPNOTIFY_Always会触发OnRep函数。
	
	// DOREPLIFETIME_CONDITION(UAG_Attribute, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAG_AttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAG_AttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
}
