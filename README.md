# GAU8 

GAU8 is an SSH bruteforcer, built to allow as many threads as possible. 
Built using C++, it works by creating a vector of thread objects, each thread opens a new SSH session, attempts X amount of passwords for the current user, then dumps the SSH session.  
This tool is designed to be as fast as possible, not efficient. 

I am **NOT** responsible for any illegal actions performed by using this tool.

This software is provided as-is with no warranty and for testing purposes only! 

## Installation

```bash
git clone https://github.com/gabrielhutu/gau8
cd gau8
chmod +x ./build.sh
./build.sh
```

## Usage

```bash
USAGE: ./gau8 + options
OPTIONS:
      -i/--host                    IP Address of one target
      -iL/--hosts-file             File Containing targets
      -u/--user                    Username
      -w/--wordlist                Wordlist
      -p/--port                    Port (Optional)
      -th/--threads-per-host       Threads per host (Optional)
      -a/--attempts-per-session    Password Attempts per SSH session
      -o/--output                  Output File
```

Currently, GAU8 works by opening X amount of threads (specify by -th) for each host from the host file (-iL). It will create a temporary file under /tmp in order to copy the wordlist passed by -w for each host. 

## Roadmap
Currently, I'm working on adding the following: 
- Adaptive Threading
    - Allow GAU8 to calculate in real time the most optimal number of threads. Based on previous tests, I noticed that the highest number of threads is not necessarily the best option, and we may want to constantly adjust the number of threads (using the main thread) to provide more threads for target hosts that are able to / allow more authentication requests per minute. 
- Bruteforce through users / password spraying
    - Allow the option to attempt multiple users per password
- Add more protocols
    - Currently, only SSH is supported, but all authentication functions are in a separate class, this would allow me to define more protocols that could be used for bruteforce, like RADIUS, FTP and so on.. 

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
