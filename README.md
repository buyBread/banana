# Banana
An ongoing engine-reconstruction project for the PC version of "Spider-Man: Web of Shadows".
  
The goal of this project is "simple": reach full behavioral parity with the game's binary and translate the assembly into human-readable engine code, afterwards -- branch into modifying the codebase to our liking. Modifications will primarily seek to:
* Fix flaws with the game's rendering code (i.e. Z-fighting, broken post-processing, etc.);
* Modernize the engine's stack (i.e. `DirectX9` -> `DirectX12`);
* Use the recovered semantics to produce tooling for modifying in-game assets;
* ...and whatever else comes up along the way.
  
Contributions are highly appreciated, please consult [CONTRIBUTING.md](CONTRIBUTING.md) for further information.  

You can check out the unit tests for getting a basic overview of certain key engine concepts. They're intentionally kept comment-heavy specifically for this purpose in the absence of any sort of traditional documentation (...at the moment).

## Discord
When the need arises, I'll see to starting a proper community space for this project's needs specifically. Until then, feel free to contact me over Discord DMs (`@buybread`) at any time.

## Project Status
* At the moment, Banana builds into a shared library that patches several rel32 call sites in the binary at runtime. To keep it from becoming stupid, these are kept at a minimum and coverage typically happens downstream of `WinMain` rather than needlessly patching thousands of addresses.
* It's difficult to put a meaningful percentage on the project's progress, as there's the entire VC8 STL compiled into the binary. Extensive inlining and template instantiation also introduce a lot of duplication throughout the binary, making it even harder to estimate overall progress.
* If I really had to produce at least a vague number, it'd likely be around ~10% as of commit `27f37ec`. Likewise, listing out what exactly is source-owned here at any given moment is a largely useless endeavour, as coverage is constantly expanding.
* In the future, I do hope to set up more public-facing means of viewing Banana's progress.

## Compiling
### Prerequisites
* `MSVC Build Tools 18`
* `xmake`
* v1.1 of the video game in question.

### Installing
1. Compile with `xmake`. (yes, just `xmake`)
    - Select the build mode with `xmake f -m <MODE>` -- `release` / `devel` / `debug`.
2. Place the resulting `d3d9.dll` in `image/pc/` and launch the game.

> ...*log files are stored in* `%LOCALAPPDATA%\banana\logs\`.

You may also grab a `d3d9.dll` from the [daily build](https://github.com/buyBread/banana/releases/tag/master). If the build is trailing behind the latest commit, blame GitHub's scheduler ~~or request to manually trigger a daily build through the Discord bot~~ (whenever there's a Discord...).

## Compatibility
* **DXVK**: when there is a file in `image/pc/` named `dxvk.dll`.
* **ReShade**: when there is a file in `image/pc/reshade/` named `d3d9.dll`; installing ReShade for Vulkan works with DXVK too.
* **exWoS** / **WOSTweaks**: Banana's capability to load alongside exWoS or WOSTweaks is a non-consideration.

## Credits
* **kirbystealer** -- most of my early poking at this game was on pack formats & loading, much of which I confirmed by comparing my findings against his Python scripts. As such, a chunk of my behind-the-scenes tooling owes its initial existence to him.
* **Archiver of Triviality** -- without WOSTweaks, the Web of Shadows scene would be preeeettty dead. Seeing the amount of neat things you can do within the game's engine also inspired me to actually apply myself towards this game more seriously.
* **MrMartinIden** -- OpenUSM really skyrocketed my understanding of the engine when I first began to poke around the various binaries I could get my hands on. Massive head start to be honest.