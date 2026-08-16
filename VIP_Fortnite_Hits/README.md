[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [Fortnite Hits](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Fortnite_Hits)

## Connects VIP access to the Fortnite Hits plugin. This module does not render the damage display itself; it calls the external plugin's `IFortniteHitsApi` to grant or revoke access for VIP players.

Install and load the Fortnite Hits plugin before this module.

### Feature key

In `groups.ini` add:

```
"fortnite_hits" "1/0"
```

### Translation key

In `vip.phrases.txt` add:

```
"fortnite_hits"
{
    "en" "Damage Display"
    "ru" "Отображение урона"
}
```
