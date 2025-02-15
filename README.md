# Bulanci

***Bulanci*** je strategická hra vyvinutá pomocí knihovny SDL2. Hráči ovládají postavu, která se pohybuje po mapě, střílí na nepřátele a sbírá body za úspěšné zásahy. Cílem je dosáhnout co nejvyššího skóre během stanoveného času.

## Požadavky

- **Operační systém:** Linux, macOS nebo Windows
- **Kompilátor:** GCC
- **Knihovny:**
  - SDL2
  - SDL2_image
  - SDL2_ttf
  - SDL2_mixer
  - Math library (-lm)
  - AddressSanitizer (volitelně pro ladění)

## Instalace závislostí

### Pro Ubuntu/Debian

Otevřete terminál a spusťte následující příkazy:

```bash
sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev pkg-config
```

### Pro macOS

```bash
brew update
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer pkg-config
```

## Kompilace hry

Použijte poskytnutý Makefile pro kompilaci hry. Spusťte následující příkazy v adresáři projektu:

```bash
make
```

Pro vyčištění zkompilovaných souborů použijte:

```bash
make clean
```

## Ovládání hry

### První hráč:

- Pohyb: W, A, S, D
- Střelba: Levý Ctrl
- Nabíjení: R
- Výběr zbraně: 1, 2, 3

### Druhý hráč:

- Pohyb: Šipky
- Střelba: M
- Nabíjení: P
- Výběr zbraně: 8, 9, 0

### Obecně:

- **Escape**: V každé situaci vrací do hlavního menu
- **F1**: Debugovací menu pro kontrolu levelMap

## Jak hrát

1. Spusťte spustitelný soubor hry:
   ```bash
   ./Bulanci
   ```
2. Použijte ovládání popsané v předchozí sekci pro hraní hry.
3. Cílem je dosáhnout co nejvyššího skóre střelbou na nepřátele.

## Příspěvky

Příspěvky jsou vítány! Forkněte tento repozitář, proveďte změny a pošlete pull request.
