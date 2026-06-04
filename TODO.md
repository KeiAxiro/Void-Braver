# TODO

- [ ] Edit `src/states/battle_dungeon.cpp`: saat `currentNode->isFinalNode` selesai dan reward depth sudah diberikan, jangan `leaveBattleState(); return;`.
- [ ] Ganti dengan pemanggilan menu pemilihan depth untuk dungeon yang sama (gunakan `selectDungeonDepth(ctx, ...)`).
- [ ] Pastikan state stack tidak menyebabkan battle dobel / stuck.
- [ ] Build & jalankan, verifikasi: setelah clear depth -> menu pilih depth dungeon terkait, bukan kembali ke Hub.

