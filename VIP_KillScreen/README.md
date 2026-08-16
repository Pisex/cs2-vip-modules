[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] KillScreen

Shows a colored kill effect to a VIP attacker when they kill another player.

The module reads `addons/configs/vip/vip_ks.ini` (included in the package):

```ini
"VIP"
{
    "effect_time" "0.7"
}
```

`effect_time` is the effect duration in seconds.

In `groups.ini` add:

```
"killscreen" "1/0"
```

In `vip.phrases.txt` add:

```
"killscreen"
{
    "en" "Kill Screen"
    "ru" "Цветной экран убийства"
}
```
[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)
