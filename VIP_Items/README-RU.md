[EN](README.md) | [UA](README-UA.md)

# [VIP] [Items](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Items)

## Выдаёт VIP-игрокам настроенные предметы при появлении после заданного раунда.

### Конфигурация

Модуль читает `addons/configs/vip/vip_items.ini`:

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` определяет первый раунд, в котором выдаются предметы.

### Ключ возможности

В `groups.ini` добавьте имена entity через пробел:

```
"items" "weapon_ak47 weapon_flashbang"
```

### Ключ перевода

В `vip.phrases.txt` добавьте ключ `items`.

```
"items"
{
    "en" "Weapons at the start of a round"
    "ru" "Оружие в начале раунда"
}
```
