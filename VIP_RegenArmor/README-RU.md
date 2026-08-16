[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Regen Armor

Добавляет VIP-игрокам регенерацию брони.

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

В `vip.phrases.txt` добавьте ключ `RegenArmor`.

```
	"RegenArmor"
	{
		"en"	"Regen Armor"
		"ru"	"Реген Брони"
	}
```
