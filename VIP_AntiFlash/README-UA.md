[EN](README.md) | [RU](README-RU.md)

# [VIP] [AntiFlash](https://github.com/bywinsty/cs2-vip-modules/tree/main/VIP_AntiFlash)
## Додає VIP-гравцям можливість не засліплюватися від гранат

Додайте ключ можливості модуля до `addons/configs/vip/groups.ini` та ключ перекладу — до `addons/translations/vip.phrases.txt`.

### Ключ можливості

У `groups.ini` додайте:
```
"antiflash" "значення"
```
**Можливі значення:**
- 1 — не засліплюватися від команди;
- 2 — не засліплюватися від власної гранати;
- 3 — не засліплюватися від гранат союзників і власних гранат;
- 4 — повністю вимкнути засліплення.

### Ключ перекладу

У `vip.phrases.txt` додайте ключ `antiflash`:
```
	"antiflash"
	{
		"en"	"Anti-Flash"
		"ua"	"Антифлеш"
		"ru"	"Антифлеш"
	}
```
