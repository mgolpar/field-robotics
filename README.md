# field-robotics
This folder contains templates for communication between nodes.

How to use:
   - Create a workspace: http://wiki.ros.org/catkin/Tutorials/create_a_workspace. Finish this tutorial and make sure your .bash file is properly sourced
   - Copy src/ and launch/ into your workspace
   - At this point you may not be able to build workspace since package tango-driver requires PCL packag
   - Build individual packages with: catkin_make packagename
   - Runs individual nodes with: rosrun packagename packagename_node
   
Chad