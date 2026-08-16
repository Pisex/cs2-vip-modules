[EN](README.md) | [RU](README-RU.md)

# [VIP] [Regen HP](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_RegenHP)

## Додає VIP-гравцям регенерацію здоров'я.

### Ключ можливості

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

### Ключ перекладу

У `vip.phrases.txt` додайте ключ `RegenHP`.

```
	"RegenHP"
	{
		"en"	"Regen HP"
		"ru"	"Реген ХП"
	}
```
