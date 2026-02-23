import tkinter as tk
import random
import time
import sys
import os
from tkinter import ttk
from panda3d.core import *
from direct.showbase.ShowBase import ShowBase

health = 100
def update():
    Game.__init__().window.update()
    update_healthbar()
    check_health()
    time.sleep(0.05)

def check_health():
    pass

def update_healthbar():
    Game.healthbar["value"] == Game.player.health


class MenuScreen:
    def __init__(self):
        self.window = tk.Tk()
        self.window.title("Fuzzy Playtime")
        self.window.geometry("1152x648")

        #The background Image
        
        self.bg_image = tk.PhotoImage(file="Menu_Background.png")

        self.bg_label = tk.Label(self.window, image=self.bg_image)
        self.bg_label.place(x=0, y=0, relwidth=1, relheight=1)

        self.bg_label.image = self.bg_image
        
        #The Footer
        
        self.footer = tk.Label(self.window, text="Fuzzy Playtime 2026 - All rights reserved", font="Arial, 10", bg="black", fg="white")
        self.footer.pack(side="bottom")
        
        #The menu
        
        self.exit_btn = tk.Button(self.window, text="Exit", command=self.exit)
        self.exit_btn.config(pady="2", padx="300", bg="darkred", fg="white", font="Arial, 18")
        self.exit_btn.pack(side="bottom")

        self.setting_btn = tk.Button(self.window, text="Settings", command=self.enter_settings)
        self.setting_btn.config(pady="2", padx="300", bg="black", fg="white", font="Arial, 18")
        self.setting_btn.pack(side="bottom")
        
        self.start_btn = tk.Button(self.window, text="Start", command=self.start_game)
        self.start_btn.config(pady="2", padx="300", bg="black", fg="white", font="Arial, 18")
        self.start_btn.pack(side="bottom")

    def start_game(self):
        game = Game()
        game.window.mainloop()

    def enter_settings(self):
        pass

    def exit(self):
        self.window.destroy()
        sys.exit()

class Game:
    def __init__(self):
        self.window = tk.Tk()
        self.window.title("Fuzzy Playtime")
        self.window.geometry("300x452")
        self.window.config(bg="black")

        #HeaLthbar

        self.healthbar = tk.ttk.Progressbar(self.window, length=200, mode='determinate')
        self.healthbar.pack()

        #Crosshair

        
        #self.crosshair = tk.PhotoImage(file="Crosshair.png")

        #self.crosshair = tk.Label(self.window, image=self.crosshair)
        #self.crosshair.place(x=0, y=0, relwidth=1, relheight=1)

        #self.crosshair_label.image = self.crosshair

        self.create_world()

    def create_world(self):
        pass

class Player:
    player = {
        "Health": 100,
        "speed_walking": 7,
        "speed_sprinting": 11,
        "inventory": [],
        "equipted_item": None,
        "sprinting": False,
        "inventory_full": False
    }
    animations = {
        "animation_playing": False,
        "animation_loop": False
    }
if __name__ == "__main__":
    start = MenuScreen()
    start.window.mainloop()

