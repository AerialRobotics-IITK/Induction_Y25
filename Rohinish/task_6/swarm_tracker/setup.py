from glob import glob
import os

from setuptools import setup

package_name = 'swarm_tracker'

setup(
    name=package_name,
    version='0.1.0',
    # The task spec mandates the node sources live under ``src/``.  We expose
    # that directory as a real Python package called ``src`` so that
    # ament_python / colcon can install it and ``ros2 run`` can resolve the
    # console-script entry points below.
    packages=['src'],
    data_files=[
        ('share/ament_index/resource_index/packages',
         ['resource/' + package_name]),
        (os.path.join('share', package_name), ['package.xml']),
        (os.path.join('share', package_name, 'launch'),
         glob('launch/*.launch.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Rohinish',
    maintainer_email='rohinish@iitk.ac.in',
    description='Vision-based ArUco/PID swarm follower for Task 6.',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'follower_node = src.follower_node:main',
            'leader_evasion = src.leader_evasion:main',
            'sim_demo = src.sim_demo:main',
        ],
    },
)
