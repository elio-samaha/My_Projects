from turtle import Turtle
STARTING_POSITIONS = [(0 , i) for i in range(280 , -300 , -50)]
print(STARTING_POSITIONS)

class Decoration:

    def __init__(self):
        self.segments = []
        self.create_segment()

    def create_segment(self):
        for position in STARTING_POSITIONS:
            self.add_segment(position)

    def add_segment(self, position):
        new_segment = Turtle("square")
        new_segment.color("white")
        new_segment.penup()
        new_segment.shapesize(stretch_wid = 1 , stretch_len = 0.2)
        new_segment.goto(position)
        self.segments.append(new_segment)

    def extend(self):
        self.add_segment(self.segments[-1].position())