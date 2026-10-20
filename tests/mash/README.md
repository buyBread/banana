# mash tests
New to mash? Read these in order. Each one builds on the ones before it.

1. [`fixtures.hh`](fixtures.hh) -- the big picture, plus the stand-in objects and helpers every test uses.
2. [`reader.cc`](reader.cc) -- how the reader walks an image's two streams.
3. [`traversal.cc`](traversal.cc) -- turning bytes back into objects and their pointers.
4. [`virtual_types.cc`](virtual_types.cc) -- how saved type keys become vtables again.
5. [`containers.cc`](containers.cc) -- vectors that borrow their contents from the image.
6. [`arrays.cc`](arrays.cc) -- fixed-size arrays.
7. [`avl_tree.cc`](avl_tree.cc) -- a sorted tree that rebuilds its shape while loading.
8. [`strings.cc`](strings.cc) -- strings and the many quirks they have.
9. [`graphs.cc`](graphs.cc) -- all of the above together, using real resource types from the game.
