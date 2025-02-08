## Migrating from Shutter Tarzan

There are four main migration steps:

1. Clone the new `shutter-buttons` repository (this repository!) into the catkin workspace.
1. Rebuild catkin workspace to include only the new buttons packages (i.e., the packages in this repository).
1. Update message namespacing to `shutter_buttons_msg`.
1. Flash updated firmware to Adafruit Metro (only needs to be done once per board).


### Cloning the New Repository

Make sure to clone into the `src` sub-directory of your catkin workspace.
It is also possible that this repository has already been cloned with vcstool or as a git submodule.


### Rebuilding the Workspace

First, remove the old `shutter_buttons` package definition.

You can remove the directory manually (e.g. with `rm -r`).
Or, if you want to keep the old code, you can create an empty file `CATKIN_IGNORE` in the package root.

```console
# To keep the code
roscd shutter_buttons && touch CATKIN_IGNORE

# To delete the code
roscd shutter_buttons/.. && rm -rf shutter_buttons
```

It is also possible that the old `shutter_buttons` directory has already been removed, for example by pulling from the master branch of `shutter-tarzan`.

After removing the old package definition, clean the old package from your workspace.

```console
cd <path to your workspace root>
catkin clean shutter_buttons
```

Finally, rebuild the `shutter_buttons` package and the new message definitions.

```console
catkin build shutter_buttons shutter_buttons_msgs

# verify with rospack
rospack find shutter_buttons
<path to new shutter-buttons>/shutter_buttons

rospack find shutter_buttons_msgs
<path to new shutter-buttons>/msgs
```


## Update Message Namespacing

Because the ROS message and service definitions now reside in their own package, you will need to update existing code references to the new package namespacing, as enumerated below:

```python
# previous, deprecated imports:
from shutter_buttons.msg import LightChange
from shutter_buttons.srv import LightChangeArray

# change to:
from shutter_buttons_msgs.msg import LightChange
from shutter_buttons_msgs.srv import LightChangeArray
```

Note that the core behaviour for changing button lights in `shutter_behaviors` has already been updated on the `master` branch, so pulling in the latest changes should apply the update automatically.


## Flash Updated Firmware to Adafruit Metro

Follow the instructions in the [PlatformIO wiki](https://gitlab.com/interactive-machines/shutter/shutter-buttons/-/wikis/platformio#generating-ros_lib) to rebuild ROS library headers.
Then, rebuild the PlatformIO project and upload to the Adafruit Metro.

> **NOTE:** This step will force every project that shares the same physical board to also update their messages, so communicate clearly that new firmware lives on your Adafruit Metro!
