[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Test

Console aliases: `mm_viptest`, `sm_viptest`, `mm_testvip` and `sm_testvip`.

The module also uses the `AlreadyVIP` translation key.

Allows a regular player to claim temporary VIP status using a configured group and duration.

Create or edit `addons/configs/vip/vip_test.ini`:

```ini
"VIP"
{
    "group" "Example"
    "time" "60"
    "timeout" "0"
}
```

- `group` is the VIP group to grant.
- `time` is the granted VIP duration in seconds.
- `timeout` controls when the player may claim again; `0` disables re-claims.

Commands: `!viptest`/`viptest` and `!testvip`/`testvip` (console aliases are also registered).
[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)
