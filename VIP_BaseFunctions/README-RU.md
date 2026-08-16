[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Base Functions

Добавляет в VIP-группы настройки здоровья, брони, шлема, дефузера и денег. Значения применяются при возрождении VIP-игрока после заданного раунда.

Модуль читает `addons/configs/vip/vip_bf.ini`:

```ini
"VIP"
{
    "round_min" "1"
}
```

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

В `vip.phrases.txt` добавьте метки `health`, `armor`, `helmet`, `money` и `defuser`.
