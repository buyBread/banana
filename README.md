# Banana
"Spider-Man: Web of Shadows" reverse-engineering playground (kind off...). Basically, this is a scope explosion problem on my end. This was initially just a silly codebase where I messed around with ideas here and there, then decided to own the rendering (NGL) and the rest is just me slowly being in denial that I've started a decompilation project.

## Project Status
A lot of the game's rendering goes through owned methods, but basically every other part is still majority native.  
  
The goal is "simple": reach full (or near-full) parity with the binary and branch into modifying things to make the game better. There are definitely better choices for doing individual fixes that everyone wants (i.e. making post processing work), but my long-term vision includes extending the entire game with cut content as well one day, so this is the only sane choice to be honest.  

## Prerequisites
* `(MSVC) Build Tools`
* `xmake`
* v1.1 of the video game in question.

## Installing
1. Select the build mode with `xmake f -m MODE` (`release`/`devel`/`debug`).
2. Compile with `xmake`.
3. Place the resulting `.dll` in `/image/pc/` and launch the game.

...*log files are stored in* `%LOCALAPPDATA%\banana\logs\`

## Compatibility
* **DXVK**: when there is a file in `image/pc/` named `dxvk.dll`.
* **ReShade**: when there is a file in `image/pc/reshade/` named `d3d9.dll`.
* **exWoS** / **WOSTweaks**: Banana's capability to load alongside exWoS or WOSTweaks is basically a non-consideration. Unlike a traditional "cheat-like" project, Banana owns (and will continue to own more) parts of the executable. This can easily produce UBs for the two projects or eliminate hook points that the two projects might be relying on.

## Credits
* **kirbystealer** -- most of my early poking at game pack loading was basically confirmed by templating it against his Python scripts. A chunk of my behind-the-scenes tooling owes its initial existence to him.
* **Archiver of Triviality** -- without WOSTweaks, the Web of Shadows scene would be preeeettty dead. Seeing the amount of neat things you can do within the game's engine also inspired me to actually apply myself towards a part of my childhood that I'm quite fond of...