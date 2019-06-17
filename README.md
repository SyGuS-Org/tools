# SyGuS Tools

This repository contains tools to parse and manipulate [SyGuS] files.

[Bug reports](../../issues) are very much appreciated,
while  pull requests with fixes for the bugs are even more highly appreciated!


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


[SyGuS]:                    https://sygus.org/
[SyGuS language standard]:  https://sygus.org/language/