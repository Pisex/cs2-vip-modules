# [VIP] Regen Armor
My Discord server - https://discord.com/invite/g798xERK5Y

Adds armor regeneration feature to VIP

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

In **vip.phrases.txt** add:
```
	"RegenArmor"
	{
		"en"	"Regen Armor"
		"ru"	"Реген Брони"
	}
```