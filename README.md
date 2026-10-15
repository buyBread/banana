# Banana
The goal of this project is "simple": reach full behavioral parity with the game's binary and translate the assembly into human-readable engine code, aftewards -- branch into modifying the codebase to our liking. Modifications will primary seek to:
* Fix flaws with the game's rendering code (i.e. Z-fighting, broken post processing, etc.);
* Modernize the engine's stack (i.e. `DirectX9` -> `DirectX12`);
* Use the recovered semantics to produce tooling for modifying in-game assets;
* ...and whatever else comes up along the way.
  
Contributions are highly appreciated, please consult [CONTRIBUTING.md](CONTRIBUTING.md) for further information.  

## Discord
When the need arises, I'll see to starting a proper community space for this project's needs specifically. Until then, feel free to contact me over Discord DMs (`@buybread`) at any time.

## Project Status
It's hard to estimate concrete percentages, as there's an entire STL compiled into the binary, much of it not needed to produce retail behavior (modern STL does the job in many cases). Currently, most of the work is regarding concrete game systems rather than backend components or concepts, the majority of which have been already recovered and often get heavily inlined too or duplicated due to templating, further complicating any attempts to estimate at this stage. If I really had to produce at least a vague number, it'd likely be around ~10% as of commit `27f37ec`.

## Compiling
### Prerequisites
* `MSVC Build Tools 18`
* `xmake`
* v1.1 of the video game in question.

### Installing
1. Compile with `xmake`.
    - Select the build mode with `xmake f -m <MODE>` -- `release` / `devel` / `debug`.
2. Place the resulting `d3d9.dll` in `/image/pc/` and launch the game.

...*log files are stored in* `%LOCALAPPDATA%\banana\logs\`.

## Compatibility
* **DXVK**: when there is a file in `image/pc/` named `dxvk.dll`.
* **ReShade**: when there is a file in `image/pc/reshade/` named `d3d9.dll`; installing ReShade for Vulkan works with DXVK too.
* **exWoS** / **WOSTweaks**: Banana's capability to load alongside exWoS or WOSTweaks is a non-consideration.

## Credits
* **kirbystealer** -- most of my early poking at this game was on pack formats & loading, much it getting confirmed by templating it against his Python scripts. As such, a chunk of my behind-the-scenes tooling owes its initial existence to him.
* **Archiver of Triviality** -- without WOSTweaks, the Web of Shadows scene would be preeeettty dead. Seeing the amount of neat things you can do within the game's engine also inspired me to actually apply myself towards this game more seriously.
* **MrMartinIden** -- OpenUSM really skyrocketed my understanding of the engine when I first began to poke around the various binaries I could get my hands on. Massive head start to be honest.