[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tiqo3ZpS)
# Project 10

## Project Goals
The goal of this project is to:
1.	familiarize students with **2D Arrays** as **Arrays of Strings**
    - use the <a href="https://erinkeith.github.io/135/quick_guides/2d_arrays">2D Arrays Quick Guide</a> for short syntax reminders!
2.	provide students with continued practice using **everything else!**
    - use my <a href="https://erinkeith.github.io/cs135">website</a> for refreshers on previous topics!
   
## Important Notes
1.	**Filename**: Save your program as `twordle.c`

# Program
Twordle! (just in case Wordle is trademarked)

## Background
This project is loosely based on the game <a href="https://en.wikipedia.org/wiki/Wordle">Wordle</a>.

## Behavior
- The program should load a 5 letter word from the `word.txt` file.
   - For simplicity-sake, you may assume the mystery word will not have duplicate letters.
- The user should be prompted for their guess until they guess the mystery word or they've guessed 6 times, whichever comes first.
   - The user should be prompted to re-enter their guess if it is not 5 letters long or if any non-letter characters are entered (but these invalid entries are not counted as "guesses").
- All of the user's previous guesses should be displayed back to the user.
   - Letters in the correct spot should be capitalized.
   - Correct letters in the incorrect spot should be "pointed to" from the line below, using the `^` (above the `6` on your keyboard).
      - Letters which have been correctly guessed should not be "pointed to" if they're elsewhere in the word; even though the mystery word cannot have duplicate letters, the user entries can.
- Once the game has ended (because they user has won or run out of guesses), an appropriate message should be displayed.

## Hints
- In my program, the guess is converted to all lower-case before the program starts checking it, just in case the user enters any capital letters.
- This repository already contains a file with a mystery word, but feel free to update it with a new word!
- **Remember!** If a chunk of code starts to feel too compilicated because it's doing too much, you may need to split it up into functions!

## Requirements
- Your program must have ***at least 6 functions***, in addition to the `main` function (my program had 10).
- **You may *NOT* use any libraries other than `stdio` and `stdbool`.**
- There is not a requirement for the File IO to be in the main; I put mine in its own function.

## Example Program Execution
An example executable (program) is provided in this repository. You should be able to run it from your project folder after cloning the repository in your Linux environment, in the same way you would run an executable you create.
- To **_run_** the example executable, use the ```./twordleExecutable``` command after changing directories into your repository folder.
- If you encounter a **permission denied** error when attempting to run the executable, use the ```chmod u+x twordleExecutable``` command in the terminal before trying again (using the command above).

# Extra Credit
***Extra credit code will not be graded unless the required functionality described in the previous sections is complete.***
   
## Important Notes
1.	**Filename**: Save your extra credit program in a ***<ins>different file</ins>*** called `twordle_ec.c`
2.	Submitted code will be graded for up to 25% extra points.

# Program
Add one or more of the following features
1. pick a random word from a file with multiple words
2. handle words with duplicate letters
3. keep the top 5 players and their scores in a file and allow the user to see a "scoreboard"
   - the score is the number of guesses ("high score" is actually fewer guesses)
