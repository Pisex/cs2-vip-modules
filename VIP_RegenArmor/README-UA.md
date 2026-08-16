[EN](README.md) | [RU](README-RU.md)

# [VIP] [Regen Armor](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_RegenArmor)

## Додає VIP-гравцям регенерацію броні.

### Ключ можливості

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

### Ключ перекладу

У `vip.phrases.txt` додайте ключ `RegenArmor`.

```
	"RegenArmor"
	{
		"en"	"Regen Armor"
		"ru"	"Реген Брони"
	}
```
