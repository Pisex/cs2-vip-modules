[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Regen HP

Додає VIP-гравцям регенерацію здоров'я.

У `groups.ini` додайте:

```
"RegenHP" "Number of HP per interval"
"DelayRegenHP" "Delay before starting recovery (in seconds)"
"IntervalRegenHP" "Recovery interval (in seconds)"
```

Приклад:

```
"RegenHP" "10"
"DelayRegenHP" "5"
"IntervalRegenHP" "2"
```

Після отримання шкоди проходить 5 секунд, потім відновлюється 10 HP кожні 2 секунди.

У `vip.phrases.txt` додайте ключ `RegenHP`.

```
	"RegenHP"
	{
		"en"	"Regen HP"
		"ru"	"Реген ХП"
	}
```
