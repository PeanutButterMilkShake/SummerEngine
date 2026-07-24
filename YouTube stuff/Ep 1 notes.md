---
file: TODO.md
status: Notes
Swimlane: Youtube
---
## what I've done so far
created rendering classes (shader, ebo, vao, vbo, mesh)
stress tested with 1 dragon (200k tris, ~5k fps)
stress tested with 10 dragon (2m tris, ~1k fps)
created a resource manager to cut down on memory costs
stress tested with 10 dragon (2m tris, ~3k fps)
created the base of a rendering pipeline (change from each mesh renders itself to the engine renders each mesh)
added light objects instead of a fixed light inside of the shader
directional lights only right now
created a light test program!!!!
made a simple steamworks implementation to get steam to recognise the game engine and allow players to host and join lobbies.
make a packet system to send data of any size to players.
made a simple test demo that can create player models when joining a lobby and update their position
converted engine to use gameobject - component system like unity instead of oop (talk ab wanting to implement true ecs later)
added materials and more resource types (shaders, materials)