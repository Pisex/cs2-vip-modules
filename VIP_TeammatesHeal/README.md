[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [Teammates Heal](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_TeammatesHeal)

## Heals a teammate when a VIP damages them with a non-blacklisted weapon.

[Video demonstration](https://www.youtube.com/watch?v=SjS9edeV5zQ)

### Feature key

In `groups.ini` add:

```
"heal_teammates" "50"
```

The value is the percentage of damage restored to the teammate.

The module reads `addons/configs/vip/teammates_heal.ini` (included in the package):

```ini
"VIP_Teammates_Heal"
{
    "syringe_effect" "1"
    "effect_time" "1.2"
    "weapon_blacklist" "weapon_molotov;weapon_hegrenade;"
    "max_shot_hp" "50"
}
```

`max_shot_hp` caps healing from one hit. Fall damage is ignored, and the module does not heal a teammate above their maximum health.

### Translation key

In `vip.phrases.txt` add:

```
"heal_teammates"
{
    "en" "Heal Teammates"
    "ru" "Лечение союзников"
}
```
