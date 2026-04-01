# Shutter Buttons

ROS2 Jazzy interfaces to Arduino arcade buttons with GUI.

## Packages

- **shutter_buttons_msgs** — Message and service definitions (`LightChange.msg`, `LightChangeArray.srv`)
- **shutter_buttons** — ROS2 Python node with Qt GUI and py_trees behaviour
- **platformio/buttons_interface** — micro-ROS firmware for Adafruit Metro M4

## Building

Clone into the `src` directory of your colcon workspace and build:

```console
cd <workspace>/src
git clone <this-repo> shutter-buttons

cd <workspace>
colcon build --packages-select shutter_buttons_msgs shutter_buttons
source install/setup.bash
```

### Dependencies

Install ROS dependencies via rosdep:

```console
rosdep install --from-paths src/shutter-buttons --ignore-src -y
```

Dependencies:
- `rclpy`, `std_msgs` — ROS2 core
- `shutter_buttons_msgs` — custom messages (this repo)
- `ros-jazzy-py-trees` — behaviour trees (installed as a ROS package, not via pip)
- `micro_ros_agent` — micro-ROS agent for serial communication with hardware
- `PySide2` — Qt GUI (`pip install PySide2==5.15.0`)

## Message Namespacing

Python imports use the `shutter_buttons_msgs` package:

```python
from shutter_buttons_msgs.msg import LightChange
from shutter_buttons_msgs.srv import LightChangeArray
```

## Launch Files

For the virtual buttons GUI:

```console
ros2 launch shutter_buttons virtual_buttons_launch.py
```

For the physical buttons (starts micro-ROS agent):

```console
ros2 launch shutter_buttons shutter_buttons_launch.py

# optional: specify port and baud rate
ros2 launch shutter_buttons shutter_buttons_launch.py port:=/dev/ttyACM0 baud:=115200
```

A very simple test with the physical buttons, after launching the previous command:

```console
ros2 run shutter_buttons rainbow
```

## Flashing Firmware (PlatformIO)

The Arduino firmware uses micro-ROS with custom message headers generated from `shutter_buttons_msgs`.

> **NOTE:** flashing does not seem to work with the PlatformIO VSCode extension.
> Instead, install the [PlatformIO CLI](https://docs.platformio.org/en/stable/core/installation/methods/installer-script.html#super-quick-macos-linux) and [shell commands](https://docs.platformio.org/en/stable/core/installation/shell-commands.html#method-1)

To rebuild and flash:

```console
cd platformio/buttons_interface

# clean previous micro-ROS build artifacts (required after renaming the shutter_buttons_msgs package)
rm -rf .pio

# build and upload
pio run -t upload
```

The micro-ROS PlatformIO plugin will automatically pick up the `shutter_buttons_msgs` package symlinked from `extra_packages/` and generate the C headers during build.

> **NOTE:** Flashing new firmware will require every project sharing the same physical board
> to also use the updated message definitions.

If the PlatformIO compiler warns about udev rules, [update the rules from PlatformIO](https://docs.platformio.org/en/stable/core/installation/udev-rules.html) and restart the udev daemon (requires `sudo`).
