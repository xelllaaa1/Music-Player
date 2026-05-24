#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sortAndShuffle.h"

// A function to get user input to fill the artists array. 
int inputArtist(char buffer[80], char artists[][80], int *numArtists);

// A function to get user input to fill the songs array.
void inputSongs(char buffer[80], char targetSongsArray[][80], int *artistSongs, char currentArtist[80]);

// A function to find the original index of a certain artist in the original artists array.
// See function definition below main function for further details.
int findOriginalIndex(char unsortedArtists[][80], char sortedArtists[][80], int numOfArtists, int indexInSorted);

// A function to print out the artists names to the console.
void printArtists(char artists[4][80], int numArtists);

// A function to print out the names of the songs to the console.
void printSongs(char songs[3][80], int songNumber);

// A function to print out the final shuffled playlist to the console.
void printPlaylist(char playlist[24][160], int playlistSize);

// A function to concatenate song names to their respective artists.
// See function definition below main function for further details.
void combineNames(char [24][160], int *totalPlaylistSize, char artist[80], char song[][80], int songCount);

int main(void)
{
    // Generate a random number for future shuffling.
    srand(time(NULL));

    // The array containing artists names to be filled from user input.
    char artists[4][80];

    // The array containing artists names sorted in alphabetical order.
    char sortedArtists[4][80];

    //Arrays for songs of Artist 1, 2, 3, and 4.
    char songsArtist1[3][80];
    char songsArtist2[3][80];
    char songsArtist3[3][80];
    char songsArtist4[3][80];

    // Array containing the final playlist.
    char playlist[24][160];
    // Counter for filling the playlist 2D array.
    int totalPlaylistSize = 0;
    // The total number of artists tracked.
    int numOfArtists = 0;
    //The total number of songs for each artist. (Note that less than 3 songs can be provided for each artist)
    int numSongsPerArtist[4] = {0};

    // 1 iteration of this while loop = 1 artist inputed.
    while(numOfArtists < 4)
    {
        // A buffer array used for user input.
        // Will be used to copy from it to the target arrays.
        char buffer[80];

        // A flag variable to track whether an artist has been added to artists[] successfully.
        int success = inputArtist(buffer, artists, &numOfArtists);
        // A variable to track current artist. Set to numOfArtists - 1 to account for 0-indexing.
        int current_artist = numOfArtists - 1;

        // If at some point during reading the input success is set to 0, break out of the
        // while loop indicating the end of user input.
        if (success == 0) break;
        
        // A switch statement to determine which songs array is to be filled depending
        // on the current iteration of the while loop.
        switch(current_artist)
        {
            case 0:
                inputSongs(buffer, songsArtist1, &numSongsPerArtist[0], artists[0]);
                break;

            case 1:
                inputSongs(buffer, songsArtist2, &numSongsPerArtist[1], artists[1]);
                break;

            case 2:
                inputSongs(buffer, songsArtist3, &numSongsPerArtist[2], artists[2]);
                break;

            case 3:
                inputSongs(buffer, songsArtist4, &numSongsPerArtist[3], artists[3]);
                break;

            default:
            puts("wth happened?");
            break;
        }
        puts("");
    }

    // A for-loop to copy the artists names from artists[] into sortedArtists[]
    // to then sort them.
    for (size_t i = 0; i < numOfArtists; i++)
    {
        strcpy(sortedArtists[i], artists[i]);
    }
    selectionSort(sortedArtists, numOfArtists);

    // A for-loop to sort the songs of the artists using selectionSort().
    for(size_t i = 0; i < numOfArtists; i++)
    {
        if (i == 0) selectionSort(songsArtist1, numSongsPerArtist[i]);
        if (i == 1) selectionSort(songsArtist2, numSongsPerArtist[i]);
        if (i == 2) selectionSort(songsArtist3, numSongsPerArtist[i]);
        if (i == 3) selectionSort(songsArtist4, numSongsPerArtist[i]);
    }

    // Since the artists were sorted, their respective indices in original artists[] array changed
    // to new ones, e.g. if Evanescence had index 2 in artists[], it may now have a different
    // index in sortedArtists[].
    // At the same time, the songs that used to correspond to one artists now correspond to another one.
    // e.g. After the index swap, Evanescence's songs are still in songsArtistX[], when they now correspond
    // to another song array, say, songsArtist4[].

    puts("Sorted list of songs:");
    for(size_t i = 0; i < numOfArtists; i++)
    {
        int indexInSorted = i;
        printf("%s\n", sortedArtists[i]);
        int originalIndex = findOriginalIndex(artists, sortedArtists, numOfArtists, i);
        if (originalIndex == 0) printSongs(songsArtist1, numSongsPerArtist[0]);
        if (originalIndex == 1) printSongs(songsArtist2, numSongsPerArtist[1]);
        if (originalIndex == 2) printSongs(songsArtist3, numSongsPerArtist[2]);
        if (originalIndex == 3) printSongs(songsArtist4, numSongsPerArtist[3]);
    }
    puts("");

    // Concatenate song names to their respective artists' names.
    combineNames(playlist, &totalPlaylistSize, artists[0], songsArtist1, numSongsPerArtist[0]);
    combineNames(playlist, &totalPlaylistSize, artists[1], songsArtist2, numSongsPerArtist[1]);
    combineNames(playlist, &totalPlaylistSize, artists[2], songsArtist3, numSongsPerArtist[2]);
    combineNames(playlist, &totalPlaylistSize, artists[3], songsArtist4, numSongsPerArtist[3]);

    // Use here the function shuffleSongs to shuffle all the songs.
    // Call shuffleSongs() method to shuffle the songs in playlist[].
    // Print the resulting shuffled playlist to the console.
    puts("Shuffled playlist");
    shuffleSongs(playlist, totalPlaylistSize);
    printPlaylist(playlist, totalPlaylistSize);

    // ASCII-art
    FILE *file = fopen("banner.txt", "r");
    if(!file)
    {
        puts("Error opening file.");
        return -1;
    }

    puts("");

    // Ascii art
    // I've decided to go for "Coded by AK" signature, which is just my initials.
    char line[200];
    while(fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    fclose(file);

    return 0;
}

// A function to read the user input in order to fill the artists[] and songArtistX[].
int inputArtist(char buffer[80], char artists[][80], int *numArtists)
{
    
    // 1. Prompt "Insert an artist/group name: ".
    // 2. Read the name in using fgets into a buffer array.
    printf("Insert an artist/group name: ");
    fgets(buffer, 80, stdin);
    
    // 3. In order to handle an edge case where the user inputs 0 artists:
    //  - While the buffer is a '\0' (a sentinel value) AND the number of artists currently stored
    //  is less than 1 (meaning no artist name has been read) prompt the user for input again.
    while((strcmp(buffer, "\n") == 0) && *numArtists < 1)
    {
        printf("Enter at least one artist: ");
        fgets(buffer, 80, stdin);
    }
    
    // 4. If the user has entered a desired amount of artists, they press "Enter" to indicate
    //  they are finished.
    // In that case, if the buffer is a '\n', clear buffer for future use and return 0 indicating
    // no artist name has been inputed in that iteration of the for-loop in main.
    if ((strcmp(buffer, "\n")) == 0)
    {
        buffer[0] = '\0';
        return 0;
    }
    
    // 5. Remove a '\n' from the buffer so it doesn't mess with the output.
    // 6. Copy artist's name from buffer into respective row of 2D artists[] array.
    // 7. Clear buffer for future use.
    // 8. Increment the numArtists counter for use in main() loops.
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(artists[*numArtists], buffer);
    buffer[0] = '\0';
    *numArtists += 1;

    // 9. Return 1 indicating success.
    return 1;
}

void inputSongs(char buffer[80], char targetSongsArray[][80], int *artistSongs, char currentArtist[80])
{
    // 3. If it is not the first song, and the buffer is \n, return.
    // 4. If it is a proper song name (not a \n), put it into respective artist song array.
    // 5. If it has already been 3 songs, return.
    
    // A while-loop where 1 iteration = 1 song inputed.
    // (Note that less than 3 songs can be provided for each artist.)
    int songCount = 0;
    while (songCount < 3)
    {
        // 1. Prompt the user to "Insert song X for A: ";
        // 2. Read the name in using fgets into a buffer array.
        printf("Insert song %d for %s: ", songCount + 1, currentArtist);
        fgets(buffer, 80, stdin);

        // 3. In order to handle the edge case where the user inputs 0 songs and tries to proceed:
        // - Prompt for input again until he inputs at least 1 song.
        while((strcmp(buffer, "\n") == 0) && songCount < 1)
        {
            printf("Enter at least one song: ");
            fgets(buffer, 80, stdin);
        }

        // 4. If the user has entered a desired amount of songs, they press "Enter" to indicate they are finished.
        // In that case, if the buffer is a '\n', clear buffer for future use and finish
        // the execution of the function.
        if(strcmp(buffer, "\n") == 0)
        {
            *artistSongs = songCount;
            return;
        }

        // 5. Remove a '\n' from the buffer so it doesn't mess with the output.
        // 6. Copy the song's name from buffer into respective row of 2D artists[] array.
        // 7. Clear buffer for future use.
        // 8. Increment the songCount counter for use in main() loops.
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy(targetSongsArray[songCount], buffer);
        buffer[0] = '\0';
        songCount += 1;
    }

    // 9. Set the value at artistSongs (value in numOfSongsPerArtist[x] passed in by-reference)
    // to the final amount of songs inputed for a certain artist for use in main().
    *artistSongs = songCount;
}

int findOriginalIndex(char unsortedArtists[][80], char sortedArtists[][80], int numOfArtists, int indexInSorted)
{
    // Find out the original index of artists from sortedArtists[], findOriginalIndex() function.
    // Iterate over the sortedArtists[i] and compare the artist's name that was passed into the
    // function as parameter.
    // When a match was found, return the index it was found at.
    // Use this index to print the right songs.
    for(size_t i = 0; i < numOfArtists; i++)
    {
        if(strcmp(sortedArtists[indexInSorted], unsortedArtists[i]) == 0) return i;
    }
}

// A debug function to check if the program read in the artist names correctly.
void printArtists(char artists[4][80], int numArtists)
{
    for(size_t i = 0; i < numArtists; i++)
    {
        printf("%s\n", artists[i]);
    }
}

// A debug function to check if the program read in the song names correctly.
void printSongs(char songs[3][80], int songNumber)
{
    for(size_t i = 0; i < songNumber; i++)
    {
        printf("  - %2s\n", songs[i]);
    }
}

// A function to print the final shuffled playlist.
void printPlaylist(char playlist[24][160], int playlistSize)
{
    for(size_t i = 0; i < playlistSize; i++)
    {
        printf("%s\n", playlist[i]);
    }
}

// A function to find the original index of a certain artist in the original artists array.
void combineNames(char playlist[24][160], int *totalPlaylistSize, char artist[80], char songs[][80], int songCount)
{
    // 3. Concatenate each song onto the artist's name.
    // 4. Add this concatenated string to the playlist array.
    // 5. Increment totalPlaylistSize for future use in shuffling.

    // A buffer string containing the current concatenated string.
    char combinedString[160];
    for(size_t i = 0; i < songCount; i++)
    {
        // sprintf() used to concatenate 2 strings.
        sprintf(combinedString, "%s - %s", artist, songs[i]);

        // Put 2 copies of the same song into playlist[] array.
        for(size_t j = 0; j < 2; j++)
        {
            // Copy this new "combined" string into the playlist[].
            strcpy(playlist[*totalPlaylistSize], combinedString);
            // Increment the totalPlaylistSize counter variable passed into the function by-reference
            // for use in main().
            *totalPlaylistSize += 1;
        }
    }
}