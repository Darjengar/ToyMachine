# Toy Machine
Emulator of the TOY machine.

This is the first version of the TOY machine emulator.
The emulator can execute a toy program written in binary format and print the program states in each step.
The last state is the result of the program.

Additionally I have added the program toyasm and
toydisasm.

## Programs
- toyasm - translates TOY programs into binary format as *.bin file for the TOY Machine. (Builded under Linux distribution)
- toydisasm - translates a TOY binary file back into a TOY (Builded under Linux distribution)
program and prints it in terminal.
- toymachine.py - TOY Machine emulator.

## TODO
- [x] Write a TOY assembler (TOY assembly -> TOY machine code)
- [x] Write a Python API with GUI
- [ ] Refactoring
- [x] Write a TOY machine documentation
- [x] Add some TOY example programs

## Building
TOY machine is only tested on Linux.
Therefore the building guide needs to be adapt for your os maybe.
The TOY Machine is written in Python 3.9.

So that you keep your standard Python installation clean I highly recommend you to setup a virtual environment (e.g.venv) for the following intallation guide.

Setup a virtual environment
```bash
python3 -m venv test-env
source test-env/bin/activate
```
Further reading:
[Python Virtual Environment](https://docs.python.org/3/tutorial/venv.html).

### Install toy package
```bash
python3 toy/setup.py install
```
### Run Toy Machine
```bash
python3 toy/ToyMachine.py
```

