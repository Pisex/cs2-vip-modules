[UA](README-UA.md) | [RU](README-RU.md)

# [VIP] [MultiTest](https://github.com/bywinsty/cs2-vip-modules/tree/dev/VIP_MultiTest)

## Allows regular players to claim VIP status for a configured group and duration. Customize `addons/configs/vip/vip_multitest.ini` before installing.

### Commands

Commands:

```text
mm_vipmultitest
sm_vipmultitest
!vipmultitest
vipmultitest
```

Installed files:

```text
addons/
├── configs/vip/vip_multitest.ini
├── metamod/vip_multitest.vdf
└── vip_modules/vip_multitest.so
```

### Translation key

In `vip.phrases.txt` add:

```
"VIPMultiTest_Title"
{
    "en" "Choose a VIP group"
    "ru" "Выберите вип группу"
}
```

The module also uses the `AlreadyVIP` translation key.

### Upgrade from an older version

The current `VIP_MultiTest` uses the separate `vip_multitest` namespace:

| Purpose | New path/key |
| --- | --- |
| Binary | `addons/vip_modules/vip_multitest.so` |
| Metamod VDF | `addons/metamod/vip_multitest.vdf` |
| Config | `addons/configs/vip/vip_multitest.ini` |
| Cookie | `vip_multitest` |
| Commands | `mm_vipmultitest`, `sm_vipmultitest`, `!vipmultitest`, `vipmultitest` |
| Translation key | `VIPMultiTest_Title` |

Do not delete legacy `vip_test.so`, `vip_test.vdf` or `vip_test.ini` automatically. The namespace now belongs to `VIP_Test`; stop the server and verify that those files are not used by `VIP_Test` before removing them.

Read-only preflight check:

```text
python3 .github/scripts/check_vip_multitest_upgrade.py /path/to/game
```

Exit code `0` means no legacy files were found, `2` requires manual review and `1` means the installation path is invalid. The check does not delete or change anything.
