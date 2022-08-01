from turtle import Turtle, Screen
from paddle_class import Paddle
from ball_class import Ball
import time
from score_board_class import Scoreboard
from decoration_class import Decoration

screen = Screen()
screen.bgcolor("black")
screen.setup(width = 800 , height = 600)
screen.title("Pong")

user_max_score = screen.numinput(title="Maximum score", prompt= "choose a maximum score")

screen.tracer(0)

l_paddle = Paddle(position = (350 , 0) , name = screen.textinput(title=" Player 1 name", prompt= "choose a name"))
r_paddle = Paddle(position = (-350 , 0) , name = screen.textinput(title="Player 2 name", prompt= "choose a name"))
segments = Decoration()
ball = Ball()
scoreboard = Scoreboard()

screen.listen()
screen.onkey(r_paddle.go_up , "Up")
screen.onkey(r_paddle.go_down , "Down")

screen.onkey(l_paddle.go_up , "w")
screen.onkey(l_paddle.go_down , "s")

game_is_on = True
while game_is_on:
    time.sleep(ball.move_speed)
    screen.update()
    ball.move()

    #detect collision with wall
    if ball.ycor() > 280 or ball.ycor() < -280 :
        ball.bounce_y()

    #detect collision with right paddle
    if ball.distance(r_paddle) < 50 and ball.xcor() > 320 or ball.distance(l_paddle) < 50 and ball.xcor() < -320 :
        ball.bounce_x()

    #detect when r_paddle miss
    if ball.xcor() > 380 :
        ball.reset_position()
        scoreboard.l_point()
        if scoreboard.l_score >= user_max_score:
            game_is_on = False
            scoreboard.game_over(l_paddle.name)

    #detect when l_paddle miss
    if ball.xcor() < -380 :
        ball.reset_position()
        scoreboard.r_point()
        if scoreboard.r_score >= user_max_score:
            game_is_on = False
            scoreboard.game_over(r_paddle.name)

    

screen.exitonclick()
