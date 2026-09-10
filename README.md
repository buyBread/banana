# Banana
"Spider-Man: Web of Shadows" reverse-engineering playground.

## Project Status
At the moment, Banana doesn't provide any truly unique functionality to Web of Shadows beyond a handful of tiny miscellaneous additions. Development is mainly focused on achieving an implementation of NGL that has parity with the retail PC version of the game. The code also lacks clarity in places and only contains comments sparsely, but the good news is that I've largely stuck to what the IDA disassembler had shown me, so it should be possible to "intuit" your way around the codebase by viewing the game's assembly side-by-side.

No, I'm not sure what percentage of NGL I've covered at this point. While it is at a point where it's rendering a substantial amount of the game, a large portion of that work still relies on binary-owned callbacks, constructors and so on.

## Installing
> prerequisites: `(MSVC) Build Tools` & `xmake`
1. Select the build mode with `xmake f -m MODE` (`release`/`devel`/`debug`).
2. Compile with `xmake`.
3. Place the resulting `.dll` in `/image/pc/` and launch the game.

## Compatibility
Banana supports loading DXVK and ReShade under these conditions:
* **DXVK**: there is a file in `image/pc/` named `dxvk.dll`.
* **ReShade**: there is a file in `image/pc/reshade/` named `d3d9.dll`.
* **exWoS** / **WOSTweaks**: Banana's capability to load alongside exWoS or WOSTweaks is basically a non-consideration. Unlike a traditional "cheat-like" project, Banana owns (and will continue to own more) parts of the executable. This effectively kills (and will continue to kill) hooks that the two projects rely on.

## Credits
* **kirbystealer** -- most of my early poking at game pack loading was basically confirmed by templating it against his Python scripts. A chunk of my behind-the-scenes tooling owes its initial existence to him.
* **Archiver of Triviality** -- without WOSTweaks, the Web of Shadows scene would be preeeettty dead. Seeing the amount of neat things you can do within the game's engine also inspired me to actually apply myself towards a part of my childhood that I'm quite fond of...