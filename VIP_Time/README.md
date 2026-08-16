[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] Time

Grants a VIP group during a recurring time window. The module checks the window once per minute, grants the configured group to connected non-VIP players, and removes only the status it granted when the window ends.

Create `addons/configs/vip/vip_time.ini`; this file is read by the module but is not included in the current package manifest:

```ini
"VIP"
{
    "group" "Example"
    "start_time" "18:00"
    "end_time" "22:00"
}
```

Use 24-hour `HH:MM` values. Windows that cross midnight are supported.
[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)
