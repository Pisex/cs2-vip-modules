[EN](README.md) | [UA](README-UA.md)

# [VIP] [Regen Armor](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_RegenArmor)

## Добавляет VIP-игрокам регенерацию брони.

### Ключ возможности

В `groups.ini` добавьте:

```
"RegenArmor" "Number of armor units per interval"
"DelayRegenArmor" "Delay before starting recovery (in seconds)"
"IntervalRegenArmor" "Recovery interval (in seconds)"
```

Пример:

```
"RegenArmor" "10"
"DelayRegenArmor" "5"
"IntervalRegenArmor" "2"
```

После получения урона проходит 5 секунд, затем восстанавливается 10 единиц брони каждые 2 секунды.

### Ключ перевода

В `vip.phrases.txt` добавьте ключ `RegenArmor`.

```
	"RegenArmor"
	{
		"en"	"Regen Armor"
		"ru"	"Реген Брони"
	}
```
