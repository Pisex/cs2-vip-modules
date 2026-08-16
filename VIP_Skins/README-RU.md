[EN](README.md) | [UA](README-UA.md)

# [VIP] [Skins](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Skins)

## Позволяет VIP-игрокам выбирать модели игроков.

### Конфигурация

Модуль создаёт файл `addons/configs/vip/vip_skins.ini`. Создайте или отредактируйте его следующим образом:

### Ключ перевода

Значение возможности `Skins` определяет доступные модели. В `groups.ini` задаются ключи моделей, а в `vip.phrases.txt` — ключи их фраз.

```ini
"Skins"
{
    "time" "2"
    "cooldown" "30"

    "vip_model"
    {
        "name" "VIP model"
        "model_ct" "models/example_ct.vmdl"
        "model_t" "models/example_t.vmdl"
    }
}
```

`time` — задержка перед открытием меню. `cooldown` — время в секундах между сменами модели.

Добавьте возможности в `addons/configs/vip/groups.ini`:

```
"Skins" "vip_model another_model"
```

Ключи фраз перевода: `Select_Skin`, `Disable_Skin`, `Cooldown`.
