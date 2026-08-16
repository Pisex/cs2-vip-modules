[EN](README.md) | [RU](README-RU.md)

# [VIP] [Items](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Items)

## Видає VIP-гравцям налаштовані предмети під час появи після заданого раунду.

### Конфігурація

Модуль читає `addons/configs/vip/vip_items.ini`:

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` визначає перший раунд, у якому видаються предмети.

### Ключ можливості

У `groups.ini` додайте назви entity через пробіл:

```
"items" "weapon_ak47 weapon_flashbang"
```

### Ключ перекладу

У `vip.phrases.txt` додайте ключ `items`.

```
"items"
{
    "en" "Weapons at the start of a round"
    "ru" "Оружие в начале раунда"
}
```
