from setuptools import find_packages, setup
import os
from glob import glob

package_name = 'swarm_tracker'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.launch.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='furzaan',
    maintainer_email='furzaan.s.ullah@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'follower_node = swarm_tracker.follower_node:main',
            'leader_evasion.py = swarm_tracker.leader_evasion:main',
        ],
    },
)
