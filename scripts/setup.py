from setuptools import setup, find_packages

setup(
    name="polygamex-scripts",
    version="1.0.0",
    description="Python AI and scripting layer for PolyGameX Engine",
    author="PolyGameX Team",
    packages=find_packages(),
    install_requires=[
        "numpy>=1.20.0",
        "noise>=1.2.2",
        "scipy>=1.7.0",
    ],
    python_requires=">=3.8",
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
    ],
)
