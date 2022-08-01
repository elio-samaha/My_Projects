from turtle import Turtle
ALIGNMENT = "center"
FONT = ("Courier", 24, "normal")

class Scoreboard(Turtle):

    def __init__(self) -> None:
        super().__init__()
        self.color("white")
        self.penup()
        self.hideturtle()
        self.l_score = 0
        self.r_score = 0
        self.update_scoreboard()
    
    def update_scoreboard(self):
        self.clear()
        self.goto(-100 , 200)
        self.write(self.l_score , align = "center" , font = ("courier" , 80 , "normal"))
        self.goto(100 , 200)
        self.write(self.r_score , align = "center" , font = ("courier" , 80 , "normal"))

    def l_point(self):
        self.l_score += 1
        self.update_scoreboard()

    def r_point(self):
        self.r_score += 1
        self.update_scoreboard()

    def game_over(self , winner):
        self.goto(0, 0)
        self.write(f"GAME OVER \n {winner} wins ! ({self.l_score} - {self.r_score}) " , align=ALIGNMENT, font=FONT)
