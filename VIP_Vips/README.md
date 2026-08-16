[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)

# [VIP] VIPs

Console aliases are `sm_vips`, `mm_vips` and `vips`.

Lists connected VIP players with `!vips`/`vips`.

Create or edit `addons/configs/vip/vip_vips.ini`; this file is read by the module but is not declared in the current package manifest:

```ini
"VIP"
{
    "type" "0" // 0: show the group, 1: hide the group
}
```

The module uses `VIPS_Title`, `VIPS_Player`, `VIPS_Player2` and `VIPS_NoPlayers` from `vip.phrases.txt`.
[EN](README.md) · [UA](README-UA.md) · [RU](README-RU.md)
