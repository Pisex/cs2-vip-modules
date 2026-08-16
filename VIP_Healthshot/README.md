[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Healthshot

Gives VIP players the configured number of healthshots on spawn.

The module reads `addons/configs/vip/vip_healthshot.ini` (included in the package):

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` controls the first round in which healthshots are given.

In `groups.ini` add:

```
"healthshot" "count"
```

In `vip.phrases.txt` add:

```
"healthshot"
{
    "en" "Healthshots"
    "ru" "Шприцы"
}
```
[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)
