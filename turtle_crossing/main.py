import time
from turtle import Screen
from player import Player
from car_manager import CarManager
from scoreboard import Scoreboard

screen = Screen()
screen.setup(width=600, height=600)
screen.tracer(0)
player = Player()
cars = CarManager()
scoreboard = Scoreboard()
screen.listen()
screen.onkey(fun = player.move_up , key = "Up")

game_is_on = True
while game_is_on:
    time.sleep(0.1)
    screen.update()
    cars.generate()
    cars.move()

    #detect colision with a car
    for car in cars.all_cars:
        if car.distance(player) < 20:
            game_is_on = False
            scoreboard.game_over()

    #detect when the player reaches the other side of the screen
    if player.is_at_finish_line():
        player.level_up()
        cars.increase()
        scoreboard.next_level()

screen.exitonclick()
