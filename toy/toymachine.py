# Author : Darjengar 08/23/2021, License: GPL 3.0
# written in Python 3.9

#!/usr/bin/env python
#from tkinter import *
#from tkinter import ttk
import tkinter as tk
from tkinter import ttk
from tkinter.constants import FALSE, VERTICAL
from tkinter import filedialog
import toy

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

        self.btncounter = 1
        self.file_name = ""
        self.program_str = ""
        self.log_str = ""
        self.pause = False
        self.input = tk.IntVar()
        self.text_id = 0

        self.testframe = tk.Frame(self)
        self.testframe.grid(column=2, row=0, sticky=(tk.N))

        self.canvas = tk.Canvas(self.testframe, width=300, height= 100, background="#697059")
        self.rect_id = self.canvas.create_rectangle(2, 2, 300, 100, outline="black", width=20)
        self.text_id = self.canvas.create_text(150, 60, text="", font=("Times", -64, "bold"), fill="lightblue")
        self.canvas.grid(column=2, row=0, columnspan=2)

        self.textfield = ttk.Entry(self.testframe, textvariable=self.input)
        self.textfield.state(["disabled"])
        self.textfield.grid(column=2, row=1, columnspan=2, sticky=(tk.W, tk.E))

        self.btn7 = ttk.Button(self.testframe, text="Enter", command=self.btnev_enter_input)
        self.btn7.grid(row=2, column=2, columnspan=2, sticky=(tk.W, tk.E))
        self.btn1 = ttk.Button(self.testframe, text="Start", command=self.btnev_start_machine)
        self.btn1.grid(column=2, row=3, sticky=(tk.W, tk.E))
        self.btn2 = ttk.Button(self.testframe, text="Step", command=self.btnev_step_machine)
        self.btn2.grid(column=2, row=4, columnspan=2, sticky=(tk.W, tk.E))
        self.btn3 = ttk.Button(self.testframe, text="Step backward")
        #self.btn3.grid(column=2, row=3, sticky=(tk.W, tk.E))
        self.btn4 = ttk.Button(self.testframe, text="Stop", command=self.btnev_stop_machine)
        self.btn4.grid(column=3, row=3, sticky=(tk.W, tk.E))
        self.btn5 = ttk.Button(self.testframe, text="Reset", command=self.btnev_reset_machine)
        self.btn5.grid(column=3, row=5, sticky=(tk.W, tk.E))
        self.btn6 = ttk.Button(self.testframe, text="Restart", command=self.btnev_restart_machine)
        self.btn6.grid(column=2, row=5, sticky=(tk.W, tk.E))

        self.btn1.state(["disabled"])
        self.btn2.state(["disabled"])
        self.btn3.state(["disabled"])
        self.btn4.state(["disabled"])
        self.btn5.state(["disabled"])
        self.btn6.state(["disabled"])
        self.btn7.state(["disabled"])

        self.menubar = tk.Menu(parent)
        parent['menu'] = self.menubar
        self.menu_file = tk.Menu(self.menubar)
        self.menubar.add_cascade(menu=self.menu_file, label="File")
        self.menu_file.add_command(label="Load program...", command=self.openprogram)
        self.menu_file.add_command(label="Exit", command=parent.destroy)

        self.testframe2 = tk.Frame(self)
        self.testframe2.grid(column=0, row=0, sticky=(tk.N), rowspan=2)
        self.yscroll = ttk.Scrollbar(self.testframe2, orient=VERTICAL)
        self.yscroll.grid(row=0, column=1, rowspan=2, sticky=(tk.N, tk.S))
        self.textbox = tk.Text(self.testframe2, width=52, height=35, yscrollcommand=self.yscroll.set)
        self.textbox.grid(column=0, row=0)
        self.yscroll['command'] = self.textbox.yview

        self.textbox["state"] = "disabled"

        self.testframe3 = tk.Frame(self)
        self.testframe3.grid(column=3, row=0, rowspan=2, sticky=(tk.N))
        self.yscroll2 = ttk.Scrollbar(self.testframe3, orient=VERTICAL)
        self.yscroll2.grid(column=4, row=0, sticky=(tk.N, tk.S), rowspan=2)
        self.textbox2 = tk.Text(self.testframe3, width=52, height=35, yscrollcommand=self.yscroll2.set)
        self.textbox2.grid(column=3, row=0, rowspan=1)
        self.yscroll2['command'] = self.textbox2.yview

        self.textbox2["state"] = "disabled"

        ttk.Label(self, text="Toy Machine\n16-bit Architecture", foreground="black", font=("Helvetica", -32), justify=tk.CENTER).grid(column=2, row=1, sticky=(tk.S))

        #ttk.Label(self, text="Name of program").grid(column=1, row=0)
        #ttk.Label(self, text="State: ").grid(column=2, row=0)

    def openprogram(self):
        #file = filedialog.askopenfile()
        #self.textbox["state"] = "normal"
        #iii = 1
        #for line in file:
         #   self.textbox.insert("{0}.0".format(iii), "{0} | ".format(iii))
          #  self.textbox.insert(INSERT, line)
           # iii += 1
        #self.textbox.insert("1.end", "\t\t|<--")
        #self.textbox["state"] = "disabled"
        self.file_name = filedialog.askopenfilename(filetypes=[("binary file", "*.bin")])
        if not isinstance(self.file_name, str):
            return
        elif self.file_name == "":
            return
        self.log_str = toy.init_toy(self.file_name)
        self.program_str = toy.toydisasm()

        self.textbox["state"] = "normal"
        self.textbox.delete("1.0", tk.END)
        self.textbox.insert("1.0", self.program_str)
        self.textbox["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.delete("1.0", tk.END)
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"

        self.btn1.state(["!disabled"])
        self.btn5.state(["!disabled"])

    def btnev_start_machine(self):
        self.btn1.bell()
        self.rect_id = self.canvas.create_rectangle(2, 2, 300, 100, outline="black", width=20)
        self.text_id = self.canvas.create_text(150, 60, text="", font=("Times", -64, "bold"), fill="lightblue")
        self.canvas.configure(background="blue")
        self.btn1.state(["disabled"])
        self.btn5.state(["disabled"])
        self.after(1, self.intro_anim1)

    def btnev_start_machine2(self):
        self.canvas.itemconfigure(self.text_id, text="FFFF")
        halt_flag, input_flag, output_flag, jmp_flag, pc = toy.start_toy()
        self.textbox["state"] = "normal"
        self.textbox.delete("1.0", tk.END)
        self.textbox.insert("1.0", self.program_str)
        self.textbox.insert("1.end", "\t\t|<--")
        self.textbox["state"] = "disabled"

        if halt_flag == 1:
            self.btn6.state(["!disabled"])
            self.btn2.state(["disabled"])
            self.btn4.state(["disabled"])
            self.btn5.state(["!disabled"])
            self.btncounter = 1
            return

        if jmp_flag == 1:
            self.btncounter = pc - 0x10 + 1

        if output_flag == 1:
            value = toy.output_toy()
            self.canvas.itemconfigure(self.text_id, text=format(value, 'X').zfill(4))
            self.btncounter += 1

        if input_flag == 1:
            self.textfield.state(["!disabled"])
            self.btn2.state(["disabled"])
            self.btn7.state(["!disabled"])
            self.btncounter += 1
        else:
            self.btn2.state(["!disabled"])
            self.log_str = toy.show_state()
            self.textbox2["state"] = "normal"
            self.textbox2.insert("1.0", self.log_str)
            self.textbox2["state"] = "disabled"
            self.btncounter += 1

        #self.btn2.state(["!disabled"])
        self.btn3.state(["!disabled"])
        self.btn4.state(["!disabled"])

    def intro_anim1(self):
        self.canvas.itemconfigure(self.text_id, text="Toy Machine", font=("Times", -32, "bold"))
        self.after(1500, self.intro_anim2)

    def intro_anim2(self):
        self.canvas.itemconfigure(self.text_id, text="")
        self.canvas.itemconfigure(self.text_id, text="presented by...", font=("Times", -32, "bold"))
        self.after(1500, self.intro_anim3)

    def intro_anim3(self):
        self.canvas.itemconfigure(self.text_id, text="")
        self.canvas.itemconfigure(self.text_id, text="Darjengar", font=("Times", -32, "bold"))
        self.after(1500, self.intro_anim4)

    def intro_anim4(self):
        self.canvas.itemconfigure(self.text_id, text="The concept was not my\nidea but the implementation.", font=("Times", -20, "bold"))
        self.after(1700, self.intro_anim5)

    def intro_anim5(self):
        self.canvas.itemconfigure(self.text_id, text="", font=("Times", -64, "bold"))
        self.after(1000, self.btnev_start_machine2)

    def btnev_step_machine(self):
        halt_flag, input_flag, output_flag, jmp_flag, pc = toy.start_toy()
        self.log_str = toy.show_state()
        self.textbox["state"] = "normal"
        self.textbox.delete("1.0", tk.END)
        self.textbox.insert("1.0", self.program_str)
        self.textbox.insert("{0}.end".format(self.btncounter), "\t\t|<--")
        self.textbox["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"
        if halt_flag == 1:
            self.btn6.state(["!disabled"])
            self.btn2.state(["disabled"])
            self.btn4.state(["disabled"])
            self.btn5.state(["!disabled"])
            self.btncounter = 1
        elif input_flag == 1:
            self.textfield.state(["!disabled"])
            self.btn2.state(["disabled"])
            self.btn7.state(["!disabled"])
            self.btncounter += 1
        elif output_flag == 1:
            value = toy.output_toy()
            self.canvas.itemconfigure(self.text_id, text=format(value, 'X').zfill(4))
            self.btncounter += 1
        elif jmp_flag == 1:
            self.btncounter = pc - 0x10 + 1
        else:
            self.btncounter += 1

    def btnev_stop_machine(self):
        self.btn4.bell()
        self.btn1.state(["!disabled"])
        self.btn2.state(["disabled"])
        self.btn4.state(["disabled"])
        self.btn5.state(["!disabled"])
        self.btn7.state(["disabled"])
        self.btncounter = 1

        self.canvas.configure(background="#697059")
        self.canvas.itemconfigure(self.text_id, text="")
        self.input.set(0)
        self.textfield.state(["disabled"])

        toy.stop_toy()
        self.log_str = toy.init_toy(self.file_name)

        self.textbox["state"] = "normal"
        self.textbox.delete("1.0", tk.END)
        self.textbox.insert("1.0", self.program_str)
        self.textbox["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.delete("1.0", tk.END)
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"

    def btnev_reset_machine(self):
        self.btn1.state(["disabled"])
        self.btn2.state(["disabled"])
        self.btn3.state(["disabled"])
        self.btn4.state(["disabled"])
        self.btn5.state(["disabled"])
        self.btn6.state(["disabled"])

        self.textbox["state"] = "normal"
        self.textbox.delete("1.0", tk.END)
        self.textbox["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.delete("1.0", tk.END)
        self.textbox2["state"] = "disabled"

        self.btn4.bell()
        self.canvas.configure(background="#697059")
        self.canvas.itemconfigure(self.text_id, text="")

        self.btncounter = 1
        self.file_name = ""

        toy.reset_toy()

    def btnev_restart_machine(self):
        self.btn1.state(["!disabled"])
        self.btn6.state(["disabled"])
        self.log_str = toy.restart_toy()
        self.textbox["state"] = "normal"
        self.textbox.delete("1.0", tk.END)
        self.textbox.insert("1.0", self.program_str)
        self.textbox["state"] = "disabled"
        self.textbox2["state"] = "normal"
        self.textbox2.delete("1.0", tk.END)
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"
        self.canvas.configure(background="#697059")
        self.canvas.itemconfigure(self.text_id, text="")

    def btnev_enter_input(self):
        self.btn7.state(["disabled"])
        self.btn2.state(["!disabled"])
        self.textfield.state(["disabled"])
        number_str = ""
        number = self.input.get()
        number_str = format(number, 'X').zfill(4)
        toy.input_toy(number)
        self.canvas.itemconfigure(self.text_id, text=number_str)
        self.log_str = toy.show_state()
        self.textbox2["state"] = "normal"
        self.textbox2.insert("1.0", self.log_str)
        self.textbox2["state"] = "disabled"
        self.input.set(0)

if __name__ == '__main__':
    app = ToyMachineGUI()
    app.mainloop()
