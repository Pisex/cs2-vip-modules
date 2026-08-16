[EN](README.md) | [UA](README-UA.md)

# [VIP] [MultiTest](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_MultiTest)

## Позволяет обычным игрокам получить VIP-статус для настроенной группы и на заданное время. Перед установкой настройте `addons/configs/vip/vip_multitest.ini`.

### Команды

Команды:

```text
mm_vipmultitest
sm_vipmultitest
!vipmultitest
vipmultitest
```

Файлы установки:

```text
addons/
├── configs/vip/vip_multitest.ini
├── metamod/vip_multitest.vdf
└── vip_modules/vip_multitest.so
```

### Ключ перевода

В `vip.phrases.txt` добавьте ключ `VIPMultiTest_Title`.

Модуль также использует ключ перевода `AlreadyVIP`.

### Обновление со старой версии

Текущий `VIP_MultiTest` использует отдельный namespace `vip_multitest`:

| Назначение | Новый путь/ключ |
| --- | --- |
| Binary | `addons/vip_modules/vip_multitest.so` |
| Metamod VDF | `addons/metamod/vip_multitest.vdf` |
| Config | `addons/configs/vip/vip_multitest.ini` |
| Cookie | `vip_multitest` |
| Commands | `mm_vipmultitest`, `sm_vipmultitest`, `!vipmultitest`, `vipmultitest` |
| Translation key | `VIPMultiTest_Title` |

Не удаляйте автоматически legacy-файлы `vip_test.so`, `vip_test.vdf` и `vip_test.ini`. Теперь namespace принадлежит `VIP_Test`; перед удалением остановите сервер и убедитесь, что `VIP_Test` не использует эти файлы.

Read-only проверка:

```
"VIPMultiTest_Title"
{
    "en" "Choose a VIP group"
    "ru" "Выберите вип группу"
}
```

Код `0` означает, что legacy-файлы не найдены, `2` требует ручной проверки, `1` означает неверный путь установки. Проверка ничего не удаляет и не изменяет.

```text
python3 .github/scripts/check_vip_multitest_upgrade.py /path/to/game
```
