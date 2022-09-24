# mooncli 🌚

A tiny client to demonstrate using libcurl to handle HTTP requests in C.

## Installation (MacOS)

```bash
brew install curl
git clone
cd mooncli
ln -s /usr/local/Cellar/curl/7.85.0/include/curl ./include/curl
ln -s /usr/local/Cellar/curl/7.85.0/lib ./lib/libcurl
make
```

## Uninstallation

```bash
make clean
```

## Usage

```bash
./build/mooncli
```

