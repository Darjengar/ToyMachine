#!/usr/bin/env python
#from tkinter import *
#from tkinter import ttk
from os import close
import tkinter as tk
from tkinter import Grid, ttk
from tkinter.constants import FALSE, VERTICAL


class ToyMachineGUI(tk.Tk):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.title("Toy Machine Emulator")
        self.geometry("1176x600")
        self.resizable(width=False, height=False)
        self.option_add("*tearOff", FALSE)
        MainFrame(self)

class MainFrame(tk.Frame):
    def __init__(self, parent, *args, **kwargs):
        super().__init__(parent, *args, **kwargs)
        self.grid()

        self.testframe = tk.Frame(self)
        self.testframe.grid(column=2, row=0, sticky=(tk.N))

        self.canvas = tk.Canvas(self.testframe, width=300, height= 100,background="blue")
        self.canvas.create_rectangle(2, 2, 300, 100, outline="black", width=20)
        self.canvas.create_text(150, 60, text="FF FF", font=("Times", -64, "bold"), fill="lightblue")
        self.canvas.grid(column=2, row=0, columnspan=2)

        self.textfield = ttk.Entry(self.testframe)
        self.textfield.state(["disabled"])
        self.textfield.grid(column=2, row=1, columnspan=2, sticky=(tk.W, tk.E))

        self.btn1 = ttk.Button(self.testframe, text="Start")
        self.btn1.grid(column=2, row=2, sticky=(tk.W, tk.E))
        self.btn2 = ttk.Button(self.testframe, text="Step forward")
        self.btn2.grid(column=3, row=3, sticky=(tk.W, tk.E))
        self.btn3 = ttk.Button(self.testframe, text="Step backward")
        self.btn3.grid(column=2, row=3, sticky=(tk.W, tk.E))
        self.btn4 = ttk.Button(self.testframe, text="Stop")
        self.btn4.grid(column=3, row=2, sticky=(tk.W, tk.E))

        self.btn1.state(["disabled"])
        self.btn2.state(["disabled"])
        self.btn3.state(["disabled"])
        self.btn4.state(["disabled"])

        self.menubar = tk.Menu(parent)
        parent['menu'] = self.menubar
        self.menu_file = tk.Menu(self.menubar)
        self.menubar.add_cascade(menu=self.menu_file, label="File")
        self.menu_file.add_command(label="Load program...")
        self.menu_file.add_command(label="Exit", command=parent.destroy)

        self.testframe2 = tk.Frame(self)
        self.testframe2.grid(column=0, row=0, sticky=(tk.N), rowspan=2)
        self.yscroll = ttk.Scrollbar(self.testframe2, orient=VERTICAL)
        self.yscroll.grid(row=0, column=1, rowspan=2, sticky=(tk.N, tk.S))
        self.textbox = tk.Text(self.testframe2, width=52, height=35, yscrollcommand=self.yscroll.set)
        self.textbox.grid(column=0, row=0)
        self.yscroll['command'] = self.textbox.yview

        self.testframe3 = tk.Frame(self)
        self.testframe3.grid(column=3, row=0, rowspan=2, sticky=(tk.N))
        self.yscroll2 = ttk.Scrollbar(self.testframe3, orient=VERTICAL)
        self.yscroll2.grid(column=4, row=0, sticky=(tk.N, tk.S), rowspan=2)
        self.textbox2 = tk.Text(self.testframe3, width=52, height=35, yscrollcommand=self.yscroll2.set)
        self.textbox2.grid(column=3, row=0, rowspan=1)
        self.yscroll2['command'] = self.textbox2.yview

        ttk.Label(self, text="Toy Machine\n16-bit Architecture", foreground="black", font=("Helvetica", -32), justify=tk.CENTER).grid(column=2, row=1, sticky=(tk.S))

        #ttk.Label(self, text="Name of program").grid(column=1, row=0)
        #ttk.Label(self, text="State: ").grid(column=2, row=0)

if __name__ == '__main__':
    app = ToyMachineGUI()
    app.mainloop()
