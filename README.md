# Bulanci-Kitty

2D střílečka z pohledu shora napsaná v jazyce **C** s využitím knihovny **SDL2**. Vytvořena jako semestrální projekt na VŠB-TUO.

Hráč bojuje na ručně sestavené mapě proti AI botům. Hra obsahuje hlavní nabídku, nastavení, různé zbraně, detekci kolizí, HUD a systém záznamů výsledků.

***

## Technologie

- **Jazyk:** C
- **Grafika / vstup / zvuk:** SDL2, SDL2_image, SDL2_mixer, SDL2_ttf
- **Sestavení:** Makefile + GCC
- **Platforma:** Linux / WSL

***

## Funkce

- 🎮 Pohyb hráče a střelba
- 🤖 AI boti se základní logikou
- 💥 Systém střel a detekce kolizí
- 🗺️ Ručně sestavená mapa s umístěnými objekty
- 🔫 Více typů zbraní
- 🎵 Hudba na pozadí a zvukové efekty
- 📋 Hlavní nabídka, nastavení, žebříček
- 🏆 Výsledky her ukládány do textového souboru

***

## Struktura projektu

```text
Bulanci-Kitty/
├── src/               # Zdrojové kódy (.c)
├── include/           # Hlavičkové soubory (.h)
├── build/             # Zkompilované objektové soubory (.o)
├── Assets/            # Sprity, dlaždice mapy, zvuky (volné assety z itch.io)
├── Records/           # Záznamy výsledků
├── Makefile           # Soubor pro sestavení projektu (kompilaci)
├── setup.sh           # Bash skript pro instalaci závislostí
├── README.md          # Dokumentace projektu
└── game_results.txt   # Trvalé záznamy výsledků
```

***

## Sestavení a spuštění

### Závislosti

```bash
chmod +x setup.sh
./setup.sh
```

### Případně je můžete nainstalovat ručně (Debian/Ubuntu/WSL):

```bash
sudo apt update
sudo apt install -y build-essential libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

### Sestavení

```bash
make
```

### Spuštění

```bash
./Bulanci
```

### Vyčištění build souborů

```bash
make clean
```

***

## Assety

Herní assety (sprity postav, dlaždice mapy, objekty) jsou volně dostupné zdroje z [itch.io](https://itch.io/game-assets/free). Rozmístění mapy bylo sestaveno a upraveno ručně přímo v kódu.

***

## Poznámka

Semestrální projekt — VŠB-TUO. Není určen pro produkční nasazení.
