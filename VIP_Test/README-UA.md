[EN](README.md) | [RU](README-RU.md)

# [VIP] [Test](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Test)

## Дозволяє гравцям тимчасово випробувати VIP-можливості.

### Команди

Консольні псевдоніми: `mm_viptest`, `sm_viptest`, `mm_testvip` і `sm_testvip`.

### Ключ перекладу

Модуль також використовує ключ перекладу `AlreadyVIP`.

### Конфігурація

Файл конфігурації: `addons/configs/vip/vip_test.ini`

```
"VIP"
{
    "group" "Example"
    "time" "60"
    "timeout" "0"
}
```

- `group` — група VIP, яку буде видано гравцю.
- `time` — тривалість тесту в секундах.
- `timeout` — час очікування перед повторним тестом у секундах; `0` вимикає обмеження.

### Команди

- `!viptest` або `viptest` — розпочати тест.
- `!testvip` або `testvip` — альтернативна команда.
- Для консолі доступні відповідні команди без префікса `!`.
