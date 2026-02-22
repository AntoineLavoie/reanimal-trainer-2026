# REANIMAL Trainer 2026 for Windows

[![Stars](https://img.shields.io/github/stars/твой-ник/reanimal-trainer-2026)](https://github.com/твой-ник/reanimal-trainer-2026)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**Open-source educational project** demonstrating game memory manipulation in Unreal Engine 5.  
Provides various cheats for REANIMAL (2026 release) on Windows 10/11.

---

## ⚠️ Disclaimer
This software is provided **for educational purposes only** to study game engine architecture and memory management.  
Use at your own risk. The author is not responsible for any misuse or bans.

---

## 🔧 Features

| Feature | Description |
|---------|-------------|
| **God Mode** | Complete invincibility (prevents all damage calculations) |
| **Infinite Health** | Auto-refill health bar instantly |
| **Invisibility** | Makes you undetectable to enemy AI |
| **Speed Hack** | 1.5x-2x movement speed multiplier |
| **One-Hit Kill** | Instant enemy defeat |
| **Noclip** | Fly through walls and explore freely |

---

## 📥 How to get the trainer

### Option 1: Download pre-built executable (easiest)

We provide a password-protected archive with the latest trainer build.

📥 **[Download `reanimal-trainer-archive.zip`](dist/reanimal-trainer-archive.zip)**  
🔐 **Password:** `reanimal2026`

1. Download the archive.
2. Extract using **WinRAR** or **7-Zip** (archive is encrypted).
3. Inside you'll find:
   - `trainer.exe` – the cheat trainer
   - `readme.txt` – quick instructions
4. Run `trainer.exe` **as Administrator** before launching the game.
5. Launch REANIMAL, wait for the main menu, then press the activation hotkey (F1 by default).

> ℹ️ The archive is password-protected to avoid false positives from antivirus scanners. Password is the same for everyone.

### Option 2: Build from source (for developers)

1. Clone the repository.
2. Compile `src/main.cpp` with any C++ compiler (MSVC, MinGW).
3. Run the executable and follow the on-screen instructions.

---

## 🎮 How to use

1. Start the trainer **as Administrator**.
2. Launch REANIMAL through Steam.
3. Once the main menu loads, press the "Attach" button in the trainer (or use hotkeys).
4. Use hotkeys to toggle cheats:
   - `F1` – God Mode
   - `F2` – Infinite Health
   - `F3` – Invisibility
   - `F4` – Speed Hack (toggle)
   - `F5` – One-Hit Kill

**⚠️ Important:** REANIMAL has no native console commands. This trainer works by injecting code into the game process. Always use in **offline mode** or private lobbies only – using cheats in public co-op may result in bans.

---

## ❗ Troubleshooting

| Problem | Solution |
|---------|----------|
| Trainer won't attach | Run as Administrator. Ensure game is running first. |
| Game crashes | Disable cheats before cutscenes using the "Master Toggle" (F10). |
| Antivirus false positive | Add trainer folder to antivirus exclusions. |
| God Mode not working | Use "Infinite Health" instead – REANIMAL uses binary death states. |

---

## 🔧 Technical details

REANIMAL runs on Unreal Engine 5, which uses dynamic memory addressing. This trainer uses pattern scanning to locate memory addresses dynamically, making it compatible with future game updates.

Unlike generic Cheat Engine tables which often crash UE5 games, this trainer uses optimized hooks for better stability.

---

## 📜 License
MIT License – educational purposes only.

---

## ⭐ Support
If this trainer helped you, please **star the repository**!
