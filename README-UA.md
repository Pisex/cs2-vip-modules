[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Модулі

Необов'язкові C++-модулі для [ядра cs2-vip](https://github.com/bywinsty/cs2-vip). Кожен модуль реєструє власні можливості через `IVIPApi`; встановлюйте лише потрібні модулі.

Гілка `dev` є розробницькою. За можливості використовуйте сумісні версії ядра та модулів.

## Вимоги

- Виділений сервер CS2 з установленим [Metamod:Source](https://www.sourcemm.net/).
- Установлене та завантажене перед модулями [ядро cs2-vip](https://github.com/bywinsty/cs2-vip).
- Залежність ядра для бази даних `sql_mm` і налаштований `addons/configs/databases.cfg`, якщо використовуються функції ядра, що працюють із базою даних.
- `VIP_Fortnite_Hits` додатково потребує плагін/API Fortnite Hits.

## Установлення

1. Спочатку встановіть і налаштуйте ядро.
2. Розпакуйте архів модуля в каталог сервера `game/csgo`, зберігаючи структуру шляхів:

   ```text
   addons/
   ├── metamod/<module>.vdf
   └── vip_modules/<module>.so
   ```

3. Додайте ключі можливостей модуля до `addons/configs/vip/groups.ini`, а ключі перекладів — до `addons/translations/vip.phrases.txt`, якщо цього вимагає README модуля.
4. Створіть або відредагуйте файли модуля в `addons/configs/vip/`.
5. Перезапустіть сервер і перевірте завантаження ядра та вибраного модуля.

Маніфест пакетів цієї гілки визначає файли кожного пакета. Модулі, для яких конфіг потрібно створити вручну, вказують це у власній документації.

## Модулі

| Модуль | EN | UA | RU |
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
