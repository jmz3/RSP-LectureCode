<h1 align="center">ROS Debug Log</h1>
<br></br>




# Noetic

## Catkin Error
### 1. Cannot find catkin or Catkin command cannot autocomplete
The reason is that the original catkin tools has been integrated to the python3 package.
The original package is python-catkin-tools, and the updated version should be python3-catkin-tools

Hence, this problem can be solved by:

```bash
sudo apt-get install python3-catkin-tools
```
Remember to resource the terminal after the installation is complete.

### 2. Catkin build unable to find either executable ‘empy‘ or Python module ‘em‘
```bash
CMake Error at /opt/ros/noetic/share/catkin/cmake/empy.cmake:30 (message): 
Unable to find either executable 'empy' or Python module 'em'... try 
installing the package 'python3-empy' 
```

This is because there is a confilct between the ros python env and anaconda python env, it looks like anaconda will directly change the env variable named $PYTHONPATH. Ideally, this could be solved by sourcing the setup.bash file, but for some reason, anaconda just doesn't let you do this.

To fix this problem, we can install python3-empy to the anaconda venv.
```bash
conda install -c conda-forge empy
```

### 3. No module named 'catkin_pkg'

```bash
ImportError: "from catkin_pkg.package import parse_package" failed: No module named 'catkin_pkg'
Make sure that you have installed "catkin_pkg", it is up to date and on the PYTHONPATH.
CMake Error at /opt/ros/noetic/share/catkin/cmake/safe_execute_process.cmake:11 (message):
  execute_process(/home/jeremy/anaconda3/bin/python3
  "/opt/ros/noetic/share/catkin/cmake/parse_package_xml.py"
  "/opt/ros/noetic/share/catkin/cmake/../package.xml"
  "/home/jeremy/Documents/cable/build/catkin_tools_prebuild/catkin/catkin_generated/version/package.cmake")

```
Solution
```bash
pip install catkin_pkg
```

### 4. Other related build errors
```bash
fatal error: numpy/arrayobject.h: No such file or directory
   20 | #  include <numpy/arrayobject.h>
```
This can be solved by specifying the python directory
```bash
catkin build -DPYTHON_EXECUTABLE=/usr/bin/python3 -DPYTHON_INCLUDE_DIR=/usr/include/python3.8
```
Note that here you have to modify the python3.7m to whatever version you have on your local env.

