# banana
"Spider-Man: Web of Shadows" reverse-engineering playground, not really anything with a clear purpose that's usable.

## Goals (loosely)
* Finish renderer bootstrap (boundaries are TBD, hopefully full w/ enough motivation).
* Like, maybe downgrade graphics to more clay-ish(?) look from pre-release gameplay.
* Do something with the reversed script VM beyond asking for vibe-coded UIs to admire it in action.
* ~~Go through notes and actually document the code instead of just implicitly assuming everything makes sense, lol.~~ (harder than anything on this list)

## Installing
> This project is compiled using `MSVC Build Tools 18` and managed with `xmake`, rest of the prerequisites are taken care of by `xmake`.
1. Select the build mode with `xmake f -m MODE` (`release`/`devel`/`debug`).
2. Compile with `xmake`.
3. Place the resulting `.dll` in `/image/pc/` and launch the game.