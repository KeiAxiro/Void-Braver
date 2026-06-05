const fs = require("fs");

const targetPath = "bin/data/game_data/dungeons.json";
const srcPath = "scripts/talesdialogue/dungeon1_valor_ember.txt";

function parseTxt(txt) {
  // Returns map: depthNumber -> {pre:[{speaker,text}], post:[{speaker,text}]}
  const lines = txt.split(/\r?\n/);
  const result = new Map();
  let currentDepth = null;
  let currentSection = null; // 'pre'|'post'
  let currentSpeaker = null;

  const depthRe = /^- Depth (\d+):\s+".*"\s*$/;
  const sectionRe = /^\s*(Pre|Post):\s*$/;
  const speakerLineRe = /^\s*([^:]+):\s+"(.*)"\s*$/;

  for (const raw of lines) {
    const line = raw;
    const mDepth = line.match(depthRe);
    if (mDepth) {
      currentDepth = parseInt(mDepth[1], 10);
      if (!result.has(currentDepth))
        result.set(currentDepth, { pre: [], post: [] });
      currentSection = null;
      currentSpeaker = null;
      continue;
    }

    const mSec = line.match(sectionRe);
    if (mSec) {
      currentSection = mSec[1].toLowerCase();
      continue;
    }

    const mSpeaker = line.match(speakerLineRe);
    if (mSpeaker && currentDepth != null && currentSection) {
      const speaker = mSpeaker[1].trim();
      const text = mSpeaker[2].replace(/\\n/g, "\n");
      result.get(currentDepth)[currentSection].push({ speaker, text });
    }
  }

  return result;
}

const target = JSON.parse(fs.readFileSync(targetPath, "utf8"));
const txt = fs.readFileSync(srcPath, "utf8");
const parsed = parseTxt(txt);

const dungeon = target.dungeons.find(
  (d) => d.id === "dungeon_of_valor_and_ember",
);
if (!dungeon) throw new Error("Dungeon not found");

for (const depthRow of dungeon.depths) {
  const depth = depthRow.depth;
  if (!parsed.has(depth)) continue;
  depthRow.dialog = depthRow.dialog || {};
  depthRow.dialog.on_enter = parsed.get(depth).pre;
  depthRow.dialog.on_clear = parsed.get(depth).post;
}

fs.writeFileSync(targetPath, JSON.stringify(target, null, 2) + "\n", "utf8");
console.log("Patched", srcPath, "->", targetPath);
