**[VIP] Reset Deaths** - добавляет возможность VIP-игрокам сбрасывать количество своих смертей с помощью команд `!rd` или через VIP меню с командой `!vip`.

В `groups.ini` добавьте следующее:
```
  "ResetDeaths" "1"  // Установите значение "1", чтобы разрешить сброс смертей
```

В файл `vip.phrases.txt` добавьте следующее:
```
	"ResetDeaths"
	{
		"en"    "Reset deaths"
		"ru"    "Сброс смертей"
	}
	"deaths_reset"
	{
		"en"    "Your deaths have been reset!"
		"ru"    "Ваши смерти были сброшены!."
	}
	"deaths_already_zero"
	{
		"en"    "Your deaths are already zero."
		"ru"    "Ваши смерти уже равны нулю."
	}
	"kills_already_zero"
	{
		"en"  "Your kills are already zero."
		"ru"  "Ваши убийства уже равны нулю."
	}
	"kills_reset"
	{
		"en"  "Your kills have been reset!"
		"ru"  "Ваши убийства были сброшены!"
	}	
```
