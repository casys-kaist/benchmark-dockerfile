#!/usr/bin/env python3

"""
   mix.py

    Created on: Jul. 27, 2019
        Author: Taekyung Heo <tkheo@casys.kaist.ac.kr>
"""

import os
import sys

def get_lru_favor_cmd(cmd):
    splitted_name = cmd.split(":")
    num_pages = int(splitted_name[1])
    num_working_set_partitions = int(splitted_name[2])
    working_set_access_duration = int(splitted_name[3])
    num_iterations = int(splitted_name[4])
    cmd = "/root/src/lru_favor "\
            "--num_pages %d "\
            "--num_working_set_partitions %d "\
            "--working_set_access_duration %d "\
            "--num_iterations %d"\
            % (num_pages,
                    num_working_set_partitions,
                    working_set_access_duration,
                    num_iterations)
    return cmd

def get_lfu_favor_cmd(cmd):
    splitted_name = cmd.split(":")
    num_pages = int(splitted_name[1])
    hot_working_set_ratio = float(splitted_name[2])
    num_iterations = int(splitted_name[3])
    cmd = "/root/src/lfu_favor "\
            "--num_pages %d "\
            "--hot_working_set_ratio %f "\
            "--num_iterations %d"\
            % (num_pages,
                    hot_working_set_ratio,
                    num_iterations)
    return cmd

def get_random_favor_cmd(cmd):
    splitted_name = cmd.split(":")
    num_pages = int(splitted_name[1])
    num_iterations = int(splitted_name[2])
    cmd = "/root/src/random_favor "\
            "--num_pages %d --num_iterations %d"\
            % (num_pages, num_iterations)
    return cmd

def get_cmd(cmd):
    if "lru_favor" in cmd:
        cmd = get_lru_favor_cmd(cmd)
    elif "lfu_favor" in cmd:
        cmd = get_lfu_favor_cmd(cmd)
    elif "random_favor" in cmd:
        cmd = get_random_favor_cmd(cmd)
    return cmd

def main():
    cmd_str = sys.argv[1]
    cmd_li = cmd_str.split("+")
    for cmd in cmd_li:
        cmd = get_cmd(cmd)
        print("switch")
        os.system(cmd)

if __name__ == "__main__":
    main()
