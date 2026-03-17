import os
import shutil
from pathlib import Path

def main():
    print("🚀 Memulai proses restrukturisasi folder...")

    # 1. Daftar folder baru yang akan dibuat di dalam 'include/'
    new_folders = [
        "include/Combat",
        "include/Core",
        "include/Entities",
        "include/Models",
        "include/States",
        "include/Utils",
        "include/lib"
    ]

    for folder in new_folders:
        Path(folder).mkdir(parents=True, exist_ok=True)
        print(f"📁 Dibuat/Dicek: {folder}")

    # 2. Mapping pemindahan file (Dari -> Ke)
    # Path disesuaikan dengan struktur awalmu
    file_moves = {
        # Combat
        "src/Combat/Include/CombatMath.h": "include/Combat/CombatMath.h",
        "src/Combat/Include/Commands.h": "include/Combat/Commands.h",
        # Core
        "src/Core/Include/AccountManager.h": "include/Core/AccountManager.h",
        "src/Core/Include/Character.h": "include/Core/Character.h",
        "src/Core/Include/Game.h": "include/Core/Game.h",
        "src/Core/Include/GameContext.h": "include/Core/GameContext.h",
        "src/Core/Include/StateManager.h": "include/Core/StateManager.h",
        "src/Core/Include/Stats.h": "include/Core/Stats.h",
        # Entities
        "src/Entities/Include/Enemy.h": "include/Entities/Enemy.h",
        "src/Entities/Include/Entity.h": "include/Entities/Entity.h",
        "src/Entities/Include/Player.h": "include/Entities/Player.h",
        # States
        "src/States/Include/CharSelectionState.h": "include/States/CharSelectionState.h",
        "src/States/Include/GameState.h": "include/States/GameState.h",
        "src/States/Include/HubState.h": "include/States/HubState.h",
        "src/States/Include/LoginState.h": "include/States/LoginState.h",
        "src/States/Include/MainMenuState.h": "include/States/MainMenuState.h",
        "src/States/Include/RegisterState.h": "include/States/RegisterState.h",
        # Utils (Utils mu sebelumnya nggak pakai folder Include)
        "src/Utils/ConsoleUI.h": "include/Utils/ConsoleUI.h",
        # lib
        "src/lib/json.hpp": "include/lib/json.hpp",
        "src/lib/siphash.h": "include/lib/siphash.h",
    }

    # Proses pindah file
    print("\n🚚 Memindahkan file header...")
    for old_path_str, new_path_str in file_moves.items():
        old_path = Path(old_path_str)
        new_path = Path(new_path_str)
        
        if old_path.exists():
            shutil.move(str(old_path), str(new_path))
            print(f"  ✅ {old_path.name} -> {new_path.parent}")
        elif new_path.exists():
            print(f"  ⚡ {new_path.name} sudah ada di tempat baru, di-skip.")
        else:
            print(f"  ❌ Gagal menemukan: {old_path_str}")

    # 3. Bersih-bersih folder 'Include' yang sudah kosong di src
    print("\n🧹 Membersihkan folder 'Include' lama yang sudah kosong...")
    old_include_dirs = [
        "src/Combat/Include",
        "src/Core/Include",
        "src/Entities/Include",
        "src/States/Include"
    ]

    for dir_str in old_include_dirs:
        dir_path = Path(dir_str)
        if dir_path.exists() and dir_path.is_dir():
            try:
                dir_path.rmdir() # Hanya akan menghapus jika foldernya kosong
                print(f"  🗑️ Dihapus: {dir_str}")
            except OSError:
                print(f"  ⚠️ Tidak bisa dihapus (mungkin tidak kosong): {dir_str}")

    print("\n🎉 Restrukturisasi selesai! Silakan cek foldermu.")

if __name__ == "__main__":
    main()