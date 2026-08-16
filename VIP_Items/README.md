[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Items

Gives VIP players the configured items on spawn after the configured round threshold.

The module reads `addons/configs/vip/vip_items.ini` (included in the package):

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` controls the first round in which items are given.

In `groups.ini` add:

```
"items" "weapon_ak47 weapon_flashbang"
```

List item entity names separated by spaces.

In `vip.phrases.txt` add:

```
"items"
{
    "en" "Weapons at the start of a round"
    "ru" "Оружие в начале раунда"
}
```
[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)
