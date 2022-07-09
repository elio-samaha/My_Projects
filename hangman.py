import random

HANGMANPICS = ['''
  +---+
  |   |
      |
      |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
      |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
  |   |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|   |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|\  |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|\  |
 /    |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|\  |
 / \  |
      |
=========''']

pic = '''

 _                                             
| |                                            
| |__   __ _ _ __   __ _ _ __ ___   __ _ _ __  
| '_ \ / _` | '_ \ / _` | '_ ` _ \ / _` | '_ \ 
| | | | (_| | | | | (_| | | | | | | (_| | | | |
|_| |_|\__,_|_| |_|\__, |_| |_| |_|\__,_|_| |_|
                    __/ |                      
                   |___/                       
'''

word_list = ["levi" , "hisoka" , "eren" , "mikasa" , "ichigo" , "gojo" , "killua", "gon" , "luffy" , "naruto" , "jean" , "nezuko" , "tanjiro" , "yagami" , "netero" , "midoriya"]

chosen_word = random.choice(word_list)

s_chosen_word = "".join(chosen_word)

trials = 6

lenght = len(chosen_word)

letters_left = lenght 

li = ["_"] * lenght

used = set()

print(pic , "\n")

print(" ".join(li), "\n")
print(HANGMANPICS[0] , "\n")

while trials > 0 and letters_left > 0 :
    guess = input("choose a letter:   ").lower()
    if guess in used:
        print("\nAlready used or guessed\n")
        print(f"\nyou still have {trials} left and {letters_left} to guess \n" , " ".join(li) , '\n')
        print(HANGMANPICS[6 - trials] , "\n")
        continue

    used.add(guess)
    x = False

    for i in range(lenght):
        if guess == chosen_word[i]:
                li[i] = guess
                letters_left -= 1
                x = True
        

    if x == False :
        trials -= 1

    if letters_left != 0 and trials != 0 :   
        print(HANGMANPICS[6 - trials] , "\n") 
        print(f"\nyou still have {trials} left and {letters_left} to guess \n" , " ".join(li) , "\n")

if letters_left == 0 :
    print(HANGMANPICS[6 - trials] , "\n")
    print(f"Congrats ! you won :) \nThe word was {s_chosen_word}\n")
else:
    print(HANGMANPICS[-1] , "\n")
    print(f"Unlucky ! You lost :(\nThe word was {s_chosen_word}\n")
