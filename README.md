# Toy Machine
Emulator of the TOY machine.

This is the first version of the TOY machine emulator.
The emulator can execute a toy program written in binary format
and print the program states in each step.
The last state is the result of the program.

TODO:
- [x] Write a TOY assembler (TOY assembly -> TOY machine code)
- [x] Write a Python API with GUI
- [ ] Refactoring
- [ ] Write a TOY machine documentation
- [ ] Add some TOY example programs

## Building

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
python3 toy/toymachine.py
```

