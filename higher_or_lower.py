from data_sample_hol import *
import random
from replit import clear



def game(score = 0 , new_game = True , account_a = None , account_b = None):
    #score = 0
    if new_game :
        print(logo)
        account_a , account_b = random.choices(data , k = 2)
        while account_a == account_b :
            account_a , account_b = random.choices(data , k = 2)
    else:
        account_b = random.choice(data)
        while account_a == account_b :
            account_b = random.choice(data)

    
    name_a = account_a["name"]
    follower_a = account_a["follower_count"]
    description_a = account_a["description"]
    country_a = account_a["country"]

    name_b = account_b["name"]
    follower_b = account_b["follower_count"]
    description_b = account_b["description"]
    country_b = account_b["country"]

    print(f"Compare between : A -> {name_a}, a {description_a} from {country_a}")
    print(vs)
    print(f"B -> {name_b}, a {description_b} from {country_b}\n")

    guess = input("Who has more followers ? Type 'A' or 'B' : ").upper()
    i = 0
    while guess != 'A' and guess != 'B' and i < 10:
        print("wrong input, try again\n")
        guess = input("Who has more followers ? Type 'A' or 'B' : ").upper()
    
    if follower_a >= follower_b :
        answer = 'A'
    else :
        answer = 'B'


    if answer == guess :
        clear()
        score += 1
        print(logo)
        print(f"Correct ! ({follower_a} against {follower_b} millions) , your score is {score}\n")
        game(score , False , account_b)
    else :
        print(f"You lost ! ({follower_a} against {follower_b} millions) Your score was {score}\n")
    


game()