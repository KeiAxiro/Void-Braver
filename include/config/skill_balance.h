#pragma once

// Konstanta skill sengaja dipisah supaya balancing battle
// bisa dilakukan tanpa membongkar logika battle satu per satu.
namespace skill_balance
{
    inline constexpr double kPommelStrikeMultiplier = 1.20;
    inline constexpr int kPommelStrikeStunTurns = 1;

    inline constexpr double kIronCleaveMultiplier = 1.45;
    inline constexpr double kManaBoltMultiplier = 1.45;
    inline constexpr int kChallengerRoarDefenseBuffTurns = 2;
    inline constexpr double kImpactCraterMultiplier = 1.90;

    inline constexpr double kBloodAndIronSelfDamageRatio = 0.05;
    inline constexpr int kBloodAndIronAttackBuffTurns = 3;
    inline constexpr int kBloodAndIronDefenseBuffTurns = 3;

    inline constexpr double kShatteringOnslaughtMultiplier = 1.35;
    inline constexpr int kShatteringOnslaughtDefenseDebuffTurns = 3;

    inline constexpr double kCalamityEndMultiplier = 2.60;
    inline constexpr double kCalamityEndExecuteThreshold = 0.20;

    inline constexpr int kDominionAuraDefenseBuffTurns = 3;
    inline constexpr int kDominionAuraEnemyAttackDebuffTurns = 3;

    inline constexpr int kUndyingJuggernautInvincibleTurns = 1;
    inline constexpr int kUndyingJuggernautAttackBuffTurns = 2;

    inline constexpr double kIgnitePrimaryMultiplier = 0.80;
    inline constexpr double kIgniteDotMultiplier = 0.40;
    inline constexpr int kIgniteDotTurns = 3;

    inline constexpr int kArcaneShieldMinimum = 30;
    inline constexpr double kArcaneShieldPrimaryMultiplier = 2.5;

    inline constexpr double kChainLightningMultiplier = 1.55;
    inline constexpr int kChainLightningSilenceTurns = 1;

    inline constexpr double kFrostNovaMultiplier = 1.35;
    inline constexpr int kFrostNovaStunTurns = 1;

    inline constexpr double kMeteorFallMultiplier = 2.80;

    inline constexpr double kAbyssalSingularityMultiplier = 2.10;
    inline constexpr int kAbyssalSingularityMinimumDrain = 10;
    inline constexpr int kAbyssalSingularityDrainDivisor = 4;

    inline constexpr int kAstralRebirthMinimumManaRestore = 25;
    inline constexpr int kAstralRebirthManaRestoreDivisor = 2;
    inline constexpr int kAstralRebirthAttackBuffTurns = 2;

    inline constexpr double kVoidApocalypseMultiplier = 3.50;
    inline constexpr double kQuickDrawMultiplier = 1.25;

    inline constexpr double kToxicArrowMultiplier = 1.10;
    inline constexpr double kToxicArrowDotAgiMultiplier = 0.50;
    inline constexpr int kToxicArrowDotTurns = 3;

    inline constexpr int kAgilityBoostTurns = 3;
    inline constexpr double kShadowStrikeMultiplier = 1.75;
    inline constexpr int kPhantomVeilEvadeTurns = 1;

    inline constexpr int kBladeDanceMinHits = 3;
    inline constexpr int kBladeDanceMaxHits = 5;
    inline constexpr double kBladeDanceHitMultiplier = 0.45;

    inline constexpr int kOblivionEdgeExecuteChance = 10;
    inline constexpr double kOblivionEdgeMultiplier = 2.30;

    inline constexpr double kEternalSilenceMultiplier = 1.70;
    inline constexpr int kEternalSilenceTurns = 2;

    inline constexpr int kShadowMeldEvadeTurns = 1;
    inline constexpr double kPlayerCriticalDamageMultiplier = 1.50;
    inline constexpr double kSkillCriticalDamageMultiplier = 1.60;
} // namespace skill_balance
