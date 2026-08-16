[EN](README.md) | [UA](README-UA.md)

# [VIP] [Teammates Heal](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_TeammatesHeal)

## Восстанавливает здоровье напарника, когда VIP наносит ему урон оружием, не внесённым в чёрный список.

Видео: https://www.youtube.com/watch?v=SjS9edeV5zQ

### Возможность

Добавьте возможность в `addons/configs/vip/groups.ini` (файл `groups.ini`):

```
"heal_teammates" "50"
```

Значение определяет максимальное количество здоровья, которое можно восстановить за один выстрел.

### Конфигурация

Файл конфигурации: `addons/configs/vip/teammates_heal.ini`

```ini
"VIP_Teammates_Heal"
{
    "syringe_effect" "1"
    "effect_time" "1.2"
    "weapon_blacklist" "weapon_molotov;weapon_hegrenade;"
    "max_shot_hp" "50"
}
```

`max_shot_hp` ограничивает количество здоровья, которое можно восстановить за один выстрел. Падение не считается уроном от VIP, а здоровье не увеличивается выше максимального значения.

### Ключ перевода

В `vip.phrases.txt` добавьте ключ фразы `heal_teammates`.

```
"heal_teammates"
{
    "en" "Heal Teammates"
    "ru" "Лечение союзников"
}
```
