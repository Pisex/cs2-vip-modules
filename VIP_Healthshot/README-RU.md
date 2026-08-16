[EN](README.md) | [UA](README-UA.md)

# [VIP] [Healthshot](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Healthshot)

## Выдаёт VIP-игрокам настроенное количество healthshot при появлении.

### Конфигурация

Модуль читает `addons/configs/vip/vip_healthshot.ini`:

```ini
"VIP"
{
    "round_min" "2"
}
```

`round_min` определяет первый раунд, в котором выдаются healthshot.

### Ключ возможности

В `groups.ini` добавьте:

```
"healthshot" "count"
```

### Ключ перевода

В `vip.phrases.txt` добавьте ключ `healthshot`.

```
"healthshot"
{
    "en" "Healthshots"
    "ru" "Шприцы"
}
```
