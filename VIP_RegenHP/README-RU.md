[EN](README.md) | [UA](README-UA.md)

# [VIP] [Regen HP](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_RegenHP)

## Добавляет VIP-игрокам регенерацию здоровья.

### Ключ возможности

В `groups.ini` добавьте:

```
"RegenHP" "Number of HP per interval"
"DelayRegenHP" "Delay before starting recovery (in seconds)"
"IntervalRegenHP" "Recovery interval (in seconds)"
```

Пример:

```
"RegenHP" "10"
"DelayRegenHP" "5"
"IntervalRegenHP" "2"
```

После получения урона проходит 5 секунд, затем восстанавливается 10 HP каждые 2 секунды.

### Ключ перевода

В `vip.phrases.txt` добавьте ключ `RegenHP`.

```
	"RegenHP"
	{
		"en"	"Regen HP"
		"ru"	"Реген ХП"
	}
```
