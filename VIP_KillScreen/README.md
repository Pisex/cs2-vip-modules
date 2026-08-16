[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [KillScreen](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_KillScreen)

## Shows a colored kill effect to a VIP attacker when they kill another player.

### Configuration

The module reads `addons/configs/vip/vip_ks.ini` (included in the package):

```ini
"VIP"
{
    "effect_time" "0.7"
}
```

`effect_time` is the effect duration in seconds.

### Feature key

In `groups.ini` add:

```
"killscreen" "1/0"
```

### Translation key

In `vip.phrases.txt` add:

```
"killscreen"
{
    "en" "Kill Screen"
    "ru" "Цветной экран убийства"
}
```
