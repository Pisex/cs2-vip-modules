[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Regen Armor

Додає VIP-гравцям регенерацію броні.

У `groups.ini` додайте:

```
"RegenArmor" "Number of armor units per interval"
"DelayRegenArmor" "Delay before starting recovery (in seconds)"
"IntervalRegenArmor" "Recovery interval (in seconds)"
```

Приклад:

```
"RegenArmor" "10"
"DelayRegenArmor" "5"
"IntervalRegenArmor" "2"
```

Після отримання шкоди проходить 5 секунд, після чого відновлюється 10 одиниць броні кожні 2 секунди.

У `vip.phrases.txt` додайте ключ `RegenArmor`.

```
	"RegenArmor"
	{
		"en"	"Regen Armor"
		"ru"	"Реген Брони"
	}
```
