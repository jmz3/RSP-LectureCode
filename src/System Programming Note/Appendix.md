<h1 align="center"> SYSTEM PROGRAMMING </h1>
<h2 align="center"> APPENDIX </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>



# CMAKE CATKIN_MAKE AND CATKIN BUILD

The main difference is the isolated environment that you get with `catkin build`. This makes the whole build configuration much more compartmentalized  and robust to changes in the configuration (add/remove package, modify a cmake variable etc.)

Apart from that you also get much better and easily-readable colored  cmdline output which makes the whole experience much more pleasant.

Adding to `catkin build` you also get many other useful subcommands with `catkin` including `catkin clean` for cleaning the `build`, `devel` and `install` spaces, as well as `catkin list` , `catkin locate`, `catkin profile`

Apart from the isolated build, it is much more convenient to use:

- Independent packages are build in parallel which can make it much faster.
- `catkin build` can be used from any directory in the workspace while `catkin_make` only works in the top level directory.
- It is easy to only build a single package (+ dependencies): `catkin build package_name` or, when called from the package directory, `catkin build --this`.  Add `--no-deps` to skip dependencies.
   (Note that `catkin_make` has a  `--only-pkg-with-deps` flag for this, but there it will result in a persistent setting so next call of just `catkin_make` would also only build this package which is very unintuitive.)
- `catkin clean` makes it easy to wipe build/devel without having to use a scary `rm -rf`.
- Some other useful commands (list, locate, ...) as already mentioned by @bergercookie.



# Debug and Configure Environment in VSCode

- First you need to have some extensions installed on your vscode, they are:

  c/c++

  c/c++ extension pack

  cmake

  code runner

  msg language support

  ros

- second, in the run and debug section, create a launch.json. You can also press ctrl + shift + D to do this

- Once you click "create a launch.json", a menu will prompt out, choose ROS

- Then there will appear two ROS options, first one is ROS:Launch the second one is ROS:Attach

  ROS:Launch is used for debug the launch files

  ROS:Attach is used for debug the node (cpp) files

- choose ROS:Launch and you will see all available launch packages, choose the one you need to debug, then you will see a launch.json file will automatically created

Inside the launch.json, you can add or delete launch blocks freely and you can choose to run any one of them