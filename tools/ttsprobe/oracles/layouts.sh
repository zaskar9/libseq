#!/bin/bash
# layouts.sh VER SETUPFILE SPEC...: tries each SPEC "NAME|layout1|layout2..." after the setup lines
# (one process per try) and prints the first layout whose call returns and leaves the probe working,
# with the lines the probe printed after the setup.
here=$(cd "$(dirname "$0")" && pwd); ver=$1; setup=$2; shift 2
for spec in "$@"; do
  name=${spec%%|*}; rest=${spec#*|}; IFS='|' read -ra lays <<< "$rest"; ok=""
  for lay in "${lays[@]}"; do
    out=$({ cat "$setup"; echo "echo >>>"; echo "call $name $lay"; echo "dump 0 0 32"; echo "dump 1 0 16"; echo "echo <<<"; } | "$here/try.sh" $ver 15)
    if echo "$out" | grep -q "^<<<"; then
      echo "== $name [$lay]"; echo "$out" | sed -n '/^>>>/,/^<<</p' | grep -v '^>>>\|^<<<'; ok=1; break
    fi
  done
  [ -z "$ok" ] && echo "== $name: no layout worked"
done
