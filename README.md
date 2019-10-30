# SyGuS Tools

This repository contains tools to parse and manipulate [SyGuS] files.

[Bug reports](../../issues) are very much appreciated,
while  pull requests with fixes for the bugs are even more highly appreciated!

Use the `install_deps.sh` script to install Python 3+ dependencies.


## Checkers and Converters

1. Checking if a file conforms to the [SyGuS language standard]:
```bash
# Check if /path/to/file.sl is a valid SyGuS v2 file

python3 -m sygus.bin.check -s 2 /path/to/file.sl
```

2. Converting a file to a different version of the [SyGuS language standard]:

```bash
# Convert /path/to/file.sl from SyGuS v1 to SyGuS v2

python3 -m sygus.bin.convert -s 1 -t 2 /path/to/file.sl
```


## Scripts for Batch Processing

1. Use the `batch-check.sh` script to check entire directories (recursively):
```bash
./scripts/batch-check.sh -s 1 /path/to/dir_1 /path/to/dir_2
```
See `./scripts/batch-check.sh -h` for more options.

2. Use the `batch-convert.sh` script to convert an entire directory (recursively):
```bash
./scripts/batch-convert.sh -s 1 -t 2 -o /path/to/out_dir /path/to/in_dir
```
The script generates an identical directory structure under the `out_dir` output directory.
It generates a file named `sygus.sl` for every successfully converted `sygus.sl` file,
and a file named `sygus.sl.err` with the error messages for every `sygus.sl` file that it failed to convert.
<br>
See `./scripts/batch-convert.sh -h` for more options.

[SyGuS]:                    https://sygus.org/
[SyGuS language standard]:  https://sygus.org/language/
