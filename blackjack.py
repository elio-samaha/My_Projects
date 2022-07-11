import random
from replit import clear
import sys

logo = '''
 _     _            _    _            _    
| |   | |          | |  (_)          | |   
| |__ | | __ _  ___| | ___  __ _  ___| | __
| '_ \| |/ _` |/ __| |/ / |/ _` |/ __| |/ /
| |_) | | (_| | (__|   <| | (_| | (__|   < 
|_.__/|_|\__,_|\___|_|\_\ |\__,_|\___|_|\_\\
                       _/ |                
                      |__/                    
                      
                      
                      '''


def deal_cards():
    cards = [11 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 10 , 10 , 10]
    return random.choice(cards)


def calculate_score(card):
    score = sum(card)
    if score == 21 and len(card) == 2:
        return 0
    if 11 in card and score > 21:
        return score -10

    return score

def compare(player_score , computer_score):
    if player_score == computer_score:
        return "Draw !\n"
    elif player_score == 0 :
        return "You won with a Blackjack !\n"
    elif computer_score == 0:
        return "You lost, the opponent had a Blackjack :(\n"
    elif player_score > 21:
        return "You went over, you lost :(\n"
    elif computer_score > 21 :
        return "Opponent went over, you won !\n"
    elif player_score > computer_score :
        return "You won ! Congrats :)\n"
    return "You lost :(\n"

def play_game():   

    print(logo)
    
    player_cards = []

    computer_cards = []

    is_game_over = False

    for _ in range(2):
        player_cards.append(deal_cards())
        computer_cards.append(deal_cards())

    print("")

    while not is_game_over :
        player_score = calculate_score(player_cards)
        computer_score = calculate_score(computer_cards)

        print(f"Your cards are : {player_cards} and your score is : {player_score}\n")
        print(f"The opponent card is : {computer_cards[0]}\n")

        if player_score == 0 or computer_score == 0 or player_score > 21:
            is_game_over = True
        else :
            user_should_deal = input("Type 'y' to get another card, type 'n' to pass: ").lower()
            print("")
            if user_should_deal == "y":
                player_cards.append(deal_cards())
            elif user_should_deal == 'n':
                is_game_over = True
            else : 
                print("Wrong input, Try again\n")

    computer_stop_choice = 21 - random.choices([1 , 2 , 3 , 4 , 5 , 6] , weights = (7 , 8 , 10 , 25 , 35 , 15) , k = 1)[0]

    while computer_score != 0 and computer_score <= computer_stop_choice:
        computer_cards.append(deal_cards())
        computer_score = calculate_score(computer_cards)


    print(f"Your final hand is : {player_cards} and your final score is {player_score}\n")
    print(f"The opponent final hand is : {computer_cards} and their final score is {computer_score}\n")
    print(compare(player_score , computer_score))

    again = input("Type 'y' to play again, anything else to stop: ").lower() 
    print("")
    if again == 'y' :
        clear()
        play_game()
    else :
        sys.exit()

if __name__ == '__main__' :
    play_game()