[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Healthshot

Выдаёт VIP-игрокам настроенное количество healthshot при появлении.

Модуль читает `addons/configs/vip/vip_healthshot.ini`:

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` определяет первый раунд, в котором выдаются healthshot.

В `groups.ini` добавьте:

```
"healthshot" "count"
```

В `vip.phrases.txt` добавьте ключ `healthshot`.

```
"healthshot"
{
    "en" "Healthshots"
    "ru" "Шприцы"
}
```
