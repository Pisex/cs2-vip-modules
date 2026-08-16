[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [Items](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Items)

## Gives VIP players the configured items on spawn after the configured round threshold.

### Configuration

The module reads `addons/configs/vip/vip_items.ini` (included in the package):

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` controls the first round in which items are given.

### Feature key

In `groups.ini` add:

```
"items" "weapon_ak47 weapon_flashbang"
```

List item entity names separated by spaces.

### Translation key

In `vip.phrases.txt` add:

```
"items"
{
    "en" "Weapons at the start of a round"
    "ru" "Оружие в начале раунда"
}
```
