import os
from glob import glob
from setuptools import find_packages, setup

package_name = 'swarm_tracker'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        # Installs your bridge.yaml to share/swarm_tracker/
        (os.path.join('share', package_name), glob('*.yaml')),
        # FIX: Installs your launch files to share/swarm_tracker/launch/
        (os.path.join('share', package_name, 'launch'), glob('launch/*.launch.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='furzaan',
    maintainer_email='furzaan.s.ullah@gmail.com',
    description='Vision-Based Swarm Navigation Tracking Package',
    license='MIT',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'follower_node = swarm_tracker.follower_node:main',
            'leader_evasion = swarm_tracker.leader_evasion:main',
        ],
    },
)
