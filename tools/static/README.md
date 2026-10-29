# Static inspection (step 1)

The scripts used on the first day to look at the files of the Cakewalk Apprentice 1.04
installation disk without running them (`docs/reverse-engineering/installation-media.md`). They
read file structures only: headers, tables, resources. None of them disassembles code.

| Script | Reads |
|---|---|
| `ne.py FILE...` | the NE header: linker, flags, module name, imported modules, exports, segments |
| `neimp.py FILE...` | the imported functions, by name or by ordinal, from the relocation records |
| `resolve.py FILE...` | the same, with the ordinals of the Windows system modules resolved to names through Wine's `.spec` files (`WINE_SPECS`, see the script) |
| `neres.py FILE` | the resources: menus, string table, dialogs, version |
| `hlp.py FILE.HLP` | the text of a WinHelp 3.1 file, after the published description of the format |
| `wrk.py FILE...` | a chunk dump of `.WRK` files, as first written; some layouts were corrected later |

The compressed files on the disk (`*.EX_`, `*.DL_`, ...) must be expanded first: `msexpand` from
libmspack reads both formats on the disk (`KWAJ` and `SZDD`). The outputs of `ne.py` and
`resolve.py` for the disk are in `docs/reverse-engineering/static/`.
