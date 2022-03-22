#!/usr/bin/env python
# Author : Darjengar 08/23/2021, License: GPL 3.0
# written in Python 3.9

import tkinter as tk
from tkinter.constants import FALSE
import MainFrame

class ToyMachineGUI(tk.Tk):
    """
    TOY Machine graphical user interface.
    """
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.title("Toy Machine Emulator")
        self.geometry("1176x600")
        self.resizable(width=False, height=False)
        self.option_add("*tearOff", FALSE)
        MainFrame.MainFrame(self)

if __name__ == '__main__':
    app = ToyMachineGUI()
    app.mainloop()