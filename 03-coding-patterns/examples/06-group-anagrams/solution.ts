// Group Anagrams — TypeScript
function groupAnagrams(strs: string[]): string[][] {
    const groups = new Map<string, string[]>();
    for (const s of strs) {
        const count = new Array(26).fill(0);
        for (const ch of s) {
            count[ch.charCodeAt(0) - 97]++;
        }
        const key = count.join("#");
        const bucket = groups.get(key);
        if (bucket) bucket.push(s);
        else groups.set(key, [s]);
    }
    return Array.from(groups.values());
}

function canon(v: string[][]): string {
    return JSON.stringify(v.map(g => g.slice().sort()).sort());
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

check(
    canon(groupAnagrams(["eat", "tea", "tan", "ate", "nat", "bat"])) ===
    canon([["eat", "tea", "ate"], ["tan", "nat"], ["bat"]]),
    "main-case",
);
check(groupAnagrams([]).length === 0, "empty");
check(canon(groupAnagrams([""])) === canon([[""]]), "empty-string");

console.log("PASS");
