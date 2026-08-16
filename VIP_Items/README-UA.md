[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Items

Видає VIP-гравцям налаштовані предмети під час появи після заданого раунду.

Модуль читає `addons/configs/vip/vip_items.ini`:

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` визначає перший раунд, у якому видаються предмети.

У `groups.ini` додайте назви entity через пробіл:

```
"items" "weapon_ak47 weapon_flashbang"
```

У `vip.phrases.txt` додайте ключ `items`.

```
"items"
{
    "en" "Weapons at the start of a round"
    "ru" "Оружие в начале раунда"
}
```
