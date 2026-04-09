# Bulanci-Kitty

2D střílečka z pohledu shora napsaná v jazyce **C** s využitím knihovny **SDL2**. Vytvořena jako semestrální projekt na VŠB-TUO.

Hráč bojuje na ručně sestavené mapě proti AI botům. Hra obsahuje hlavní nabídku, nastavení, různé zbraně, detekci kolizí, HUD a systém záznamů výsledků.

***

## Technologie

- **Jazyk:** C
- **Grafika / vstup / zvuk:** SDL2, SDL2_image, SDL2_mixer, SDL2_ttf
- **Sestavení:** Makefile + GCC
- **Platforma:** Linux

***

## Funkce

- 🎮 Pohyb hráče a střelba
- 🤖 AI boti se základní logikou (`bots.c`)
- 💥 Systém střel a detekce kolizí
- 🗺️ Ručně sestavená mapa s umístěnými objekty
- 🔫 Více typů zbraní (`weapons.c`)
- 🎵 Hudba na pozadí a zvukové efekty
- 📋 Hlavní nabídka, nastavení, žebříček
- 🏆 Výsledky her ukládány do `game_results.txt`

***

## Struktura projektu

```
Bulanci-Kitty/
├── Assets/            # Sprity, dlaždice mapy, zvuky (volné assety z itch.io)
├── Records/           # Záznamy výsledků
├── main.c             # Vstupní bod, herní smyčka
├── player.c/h         # Logika hráče
├── bots.c/h           # AI botů
├── bullet.c/h         # Pohyb a životnost střel
├── collision.c/h      # Detekce kolizí
├── weapons.c/h        # Typy zbraní a jejich parametry
├── hud.c/h            # Vykreslení HUD (HP, náboje, skóre)
├── init.c/h           # Inicializace SDL2 a okna
├── show_window.c/h    # Vykreslení mapy a objektů
├── main_menu.c/h      # Hlavní nabídka
├── options_menu.c/h   # Obrazovka nastavení
├── records_menu.c/h   # Žebříček výsledků
├── buttons.c/h        # Logika tlačítek
├── music.c/h          # Hudba a zvukové efekty
├── character.c/h      # Sdílená data postav
├── common.h           # Sdílené konstanty a struktury
├── game_results.txt   # Trvalé záznamy výsledků
└── Makefile           # Soubor pro sestavení
```

***

## Sestavení a spuštění

### Závislosti

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
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
