#!/usr/bin/env python

# Author : Darjengar 08/23/2021, License: GPL 3.0
# written in Python 3.9

import tkinter as tk
from tkinter import ttk
from tkinter.constants import VERTICAL
from tkinter import filedialog
import toy

class MainFrame(tk.Frame):
    """
    Container class for TOY Machine GUI.

    Class Variables
    ---------------
    None

    Attributes
    ----------
    program_pos : int
    file_name : str
    program_str : str
    log_str : str
    input_var : int
    toy_disp_txt_id : int
    rect_id : int
    toy_disp_num_str : str
    toyframe : tk.Frame
    logframe1 : tk.Frame
    logframe2 : tk.Frame
    toy_display : tk.Canvas
    textfield : ttk.Entry
    btn_enter : ttk.Button
    btn_start : ttk.Button
    btn_stop : ttk.Button
    btn_step : ttk.Button
    btn_reset : ttk.Button
    btn_restart : ttk.Button
    menubar : tk.Menu
    menu_file : tk.Menu
    textbox1 : tk.Text
    textbox2 : tk.Text
    yscroll1 : ttk.Scrollbar
    yscroll2 : ttk.Scrollbar

    Methods
    -------
    openprogram
    btnev_start_machine1
    btnev_start_machine2
    btnev_step_machine
    btnev_stop_machine
    btnev_reset_machine
    btnev_restart_machine
    btnev_enter_input
    intro_anim1
    intro_anim2
    intro_anim3
    intro_anim4
    intro_anim5
    """
    def __init__(self, parent, *args, **kwargs):
        super().__init__(parent, *args, **kwargs)
        self.grid()

        self.program_pos = 1
        self.file_name = ""
        self.program_str = ""
        self.log_str = ""
        self.input_var = tk.IntVar()
        self.toy_disp_txt_id = 0
        self.toy_disp_num_str = ""

        self.toyframe = tk.Frame(self)
        self.toyframe.grid(column=2, row=0, sticky=(tk.N))

        self.toy_display = tk.Canvas(
            self.toyframe, width=300, height=100, background="#697059")
        self.rect_id = self.toy_display.create_rectangle(
            2, 2, 300, 100, outline="black", width=20)
        self.toy_disp_txt_id = self.toy_display.create_text(
            150, 60, text="", font=("Times", -64, "bold"),
            fill="lightblue")
        self.toy_display.grid(column=2, row=0, columnspan=2)

        self.textfield = ttk.Entry(
            self.toyframe, textvariable=self.input_var)
        self.textfield.state(["disabled"])
        self.textfield.grid(
            column=2, row=1, columnspan=2, sticky=(tk.W, tk.E))

        self.btn_enter = ttk.Button(
            self.toyframe, text="Enter", command=self.btnev_enter_input)
        self.btn_enter.grid(
            row=2, column=2, columnspan=2, sticky=(tk.W, tk.E))
        self.btn_start = ttk.Button(
            self.toyframe, text="Start",
            command=self.btnev_start_machine1)
        self.btn_start.grid(column=2, row=3, sticky=(tk.W, tk.E))
        self.btn_step = ttk.Button(
            self.toyframe, text="Step", command=self.btnev_step_machine)
        self.btn_step.grid(
            column=2, row=4, columnspan=2, sticky=(tk.W, tk.E))
        self.btn_stop = ttk.Button(
            self.toyframe, text="Stop", command=self.btnev_stop_machine)
        self.btn_stop.grid(column=3, row=3, sticky=(tk.W, tk.E))
        self.btn_reset = ttk.Button(
            self.toyframe, text="Reset",
            command=self.btnev_reset_machine)
        self.btn_reset.grid(column=3, row=5, sticky=(tk.W, tk.E))
        self.btn_restart = ttk.Button(
            self.toyframe, text="Restart",
            command=self.btnev_restart_machine)
        self.btn_restart.grid(column=2, row=5, sticky=(tk.W, tk.E))

        self.btn_start.state(["disabled"])
        self.btn_step.state(["disabled"])
        self.btn_stop.state(["disabled"])
        self.btn_reset.state(["disabled"])
        self.btn_restart.state(["disabled"])
        self.btn_enter.state(["disabled"])

        self.menubar = tk.Menu(parent)
        parent['menu'] = self.menubar
        self.menu_file = tk.Menu(self.menubar)
        self.menubar.add_cascade(menu=self.menu_file, label="File")
        self.menu_file.add_command(
            label="Load program...", command=self.openprogram)
        self.menu_file.add_command(label="Exit", command=parent.destroy)

        self.logframe1 = tk.Frame(self)
        self.logframe1.grid(column=0, row=0, sticky=(tk.N), rowspan=2)
        self.yscroll1 = ttk.Scrollbar(self.logframe1, orient=VERTICAL)
        self.yscroll1.grid(
            row=0, column=1, rowspan=2, sticky=(tk.N, tk.S))
        self.textbox1 = tk.Text(
            self.logframe1, width=52, height=35,
            yscrollcommand=self.yscroll1.set)
        self.textbox1.grid(column=0, row=0)
        self.yscroll1['command'] = self.textbox1.yview
        self.textbox1["state"] = "disabled"

        self.logframe2 = tk.Frame(self)
        self.logframe2.grid(column=3, row=0, rowspan=2, sticky=(tk.N))
        self.yscroll2 = ttk.Scrollbar(self.logframe2, orient=VERTICAL)
        self.yscroll2.grid(
            column=4, row=0, sticky=(tk.N, tk.S), rowspan=2)
        self.textbox2 = tk.Text(
            self.logframe2, width=52, height=35,
            yscrollcommand=self.yscroll2.set)
        self.textbox2.grid(column=3, row=0, rowspan=1)
        self.yscroll2['command'] = self.textbox2.yview
        self.textbox2["state"] = "disabled"

        ttk.Label(
            self, text="Toy Machine\n16-bit Architecture",
            foreground="black", font=("Helvetica", -32),
            justify=tk.CENTER
            ).grid(column=2, row=1, sticky=(tk.S))

    def openprogram(self):
        """
        Opens a TOY machine program and initializes the start state.

        Parameters
        ----------
        None

        Returns
        -------
        None

        Raises
        ------
        None

        Yields
        ------
        None
        """
        self.file_name = filedialog.askopenfilename(
            filetypes=[("binary file", "*.bin")])
        if not isinstance(self.file_name, str):
            return
        if self.file_name == "":
            return
        self.log_str = toy.init_toy(self.file_name)
        self.program_str = toy.toydisasm()

        self.textbox1["state"] = "normal"
        self.textbox1.delete("1.0", tk.END)
        self.textbox1.insert("1.0", self.program_str)
        self.textbox1["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.delete("1.0", tk.END)
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"

        self.btn_start.state(["!disabled"])
        self.btn_reset.state(["!disabled"])

    def btnev_start_machine1(self):
        """
        Opens a TOY machine program and initializes the start state.
        """
        self.btn_start.bell()
        self.rect_id = self.toy_display.create_rectangle(
            2, 2, 300, 100, outline="black", width=20)
        self.toy_disp_txt_id = self.toy_display.create_text(
            150, 60, text="", font=("Times", -64, "bold"),
            fill="lightblue")
        self.toy_display.configure(background="blue")
        self.btn_start.state(["disabled"])
        self.btn_reset.state(["disabled"])
        self.after(1, self.intro_anim1)

    def btnev_start_machine2(self):
        """
        See btnev_start_machine1.
        """
        self.toy_display.itemconfigure(
            self.toy_disp_txt_id, text="FFFF")
        halt_flag, input_flag, output_flag, jmp_flag, pc = toy.start_toy()
        self.textbox1["state"] = "normal"
        self.textbox1.delete("1.0", tk.END)
        self.textbox1.insert("1.0", self.program_str)
        self.textbox1.insert("1.end", "\t\t|<--")
        self.textbox1["state"] = "disabled"

        if halt_flag == 1:
            self.btn_restart.state(["!disabled"])
            self.btn_step.state(["disabled"])
            self.btn_stop.state(["disabled"])
            self.btn_reset.state(["!disabled"])
            self.program_pos = 1
            return

        if jmp_flag == 1:
            self.program_pos = pc - 0x10 + 1

        if output_flag == 1:
            value = toy.output_toy()
            self.toy_display.itemconfigure(
                self.toy_disp_txt_id, text=format(value, 'X').zfill(4))
            self.program_pos += 1

        if input_flag == 1:
            self.textfield.state(["!disabled"])
            self.btn_step.state(["disabled"])
            self.btn_enter.state(["!disabled"])
            self.program_pos += 1
        else:
            self.btn_step.state(["!disabled"])
            self.log_str = toy.show_state()
            self.textbox2["state"] = "normal"
            self.textbox2.insert("1.0", self.log_str)
            self.textbox2["state"] = "disabled"
            self.program_pos += 1

        self.btn_stop.state(["!disabled"])

    def btnev_step_machine(self):
        """
        Steps to the next program instruction.
        """
        halt_flag, input_flag, output_flag, jmp_flag, pc = toy.start_toy()
        self.log_str = toy.show_state()
        self.textbox1["state"] = "normal"
        self.textbox1.delete("1.0", tk.END)
        self.textbox1.insert("1.0", self.program_str)
        self.textbox1.insert(
            "{0}.end".format(self.program_pos), "\t\t|<--")
        self.textbox1["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"
        if halt_flag == 1:
            self.btn_restart.state(["!disabled"])
            self.btn_step.state(["disabled"])
            self.btn_stop.state(["disabled"])
            self.btn_reset.state(["!disabled"])
            self.program_pos = 1
        elif input_flag == 1:
            self.textfield.state(["!disabled"])
            self.btn_step.state(["disabled"])
            self.btn_enter.state(["!disabled"])
            self.program_pos += 1
        elif output_flag == 1:
            value = toy.output_toy()
            self.toy_display.itemconfigure(
                self.toy_disp_txt_id, text=format(value, 'X').zfill(4))
            self.program_pos += 1
        elif jmp_flag == 1:
            self.program_pos = pc - 0x10 + 1
        else:
            self.program_pos += 1

    def btnev_stop_machine(self):
        """
        Stops the execution of the current program in TOY machine.
        """
        self.btn_stop.bell()
        self.btn_start.state(["!disabled"])
        self.btn_step.state(["disabled"])
        self.btn_stop.state(["disabled"])
        self.btn_reset.state(["!disabled"])
        self.btn_enter.state(["disabled"])
        self.program_pos = 1

        self.toy_display.configure(background="#697059")
        self.toy_display.itemconfigure(self.toy_disp_txt_id, text="")
        self.input_var.set(0)
        self.textfield.state(["disabled"])

        toy.stop_toy()
        self.log_str = toy.init_toy(self.file_name)

        self.textbox1["state"] = "normal"
        self.textbox1.delete("1.0", tk.END)
        self.textbox1.insert("1.0", self.program_str)
        self.textbox1["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.delete("1.0", tk.END)
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"

    def btnev_reset_machine(self):
        """
        Resets the TOY Machine to the start configuration.
        """
        self.btn_start.state(["disabled"])
        self.btn_step.state(["disabled"])
        self.btn_stop.state(["disabled"])
        self.btn_reset.state(["disabled"])
        self.btn_restart.state(["disabled"])

        self.textbox1["state"] = "normal"
        self.textbox1.delete("1.0", tk.END)
        self.textbox1["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.delete("1.0", tk.END)
        self.textbox2["state"] = "disabled"

        self.btn_stop.bell()
        self.toy_display.configure(background="#697059")
        self.toy_display.itemconfigure(self.toy_disp_txt_id, text="")

        self.program_pos = 1
        self.file_name = ""

        toy.reset_toy()

    def btnev_restart_machine(self):
        """
        Restarts the current TOY machine program.
        """
        self.btn_start.state(["!disabled"])
        self.btn_restart.state(["disabled"])
        self.log_str = toy.restart_toy()
        self.textbox1["state"] = "normal"
        self.textbox1.delete("1.0", tk.END)
        self.textbox1.insert("1.0", self.program_str)
        self.textbox1["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.delete("1.0", tk.END)
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"
        self.toy_display.configure(background="#697059")
        self.toy_display.itemconfigure(self.toy_disp_txt_id, text="")

    def btnev_enter_input(self):
        """
        Enable the textfield of TOY machines UI for user input.
        The input has to be a number 16-bit number.
        """
        self.btn_enter.state(["disabled"])
        self.btn_step.state(["!disabled"])
        self.textfield.state(["disabled"])
        self.toy_disp_num_str = ""
        number = self.input_var.get()
        self.toy_disp_num_str = format(number, 'X').zfill(4)
        toy.input_toy(number)
        self.toy_display.itemconfigure(self.toy_disp_txt_id, text=self.toy_disp_num_str)
        self.log_str = toy.show_state()
        self.textbox2["state"] = "normal"
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"
        self.input_var.set(0)

    def intro_anim1(self):
        """
        Intro animation in TOY machines display.
        """
        self.toy_display.itemconfigure(
            self.toy_disp_txt_id, text="Toy Machine",
            font=("Times", -32, "bold"))
        self.after(1500, self.intro_anim2)

    def intro_anim2(self):
        """
        See intro_anim1.
        """
        self.toy_display.itemconfigure(self.toy_disp_txt_id, text="")
        self.toy_display.itemconfigure(
            self.toy_disp_txt_id, text="presented by...",
            font=("Times", -32, "bold"))
        self.after(1500, self.intro_anim3)

    def intro_anim3(self):
        """
        See intro_anim1.
        """
        self.toy_display.itemconfigure(self.toy_disp_txt_id, text="")
        self.toy_display.itemconfigure(
            self.toy_disp_txt_id, text="Darjengar", font=("Times", -32, "bold"))
        self.after(1500, self.intro_anim4)

    def intro_anim4(self):
        """
        See intro_anim1.
        """
        self.toy_display.itemconfigure(
            self.toy_disp_txt_id,
            text="The concept was not my\nidea but the implementation.",
            font=("Times", -20, "bold"))
        self.after(1700, self.intro_anim5)

    def intro_anim5(self):
        """
        See intro_anim1.
        """
        self.toy_display.itemconfigure(
            self.toy_disp_txt_id, text="", font=("Times", -64, "bold"))
        self.after(1000, self.btnev_start_machine2)
