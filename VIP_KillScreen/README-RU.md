[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] KillScreen

Показывает цветной эффект после убийства VIP-атакующим другого игрока.

Модуль читает `addons/configs/vip/vip_ks.ini`:

```ini
"VIP"
{
    "effect_time" "0.7"
}
```

`effect_time` — длительность эффекта в секундах.

В `groups.ini` добавьте:

```
"killscreen" "1/0"
```

В `vip.phrases.txt` добавьте ключ `killscreen`.

```
"killscreen"
{
    "en" "Kill Screen"
    "ru" "Цветной экран убийства"
}
```
