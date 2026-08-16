[EN](README.md) | [UA](README-UA.md)

# [VIP] [KillScreen](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_KillScreen)

## Показывает цветной эффект после убийства VIP-атакующим другого игрока.

### Конфигурация

Модуль читает `addons/configs/vip/vip_ks.ini`:

```ini
"VIP"
{
    "effect_time" "0.7"
}
```

`effect_time` — длительность эффекта в секундах.

### Ключ возможности

В `groups.ini` добавьте:

```
"killscreen" "1/0"
```

### Ключ перевода

В `vip.phrases.txt` добавьте ключ `killscreen`.

```
"killscreen"
{
    "en" "Kill Screen"
    "ru" "Цветной экран убийства"
}
```
