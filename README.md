Controller The Game
===

Controller The Game is a virtual controller game designed to be played with a bunch of friends.
It's inspired by the game [Lovers in a Dangerous Spacetime](https://store.steampowered.com/app/252110/Lovers_in_a_Dangerous_Spacetime/).
You control a Virtual Controller Ship which has components which you can customize.
This controller ship is a representation of a controller device which can be used to control another game. Fortnite, for example.
You can map buttons and Axes to the controller components which creates a system similar to how Steam handles its controller remapping.
You can only control one component at a time, and friends can sabotage your ship.

Requirements
---

- Unix based operating system (Only been tested on Arch-Linux)
- [SDL3](https://www.libsdl.org/)
- [Box2D](https://box2d.org/)
- [GCC](https://gcc.gnu.org/) (Installed by default on Linux)

Building
---

To build, run `make build`

By default, the application builds in debug mode.
To run in release mode, supply the NODEBUG flag.  

`make build NODEBUG=true`

Todo list
---

- [ ] Controller Emulation
- [ ] Virtual Controller Ship customization
- [ ] Virtual Controller Component
- [ ] Multiplayer
- [ ] Custom Controls
