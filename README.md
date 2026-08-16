[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Modules for [cs2-vip](https://github.com/bywinsty/cs2-vip)

Optional C++ modules for the [cs2-vip core](https://github.com/bywinsty/cs2-vip). Each module registers its own feature in the core through `IVIPApi`; install only the modules you need.

The `dev` branch is the development version. Use a matching core/module release when possible.

## Requirements

- A CS2 dedicated server with [Metamod:Source](https://www.sourcemm.net/) installed.
- The [cs2-vip core](https://github.com/bywinsty/cs2-vip) installed and loaded before the modules.
- The core's database dependency, `sql_mm`, and a configured `addons/configs/databases.cfg` when using the core's database-backed features.
- `VIP_Fortnite_Hits` additionally requires the Fortnite Hits plugin/API.

## Installation

1. Install and configure the core first.
2. Extract a module release archive into the server's `game/csgo` directory, preserving its paths:

   ```text
   addons/
   ├── metamod/<module>.vdf
   └── vip_modules/<module>.so
   ```

3. Add the module's feature keys to `addons/configs/vip/groups.ini` and its translation keys to `addons/translations/vip.phrases.txt` when the module README requires them.
4. Create or edit any module-specific files under `addons/configs/vip/`.
5. Restart the server and verify that both the core and the selected module load successfully.

The package manifest on this branch declares the files included in each module package. Some modules load a configuration file that must be created manually; those cases are called out below.

## Modules

| Module | EN | UA | RU |
| --- | --- | --- | --- |
| VIP_AntiFlash | [README](VIP_AntiFlash/README.md) | [README-UA](VIP_AntiFlash/README-UA.md) | [README-RU](VIP_AntiFlash/README-RU.md) |
| VIP_AwpManager | [README](VIP_AwpManager/README.md) | [README-UA](VIP_AwpManager/README-UA.md) | [README-RU](VIP_AwpManager/README-RU.md) |
| VIP_BaseFunctions | [README](VIP_BaseFunctions/README.md) | [README-UA](VIP_BaseFunctions/README-UA.md) | [README-RU](VIP_BaseFunctions/README-RU.md) |
| VIP_Bhop | [README](VIP_Bhop/README.md) | [README-UA](VIP_Bhop/README-UA.md) | [README-RU](VIP_Bhop/README-RU.md) |
| VIP_BuyTeamWeapon | [README](VIP_BuyTeamWeapon/README.md) | [README-UA](VIP_BuyTeamWeapon/README-UA.md) | [README-RU](VIP_BuyTeamWeapon/README-RU.md) |
| VIP_DuckSpeed | [README](VIP_DuckSpeed/README.md) | [README-UA](VIP_DuckSpeed/README-UA.md) | [README-RU](VIP_DuckSpeed/README-RU.md) |
| VIP_Endurance | [README](VIP_Endurance/README.md) | [README-UA](VIP_Endurance/README-UA.md) | [README-RU](VIP_Endurance/README-RU.md) |
| VIP_FastDefuse | [README](VIP_FastDefuse/README.md) | [README-UA](VIP_FastDefuse/README-UA.md) | [README-RU](VIP_FastDefuse/README-RU.md) |
| VIP_FastPlant | [README](VIP_FastPlant/README.md) | [README-UA](VIP_FastPlant/README-UA.md) | [README-RU](VIP_FastPlant/README-RU.md) |
| VIP_FOV | [README](VIP_FOV/README.md) | [README-UA](VIP_FOV/README-UA.md) | [README-RU](VIP_FOV/README-RU.md) |
| VIP_Fortnite_Hits | [README](VIP_Fortnite_Hits/README.md) | [README-UA](VIP_Fortnite_Hits/README-UA.md) | [README-RU](VIP_Fortnite_Hits/README-RU.md) |
| VIP_Gravity | [README](VIP_Gravity/README.md) | [README-UA](VIP_Gravity/README-UA.md) | [README-RU](VIP_Gravity/README-RU.md) |
| VIP_Healthshot | [README](VIP_Healthshot/README.md) | [README-UA](VIP_Healthshot/README-UA.md) | [README-RU](VIP_Healthshot/README-RU.md) |
| VIP_Items | [README](VIP_Items/README.md) | [README-UA](VIP_Items/README-UA.md) | [README-RU](VIP_Items/README-RU.md) |
| VIP_JoinSound | [README](VIP_JoinSound/README.md) | [README-UA](VIP_JoinSound/README-UA.md) | [README-RU](VIP_JoinSound/README-RU.md) |
| VIP_Jumps | [README](VIP_Jumps/README.md) | [README-UA](VIP_Jumps/README-UA.md) | [README-RU](VIP_Jumps/README-RU.md) |
| VIP_KillScreen | [README](VIP_KillScreen/README.md) | [README-UA](VIP_KillScreen/README-UA.md) | [README-RU](VIP_KillScreen/README-RU.md) |
| VIP_MultiTest | [README](VIP_MultiTest/README.md) | [README-UA](VIP_MultiTest/README-UA.md) | [README-RU](VIP_MultiTest/README-RU.md) |
| VIP_NoFallDamage | [README](VIP_NoFallDamage/README.md) | [README-UA](VIP_NoFallDamage/README-UA.md) | [README-RU](VIP_NoFallDamage/README-RU.md) |
| VIP_Penis | [README](VIP_Penis/README.md) | [README-UA](VIP_Penis/README-UA.md) | [README-RU](VIP_Penis/README-RU.md) |
| VIP_RainbowModel | [README](VIP_RainbowModel/README.md) | [README-UA](VIP_RainbowModel/README-UA.md) | [README-RU](VIP_RainbowModel/README-RU.md) |
| VIP_RegenArmor | [README](VIP_RegenArmor/README.md) | [README-UA](VIP_RegenArmor/README-UA.md) | [README-RU](VIP_RegenArmor/README-RU.md) |
| VIP_RegenHP | [README](VIP_RegenHP/README.md) | [README-UA](VIP_RegenHP/README-UA.md) | [README-RU](VIP_RegenHP/README-RU.md) |
| VIP_ResetDeaths | [README](VIP_ResetDeaths/README.md) | [README-UA](VIP_ResetDeaths/README-UA.md) | [README-RU](VIP_ResetDeaths/README-RU.md) |
| VIP_Respawn | [README](VIP_Respawn/README.md) | [README-UA](VIP_Respawn/README-UA.md) | [README-RU](VIP_Respawn/README-RU.md) |
| VIP_ShowDamage | [README](VIP_ShowDamage/README.md) | [README-UA](VIP_ShowDamage/README-UA.md) | [README-RU](VIP_ShowDamage/README-RU.md) |
| VIP_Skins | [README](VIP_Skins/README.md) | [README-UA](VIP_Skins/README-UA.md) | [README-RU](VIP_Skins/README-RU.md) |
| VIP_Smoke | [README](VIP_Smoke/README.md) | [README-UA](VIP_Smoke/README-UA.md) | [README-RU](VIP_Smoke/README-RU.md) |
| VIP_Speed | [README](VIP_Speed/README.md) | [README-UA](VIP_Speed/README-UA.md) | [README-RU](VIP_Speed/README-RU.md) |
| VIP_Tag | [README](VIP_Tag/README.md) | [README-UA](VIP_Tag/README-UA.md) | [README-RU](VIP_Tag/README-RU.md) |
| VIP_TeammatesHeal | [README](VIP_TeammatesHeal/README.md) | [README-UA](VIP_TeammatesHeal/README-UA.md) | [README-RU](VIP_TeammatesHeal/README-RU.md) |
| VIP_Test | [README](VIP_Test/README.md) | [README-UA](VIP_Test/README-UA.md) | [README-RU](VIP_Test/README-RU.md) |
| VIP_Time | [README](VIP_Time/README.md) | [README-UA](VIP_Time/README-UA.md) | [README-RU](VIP_Time/README-RU.md) |
| VIP_Vampirism | [README](VIP_Vampirism/README.md) | [README-UA](VIP_Vampirism/README-UA.md) | [README-RU](VIP_Vampirism/README-RU.md) |
| VIP_Vips | [README](VIP_Vips/README.md) | [README-UA](VIP_Vips/README-UA.md) | [README-RU](VIP_Vips/README-RU.md) |
