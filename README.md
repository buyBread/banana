# Banana
"Spider-Man: Web of Shadows" reverse-engineering playground (kind of...). Basically, this is a scope explosion problem on my end. This was initially just a silly codebase where I messed around with ideas here and there, then decided to own the rendering (NGL) and the rest is just me slowly being in denial that I've started a decompilation project.  
  
Contributions are very appreciated, but for anyone with plans to onboard long-term, I'd prefer to be contacted via Discord (`@buybread`) to discuss the matter.  
  
Even for any small-bite commits, I'd like to make it clear that recovering semantics is a core focus. I highly recommend cross-referencing against these two binaries:
* https://hiddenpalace.org/Spider-Man:_Web_of_Shadows_(May_30,_2007_prototype)
* https://hiddenpalace.org/Ultimate_Spider-Man_(Jun_20,_2005_Fusion_prototype)

## Project Status
It's hard to estimate concrete percentages, but the core engine concepts are essentially complete with the exception of `NAL` (the animation backend). Currently left work is on the various game systems, occasionally dipping back into the exciting internals for a moment or two as the need arises. If I really had to say some concrete number, I'd wager around ~10% at most as of commit `27f37ec` if we did some napkin math (mentally, not on a napkin).  
  
The goal is "simple": reach full parity with the binary and translate the game's assembly into human-readable engine code, then branch into modifying things. There are definitely better choices for doing individual fixes that everyone wants (i.e. making post processing work), but my long-term vision includes both upgrading the engine to a more modern stack and extending the entire game with cut content as well one day. At that point, this path is the only sane choice to be honest.  
  
I'm not particularly interested in keeping the original project structure Treyarch/Shaba were likely working in, since the ultimate goal is modernization rather than larping as an overworked office worker on a deadline to ship a peak Spider-Man game.

## Prerequisites
* `MSVC Build Tools 18`
* `xmake`
* v1.1 of the video game in question.

## Installing
0. Select the build mode with `xmake f -m MODE` (`release`/`devel`/`debug`).
1. Compile with `xmake`.
2. Place the resulting `d3d9.dll` in `/image/pc/` and launch the game.

...*log files are stored in* `%LOCALAPPDATA%\banana\logs\`.

## Compatibility
* **DXVK**: when there is a file in `image/pc/` named `dxvk.dll`.
* **ReShade**: when there is a file in `image/pc/reshade/` named `d3d9.dll`, otherwise installing ReShade into DXVK works just fine.
* **exWoS** / **WOSTweaks**: Banana's capability to load alongside exWoS or WOSTweaks is basically a non-consideration.

## Credits
* **kirbystealer** -- most of my early poking at game pack loading was basically confirmed by templating it against his Python scripts. A chunk of my behind-the-scenes tooling owes its initial existence to him.
* **Archiver of Triviality** -- without WOSTweaks, the Web of Shadows scene would be preeeettty dead. Seeing the amount of neat things you can do within the game's engine also inspired me to actually apply myself towards a part of my childhood that I'm quite fond of...
* **MrMartinIden** -- OpenUSM really skyrocketed my understanding of the engine when I first began to poke around the various binaries I could get my hands on. Massive head start to be honest.