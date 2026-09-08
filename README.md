# banana
"Spider-Man: Web of Shadows" reverse-engineering playground, not really anything with a clear purpose that's usable.

## Project Status
At the moment, banana doesn't provide any truly unique functionality to Web of Shadows beyond a handful of tiny miscellaneous additions. Development is mainly focused on achieving an implementation of NGL that has parity with the retail PC version of the game. The code also lacks clarity in places and only contains comments sparsely, but the good news is that I've largely stuck to what the IDA disassembler had shown me, so it should be possible to "intuit" your way around the codebase by viewing the game's assembly side-by-side. I'm hesitant to call this a "proper" decompilation at the moment, because I'm not interested in owning more aspects of the game beyond NGL and (mainly) the `game` class that asks to renders the actual game.

## Installing
> This project is compiled using `MSVC Build Tools 18` and managed with `xmake`, rest of the prerequisites are taken care of by `xmake`.
1. Select the build mode with `xmake f -m MODE` (`release`/`devel`/`debug`).
2. Compile with `xmake`.
3. Place the resulting `.dll` in `/image/pc/` and launch the game.