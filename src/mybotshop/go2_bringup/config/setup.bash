#!/bin/bash

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# --------------------------------------- Go2 Bringup ---------------------------------------
source /home/unitree/cyclonedds_ws/install/setup.bash
source /opt/mybotshop/install/setup.bash

# Single Robot setup
# export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
export CYCLONEDDS_URI=/opt/mybotshop/src/mybotshop/go2_bringup/config/multi_rmw.xml

# Robot ID
export GO2_NS=go2_unit_49702

alias setdate='sudo date -s "$(wget --method=HEAD -qSO- --max-redirect=0 google.com 2>&1 | sed -n "s/^ *Date: *//p")"'
alias go2_build='cd /opt/mybotshop && colcon build --symlink-install && source install/setup.bash && cd'