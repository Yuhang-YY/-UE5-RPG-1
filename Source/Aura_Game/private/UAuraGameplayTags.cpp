// Fill out your copyright notice in the Description page of Project Settings.


#include "UAuraGameplayTags.h"
#include"GameplayTagsManager.h"

FUAuraGameplayTags FUAuraGameplayTags::GameplayTags;

void FUAuraGameplayTags::InitializeNativeGameplayTags()
{
	//primary
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Strength"),FString("增加物理伤害"));
	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Intelligence"),FString("增加魔法伤害"));
	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Resilience"),FString("增加护甲和护甲穿透"));
	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Vigor"),FString("增加健康"));
	
	//Secondary
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"),FString("减少伤害"));
	GameplayTags.Attribute_Secondary_ArmorPenetration=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ArmorPenetration"),FString("减少承受的伤害，提高格挡几率"));
	GameplayTags.Attribute_Secondary_BlockChance=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.BlockChance"),FString("格挡几率：受到攻击时触发格挡的概率"));
	GameplayTags.Attribute_Secondary_CriticalHitChance=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChance"),FString("暴击几率：攻击造成暴击的概率"));
	GameplayTags.Attribute_Secondary_CriticalHitDamage=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitDamage"),FString("暴击伤害：暴击时额外造成的伤害"));
	GameplayTags.Attribute_Secondary_CriticalHitResistance=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitResistance"),FString("暴击抗性：降低被暴击的概率或暴击伤害"));
	GameplayTags.Attribute_Secondary_HealthRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.HealthRegeneration"),FString("生命恢复：每秒恢复的生命值"));
	GameplayTags.Attribute_Secondary_ManaRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ManaRegeneration"),FString("法力恢复：每秒恢复的法力值"));
	GameplayTags.Attribute_Secondary_MaxHealth=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"),FString("最大生命值：角色可拥有的生命值上限"));
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxMana"),FString("最大法力值：角色可拥有的法力值上限"));
	
	//Input
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.LMB"),FString("鼠标左键标签"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.RMB"),FString("鼠标右键标签"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.1"),FString("1键标签"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.2"),FString("2键标签"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.3"),FString("3键标签"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.4"),FString("4键标签"));
	
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Passive.1"),FString("鼠标左键标签"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Passive.2"),FString("鼠标右键标签"));
	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),FString("Damage"));
	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"),FString("Hit React"));
	
	//Resistance
	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Fire"),FString(""));
	GameplayTags.Attribute_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Lightning"),FString(""));
	GameplayTags.Attribute_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Arcane"),FString(""));
	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Physical"),FString(""));
	
	//DamageType
	GameplayTags.DamageTag_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageTag.Fire"),FString("Fire Damage Type"));
	GameplayTags.DamageTag_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageTag.Lightning"),FString("Lightning Damage Type"));
	GameplayTags.DamageTag_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageTag.Arcane"),FString("Arcane Damage Type"));
	GameplayTags.DamageTag_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageTag.Physical"),FString("Physical Damage Type"));
	
	//Debuff
	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Chance"),FString("Fire Damage Type"));
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Frequency"),FString("Lightning Damage Type"));
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Damage"),FString("Arcane Damage Type"));
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Duration"),FString("Physical Damage Type"));
	
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn"),FString("Fire Damage Type"));
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"),FString("Lightning Damage Type"));
	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Arcane"),FString("Arcane Damage Type"));
	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Physical"),FString("Physical Damage Type"));
	
	GameplayTags.DamageTypesTOResistances.Add(GameplayTags.DamageTag_Fire,GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypesTOResistances.Add(GameplayTags.DamageTag_Lightning,GameplayTags.Attribute_Resistance_Lightning);
	GameplayTags.DamageTypesTOResistances.Add(GameplayTags.DamageTag_Arcane,GameplayTags.Attribute_Resistance_Arcane);
	GameplayTags.DamageTypesTOResistances.Add(GameplayTags.DamageTag_Physical,GameplayTags.Attribute_Resistance_Physical);
	
	GameplayTags.DamageTypesTODebuffs.Add(GameplayTags.DamageTag_Fire,GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesTODebuffs.Add(GameplayTags.DamageTag_Lightning,GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesTODebuffs.Add(GameplayTags.DamageTag_Arcane,GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesTODebuffs.Add(GameplayTags.DamageTag_Physical,GameplayTags.Debuff_Physical);
	
	//CombatSocket
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"),FString());
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"),FString());
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"),FString());
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"),FString());
	
	//Montage
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"),FString());
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"),FString());
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"),FString());
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"),FString());
	
	GameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Attack"),FString());
	GameplayTags.Ability_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Summon"),FString());
	GameplayTags.Ability_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.None"),FString());
	
	
	GameplayTags.Ability_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.HitReact"),FString());
	GameplayTags.Ability_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Status.Locked"),FString());
	GameplayTags.Ability_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Status.UnLocked"),FString());
	GameplayTags.Ability_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Status.Eligible"),FString());
	GameplayTags.Ability_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Status.Equipped"),FString());
	
	GameplayTags.Ability_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Type.None"),FString());
	GameplayTags.Ability_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Type.Passive"),FString());
	GameplayTags.Ability_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Type.Offensive"),FString());
	
	//主动技能标签
	GameplayTags.Ability_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Fire.FireBolt"),FString());
	GameplayTags.Ability_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Fire.FireBlast"),FString());
	GameplayTags.Ability_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Lightning.Electrocute"),FString());
	GameplayTags.Ability_Arcane_ArcaneShard = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Arcane.ArcaneShard"),FString());
	
	
	
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"),FString());
	
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"),FString());
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"),FString());
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"),FString());
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"),FString());
	
	//被动技能标签
	GameplayTags.Ability_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Passive.HaloOfProtection"),FString());
	GameplayTags.Ability_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Passive.LifeSiphon"),FString());
	GameplayTags.Ability_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Passive.ManaSiphon"),FString());
	
	//Meta
	GameplayTags.Attribute_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Meta.IncomingXP"),FString());
}