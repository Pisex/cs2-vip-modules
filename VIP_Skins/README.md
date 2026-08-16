[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [Skins](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_Skins)

## Allows VIP players to select a model from a configured skin list. The selected skin is persisted in a VIP cookie and reapplied on spawn.

### Configuration

Create `addons/configs/vip/vip_skins.ini`; this file is read by the module but is not declared in the current package manifest. Example:

```ini
"Skins"
{
    "time" "2"
    "cooldown" "30"

    "vip_model"
    {
        "name" "VIP model"
        "model_ct" "models/example_ct.vmdl"
        "model_t" "models/example_t.vmdl"
    }
}
```

`time` is the delay before applying a model after spawn; `cooldown` is the selection cooldown in seconds. The model keys available to a player come from the `Skins` feature value.

### Feature key

In `groups.ini` add:

```
"Skins" "vip_model another_model"
```

### Translation key

In `vip.phrases.txt` add `Select_Skin`, `Disable_Skin` and `Cooldown`.
