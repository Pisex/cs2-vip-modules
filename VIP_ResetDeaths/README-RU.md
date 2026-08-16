[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Reset Deaths

Консольные псевдонимы: `sm_rd`/`mm_rd` для смертей и `sm_rs`/`mm_rs` для убийств.

Модуль также использует ключ перевода `NotAccess`.

Позволяет VIP-игрокам сбрасывать количество смертей через VIP-меню или командами `!rd`/`rd`. Отдельно доступен сброс убийств командами `!rs`/`rs`.

В `groups.ini` добавьте:

```
"ResetDeaths" "1"
```

В `vip.phrases.txt` добавьте ключи `ResetDeaths`, `deaths_reset`, `deaths_already_zero`, `kills_reset` и `kills_already_zero`.

```
"ResetDeaths"
{
    "en" "Reset deaths"
    "ru" "Сброс смертей"
}
"deaths_reset"
{
    "en" "Your deaths have been reset!"
    "ru" "Ваши смерти были сброшены!"
}
"deaths_already_zero"
{
    "en" "Your deaths are already zero."
    "ru" "Ваши смерти уже равны нулю."
}
"kills_reset"
{
    "en" "Your kills have been reset!"
    "ru" "Ваши убийства были сброшены!"
}
"kills_already_zero"
{
    "en" "Your kills are already zero."
    "ru" "Ваши убийства уже равны нулю."
}
```
