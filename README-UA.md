[EN](README.md) | [RU](README-RU.md)
# [VIP] [Модулі](https://github.com/bywinsty/cs2-vip-modules) для [VIP] [Ядра](https://github.com/bywinsty/cs2-vip)
## Вимоги:
- [Metamod:Source](https://www.sourcemm.net/downloads.php?branch=master&all=1).
- [Ядро cs2-vip](https://github.com/bywinsty/cs2-vip).
- Залежність ядра для бази даних `sql_mm` і налаштований `addons/configs/databases.cfg`, якщо використовуються функції ядра, що працюють із базою даних.
- `VIP_Fortnite_Hits` додатково потребує плагін/API Fortnite Hits.
## Установлення:
1. Спочатку встановіть і налаштуйте ядро.
2. Розпакуйте архів модуля в каталог сервера `game/csgo`, зберігаючи структуру шляхів:

   ```text
   addons/
   ├── metamod/<module>.vdf
   └── vip_modules/<module>.so
   ```

3. Додайте ключі можливостей модуля до `addons/configs/vip/groups.ini`, а ключі перекладів — до `addons/translations/vip.phrases.txt`, якщо цього вимагає README модуля.

Ключ можливості:

У `groups.ini` додайте:
```
"<module>" "значення"
```

Ключ перекладу:

У `vip.phrases.txt` додайте:

```
	"<module>"
	{
		"en/ua/ru"	"<module>"
		"en/ua/ru"	"<module>"
		"en/ua/ru"	"<module>"
	}
```

4. Створіть або відредагуйте конфігураційні файли модулів у `addons/configs/vip/`.

Лише для модулів, README яких вимагає окремого конфігураційного файла.

Наприклад:

```text
addons/configs/vip/vip_skins.ini
addons/configs/vip/vip_test.ini
addons/configs/vip/vip_time.ini
```

5. Перезапустіть сервер і перевірте, що ядро та вибраний модуль успішно завантажилися.
## Модулі:
| | | | |
| --- | --- | --- | --- |
| VIP_AntiFlash | [EN](VIP_AntiFlash/README.md) | [UA](VIP_AntiFlash/README-UA.md) | [RU](VIP_AntiFlash/README-RU.md) |
| VIP_AwpManager | [EN](VIP_AwpManager/README.md) | [UA](VIP_AwpManager/README-UA.md) | [RU](VIP_AwpManager/README-RU.md) |
| VIP_BaseFunctions | [EN](VIP_BaseFunctions/README.md) | [UA](VIP_BaseFunctions/README-UA.md) | [RU](VIP_BaseFunctions/README-RU.md) |
| VIP_Bhop | [EN](VIP_Bhop/README.md) | [UA](VIP_Bhop/README-UA.md) | [RU](VIP_Bhop/README-RU.md) |
| VIP_BuyTeamWeapon | [EN](VIP_BuyTeamWeapon/README.md) | [UA](VIP_BuyTeamWeapon/README-UA.md) | [RU](VIP_BuyTeamWeapon/README-RU.md) |
| VIP_DuckSpeed | [EN](VIP_DuckSpeed/README.md) | [UA](VIP_DuckSpeed/README-UA.md) | [RU](VIP_DuckSpeed/README-RU.md) |
| VIP_Endurance | [EN](VIP_Endurance/README.md) | [UA](VIP_Endurance/README-UA.md) | [RU](VIP_Endurance/README-RU.md) |
| VIP_FastDefuse | [EN](VIP_FastDefuse/README.md) | [UA](VIP_FastDefuse/README-UA.md) | [RU](VIP_FastDefuse/README-RU.md) |
| VIP_FastPlant | [EN](VIP_FastPlant/README.md) | [UA](VIP_FastPlant/README-UA.md) | [RU](VIP_FastPlant/README-RU.md) |
| VIP_FOV | [EN](VIP_FOV/README.md) | [UA](VIP_FOV/README-UA.md) | [RU](VIP_FOV/README-RU.md) |
| VIP_Fortnite_Hits | [EN](VIP_Fortnite_Hits/README.md) | [UA](VIP_Fortnite_Hits/README-UA.md) | [RU](VIP_Fortnite_Hits/README-RU.md) |
| VIP_Gravity | [EN](VIP_Gravity/README.md) | [UA](VIP_Gravity/README-UA.md) | [RU](VIP_Gravity/README-RU.md) |
| VIP_Healthshot | [EN](VIP_Healthshot/README.md) | [UA](VIP_Healthshot/README-UA.md) | [RU](VIP_Healthshot/README-RU.md) |
| VIP_Items | [EN](VIP_Items/README.md) | [UA](VIP_Items/README-UA.md) | [RU](VIP_Items/README-RU.md) |
| VIP_JoinSound | [EN](VIP_JoinSound/README.md) | [UA](VIP_JoinSound/README-UA.md) | [RU](VIP_JoinSound/README-RU.md) |
| VIP_Jumps | [EN](VIP_Jumps/README.md) | [UA](VIP_Jumps/README-UA.md) | [RU](VIP_Jumps/README-RU.md) |
| VIP_KillScreen | [EN](VIP_KillScreen/README.md) | [UA](VIP_KillScreen/README-UA.md) | [RU](VIP_KillScreen/README-RU.md) |
| VIP_MultiTest | [EN](VIP_MultiTest/README.md) | [UA](VIP_MultiTest/README-UA.md) | [RU](VIP_MultiTest/README-RU.md) |
| VIP_NoFallDamage | [EN](VIP_NoFallDamage/README.md) | [UA](VIP_NoFallDamage/README-UA.md) | [RU](VIP_NoFallDamage/README-RU.md) |
| VIP_Penis | [EN](VIP_Penis/README.md) | [UA](VIP_Penis/README-UA.md) | [RU](VIP_Penis/README-RU.md) |
| VIP_RainbowModel | [EN](VIP_RainbowModel/README.md) | [UA](VIP_RainbowModel/README-UA.md) | [RU](VIP_RainbowModel/README-RU.md) |
| VIP_RegenArmor | [EN](VIP_RegenArmor/README.md) | [UA](VIP_RegenArmor/README-UA.md) | [RU](VIP_RegenArmor/README-RU.md) |
| VIP_RegenHP | [EN](VIP_RegenHP/README.md) | [UA](VIP_RegenHP/README-UA.md) | [RU](VIP_RegenHP/README-RU.md) |
| VIP_ResetDeaths | [EN](VIP_ResetDeaths/README.md) | [UA](VIP_ResetDeaths/README-UA.md) | [RU](VIP_ResetDeaths/README-RU.md) |
| VIP_Respawn | [EN](VIP_Respawn/README.md) | [UA](VIP_Respawn/README-UA.md) | [RU](VIP_Respawn/README-RU.md) |
| VIP_ShowDamage | [EN](VIP_ShowDamage/README.md) | [UA](VIP_ShowDamage/README-UA.md) | [RU](VIP_ShowDamage/README-RU.md) |
| VIP_Skins | [EN](VIP_Skins/README.md) | [UA](VIP_Skins/README-UA.md) | [RU](VIP_Skins/README-RU.md) |
| VIP_Smoke | [EN](VIP_Smoke/README.md) | [UA](VIP_Smoke/README-UA.md) | [RU](VIP_Smoke/README-RU.md) |
| VIP_Speed | [EN](VIP_Speed/README.md) | [UA](VIP_Speed/README-UA.md) | [RU](VIP_Speed/README-RU.md) |
| VIP_Tag | [EN](VIP_Tag/README.md) | [UA](VIP_Tag/README-UA.md) | [RU](VIP_Tag/README-RU.md) |
| VIP_TeammatesHeal | [EN](VIP_TeammatesHeal/README.md) | [UA](VIP_TeammatesHeal/README-UA.md) | [RU](VIP_TeammatesHeal/README-RU.md) |
| VIP_Test | [EN](VIP_Test/README.md) | [UA](VIP_Test/README-UA.md) | [RU](VIP_Test/README-RU.md) |
| VIP_Time | [EN](VIP_Time/README.md) | [UA](VIP_Time/README-UA.md) | [RU](VIP_Time/README-RU.md) |
| VIP_Vampirism | [EN](VIP_Vampirism/README.md) | [UA](VIP_Vampirism/README-UA.md) | [RU](VIP_Vampirism/README-RU.md) |
| VIP_Vips | [EN](VIP_Vips/README.md) | [UA](VIP_Vips/README-UA.md) | [RU](VIP_Vips/README-RU.md) |
