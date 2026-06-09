# 2D-Graphic_Editor
A terminal-based 2D graphics editor that uses `*` and `_` characters to draw shapes on a 2D character array canvas.
## Features

- Draw **Circle**, **Rectangle**, **Line**, and **Triangle**
- **Add**, **Delete**, and **Modify** objects on the canvas
- 25×60 character canvas rendered in the terminal
- Interactive text menu

## How to Compile and Run

```bash
gcc graphics_editor.c -o graphics_editor -lm
./graphics_editor
```

## Canvas Size

- Rows: 25
- Columns: 60
- Characters used: `*` (fill) and `_` (edge)

## Sample Session

```
Add object → Circle → centre 12 30 → radius 8
Add object → Rectangle → top-left 2 2 → bottom-right 10 20
Add object → Line → start 0 0 → end 24 59
```

## Author

Madhava — Reva University
