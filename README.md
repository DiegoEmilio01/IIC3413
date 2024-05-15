# IIC3413-DB

## Important:
see [Lab 3 testing](#lab-3-testing).

## Project Build

Install Dependencies:
--------------------------------------------------------------------------------
MillenniumDB needs the following dependencies:
- GCC >= 8.1
- CMake >= 3.12
- Git

On current Debian and Ubuntu based distributions they can be installed by running:
```bash
sudo apt update && sudo apt install git g++ cmake
```

For Mac you only need to install the **Xcode Command Line Tools**.


Clone the repository
--------------------------------------------------------------------------------
 Clone this repository, enter the repository root directory and set `IIC3413DB_HOME`:
```
git clone git@github.com:DiegoEmilio01/IIC3413.git
cd IIC3413
export IIC3413DB_HOME=$(pwd)
```

Include Boost into the project
--------------------------------------------------------------------------------
Download [`boost_1_81_0.tar.gz`](https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz) using a browser or wget:
```bash
wget -q --show-progress https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz
```

and run the following in the directory where boost was downloaded:
```bash
tar -xf boost_1_81_0.tar.gz
mkdir -p $IIC3413DB_HOME/third_party/boost_1_81/include
mv boost_1_81_0/boost $IIC3413DB_HOME/third_party/boost_1_81/include
rm -r boost_1_81_0.tar.gz boost_1_81_0
```

Since we only use header-only Boost libraries it is not necessary to compile Boost separately.

Build the Project:
--------------------------------------------------------------------------------
Go back into the repository root directory and configure and build:

For the Release version:
```bash
cmake -Bbuild/Release -DCMAKE_BUILD_TYPE=Release && cmake --build build/Release/
```

For the Debug Version:
```bash
cmake -Bbuild/Debug -DCMAKE_BUILD_TYPE=Debug && cmake --build build/Debug/
```

To use multiple cores during compilation (much faster) add `-j n` at the end of the previous commands, replacing `n` with the desired number of threads. Example with 8 threads:
```bash
cmake -Bbuild/Debug -DCMAKE_BUILD_TYPE=Debug && cmake --build build/Debug/ -j 8
```

## Lab 3 testing:

To create an example database:
```bash
./build/Debug/tests/create_db
```
that will create the database folder `data/test_example`.

You can change the schema or add more tables modifying the file `src/tests/create_db.cc`

Then you can query the database with `execute_query`, passing as first parameter an existing database, and as a second parameter the path to a file with the query:
```bash
./build/Debug/bin/execute_query data/test_example data/queries/1.sql
```

You can print the logical and physical plan if you add the parameter `--explain` at the end:
```bash
./build/Debug/bin/execute_query data/test_example data/queries/1.sql --explain
```
