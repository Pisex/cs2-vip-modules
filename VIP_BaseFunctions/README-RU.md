[EN](README.md) | [UA](README-UA.md)

# [VIP] [Base Functions](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_BaseFunctions)

## Добавляет в VIP-группы настройки здоровья, брони, шлема, дефузера и денег. Значения применяются при возрождении VIP-игрока после заданного раунда.

### Конфигурация

Модуль читает `addons/configs/vip/vip_bf.ini`:

```ini
"VIP"
{
    "round_min" "1"
}
```

### Ключ возможности

В `groups.ini` добавьте нужные возможности:

```
"health" "health value"
"armor" "armor value"
"helmet" "1/0"
"money" "money value"
"defuser" "1/0"
```

Для `health`, `armor` и `money` обычное число, например `120`, устанавливает значение. Значение вида `++30` добавляет 30 к текущему значению.

`round_min` определяет, когда модуль начинает применять настройки. Значение `1` пропускает пистолетный раунд, а `0` включает модуль с начала.

### Ключ перевода

В `vip.phrases.txt` добавьте метки `health`, `armor`, `helmet`, `money` и `defuser`.
