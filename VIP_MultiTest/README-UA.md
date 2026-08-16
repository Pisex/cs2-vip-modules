[EN](README.md) | [RU](README-RU.md)

# [VIP] [MultiTest](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_MultiTest)

## Дозволяє звичайним гравцям отримати VIP-статус для налаштованої групи та тривалості. Перед установленням налаштуйте `addons/configs/vip/vip_multitest.ini`.

### Команди

Команди:

```text
mm_vipmultitest
sm_vipmultitest
!vipmultitest
vipmultitest
```

Файли встановлення:

```text
addons/
├── configs/vip/vip_multitest.ini
├── metamod/vip_multitest.vdf
└── vip_modules/vip_multitest.so
```

### Ключ перекладу

У `vip.phrases.txt` додайте ключ `VIPMultiTest_Title`.

Модуль також використовує ключ перекладу `AlreadyVIP`.

### Оновлення зі старої версії

Поточний `VIP_MultiTest` використовує окремий namespace `vip_multitest`:

| Призначення | Новий шлях/ключ |
| --- | --- |
| Binary | `addons/vip_modules/vip_multitest.so` |
| Metamod VDF | `addons/metamod/vip_multitest.vdf` |
| Config | `addons/configs/vip/vip_multitest.ini` |
| Cookie | `vip_multitest` |
| Commands | `mm_vipmultitest`, `sm_vipmultitest`, `!vipmultitest`, `vipmultitest` |
| Translation key | `VIPMultiTest_Title` |

Не видаляйте автоматично legacy-файли `vip_test.so`, `vip_test.vdf` і `vip_test.ini`. Тепер цей namespace належить `VIP_Test`; перед видаленням зупиніть сервер і перевірте, що `VIP_Test` їх не використовує.

Read-only перевірка:

```
"VIPMultiTest_Title"
{
    "en" "Choose a VIP group"
    "ru" "Выберите вип группу"
}
```

Код `0` означає, що legacy-файли не знайдено, `2` потребує ручної перевірки, `1` означає неправильний шлях установлення. Перевірка нічого не видаляє і не змінює.

```text
python3 .github/scripts/check_vip_multitest_upgrade.py /path/to/game
```
