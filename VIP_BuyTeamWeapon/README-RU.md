[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Buy an Opposing Team's Weapon

Позволяет VIP-игрокам покупать оружие, обычно доступное только противоположной команде.

Модулю нужен `addons/configs/vip/vip_btw.ini`. В нём есть секции `CT` и `T`; каждая запись определяет цену и имя entity оружия:

```ini
"CT"
{
    "ak47"
    {
        "price" "2700"
        "weapon" "weapon_ak47"
    }
}
```

Для каждой записи модуль регистрирует команды `mm_<entry>` и `!<entry>`. Скрытый ключ возможности VIP `btw` управляет доступом.

В `vip.phrases.txt` добавьте ключи `btw_round_start`, `btw_no_money` и `btw_no_buyzone`.

```
"btw_round_start"
{
    "en" "The purchase of opposing team weapons is available to you: %s"
    "ru" "Вам доступна покупка оружия команды противника: %s"
}
"btw_no_money"
{
    "en" "Not enough money to buy a gun"
    "ru" "Недостаточно денег для покупки оружия"
}
"btw_no_buyzone"
{
    "en" "You can't buy a gun when you're not in the purchasing area"
    "ru" "Вы не можете купить оружие, если вы не находитесь в зоне покупки"
}
```
