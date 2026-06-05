const fs = require('fs');

const targetPath = 'bin/data/game_data/dungeons.json';
const srcPath = 'talesdialogue/dungeon2_knowledge_wisdom.txt';

function parseTxt(txt) {
  // Returns map: depthNumber -> { pre: [{speaker,text}], post: [{speaker,text}] }
  const lines = txt.split(/\r?\n/);
  const result = new Map();

  let currentDepth = null;
  let currentSection = null; // 'pre' | 'post'

  // Example: "- Depth 1: \"Title\"" or "- Depth 1: Title"
  const depthRe = /^-\s*Depth\s+(\d+)\s*:/i;
  const sectionRe = /^\s*(Pre|Post)\s*:\s*$/i;
  // Example: "  The Chronicler: \"text...\""
  const speakerLineRe = /^\s*([^:]+):\s*"(.*)"\s*$/;

  for (const raw of lines) {
    const line = raw;

    const mDepth = line.match(depthRe);
    if (mDepth) {
      currentDepth = parseInt(mDepth[1], 10);
      if (!result.has(currentDepth)) result.set(currentDepth, { pre: [], post: [] });
      currentSection = null;
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
      const text = mSpeaker[2].replace(/\\n/g, '\n');
      result.get(currentDepth)[currentSection].push({ speaker, text });
    }
  }

  return result;
}

const target = JSON.parse(fs.readFileSync(targetPath, 'utf8'));
const txt = fs.readFileSync(srcPath, 'utf8');
const parsed = parseTxt(txt);

const dungeonId = 'dungeon_of_knowledge_and_wisdom';
const dungeon = target.dungeons.find(d => d.id === dungeonId);
if (!dungeon) throw new Error(`Dungeon not found: ${dungeonId}`);

let updatedDepths = 0;
let skippedDepths = 0;

for (const depthRow of dungeon.depths) {
  const depth = depthRow.depth;
  if (!parsed.has(depth)) {
    skippedDepths++;
    continue;
  }

  const { pre, post } = parsed.get(depth);
  depthRow.dialog = depthRow.dialog || {};
  depthRow.dialog.on_enter = pre;
  depthRow.dialog.on_clear = post;
  updatedDepths++;
}

// Write with stable formatting
fs.writeFileSync(targetPath, JSON.stringify(target, null, 2) + '\n', 'utf8');

console.log('Patched', { srcPath, targetPath, dungeonId, updatedDepths, skippedDepths });

