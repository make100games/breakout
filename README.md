# Breakout
A simple version of Breakout written in my own, homegrown C++ and OpenGL-based game engine. This project was built with XCode on Mac and therefore only runs on Mac. It uses Apple's AppKit Windowing system to create a window and host an Open GL context. AppKit is also used to provide double buffering.

https://github.com/user-attachments/assets/e5b9b237-cb6a-40a1-8e16-ed6252b62bfc

This project essentially consists of two parts: the game engine and the game itself. The engine is general purpose and does not make any assumptions about the game that is being made. The actual game is implemented by creating custom GameObjects, Scenes and Renderables.
