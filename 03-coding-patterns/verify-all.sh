#!/usr/bin/env bash
# Verify every problem in the algorithm bank against Python, C++ (clang++ -std=c++20), and TypeScript.
# Run from anywhere: `bash 03-coding-patterns/verify-all.sh`
set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/examples"
fails=0
for dir in */; do
    name="${dir%/}"
    echo "=== $name ==="

    if python3 "$dir/solution.py" >/dev/null 2>&1; then
        echo "  py OK"
    else
        echo "  PY FAIL:"
        python3 "$dir/solution.py" 2>&1 | sed 's/^/    /'
        fails=$((fails+1))
    fi

    if clang++ -std=c++20 -O2 "$dir/solution.cpp" -o /tmp/_sol_$$ 2>/tmp/_cpperr_$$; then
        if /tmp/_sol_$$ >/dev/null 2>&1; then
            echo "  cpp OK"
        else
            echo "  CPP RUN FAIL:"
            /tmp/_sol_$$ 2>&1 | sed 's/^/    /'
            fails=$((fails+1))
        fi
    else
        echo "  CPP COMPILE FAIL:"
        head -5 /tmp/_cpperr_$$ | sed 's/^/    /'
        fails=$((fails+1))
    fi
    rm -f /tmp/_sol_$$ /tmp/_cpperr_$$

    if npx --yes tsx "$dir/solution.ts" >/dev/null 2>&1; then
        echo "  ts OK"
    else
        echo "  TS FAIL:"
        npx --yes tsx "$dir/solution.ts" 2>&1 | tail -5 | sed 's/^/    /'
        fails=$((fails+1))
    fi
done
echo ""
echo "TOTAL FAILS: $fails"
exit $fails
