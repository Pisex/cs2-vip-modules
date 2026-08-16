[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [Regen Armor](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_RegenArmor)

## Adds armor regeneration feature to VIP

### Feature key

In **groups.ini** add:
```
"RegenArmor" "Number of armor units per interval"
"DelayRegenArmor" "Delay before starting recovery (in seconds)"
"IntervalRegenArmor" "Recovery interval (in seconds)"
```
Example:
```
"RegenArmor" "10"
"DelayRegenArmor" "5"
"IntervalRegenArmor" "2"
```
After damaging the player, 5 seconds must pass and recovery will begin at a rate of 10 units of armor every 2 seconds.

### Translation key

In **vip.phrases.txt** add:
```
	"RegenArmor"
	{
		"en"	"Regen Armor"
		"ru"	"Реген Брони"
	}
```
