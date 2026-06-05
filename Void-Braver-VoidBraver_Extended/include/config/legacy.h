#pragma once

#include "paths.h"
#include "rules.h"
#include "player.h"
#include "enemy.h"
#include "skill.h"

// ============================================================================
// JEMBATAN NAMESPACE LAMA
// ============================================================================
namespace game_paths {
    inline constexpr const char *kGameDataPath = Config::Paths::GAME_DATA;
    inline constexpr const char *kSaveFilePath = Config::Paths::SAVE_FILE;
}
namespace game_rules {
    inline constexpr int kStartingGold = Config::Rules::STARTING_GOLD;
    inline constexpr int kStartingHp = Config::Rules::STARTING_HP;
    inline constexpr int kStartingMp = Config::Rules::STARTING_MP;
    inline constexpr int kStartingStatPoints = Config::Rules::STARTING_STAT_POINTS;
    inline constexpr int kStatPointsPerLevel = Config::Rules::STAT_POINTS_PER_LEVEL;
    inline constexpr int kDefaultPageSize = Config::Rules::DEFAULT_PAGE_SIZE;
    inline constexpr int kMoveStepsPerDepth = Config::Rules::MOVE_STEPS_PER_DEPTH;
    inline constexpr int kMinEncountersPerMove = Config::Rules::MIN_ENCOUNTERS_PER_MOVE;
    inline constexpr int kMaxEncountersPerMove = Config::Rules::MAX_ENCOUNTERS_PER_MOVE;
    inline constexpr const char *kMenuSeparator = Config::Rules::MENU_SEPARATOR;
    inline constexpr const char *kTitleSeparator = Config::Rules::TITLE_SEPARATOR;
}
namespace player_balance {
    inline constexpr int kMaxEffectiveDefenseForDamage = Config::Player::MAX_EFFECTIVE_DEFENSE_FOR_DAMAGE;
    inline constexpr int kMaxEffectiveDefenseForEnemyDamage = Config::Player::MAX_EFFECTIVE_PLAYER_DEFENSE_FOR_ENEMY_DAMAGE;
    inline constexpr int kDefenseReducedPlayerDamageFloorDenominator = Config::Player::DEFENSE_REDUCED_PLAYER_DAMAGE_FLOOR_DENOMINATOR;



    inline constexpr int kBaseHpStart = Config::Player::BASE_HP_START;
    inline constexpr int kBaseHpPerLevel = Config::Player::BASE_HP_PER_LEVEL;
    inline constexpr int kBaseHpPerVit = Config::Player::BASE_HP_PER_VIT;
    inline constexpr int kBaseHpPerStr = Config::Player::BASE_HP_PER_STR;
    inline constexpr int kBaseMpStart = Config::Player::BASE_MP_START;
    inline constexpr int kBaseMpPerLevel = Config::Player::BASE_MP_PER_LEVEL;
    inline constexpr int kBaseMpPerInt = Config::Player::BASE_MP_PER_INT;
    inline constexpr int kBaseMpAgiDivisor = Config::Player::BASE_MP_AGI_DIVISOR;
    inline constexpr int kBaseMpPerTwoAgi = Config::Player::BASE_MP_PER_TWO_AGI;
    inline constexpr int kAttackBasePerLevel = Config::Player::ATTACK_BASE_PER_LEVEL;
    inline constexpr int kAttackPrimaryDivisor = Config::Player::ATTACK_PRIMARY_DIVISOR;
    inline constexpr int kAttackStrDivisor = Config::Player::ATTACK_STR_DIVISOR;
    inline constexpr int kAttackAgiDivisor = Config::Player::ATTACK_AGI_DIVISOR;
    inline constexpr double kAttackEquipmentMultiplier = Config::Player::ATTACK_EQUIPMENT_MULTIPLIER;
    inline constexpr double kAttackBuffMultiplier = Config::Player::ATTACK_BUFF_MULTIPLIER;
    inline constexpr int kAttackJitterMin = Config::Player::ATTACK_JITTER_MIN;
    inline constexpr int kAttackJitterMax = Config::Player::ATTACK_JITTER_MAX;
    inline constexpr int kDefenseBasePerLevel = Config::Player::DEFENSE_BASE_PER_LEVEL;
    inline constexpr int kDefenseVitDivisor = Config::Player::DEFENSE_VIT_DIVISOR;
    inline constexpr int kDefenseStrDivisor = Config::Player::DEFENSE_STR_DIVISOR;
    inline constexpr double kDefenseEquipmentMultiplier = Config::Player::DEFENSE_EQUIPMENT_MULTIPLIER;
    inline constexpr double kDefenseBuffMultiplier = Config::Player::DEFENSE_BUFF_MULTIPLIER;
    inline constexpr int kSpeedBasePerLevel = Config::Player::SPEED_BASE_PER_LEVEL;
    inline constexpr int kSpeedPerAgi = Config::Player::SPEED_PER_AGI;
    inline constexpr double kSpeedBuffMultiplier = Config::Player::SPEED_BUFF_MULTIPLIER;
    inline constexpr double kCritBaseRate = Config::Player::CRIT_BASE_RATE;
    inline constexpr double kCritPerAgi = Config::Player::CRIT_PER_AGI;
    inline constexpr double kCritCap = Config::Player::CRIT_CAP;
    inline constexpr int kPrimaryStatBonusOnNewGame = Config::Player::PRIMARY_STAT_BONUS_ON_NEW_GAME;
    inline constexpr int kDefeatRecoveryDivisor = Config::Player::DEFEAT_RECOVERY_DIVISOR;
    inline constexpr int kMinAttack = Config::Player::MIN_ATTACK;
    inline constexpr int kMinDefense = Config::Player::MIN_DEFENSE;
    inline constexpr int kMinSpeed = Config::Player::MIN_SPEED;
    inline constexpr int kMinDamage = Config::Player::MIN_DAMAGE;
    inline constexpr int kResourceFloor = Config::Player::RESOURCE_FLOOR;
    inline constexpr int kMinRecoveryResource = Config::Player::MIN_RECOVERY_RESOURCE;
    inline constexpr double kLevelUpEarlyMultiplier = Config::Player::LEVEL_UP_EARLY_MULTIPLIER;
    inline constexpr int kLevelUpMidStartLevel = Config::Player::LEVEL_UP_MID_START_LEVEL;
    inline constexpr double kLevelUpMidMultiplier = Config::Player::LEVEL_UP_MID_MULTIPLIER;
    inline constexpr int kLevelUpLateStartLevel = Config::Player::LEVEL_UP_LATE_START_LEVEL;
    inline constexpr double kLevelUpLateMultiplier = Config::Player::LEVEL_UP_LATE_MULTIPLIER;
    inline constexpr int kLevelUpEndgameStartLevel = Config::Player::LEVEL_UP_ENDGAME_START_LEVEL;
    inline constexpr double kLevelUpEndgameMultiplier = Config::Player::LEVEL_UP_ENDGAME_MULTIPLIER;
    inline constexpr int kExpFallbackLinearMultiplier = Config::Player::EXP_FALLBACK_LINEAR_MULTIPLIER;
    inline constexpr int kExpFallbackQuadraticMultiplier = Config::Player::EXP_FALLBACK_QUADRATIC_MULTIPLIER;
    inline constexpr int kMinimumExpRequirement = Config::Player::MINIMUM_EXP_REQUIREMENT;
}
namespace enemy_balance {
    inline constexpr int kFallbackHpBase = Config::Enemy::FALLBACK_HP_BASE;
    inline constexpr int kFallbackHpPerDepth = Config::Enemy::FALLBACK_HP_PER_DEPTH;
    inline constexpr int kFallbackMpBase = Config::Enemy::FALLBACK_MP_BASE;
    inline constexpr int kFallbackMpPerDepth = Config::Enemy::FALLBACK_MP_PER_DEPTH;
    inline constexpr int kFallbackAtkBase = Config::Enemy::FALLBACK_ATK_BASE;
    inline constexpr int kFallbackAtkPerDepth = Config::Enemy::FALLBACK_ATK_PER_DEPTH;
    inline constexpr int kFallbackDefBase = Config::Enemy::FALLBACK_DEF_BASE;
    inline constexpr int kFallbackDefPerDepth = Config::Enemy::FALLBACK_DEF_PER_DEPTH;
    inline constexpr int kFallbackExpBase = Config::Enemy::FALLBACK_EXP_BASE;
    inline constexpr int kFallbackExpPerDepth = Config::Enemy::FALLBACK_EXP_PER_DEPTH;
    inline constexpr double kFallbackCritRate = Config::Enemy::FALLBACK_CRIT_RATE;
    inline constexpr int kTemplateHpMinFallback = Config::Enemy::TEMPLATE_HP_MIN_FALLBACK;
    inline constexpr int kTemplateHpMaxFallback = Config::Enemy::TEMPLATE_HP_MAX_FALLBACK;
    inline constexpr int kTemplateMpMinFallback = Config::Enemy::TEMPLATE_MP_MIN_FALLBACK;
    inline constexpr int kTemplateMpMaxFallback = Config::Enemy::TEMPLATE_MP_MAX_FALLBACK;
    inline constexpr int kTemplateAtkMinFallback = Config::Enemy::TEMPLATE_ATK_MIN_FALLBACK;
    inline constexpr int kTemplateAtkMaxFallback = Config::Enemy::TEMPLATE_ATK_MAX_FALLBACK;
    inline constexpr int kTemplateDefMinFallback = Config::Enemy::TEMPLATE_DEF_MIN_FALLBACK;
    inline constexpr int kTemplateDefMaxFallback = Config::Enemy::TEMPLATE_DEF_MAX_FALLBACK;
    inline constexpr double kTemplateCritRateFallback = Config::Enemy::TEMPLATE_CRIT_RATE_FALLBACK;
    inline constexpr int kDefaultExpDrop = Config::Enemy::DEFAULT_EXP_DROP;
    inline constexpr int kDefaultDropRate = Config::Enemy::DEFAULT_DROP_RATE;
    inline constexpr int kDropQuantity = Config::Enemy::DROP_QUANTITY;
    inline constexpr double kRegularEnemyAttackMultiplier = Config::Enemy::REGULAR_ENEMY_ATTACK_MULTIPLIER;
    inline constexpr double kBossEnemyAttackMultiplier = Config::Enemy::BOSS_ENEMY_ATTACK_MULTIPLIER;
    inline constexpr double kEnemyAttackDebuffMultiplier = Config::Enemy::ENEMY_ATTACK_DEBUFF_MULTIPLIER;
    inline constexpr double kEnemyDefenseDebuffMultiplier = Config::Enemy::ENEMY_DEFENSE_DEBUFF_MULTIPLIER;
    inline constexpr int kEnemySpeedPerLevel = Config::Enemy::ENEMY_SPEED_PER_LEVEL;
    inline constexpr int kEnemySpeedAttackDivisor = Config::Enemy::ENEMY_SPEED_ATTACK_DIVISOR;
    inline constexpr int kEnemyDamageJitterMin = Config::Enemy::ENEMY_DAMAGE_JITTER_MIN;
    inline constexpr int kEnemyDamageJitterMax = Config::Enemy::ENEMY_DAMAGE_JITTER_MAX;
    inline constexpr double kEnemyCriticalDamageMultiplier = Config::Enemy::ENEMY_CRITICAL_DAMAGE_MULTIPLIER;
    inline constexpr int kEnemyOpeningAmbushSpeedBonus = Config::Enemy::ENEMY_OPENING_AMBUSH_SPEED_BONUS;
    inline constexpr int kMinDamage = Config::Enemy::MIN_DAMAGE;
    inline constexpr int kMinDefense = Config::Enemy::MIN_DEFENSE;
    inline constexpr double kBossHpMultiplier = Config::Enemy::BOSS_HP_MULTIPLIER;
    inline constexpr double kBossAtkMultiplier = Config::Enemy::BOSS_ATK_MULTIPLIER;
    inline constexpr double kBossDefMultiplier = Config::Enemy::BOSS_DEF_MULTIPLIER;
    inline constexpr double kBossMinimumCritRate = Config::Enemy::BOSS_MINIMUM_CRIT_RATE;
    inline constexpr int kExpDropPerEnemyLevel = Config::Enemy::EXP_DROP_PER_ENEMY_LEVEL;
    inline constexpr int kExpDropPerDepth = Config::Enemy::EXP_DROP_PER_DEPTH;
    inline constexpr int kBossExpBonus = Config::Enemy::BOSS_EXP_BONUS;
    inline constexpr int kBattleGoldMinimum = Config::Enemy::BATTLE_GOLD_MINIMUM;
    inline constexpr int kBattleGoldPerEnemyLevel = Config::Enemy::BATTLE_GOLD_PER_ENEMY_LEVEL;
    inline constexpr int kBattleGoldAtkDivisor = Config::Enemy::BATTLE_GOLD_ATK_DIVISOR;
    inline constexpr int kDepthClearGoldMinimum = Config::Enemy::DEPTH_CLEAR_GOLD_MINIMUM;
    inline constexpr int kDepthClearExpMinimum = Config::Enemy::DEPTH_CLEAR_EXP_MINIMUM;
    inline constexpr int kDepthClearExpNumerator = Config::Enemy::DEPTH_CLEAR_EXP_NUMERATOR;
    inline constexpr int kDepthClearExpDenominator = Config::Enemy::DEPTH_CLEAR_EXP_DENOMINATOR;
    inline constexpr int kDepthClearGoldDivisor = Config::Enemy::DEPTH_CLEAR_GOLD_DIVISOR;
}
namespace skill_balance {
    inline constexpr int kFirstSkillIndex = Config::Skill::FIRST_SKILL_INDEX;
    inline constexpr int kSkillTreeSplitDivisor = Config::Skill::SKILL_TREE_SPLIT_DIVISOR;
    inline constexpr const int *kUnlockLevels = Config::Skill::UNLOCK_LEVELS;
    inline constexpr int kUnlockLevelCount = Config::Skill::UNLOCK_LEVEL_COUNT;
    inline constexpr int kNoDamage = Config::Skill::NO_DAMAGE;
    inline constexpr int kMinDamage = Config::Skill::MIN_DAMAGE;
    inline constexpr int kDefaultDefenseDivisor = Config::Skill::DEFAULT_DEFENSE_DIVISOR;
    inline constexpr int kBladeDanceDefenseDivisor = Config::Skill::BLADE_DANCE_DEFENSE_DIVISOR;
    inline constexpr double kPommelStrikeMultiplier = Config::Skill::POMMEL_STRIKE_MULTIPLIER;
    inline constexpr int kPommelStrikeStunTurns = Config::Skill::POMMEL_STRIKE_STUN_TURNS;
    inline constexpr double kIronCleaveMultiplier = Config::Skill::IRON_CLEAVE_MULTIPLIER;
    inline constexpr double kManaBoltMultiplier = Config::Skill::MANA_BOLT_MULTIPLIER;
    inline constexpr int kChallengerRoarDefenseBuffTurns = Config::Skill::CHALLENGER_ROAR_DEFENSE_BUFF_TURNS;
    inline constexpr double kImpactCraterMultiplier = Config::Skill::IMPACT_CRATER_MULTIPLIER;
    inline constexpr double kBloodAndIronSelfDamageRatio = Config::Skill::BLOOD_AND_IRON_SELF_DAMAGE_RATIO;
    inline constexpr int kBloodAndIronAttackBuffTurns = Config::Skill::BLOOD_AND_IRON_ATTACK_BUFF_TURNS;
    inline constexpr int kBloodAndIronDefenseBuffTurns = Config::Skill::BLOOD_AND_IRON_DEFENSE_BUFF_TURNS;
    inline constexpr double kShatteringOnslaughtMultiplier = Config::Skill::SHATTERING_ONSLAUGHT_MULTIPLIER;
    inline constexpr int kShatteringOnslaughtDefenseDebuffTurns = Config::Skill::SHATTERING_ONSLAUGHT_DEFENSE_DEBUFF_TURNS;
    inline constexpr double kCalamityEndMultiplier = Config::Skill::CALAMITY_END_MULTIPLIER;
    inline constexpr double kCalamityEndExecuteThreshold = Config::Skill::CALAMITY_END_EXECUTE_THRESHOLD;
    inline constexpr int kDominionAuraDefenseBuffTurns = Config::Skill::DOMINION_AURA_DEFENSE_BUFF_TURNS;
    inline constexpr int kDominionAuraEnemyAttackDebuffTurns = Config::Skill::DOMINION_AURA_ENEMY_ATTACK_DEBUFF_TURNS;
    inline constexpr int kUndyingJuggernautInvincibleTurns = Config::Skill::UNDYING_JUGGERNAUT_INVINCIBLE_TURNS;
    inline constexpr int kUndyingJuggernautAttackBuffTurns = Config::Skill::UNDYING_JUGGERNAUT_ATTACK_BUFF_TURNS;
    inline constexpr double kIgnitePrimaryMultiplier = Config::Skill::IGNITE_PRIMARY_MULTIPLIER;
    inline constexpr double kIgniteDotMultiplier = Config::Skill::IGNITE_DOT_MULTIPLIER;
    inline constexpr int kIgniteDotTurns = Config::Skill::IGNITE_DOT_TURNS;
    inline constexpr int kArcaneShieldMinimum = Config::Skill::ARCANE_SHIELD_MINIMUM;
    inline constexpr double kArcaneShieldPrimaryMultiplier = Config::Skill::ARCANE_SHIELD_PRIMARY_MULTIPLIER;
    inline constexpr double kChainLightningMultiplier = Config::Skill::CHAIN_LIGHTNING_MULTIPLIER;
    inline constexpr int kChainLightningSilenceTurns = Config::Skill::CHAIN_LIGHTNING_SILENCE_TURNS;
    inline constexpr double kFrostNovaMultiplier = Config::Skill::FROST_NOVA_MULTIPLIER;
    inline constexpr int kFrostNovaStunTurns = Config::Skill::FROST_NOVA_STUN_TURNS;
    inline constexpr double kMeteorFallMultiplier = Config::Skill::METEOR_FALL_MULTIPLIER;
    inline constexpr double kAbyssalSingularityMultiplier = Config::Skill::ABYSSAL_SINGULARITY_MULTIPLIER;
    inline constexpr int kAbyssalSingularityMinimumDrain = Config::Skill::ABYSSAL_SINGULARITY_MINIMUM_DRAIN;
    inline constexpr int kAbyssalSingularityDrainDivisor = Config::Skill::ABYSSAL_SINGULARITY_DRAIN_DIVISOR;
    inline constexpr int kAstralRebirthMinimumManaRestore = Config::Skill::ASTRAL_REBIRTH_MINIMUM_MANA_RESTORE;
    inline constexpr int kAstralRebirthManaRestoreDivisor = Config::Skill::ASTRAL_REBIRTH_MANA_RESTORE_DIVISOR;
    inline constexpr int kAstralRebirthAttackBuffTurns = Config::Skill::ASTRAL_REBIRTH_ATTACK_BUFF_TURNS;
    inline constexpr double kVoidApocalypseMultiplier = Config::Skill::VOID_APOCALYPSE_MULTIPLIER;
    inline constexpr double kQuickDrawMultiplier = Config::Skill::QUICK_DRAW_MULTIPLIER;
    inline constexpr double kToxicArrowMultiplier = Config::Skill::TOXIC_ARROW_MULTIPLIER;
    inline constexpr double kToxicArrowDotAgiMultiplier = Config::Skill::TOXIC_ARROW_DOT_AGI_MULTIPLIER;
    inline constexpr int kToxicArrowDotTurns = Config::Skill::TOXIC_ARROW_DOT_TURNS;
    inline constexpr int kAgilityBoostTurns = Config::Skill::AGILITY_BOOST_TURNS;
    inline constexpr double kShadowStrikeMultiplier = Config::Skill::SHADOW_STRIKE_MULTIPLIER;
    inline constexpr int kPhantomVeilEvadeTurns = Config::Skill::PHANTOM_VEIL_EVADE_TURNS;
    inline constexpr int kBladeDanceMinHits = Config::Skill::BLADE_DANCE_MIN_HITS;
    inline constexpr int kBladeDanceMaxHits = Config::Skill::BLADE_DANCE_MAX_HITS;
    inline constexpr double kBladeDanceHitMultiplier = Config::Skill::BLADE_DANCE_HIT_MULTIPLIER;
    inline constexpr int kOblivionEdgeExecuteChance = Config::Skill::OBLIVION_EDGE_EXECUTE_CHANCE;
    inline constexpr double kOblivionEdgeMultiplier = Config::Skill::OBLIVION_EDGE_MULTIPLIER;
    inline constexpr double kEternalSilenceMultiplier = Config::Skill::ETERNAL_SILENCE_MULTIPLIER;
    inline constexpr int kEternalSilenceTurns = Config::Skill::ETERNAL_SILENCE_TURNS;
    inline constexpr int kShadowMeldEvadeTurns = Config::Skill::SHADOW_MELD_EVADE_TURNS;
    inline constexpr double kPlayerCriticalDamageMultiplier = Config::Skill::PLAYER_CRITICAL_DAMAGE_MULTIPLIER;
    inline constexpr double kSkillCriticalDamageMultiplier = Config::Skill::SKILL_CRITICAL_DAMAGE_MULTIPLIER;
}
