[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] KillScreen

Показує кольоровий ефект після вбивства VIP-атакувальником іншого гравця.

Модуль читає `addons/configs/vip/vip_ks.ini`:

```ini
"VIP"
{
    "effect_time" "0.7"
}
```

`effect_time` — тривалість ефекту в секундах.

У `groups.ini` додайте:

```
"killscreen" "1/0"
```

У `vip.phrases.txt` додайте ключ `killscreen`.

```
"killscreen"
{
    "en" "Kill Screen"
    "ru" "Цветной экран убийства"
}
```
