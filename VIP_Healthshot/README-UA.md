[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Healthshot

Видає VIP-гравцям налаштовану кількість healthshot під час появи.

Модуль читає `addons/configs/vip/vip_healthshot.ini`:

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` визначає перший раунд, у якому видаються healthshot.

У `groups.ini` додайте:

```
"healthshot" "count"
```

У `vip.phrases.txt` додайте ключ `healthshot`.

```
"healthshot"
{
    "en" "Healthshots"
    "ru" "Шприцы"
}
```
