[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Respawn

Команды: `mm_respawn`, `sm_respawn`, `respawn` и `!respawn`.

Добавляет VIP-игрокам возможность возрождаться.

Модуль также использует ключи перевода `YourAlive`, `SelectTeam`, `EndRound` и `LimitRespawn`.

## Настройка

Добавьте возможность в нужную группу в `addons/configs/vip/groups.ini`:

```
"respawn" "respawn value"
```

Значение `respawn value` используется модулем как значение возможности.

Ключ фразы перевода: `respawn`.

```
	"respawn"
	{
		"en"	"Respawn"
		"ru"	"Респавн"
	}
```
