# SpaceAV

### Build dependencies

- C++ compiler with support for C++20
- CMake >= 3.11
- Git

### Build instructions

#### Checkout repository

```shell
git clone https://github.com/crz11111/SpaceAV.git SpaceAV
cd SpaceAv
```

#### Build with Docker

```shell
docker build -t sfc -f Dockerfile .
```

#### Run with Docker

```shell
docker run -v <csv_data_directory_absolute_path>:<container_mount_absolute_directory> sfc <csv_data_mount_absolute_path> <output_csv_data_mount_absolute_path>
```

Example:

```shell
docker run -v /Users/crz11111/SpaceAV/SFC/data:/opt/csvdata sfc /opt/csvdata/data.csv /opt/csvdata/data_output.csv
```

### CSV file format

Required CSV format as https://github.com/crz11111/SpaceAV/tree/work-on-morton/SFC/data