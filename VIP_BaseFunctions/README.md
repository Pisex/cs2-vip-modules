# [VIP] Base Functions
My Discord server - https://discord.com/invite/g798xERK5Y

Adds additional units of health, armor, helmet, defuser, money to the VIP ability setting.

In **groups.ini** add:
```
"health" "health values"
"armor" "armor values"
"helmet" "whether to give helmet(1/0)"
"money" "money value"
"defuser" "whether defuser(1/0)"
```
If the value is just a number (e.g. "120") - it will simply be set to the player.
If the value is "++30", this value will be added to the player's current value.

In **vip.phrases.txt** add:
```
	"health"
	{
		"en"	"Health"
		"ru"	"Здоровье"
	}
	"armor"
	{
		"en"	"Armor"
		"ru"	"Броня"
	}
	"helmet"
	{
		"en"	"Helmet"
		"ru"	"Шлем"
	}
	"money"
	{
		"en"	"Cash"
		"ru"	"Деньги"
	}
	"defuser"
	{
		"en"	"Defuser"
		"ru"	"Дефуза"
	}
```