from setuptools import setup, find_packages

package_name = 'swarm_tracker'

setup(
    name=package_name,
    version='0.0.0',

    packages=find_packages(),

    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', ['launch/tracker.launch.py']),
    ],

    install_requires=['setuptools'],
    zip_safe=True,

    entry_points={
        'console_scripts': [
            'follower_node = swarm_tracker.follower_node:main',
            'leader_evasion = swarm_tracker.leader_evasion:main',
        ],
    },
)