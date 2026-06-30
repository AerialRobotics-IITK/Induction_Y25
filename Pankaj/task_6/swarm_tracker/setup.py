from setuptools import setup
import os
from glob import glob

package_name = 'swarm_tracker'

setup(
    name=package_name,
    version='0.1.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
         ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'),
         glob('launch/*.launch.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Pankaj Phogat',
    maintainer_email='pankajphogat@example.com',
    description='ARIITK Task 6 — Vision-based swarm navigation',
    license='MIT',
    entry_points={
        'console_scripts': [
            'follower_node  = swarm_tracker.follower_node:main',
            'leader_evasion = swarm_tracker.leader_evasion:main',
        ],
    },
)
