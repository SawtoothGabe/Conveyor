# Conveyor
Minecraft server software written in C++

## Dependencies

### Linux

- A compiler with C++26 support (GCC 14+ or Clang 19+)
- CMake 4.1+
- Ninja or Make
- OpenSSL development libraries

Debian/Ubuntu:

```bash
sudo apt-get install -y build-essential cmake ninja-build libssl-dev
```

Fedora:

```bash
sudo dnf install -y gcc-c++ cmake ninja-build openssl-devel
```

Arch:

```bash
sudo pacman -S --needed base-devel cmake ninja openssl
```
