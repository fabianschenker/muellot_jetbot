import subprocess
from setuptools import setup, find_packages


def build_libs():
    subprocess.call(['cmake', '.'])
    subprocess.call(['make'])
    

build_libs()


setup(
    name='muellot_jetbot',
    version='0.4.3',
    description='An open-source robot based on NVIDIA Jetson Nano',
    packages=find_packages(),
    package_data={'muellot_jetbot': ['ssd_tensorrt/*.so']},
)
