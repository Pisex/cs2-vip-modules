[EN](README.md) | [RU](README-RU.md)

# [VIP] [Teammates Heal](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_TeammatesHeal)

## Зцілює напарника, коли VIP завдає йому шкоди зброєю, що не входить до чорного списку.

Відео: https://www.youtube.com/watch?v=SjS9edeV5zQ

### Можливість

Додайте можливість до `addons/configs/vip/groups.ini` (файл `groups.ini`):

```
"heal_teammates" "50"
```

Значення визначає максимальну кількість здоров’я, яку можна відновити за один постріл.

### Конфігурація

Файл конфігурації: `addons/configs/vip/teammates_heal.ini`

```ini
"VIP_Teammates_Heal"
{
    "syringe_effect" "1"
    "effect_time" "1.2"
    "weapon_blacklist" "weapon_molotov;weapon_hegrenade;"
    "max_shot_hp" "50"
}
```

`max_shot_hp` обмежує кількість здоров’я, яку можна відновити за один постріл. Падіння не вважається шкодою від VIP, а здоров’я не збільшується понад максимальне значення.

### Ключ перекладу

У `vip.phrases.txt` додайте ключ фрази `heal_teammates`.

```
"heal_teammates"
{
    "en" "Heal Teammates"
    "ru" "Лечение союзников"
}
```
