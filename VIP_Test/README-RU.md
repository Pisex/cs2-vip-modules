[EN](README.md) | [UA](README-UA.md)

# [VIP] [Test](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Test)

## Позволяет игрокам временно испытать VIP-возможности.

### Команды

Консольные псевдонимы: `mm_viptest`, `sm_viptest`, `mm_testvip` и `sm_testvip`.

### Ключ перевода

Модуль также использует ключ перевода `AlreadyVIP`.

### Конфигурация

Файл конфигурации: `addons/configs/vip/vip_test.ini`

```
"VIP"
{
    "group" "Example"
    "time" "60"
    "timeout" "0"
}
```

- `group` — VIP-группа, которая будет выдана игроку.
- `time` — длительность теста в секундах.
- `timeout` — время ожидания перед повторным тестом в секундах; `0` отключает ограничение.

### Команды

- `!viptest` или `viptest` — начать тест.
- `!testvip` или `testvip` — альтернативная команда.
- Для консоли доступны соответствующие команды без префикса `!`.
