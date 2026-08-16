[EN](README.md) | [RU](README-RU.md)

# [VIP] [Buy an Opposing Team's Weapon](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_BuyTeamWeapon)

## Дозволяє VIP-гравцям купувати зброю, яка зазвичай доступна лише протилежній команді.

### Конфігурація

Модулю потрібен `addons/configs/vip/vip_btw.ini`. У ньому є секції `CT` і `T`; кожен запис визначає ціну та назву entity зброї:

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

Для кожного запису модуль реєструє команди `mm_<entry>` і `!<entry>`. Прихований ключ можливості VIP `btw` керує доступом.

### Ключ перекладу

У `vip.phrases.txt` додайте ключі `btw_round_start`, `btw_no_money` і `btw_no_buyzone`.

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
