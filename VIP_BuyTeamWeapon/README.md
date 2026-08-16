[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [Buy an Opposing Team's Weapon](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_BuyTeamWeapon)

## Allows VIP players to buy weapons normally restricted to the opposing team.

### Configuration

The module requires `addons/configs/vip/vip_btw.ini` (included in the package). It contains separate `CT` and `T` sections; each entry defines the price and weapon entity name:

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

For every configured entry, the module registers `mm_<entry>` and `!<entry>` commands. The hidden VIP feature key `btw` controls access.

### Translation key

In `vip.phrases.txt` add:

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
