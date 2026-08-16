[EN](README.md) | [RU](README-RU.md)

# [VIP] [KillScreen](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_KillScreen)

## Показує кольоровий ефект після вбивства VIP-атакувальником іншого гравця.

### Конфігурація

Модуль читає `addons/configs/vip/vip_ks.ini`:

```ini
"VIP"
{
    "effect_time" "0.7"
}
```

`effect_time` — тривалість ефекту в секундах.

### Ключ можливості

У `groups.ini` додайте:

```
"killscreen" "1/0"
```

### Ключ перекладу

У `vip.phrases.txt` додайте ключ `killscreen`.

```
"killscreen"
{
    "en" "Kill Screen"
    "ru" "Цветной экран убийства"
}
```
