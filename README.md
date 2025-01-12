<br><br>
<div align="center">
<h3> PSE WebServer </h3>
<p> Projektgruppe 3 </p>
</div>
<br>

## Installation
Software Requirements:
- [LLVM/Clang](https://clang.llvm.org/)
- [CMake](https://cmake.org/)
- [GNU/make](https://www.gnu.org/software/make/)
- [OpenSSL](https://www.openssl.org/)
- [Libmagic](https://man7.org/linux/man-pages/man3/magic_load.3.html)
- [Git](https://git-scm.com/) (optional)

```bash
# Clone the repository locally, you may need to authenticate yourself
$ git clone https://git.fh-muenster.de/pse2023/pg5-3/pes-2.git pse-webserver
# Go into the new directory
$ cd pse-webserver
# Run cmake to create the necessary build files
$ cmake ./CMakeLists.txt
# Run make to compile the binary
$ make
# Move the target binary into the cmake folder
$ mv ./pse_webserver ./CMakeFiles/
# Go into the target directory
$ cd ./CMakeFiles
```

## Running the Server
Once you did all the steps above, run the server with:

```bash
$ ./pse_webserver
```

## Testing the Server
Requirements:
- [Python3](https://www.python.org/)
- [american fuzzy lop](https://github.com/google/AFL)

You can run the premade python tests using the following steps:

```bash
# Go into the testing directory
$ cd /path/to/pse-webserver/pewpewlaz0rt4nk
# Run the tests
$ python3 ./tests.py
```

Additionally, you may run the afl fuzzer according to this [README.md](./Fuzzer/README.md).

## Compatability Disclaimer
The server is not intended to be compatible with Windows or MacOS!