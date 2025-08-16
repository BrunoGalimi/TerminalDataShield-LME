# TerminalDataShield-LME

A project I created in high school when I needed to store passwords securely — and I was already loving programming. I’ve always wanted to share it publicly, and now here it is.

---

## 🧠 About the Project

**TerminalDataShield-LME** is the portable (terminal-based) version of my larger DataShield project.

Its goal is simple: **secure your data** in personal vaults using a custom-made encryption algorithm called **LME** (Log Modular Encryption).  
If you switch computers or want to back up your vault, just copy the corresponding `.dat` file — the encryption is portable, and designed to work across systems (once I finalize cross-platform support).

---

## ✨ Features

- 📁 Create and manage password vaults
- 🔐 Save and encrypt data locally using a custom algorithm
- 🎨 Choose between 3 different interface styles (console themes)
- ⚙️ Customize command prefix and terminal commands
- 📦 Vaults are fully portable (`.dat` files)

---

## 🚀 Portability (Manual for now)

You can manually transfer a vault to another machine by:

1. Creating a new vault on the target system **with the same name**.
2. Using **exactly the same password** (otherwise the data will be unreadable).
3. Replacing the newly created `.dat` file with the original `.dat` file from your old machine.
4. Launching the vault normally: your data will be recovered.

> In the future, this migration will be handled natively.

---

## 📌 Notes

- This is a **terminal-based CLI tool**, not a GUI.
- For now, it’s **Windows-only**, but I’m planning a Linux/macOS rewrite.
- There is also a GUI version, which I use daily — but it's not yet ready for public release.

---

## 📜 Disclaimer

This is a personal project created for learning purposes and passion.  
It is **not** meant to replace professional password managers (yet).

---

## 🛠 Built With

- Language: C
- Encryption: Custom `LME` (Log Modular Encryption)
- Platform: Windows console
