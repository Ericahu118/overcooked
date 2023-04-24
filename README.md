# Overcooked

This is the lab for Problem Solving Ⅳ in Nanjing University in 2023.


## Get Started

```shell
$ git clone https://git.nju.edu.cn/psv/overcooked.git
```

## Compile

On Windows, install CLion or Visual Studio and open lab folder.

On macOS, install CLion or use make directly:

```shell
$ make
```

## Submit

To submit manually, remove all files except `.git` and create a zip archive of your lab folder, then upload it to online judge. The archive should not exceed 10MiB in size and its content looks like:

```
XXXXXX.zip
  - Lab (any folder)
    - .git
```

or

```
XXXXXX.zip
  - .git
```

On Linux/macOS, run `make submit TOKEN=token` or submit manually.