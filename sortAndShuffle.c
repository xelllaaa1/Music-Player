#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "sortAndShuffle.h"

// A tiny helper function which converts uppercase chars to lowercase using ASCII arithmetic.
char tolowerchar(char ch)
{
    // If the char passed into tolowerchar() is uppercase, return it's ASCII + 32,
    // which would result in a lowercase letter.
    if (ch >= 'A' && ch <= 'Z')
    {
        return ch + 32;
    }
    // Otherwise (if the if's condition is never met (meaning it is already a lowercase),
    // return the char back.
    return ch;
}

// A function to compare 2 strings ignoring case.
int compareStrings(char string1[], char string2[])
{
    // 1. Iterate over both strings at the same time.
    size_t i = 0;
    while(string1[i] != '\0' && string2[i] != '\0')
    {
        // 2. Convert the current chars of both strings to lowercase.
        char ch1 = tolowerchar(string1[i]);
        char ch2 = tolowerchar(string2[i]);

        // 3. If a differing char was found, return their difference to determing
        // which string of the 2 passed into the function should come before in alphabetical order.
        // Returns < 0 if s1 comes before s2,
        // Returns > 0 if s1 comes after s2,
        // Returns 0 if they are equal.
        if(ch1 != ch2)
        {
            return ch1 - ch2;
        }

        // Increment i for next char.
        i++;
    }

    // 4. In a scenario, where one string end before the other, AND they are identical
    // but one of them has some more characters, return the difference of the last character iterated through to determine
    // which should come first.
    return tolowerchar(string1[i]) - tolowerchar(string2[i]);
}

// This method sorts the artists and songs alphabetically. It takes as input:
// - targetArray: the array of artists/songs that that should be sorted,
// - arraySize: the total number of iterations.
void selectionSort(char targetArray[][80] , int arraySize)
{
    // Selection Sort:
    for(size_t i = 0; i < arraySize - 1; i++)
    {
        // [0,3,2,4,5]
        //1. Assume smallest item is targetArray[0]. minimal_index = i.
        int minimal_index = i;
        
        // 2. Go through the array and try to find something smaller than current minimal_index.
        // If something smaller was found, update minimal_index with j.
        for (size_t j = i + 1; j < arraySize; j++)
        {
            if((compareStrings(targetArray[j], targetArray[minimal_index])) < 0)
            {
                minimal_index = j;
            }
        }
        
        //3. After the new minimal_index was found, swap the name at initial min_i with final min_i
        if(minimal_index != i) swap(targetArray[i], targetArray[minimal_index]);
    }
    // We keep doing this until we get all elements moved to correct position.
}

// A tiny helper function to swap 2 elements of a 2D character array.
void swap(char a[], char b[])
{
    char temp[160]; // A temp array for swapping.
    strcpy(temp, a); // "Move" string a to temp.
    strcpy(a, b); // "Move" b to a.
    strcpy(b, temp); // "Move" temp(old a) to b. 
}


// This method shuffles a set of songs. It takes as input:
// - songsToBeShuffled: the array of the songs that should be shuffled
// - numOfSongs: the number of songs to be shuffled
void shuffleSongs(char songsToBeShuffled[24][160], int numOfSongs)
{
   // 1. Iterate over the array from 0 to numOfSongs - 1.
   for(int i = 0; i < numOfSongs - 1; i++)
   {
       int j;
       bool noReoccurences;
       int retries = 100;
       
       do
       {
           // 2. Pick a random index between i and numOfSongs - i.
           j = i + rand() % (numOfSongs - i);
           noReoccurences = true;
           
           // 3. Set the starting index of a checking loop to i - 5.
           int start = i - 5;
           
           // 4. If i < 5, we need to change the
           // start value, otherwise we would go out of bounds by trying to access negative indices,
           // e.g. i = 3, i - 5 = -2, out of bounds, program crashes.
           if (start < 0) start = 0;
           
           // 5. Iterate over the indices of an array from songs[start] to songs[i-1]
           // to check the previous 5 songs to see if the same has already appeared.
           for(int k = start; k < i; k++)
           {
               if(strcmp(songsToBeShuffled[j], songsToBeShuffled[k]) == 0)
               {
                   // 6. If within these iterations a match was found, break out of the loop
                   // and try to find a valid song to swap to again.
                   noReoccurences = false;
                   break;
                }
           }
        // 7. We keep doing this until a valid song was found or retries are exhausted.
        retries--;
        } while(!noReoccurences && retries > 0);

        // Also check if j = i, since if they are the same, the result of switching them
        // visually would result in nothing, but the computer would need to do extra work.
        if(j != i) swap(songsToBeShuffled[i], songsToBeShuffled[j]);
    }   
}