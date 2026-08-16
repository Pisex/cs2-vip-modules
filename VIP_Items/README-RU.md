[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Items

Выдаёт VIP-игрокам настроенные предметы при появлении после заданного раунда.

Модуль читает `addons/configs/vip/vip_items.ini`:

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` определяет первый раунд, в котором выдаются предметы.

В `groups.ini` добавьте имена entity через пробел:

```
"items" "weapon_ak47 weapon_flashbang"
```

В `vip.phrases.txt` добавьте ключ `items`.

```
"items"
{
    "en" "Weapons at the start of a round"
    "ru" "Оружие в начале раунда"
}
```
