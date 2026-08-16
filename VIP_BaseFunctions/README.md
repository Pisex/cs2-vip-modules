[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [Base Functions](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_BaseFunctions)

## Adds health, armor, helmet, defuser and money settings to VIP groups. Values are applied when a VIP player spawns after the configured round threshold.

### Configuration

The module reads `addons/configs/vip/vip_bf.ini` (included in the package):

```ini
"VIP"
{
    "round_min" "1"
}
```

### Feature key

In `groups.ini` add the features you need:

```
"health" "health value"
"armor" "armor value"
"helmet" "1/0"
"money" "money value"
"defuser" "1/0"
```

For `health`, `armor` and `money`, a plain number (for example, `120`) sets the value. A value such as `++30` adds 30 to the current value.

`round_min` controls when the module starts applying settings. The default `1` skips the pistol round; `0` enables the module from the beginning.

### Translation key

In `vip.phrases.txt` add the feature labels `health`, `armor`, `helmet`, `money` and `defuser`.
