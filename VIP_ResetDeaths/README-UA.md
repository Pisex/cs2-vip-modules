[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Reset Deaths

Консольні псевдоніми: `sm_rd`/`mm_rd` для смертей і `sm_rs`/`mm_rs` для вбивств.

Модуль також використовує ключ перекладу `NotAccess`.

Дозволяє VIP-гравцям скидати кількість смертей через VIP-меню або командами `!rd`/`rd`. Окремо доступне скидання вбивств командами `!rs`/`rs`.

У `groups.ini` додайте:

```
"ResetDeaths" "1"
```

У `vip.phrases.txt` додайте ключі `ResetDeaths`, `deaths_reset`, `deaths_already_zero`, `kills_reset` і `kills_already_zero`.

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
