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
| Модуль | Англійська | Українська | Російська |
| --- | --- | --- | --- |
| VIP_AntiFlash | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_AntiFlash/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_AntiFlash/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_AntiFlash/README-RU.md) |
| VIP_AwpManager | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_AwpManager/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_AwpManager/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_AwpManager/README-RU.md) |
| VIP_BaseFunctions | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_BaseFunctions/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_BaseFunctions/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_BaseFunctions/README-RU.md) |
| VIP_Bhop | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Bhop/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Bhop/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Bhop/README-RU.md) |
| VIP_BuyTeamWeapon | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_BuyTeamWeapon/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_BuyTeamWeapon/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_BuyTeamWeapon/README-RU.md) |
| VIP_DuckSpeed | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_DuckSpeed/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_DuckSpeed/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_DuckSpeed/README-RU.md) |
| VIP_Endurance | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Endurance/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Endurance/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Endurance/README-RU.md) |
| VIP_FastDefuse | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_FastDefuse/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_FastDefuse/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_FastDefuse/README-RU.md) |
| VIP_FastPlant | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_FastPlant/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_FastPlant/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_FastPlant/README-RU.md) |
| VIP_FOV | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_FOV/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_FOV/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_FOV/README-RU.md) |
| VIP_Fortnite_Hits | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Fortnite_Hits/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Fortnite_Hits/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Fortnite_Hits/README-RU.md) |
| VIP_Gravity | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Gravity/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Gravity/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Gravity/README-RU.md) |
| VIP_Healthshot | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Healthshot/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Healthshot/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Healthshot/README-RU.md) |
| VIP_Items | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Items/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Items/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Items/README-RU.md) |
| VIP_JoinSound | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_JoinSound/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_JoinSound/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_JoinSound/README-RU.md) |
| VIP_Jumps | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Jumps/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Jumps/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Jumps/README-RU.md) |
| VIP_KillScreen | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_KillScreen/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_KillScreen/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_KillScreen/README-RU.md) |
| VIP_MultiTest | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_MultiTest/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_MultiTest/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_MultiTest/README-RU.md) |
| VIP_NoFallDamage | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_NoFallDamage/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_NoFallDamage/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_NoFallDamage/README-RU.md) |
| VIP_Penis | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Penis/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Penis/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Penis/README-RU.md) |
| VIP_RainbowModel | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_RainbowModel/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_RainbowModel/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_RainbowModel/README-RU.md) |
| VIP_RegenArmor | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_RegenArmor/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_RegenArmor/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_RegenArmor/README-RU.md) |
| VIP_RegenHP | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_RegenHP/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_RegenHP/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_RegenHP/README-RU.md) |
| VIP_ResetDeaths | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_ResetDeaths/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_ResetDeaths/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_ResetDeaths/README-RU.md) |
| VIP_Respawn | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Respawn/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Respawn/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Respawn/README-RU.md) |
| VIP_ShowDamage | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_ShowDamage/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_ShowDamage/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_ShowDamage/README-RU.md) |
| VIP_Skins | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Skins/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Skins/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Skins/README-RU.md) |
| VIP_Smoke | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Smoke/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Smoke/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Smoke/README-RU.md) |
| VIP_Speed | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Speed/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Speed/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Speed/README-RU.md) |
| VIP_Tag | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Tag/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Tag/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Tag/README-RU.md) |
| VIP_TeammatesHeal | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_TeammatesHeal/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_TeammatesHeal/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_TeammatesHeal/README-RU.md) |
| VIP_Test | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Test/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Test/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Test/README-RU.md) |
| VIP_Time | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Time/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Time/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Time/README-RU.md) |
| VIP_Vampirism | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Vampirism/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Vampirism/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Vampirism/README-RU.md) |
| VIP_Vips | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Vips/README.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Vips/README-UA.md) | [click](https://github.com/bywinsty/cs2-vip-modules/blob/main/VIP_Vips/README-RU.md) |
