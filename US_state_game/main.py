from string import capwords
import turtle
import pandas as pd

screen = turtle.Screen()
screen.title("U.S state game")
screen.addshape("blank_states_img.gif")
turtle.shape("blank_states_img.gif")

data = pd.read_csv("50_states.csv")
states = set(data["state"].to_list())
guessed_states = []

while len(guessed_states) < 50:
    
    answer_state = capwords(screen.textinput(title = f"{len(guessed_states)}/50 States correct" , prompt = "What's another state name ? (or type exit to stop)"))
    #or string.title()

    if answer_state == "Exit":
        guessed_states = set(guessed_states)

        missed_states = list(states - guessed_states)
        print(len(missed_states))

        new_data = pd.DataFrame(missed_states)
        new_data.to_csv("missed_states.csv")
        break

    if answer_state in states:
        guessed_states.append(answer_state)
        t = turtle.Turtle()
        t.hideturtle()
        t.penup()
        state_data = data[data.state == answer_state]
        t.goto(int(state_data["x"]) , int(state_data["y"]))
        t.write(answer_state) #or state_data.state.item()

