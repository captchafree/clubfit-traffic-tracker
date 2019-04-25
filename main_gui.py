import PIL
from PIL import Image, ImageTk
from tkinter import *

boxes = list()
current_box = None

class Box:
    def __init__(self):
        self.start_x = 0
        self.start_y = 0
        self.width = 0
        self.height = 0

class ExampleApp(Frame):
    def __init__(self,master):
        Frame.__init__(self,master=None)
        self.x = self.y = 0
        self.canvas = Canvas(self, width=800, height=500, cursor="cross")

        self.sbarv=Scrollbar(self,orient=VERTICAL)
        self.sbarh=Scrollbar(self,orient=HORIZONTAL)
        self.sbarv.config(command=self.canvas.yview)
        self.sbarh.config(command=self.canvas.xview)

        self.canvas.config(yscrollcommand=self.sbarv.set)
        self.canvas.config(xscrollcommand=self.sbarh.set)

        self.canvas.grid(row=0,column=0,sticky=N+S+E+W)
        self.sbarv.grid(row=0,column=1,stick=N+S)
        self.sbarh.grid(row=1,column=0,sticky=E+W)

        self.canvas.bind("<ButtonPress-1>", self.on_button_press)
        self.canvas.bind("<B1-Motion>", self.on_move_press)
        self.canvas.bind("<ButtonRelease-1>", self.on_button_release)

        self.rect = None

        self.start_x = None
        self.start_y = None

        self.im = PIL.Image.open("gui_test_image.jpg")
        self.wazil,self.lard=self.im.size
        self.canvas.config(scrollregion=(0,0,self.wazil,self.lard))
        self.tk_im = ImageTk.PhotoImage(self.im)
        self.canvas.create_image(0,0,anchor="nw",image=self.tk_im)   


    def on_button_press(self, event):
        # save mouse drag start position
        self.start_x = self.canvas.canvasx(event.x)
        self.start_y = self.canvas.canvasy(event.y)

        # create rectangle if not yet exist
        # if not self.rect:
        self.rect = self.canvas.create_rectangle(self.x, self.y, 1, 1, outline='red')

        global current_box
        current_box = Box()
        current_box.start_x = self.start_x
        current_box.start_y = self.start_y

    def on_move_press(self, event):
        curX = self.canvas.canvasx(event.x)
        curY = self.canvas.canvasy(event.y)

        """
        w, h = self.canvas.winfo_width(), self.canvas.winfo_height()
        if event.x > 0.9*w:
            self.canvas.xview_scroll(1, 'units') 
        elif event.x < 0.1*w:
            self.canvas.xview_scroll(-1, 'units')
        if event.y > 0.9*h:
            self.canvas.yview_scroll(1, 'units') 
        elif event.y < 0.1*h:
            self.canvas.yview_scroll(-1, 'units')
        """

        # expand rectangle as you drag the mouse
        self.canvas.coords(self.rect, self.start_x, self.start_y, curX, curY)    

    def on_button_release(self, event):
        curX = self.canvas.canvasx(event.x)
        curY = self.canvas.canvasy(event.y)

        global current_box
        global boxes

        current_box.width = abs(current_box.start_x - curX)
        current_box.height = abs(current_box.start_y - curY)

        boxes.append(current_box)
        current_box = None

def on_closing():
    global boxes
    config = open('config.csv','w') 

    for box in boxes:
        config.write("{},{},{},{}\n".format(int(box.start_x), int(box.start_y), int(box.width), int(box.height)))
    config.close()

    root.destroy()

if __name__ == "__main__":
    root=Tk()
    # root.geometry("1280x720")
    root.protocol("WM_DELETE_WINDOW", on_closing)
    app = ExampleApp(root)
    app.pack()
    root.mainloop()