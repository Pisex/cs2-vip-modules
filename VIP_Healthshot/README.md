[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [Healthshot](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Healthshot)

## Gives VIP players the configured number of healthshots on spawn.

### Configuration

The module reads `addons/configs/vip/vip_healthshot.ini` (included in the package):

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` controls the first round in which healthshots are given.

### Feature key

In `groups.ini` add:

```
"healthshot" "count"
```

### Translation key

In `vip.phrases.txt` add:

```
"healthshot"
{
    "en" "Healthshots"
    "ru" "Шприцы"
}
```
