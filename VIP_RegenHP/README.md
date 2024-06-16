# [VIP] Regen HP
My Discord server - https://discord.com/invite/g798xERK5Y

Adds a health regeneration feature to VIP

In **groups.ini** add:
```
"RegenHP" "Number of HP per interval"
"DelayRegenHP" "Delay before starting recovery (in seconds)"
"IntervalRegenHP" "Recovery interval (in seconds)"
```
Example:
```
"RegenHP" "10"
"DelayRegenHP" "5"
"IntervalRegenHP" "2"
```
After damaging the player, 5 seconds must pass and recovery will begin at a rate of 10 xp every 2 seconds.

In **vip.phrases.txt** add:
```
	"RegenHP"
	{
		"en"	"Regen HP"
		"ru"	"Реген ХП"
	}
```