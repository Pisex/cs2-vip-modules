[EN](README.md) | [RU](README-RU.md)

# [VIP] [Skins](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Skins)

## Дозволяє VIP-гравцям вибирати моделі гравців.

### Конфігурація

Модуль створює файл `addons/configs/vip/vip_skins.ini`. Створіть або відредагуйте його так:

### Ключ перекладу

Значення можливості `Skins` визначає доступні моделі. У `groups.ini` задаються ключі моделей, а в `vip.phrases.txt` — ключі їхніх фраз.

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

`time` — затримка перед відкриттям меню. `cooldown` — час у секундах між змінами моделі.

Додайте можливості до `addons/configs/vip/groups.ini`:

```
"Skins" "vip_model another_model"
```

Ключі фраз перекладу: `Select_Skin`, `Disable_Skin`, `Cooldown`.
