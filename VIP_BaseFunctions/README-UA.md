[EN](README.md) | [RU](README-RU.md)

# [VIP] [Base Functions](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_BaseFunctions)

## Додає до VIP-груп налаштування здоров'я, броні, шолома, дефузера та грошей. Значення застосовуються під час відродження VIP-гравця після заданого раунду.

### Конфігурація

Модуль читає `addons/configs/vip/vip_bf.ini`:

```ini
"VIP"
{
    "round_min" "1"
}
```

### Ключ можливості

У `groups.ini` додайте потрібні можливості:

```
"health" "health value"
"armor" "armor value"
"helmet" "1/0"
"money" "money value"
"defuser" "1/0"
```

Для `health`, `armor` і `money` звичайне число, наприклад `120`, встановлює значення. Значення на кшталт `++30` додає 30 до поточного значення.

`round_min` визначає, коли модуль починає застосовувати налаштування. Значення `1` пропускає пістолетний раунд, а `0` вмикає модуль із початку.

### Ключ перекладу

У `vip.phrases.txt` додайте мітки `health`, `armor`, `helmet`, `money` і `defuser`.
