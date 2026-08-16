[EN](README.md) | [RU](README-RU.md)

# [VIP] [Healthshot](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Healthshot)

## Видає VIP-гравцям налаштовану кількість healthshot під час появи.

### Конфігурація

Модуль читає `addons/configs/vip/vip_healthshot.ini`:

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` визначає перший раунд, у якому видаються healthshot.

### Ключ можливості

У `groups.ini` додайте:

```
"healthshot" "count"
```

### Ключ перекладу

У `vip.phrases.txt` додайте ключ `healthshot`.

```
"healthshot"
{
    "en" "Healthshots"
    "ru" "Шприцы"
}
```
