[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Reset Deaths

Console aliases are `sm_rd`/`mm_rd` for deaths and `sm_rs`/`mm_rs` for kills.

The module also uses the `NotAccess` translation key.

Lets VIP players reset their deaths from the VIP menu or with `!rd`/`rd`. It also provides a separate kills reset command: `!rs`/`rs`.

In `groups.ini` add:

```
"ResetDeaths" "1"
```

In `vip.phrases.txt` add the keys used by the module:

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
[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)
