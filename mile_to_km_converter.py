import tkinter

window = tkinter.Tk()
window.title("Mile to Km Converter")

window.minsize(width = 30 , height = 30)
window.config(padx = 20 , pady = 20)

def convert():
    miles = float(dist_mile.get())
    dist_km.config(text = f"{miles * 1.609}")

dist_mile = tkinter.Entry(width = 7)
dist_mile.grid(row = 0 , column = 1)

miles = tkinter.Label(text = "Miles")
miles.grid(row = 0 , column = 2)

is_eq = tkinter.Label(text = "is equal to")
is_eq.grid(row = 1 , column = 0)


dist_km = tkinter.Label(text = "0")
dist_km.grid(row = 1 , column =1 ) 

Km_label = tkinter.Label(text = "Km")
Km_label.grid(row = 2 , column = 2)


Button = tkinter.Button(text = "calculate" , command = convert)
Button.grid(row = 2 , column = 1)




window.mainloop()
