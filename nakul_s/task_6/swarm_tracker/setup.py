"""Setup configuration for the swarm_tracker ROS 2 package."""

from glob import glob
import os

from setuptools import setup


package_name = "swarm_tracker"

setup(
    name=package_name,
    version="0.1.0",
    packages=[package_name],
    package_dir={package_name: "src"},
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
        (os.path.join("share", package_name, "launch"), glob("launch/*.launch.py")),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="Nakul Sharma",
    maintainer_email="nakul@example.com",
    description="Vision-based swarm tracking using ArUco markers.",
    license="MIT",
    entry_points={
        "console_scripts": [
            "follower_node = swarm_tracker.follower_node:main",
            "leader_evasion = swarm_tracker.leader_evasion:main",
        ],
    },
)
