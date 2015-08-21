#Space Navigator Emulator

Emulates a Connexion Space Navigator using joystick input by matching the interface of the Connexion driver library.

Place the siappdll.dll into the LoadLibrary search path of your CAD program and the Connexion3D static library linked into the program will call it instead of the one that talks to the driver. 

Built to support analog joysticks and XInput devices.

Example config and log files are included under \ExampleConfig

##Setup

1. Copy contents of one config folders to %ALLUSERSPROFILE%\SpaceNavigatorEmulator
1. Build code
1. Copy siappdll.dll to the bin/obj/library folder of your CAD program.

## Features

### Axis Swapping

Since basic joysticks may only support a few axes, a joystick button can be configured to change which joystick axes map onto which Space Navigator outputs.

### Axis reversal

The config file allows the axis information to be reversed

### Axis range

Sets the maximum value passed to a CAD program when moving the joystick. Defaults to 350.

### Deadzone

Deadzone for the joystick. Range is [0-10000] and uses the standard directx deadzone settings

### Middle Mouse Drag (fakeMouseMoveWhenRotating config option)

This is for the Creo Modelling tools. If this config option is set, the software will emulate a small middle mouse button down, mouse drag and then button up, when the user starts a rotation. This is specifically included to support combination mouse and joystick devices

## Limitations

* The sync api is largely unimplemented.
* Code only understands connexion buttons 1-4 as config options.

## Licence 
[GPL v3](https://www.gnu.org/licenses/gpl.html)

SpaceNavigatorEmulator - 3dconnexion-compatible siappdll.dll replacement

Copyright (C) 2014  John Tsiombikas <nuclear@member.fsf.org>

Copyright (C) 2015  Luke Nuttall <lukenuttall@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.











