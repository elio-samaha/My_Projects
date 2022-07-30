from turtle import Turtle, Screen, penup, colormode
from random import randint
import turtle as t
import random
import turtle

screen = Screen()
screen.setup(width = 500 ,height =  400)
colors = {"red" , "orange" , "yellow" , "green" , "blue" , "purple"}
user_bet = ""
while user_bet not in colors:
    user_bet = screen.textinput(title = "make your bet !" , prompt = "which turtle will win the race ? (enter a color) :")
x = 0
colors = list(colors)
all_turtle = []

for i in range(6):
    timy = Turtle(shape = "turtle")
    timy.color(colors[i])
    timy.penup()
    timy.goto(x = -230 , y = - 70 + x)
    x += 30
    all_turtle.append(timy)

if user_bet :
    is_race_on = True

while is_race_on:
    for t in all_turtle:
        if t.xcor() > 230:
            is_race_on = False
            winning_color = t.pencolor()
            if user_bet == winning_color:
                print(f"You've won ! The {winning_color} turtle is the winner.")
            else:
                print(f"You've lost :( The {winning_color} turtle is the winner.")
        rand_dist = random.randint(0 , 10)
        t.forward(rand_dist)
        

screen.exitonclick()
