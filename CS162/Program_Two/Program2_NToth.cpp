// ******************************************************************
// File Name:  Program2_NToth.cpp
// Created By: Nicholas Toth
// E-Mail:     ntoth@pdx.edu
// Date:       2/14/2017
// Class:      CS162
// Program #:  2
//
// Program Overview: This program has two major purposes. The first
// purpose is to take in some user's full name and email address,
// and return an encoded string where both pieces of information are
// represented, but imperceptible by people. The second purpose is
// to decode information previously generated for the user.
//
// ******************************************************************


#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib> // For program termination. i.e. exit(0);

using namespace std;

const unsigned short ENC_ENT_BUF_SIZE = 128, // The size of the encoded entry buffer.
                     ALPHABET_SIZE    = 26,  // The number of letters in the English alphabet.
                     INFO_MAX_SIZE    = 21,  // One greater than the maximum size for a user's
                                             // name / email entry.
                     ASCII_A          = 65,  // The ASCII code for the character 'A'.
                     MOD_VAL          = 11;  // A number used in processing a user's entry for
                                             // shifting the alphabet.

// Front end
bool startProgram(void); // Gets the user's decision to encode or decode some entry.
void getSeed(short &seed); // Gets a numeric entry from the user for use in shifting the alphabet.
void getSeed(short &seed, char ch_coded[]); // Gets the first two characters (1 number) from the user's
                                            // encoded info for use in unshifting the subsequent values.
void getName(char *&name_buf, char *&name); // Gets the user's full name (up to 20 characters).
void getEmail(char *&email_buf, char *&email); // Gets the user's email address (up to 20 characters).
void getEncodedString(char *&ch_encoded); // Gets the user's encoded information.
void printInfo(char info[]); // Prints the program's output (encoded).
void printInfo(char name[], char email[]); // Prints the program's output (decoded).
void endProgram(bool iter_decision); // Gets the user's decision to quit or restart.

// Back end
void encode(char *&ch_encoded); // Encodes the user's name and email.
void decode(char *&ch_encoded); // Decodes the user's name and email.
void fillAlphabetMap(short alphabet_map[], short seed_mod); // Fills an array with a shifted version of
                                                            // the English alphabet.
void toDecimalASCII(char ch_entry[], short *&ascii_entry); // Converts the user's info to its ASCII
                                                           // representation.
// Converts the user's encoded info to the ascii representation of the original entry (name & email).
void toLiteralASCII(char ch_entry[], short *&ascii_entry, short seed_mod, short &ascii_len, short &sep_indx);
// Separates the user's decoded name and email.
void separateInfo(char *&name, char *&email, short *&ascii_entry, short &last_space, short &ascii_len);
// Shifts name & email entry down using the shifted alphabet map.
void shiftEntryDown(short alphabet_map[], short ascii_entry[], short entry_len, short &num_cnt);
// Converts the encoded information to an array of characters to be returned to the user.
void toCharArr(short ascii_entry[], short entry_len, char ch_entry[], short seed);
// Converts lower case letters in a string to upper case letters.
char* toupper(char str[]);
// Capitalizes the first letter for each separate word in a user's full name. e.g. nick toth -> Nick Toth.
void capitalizeName(char name[]);



int main(void)
{
    char *ch_coded, // Contains either the encoded string to be returned to the user.
         *name,  // Contains the decoded name to be returned to the user.
         *email; // Contains the decoded email to be returned to the user.

    bool start_decision = startProgram(); // Prompt for / read in user's decision
                                          // to encode an entry or decode an entry.
                                          // true -> encode. false -> decode.

    // If the user has chosen to encode information..
    if(start_decision)
        encode(ch_coded); // Get and encode the user's information.
    // Otherwise (the user has chosen to decode information)..
    else
        decode(ch_coded); // // Get and decode the user's information.

    // Delete the user's information entry.
    delete[] ch_coded;

    endProgram(start_decision); // Prompt the user to restart or quit.

    return 0;
}



/******************************************************************
 * Prompt for and read in user's decision to encode or decode
 * information. Returns true for encode, or false for decode.
 ******************************************************************/
bool startProgram(void)
{
    bool encode_first = true; // User's decision to encode or decode (boolean).

    char decision = ' '; // User's decision to encode or decode (character).

    // While the user's decision is not a valid choice (or nothing has been entered)..
    while(decision != 'E' && decision != 'D')
    {
        // Prompt the user to choose between encoding and decoding information.
        cout << "\nWould you like to encode or decode information? (E/D?): ";

        // Read in the user's response.
        cin >> decision;
        cin.ignore(100,'\n');

        // Convert the response to uppercase.
        decision = toupper(decision);

        // If the user enters 'E'
        if(decision == 'E')
            // then the program will encode.
            encode_first = true;
        // If the user enters 'D'
        else if(decision == 'D')
             // then the program will decode
            encode_first = false;
        // Otherwise (the user has entered an invalid character)..
        else
            // Alert the user that their entry was invalid and prompt them to re-enter.
            cout << "\nI'm sorry, but " << decision << " is an invalid entry!" << endl
                 << "Please enter either E(encode) or D(decode)." << endl;
    }

    return encode_first; // Return the user's decision to main().
}



/******************************************************************
 * Prompt for and read in a seed value from the user for
 * initializing the cipher. Ihe input is saved to seed.
 ******************************************************************/
void getSeed(short &seed)
{
    // Prompt the user to enter a number as a seed for creating a shifted alphabet.
    cout << "\nTo create the cipher, you will need to enter an initialization number." << endl
         << "Note that it will not be necessary for you to remember the number in" << endl
         << "order to decode the program's output.\n" << endl
         << "\nPlease enter a whole two digit number: ";

    // While the seed is not a whole 2-digit number..
    while(seed < 10 || seed > 99 )
    {
        // If this user has entered an invalid number..
        if(seed != 0)
            // Alert the user that their entry was invalid and to try again.
            cout << "\nI'm sorry, but " << seed << " is an invalid entry!" << endl
                 << "\nPlease enter a whole two digit number: ";

        // Read in the user's entry for the seeding the shifted alphabet.
        cin >> seed;
        cin.ignore(100,'\n');
    }

    // If the seed is divisible by 11
    if(seed % 11 == 0)
        --seed; // Decrement the seed. This is necessary because if the seed
                // is divisible by 11, the alphabet will not be shifted.

    return;
}



/******************************************************************
 * Gets the first two characters (1 number) from the user's
 * encoded info (ch_coded) for use in unshifting the subsequent
 * values. The result is saved to seed.
 ******************************************************************/
void getSeed(short &seed, char ch_coded[])
{
    // Save the tens place value of the seed.
    seed = 10 * static_cast<short>(ch_coded[0] - '0');
    // Add the ones place value of the seed.
    seed += static_cast<short>(ch_coded[1] - '0');

    return;
}



/******************************************************************
 * Prompts for and reads in a user's full name. Converts all
 * entries to upper case. Adds an extra space to the end of the
 * name so that name and email are separated when concatenated.
 * Stores results in name. name_buf is a generic buffer used by
 * encode() for the name ane email entries.
 ******************************************************************/
void getName(char *&name_buf, char *&name)
{
    // Stores the length of the name entry.
    short name_len = 0;

    // Prompt the user to enter their name.
    cout << "\nPlease enter your full name: ";

    // Read in the user's full name.
    cin.get(name_buf, INFO_MAX_SIZE, '\n');
    cin.ignore(100,'\n');

    // Copy the user's name from the buffer to the
    // permanent name entry array.
    name_len = strlen(name_buf);
    name = new char[name_len+2];
    strcpy(name, toupper(name_buf));

    name[name_len] = ' '; // space for separation of name and email
    name[name_len+1] = '\0';

    // Deallocate the name entry buffer.
    delete[] name_buf;
    // The buffer will be used again to get the user's email.
    // Reallocate to original size.
    name_buf = new char[INFO_MAX_SIZE + 1];

    return;
}



/******************************************************************
 * Prompts for and reads in a user's email. Converts all entries
 * to upper case. Stores results in email. email_buf is a generic
 * buffer used by encode() for the name ane email entries.
 ******************************************************************/
void getEmail(char *&email_buf, char *&email)
{
    // Stores the length of the email entry.
    short email_len = 0;

    // Prompt the user to enter their email.
    cout << "\nPlease enter your email address: ";

    // Read in the user's email address.
    cin.get(email_buf, INFO_MAX_SIZE, '\n');
    cin.ignore(100,'\n');

    // Copy the user's email from the buffer to the
    // permanent email entry array.
    email_len = strlen(email_buf);
    email = new char[email_len+1];
    strcpy(email, toupper(email_buf));

    email[email_len] = '\0';

    // Deallocate the email entry buffer.
    delete[] email_buf;
    // The buffer will not be used again. Set to null.
    email_buf = NULL;

    return;
}



/******************************************************************
 * Prompts for and reads in a user's encoded information to be
 * decoded. Stores results in ch_encoded.
 ******************************************************************/
void getEncodedString(char *&ch_encoded)
{
    // Stores the length of the encoded information entry.
    short entry_len = 0;

    // Buffer for loading the encoded information entry.
    char *encoded_ent_buf = new char[ENC_ENT_BUF_SIZE+1];

    // Prompt the user to enter their encoded information.
    cout << "\nPlease enter your encoded string: ";

    // Read in the user's encoded entry.
    cin.get(encoded_ent_buf, ENC_ENT_BUF_SIZE, '\n');
    cin.ignore(100,'\n');

    // Move the information from the buffer into the
    // permanent entry array.
    entry_len = strlen(encoded_ent_buf);
    ch_encoded = new char[entry_len + 1];
    strcpy(ch_encoded, encoded_ent_buf);
    ch_encoded[entry_len] = '\0';

    return;
}



/******************************************************************
 * Alert the user that their entry has been encoded. Print the
 * user's encoded information (stored in info).
 ******************************************************************/
void printInfo(char info[])
{
    cout << "The program has finished encoding.\n" << endl
         << "Your encoded information is:" << endl
         << info << endl;

    return;
}



/******************************************************************
 * Alert the user that their entry has been decoded. Print the
 * user's decoded name and email from the name and email arrays.
 ******************************************************************/
void printInfo(char name[], char email[])
{
    // Alert the user that the program has finished decoding..
    cout  << "\nThe program has finished decoding." << endl
          // And that their..
          << "\nYour original information is:" << endl
          << "\nDecoded Name: " << name << endl
          << "Decoded Email: " << email << endl << endl;

    return;
}



/******************************************************************
 * This is the end of the line. Either the program will close, or
 * The user may continue by entering 'R'. If the user should decide
 * to continue, main() will be called and the program will start
 * over.
 ******************************************************************/
void endProgram(bool iter_decision)
{
    char response = ' '; // User's decision for program termination.

    // Pre-prompts the user to quit or restart
    cout << "\nWould you like to quit or restart?" << endl;

    // If information was encoded, alert the user that they do not need to decode before quitting.
    if(iter_decision == true)
        cout << "Note that if you choose to quit, you will still be able" << endl
             << "to decode your information next time." << endl;

    // If the user has not yet answered a response, or the response was invalid..
    while(response != 'Q' && response != 'R')
    {
        // Prompt user to terminate/restart the program.
        cout << "\nEnter your response. (Q/R): ";

        // Read in the user's response.
        cin >> response;
        cin.ignore(100, '\n');

        // Convert to uppercase letter.
        response = toupper(response);

        // If the response was invalid
        if(response != 'Q' && response != 'R')
            // Alert the user that the entry was invalid, and that they will need to try again.
            cout << "Invalid entry. Please enter either Q(quit) or R(restart)" << endl;
    }

    // If the user enters 'Q', terminate the program.
    if(response == 'Q')
    {
        cout << "\nGoodbye!\n" << endl;
        exit(0); // Kill the program.
    }
    // If the user enters 'R', restart the program.
    if(response == 'R')
    {
        cout << "\nRestarting...\n\n" << endl;
        main(); // Restart the program.
    }

    return;
}



/******************************************************************
 * If the user chooses to encode their name and email address, this
 * function handles the process. encode reads in a number for use
 * as a seed in shifting the alphabet. Next, the user's information
 * is read in. Finally, the name and email are concatenated, the
 * alphabetic values are shifted, and the resulting encoded info
 * is returned to the user.
 ******************************************************************/
void encode(char *&ch_encoded)
{
    short *alphabet_map = new short[ALPHABET_SIZE], // Contains the shifted English alphabet.
          *ascii_entry; // Contains the ASCII codes for the user's information.

    short ch_encoded_size = 0, // The size of the character array containing the user's encoded info.
          entry_len       = 0, // The length of the user's concatenated info.
          seed_mod        = 0, // The amount that each alphabetic value will be shifted by.
          num_cnt         = 0, // Tracks the number of numbers in the user's entries.
          seed            = 0; // The user's entry for seeding the shifted alphabet.

    char *name_and_email, // The user's concatenated name and email address.
         // A buffer for reading in the user's name and email address.
         *entry_buf = new char[INFO_MAX_SIZE + 1],
         *email, // The user's email address.
         *name; // The user's name.

    // Build the cipher map
    getSeed(seed);
    seed_mod = seed % MOD_VAL;
    fillAlphabetMap(alphabet_map, seed_mod);

    // Get & prep user's name and email to encode.
    getName(entry_buf, name);   // Promp for / read in user's name.
    getEmail(entry_buf, email); // Promp for / read in user's email.

    entry_len = strlen(strcat(name, email)); // Save the length of the user's
                                             // concatenated name and email.

    name_and_email = new char[entry_len+1]; // Allocate memory for storing the user's
                                            // concatenated name and email.
    name_and_email = strcat(name,email); // Concatenate and save the user's name and email.
    name_and_email[entry_len] = '\0';

    // Encode the user's information.
    toDecimalASCII(name_and_email , ascii_entry); // Convert the user's information to
                                                  // its ASCII representation.

    shiftEntryDown(alphabet_map, ascii_entry, entry_len, num_cnt); // Shift the characters of
                                                                   // the user's information.

    ch_encoded_size = (2 * entry_len) + (num_cnt * 3) + 2; // Save memory size for storing the character
                                                           // representation of the user's encoded information,
                                                           // as well as the seed for decoding.

    ch_encoded = new char[ch_encoded_size+1];  // Allocate memory for storing the character
                                               // representation of the user's encoded information.

     // Convert the user's encoded information to its character representation.
    toCharArr(ascii_entry, entry_len, ch_encoded, seed);

    // Print out the user's encoded information.
    printInfo(ch_encoded);

    return;
}



/******************************************************************
 * If the user chooses to decode some information, this function
 * handles the process. decode reads in the user's encoded info,
 * shifts back the values, separates the name and email, prints
 * the decoded results to the user and returns to main, which
 * then proceeds to end the program.
 ******************************************************************/
void decode(char *&ch_encoded)
{
    short *ascii_entry; // The user's encoded entry.

    short last_space = 0, // The index of the space separating the name and email.
          ascii_len  = 0, // The total length of the encoded information entry.
          seed_mod   = 0, // The amount to shift up each alphabetic value in the encoded entry.
          seed       = 0; // The seed used by the user to create the shifted alphabet.

    char *email, // Stores the user's decoded email address.
         *name;  // Stores the user's decoded full name.

    // Prompt for and read in the user's encoded string.
    getEncodedString(ch_encoded);

    // Get the amount to shift each alphabetic value up by.
    getSeed(seed, ch_encoded);
    seed_mod = seed % MOD_VAL;

    // Decode the user's encoded information.
    toLiteralASCII(ch_encoded, ascii_entry, seed_mod, ascii_len, last_space);

    name = new char[last_space + 1]; // Allocate memory for storing the user's decoded name.
    email = new char[ascii_len - last_space]; // Allocate memory for storing the user's decoded email.

    // Separate the user's name and email from the decoded information.
    // Store into name and email.
    separateInfo(name, email, ascii_entry, last_space, ascii_len);

    // Capitalize the user's full name.
    capitalizeName(name);

    // Print out the user's decoded information.
    printInfo(name, email);

    delete[] name;
    delete[] email;

    return;
}



/******************************************************************
 * Creates and stores a version of the English alphabet shifted by
 * seed_mod in alphabet_map for shifting the user's name and email
 * information.
 ******************************************************************/
void fillAlphabetMap(short alphabet_map[], short seed_mod)
{
    // For every letter in the English alphabet..
    for(short ch = 0; ch < ALPHABET_SIZE; ++ch)
        // Save the ascii value of that letter, shifted by seed_mod.
        alphabet_map[ch] = ASCII_A + ch - seed_mod; // Shift letters down by seed_mod.

    return;
}



/******************************************************************
 * Converts the user's info in ch_entry to its ASCII representation
 * contained in ascii_entry.
 ******************************************************************/
void toDecimalASCII(char ch_entry[], short *&ascii_entry)
{
    short entry_length = strlen(ch_entry), // The full length of the user's concatenated name and email.
          index        = 0; // Index counter for traversing all of the user's entries.

    // Allocate space for the converted information.
    ascii_entry = new short[entry_length];

    // For every value in the user's information
    for(index = 0; index < entry_length; ++index)
        // Store the ascii code of that value in the ascii array.
        ascii_entry[index] = static_cast<short>(ch_entry[index]);

    return;
}



/******************************************************************
 * This function converts the encoded information entered by the
 * user (ch_entry) into an array of decoded ascii codes
 * (ascii_entry). seed_mod is passed in to shift values to the
 * user's original info codes. ascii_len and sep_indx are set
 * and returned to decode() for use in separating the name and
 * email address.
 ******************************************************************/
void toLiteralASCII(char ch_entry[], short *&ascii_entry, short seed_mod, short &ascii_len, short &sep_indx)
{
    short *ascii_ent_temp, // A temporary array for holdind the user's decoded name and email address.
          entry_length  = strlen(ch_entry), // The total length of the user's encoded information.
          current_code  = 0, // The value of ascii_entry at the index of the loop's current iteration,
                             // whenever the entry currently being processed is an alphabetic char code.
          ascii_indx    = 0, // The index counter for ascii array
          index         = 0; // The index counter for the loop.

    char ch_ones_place  = ' ', // The character representation of the ones place digit of current_code
         ch_tens_place  = ' ', // The character representation of the tens place digit of current_code
         current_char   = ' '; // The character representation of the value of ascii_entry for the
                               // current iteration.

    // Allocate memory for the temporary decoded info array.
    ascii_ent_temp = new short[entry_length];

    // For every (non seed) index of the encoded entry..
    for(index = 2; index < entry_length; ++index)
    {
        current_char = ch_entry[index]; // Store the current character of the encoded info.

        // If the current character is a space..
        if(current_char == ' ' )
        {
            // And either the next(+1) character is a digit and the next(+2) character is not a space, or the
            // next(+1) character is not a digit..
            if((isdigit(ch_entry[index+1]) && ch_entry[index+2] != ' ' ) || !isdigit(ch_entry[index+1]))
            {
                // AND either the last(-1) character is a digit and the last(-2) character was not a space, or the
                // last(-1) character is not a digit..
                if((isdigit(ch_entry[index-1]) && ch_entry[index-2] != ' ' ) || !isdigit(ch_entry[index-1]))
                {
                    sep_indx = ascii_indx; // Save this index as the last non-digit separating space.
                    // Save this space to the decoded info array.
                    ascii_ent_temp[ascii_indx++] = static_cast<short>(current_char);
                }
            }
        }
        // Otherwise if this and the next character are numeric values
        else if(isdigit(current_char) && isdigit(ch_entry[index+1]))
        {
            ch_tens_place = current_char; // Save the tens place value of the current character.
            ch_ones_place = ch_entry[index+1]; // Save the ones place value of the next character.

            // Convert the numeric values into their literal numeric representation.
            current_code = 10 * static_cast<short>( ch_tens_place - '0' );
            current_code += static_cast<short>( ch_ones_place - '0');

            // Save the numeric value added to the seed_mod to the decoded info array.
            ascii_ent_temp[ascii_indx++] = current_code + seed_mod;
            ++index; // Increment the index counter because of lookahead.
        }
        // Otherwise (the current value is a non-numeric and non-alphabetic character)
        else
            // Save the ascii code for the current character to the decoded info array.
            ascii_ent_temp[ascii_indx++] = static_cast<short>(current_char);

        current_code = 0; // Reset the current code to 0.
    }

    ascii_len = ascii_indx; // Store the length of the decoded info array.
    // Allocate memory to store the permanent converted & decoded info
    ascii_entry = new short[ascii_len];

    // For every converted value in the temp array
    for(index = 0; index < ascii_len; ++index)
        // Move info into the permanent converted & decoded info array.
        ascii_entry[index] = ascii_ent_temp[index];

    // Deallocate the temporary array.
    delete[] ascii_ent_temp;
    ascii_ent_temp = NULL;

    return;
}



/******************************************************************
 * Separates and moves the decoded data from ascii_entry into
 * char arrays for name & email. last_space is the index where
 * the name and email will be separated. ascii_len is the length
 * of the entire entry.
 ******************************************************************/
void separateInfo(char *&name, char *&email, short *&ascii_entry, short &last_space, short &ascii_len)
{
    short index = 0; // Index counter for separating name and email from decoded info.

    // From index 0 until the space that separates the name and email..
    for(; index < last_space; ++index)
        // Copy over the characters at the current index to name.
        name[index] = static_cast<char>(tolower(ascii_entry[index]));

    name[last_space] = '\0';

    // From the space that separates the name and email until the end of the information..
    for(index = last_space + 1; index < ascii_len; ++index)
        // Copy over the characters at the current index to email.
        email[index - (last_space + 1)] = static_cast<char>(tolower(ascii_entry[index]));

    email[ascii_len] = '\0';

    return;
}



/******************************************************************
 * Takes the ascii representation of the user's concatented name
 * and email, as well as the shifted alphabet, and shifts
 * alphabetic entries to the values in their respective values in
 * the shifted alphabet. e.g. 'C' -> alphabet[2]. Also counts the
 * number of numeric values entered by the user.
 ******************************************************************/
void shiftEntryDown(short alphabet_map[], short ascii_entry[], short entry_len, short &num_cnt)
{
    short shift_indx = 0, // Contains the index to which an unshifted alphabetic character will
                          // be shifted in the shifted alphabet. e.g. 'D' -> index 3.
          entry_indx = 0, // Index counter for traversing the unshifted entry.
          entry_code = 0; // Contains the ascii value for the unshifted info at the current entry index.

    // for every unshifted value..
    for(entry_indx = 0; entry_indx < entry_len; ++entry_indx)
    {
        entry_code = ascii_entry[entry_indx]; // Store the ascii code of the current value

        // If the entry is a digit
        if(entry_code >= 48 && entry_code <= 57)
            ++num_cnt; // Increment the number of digits.
        // If the entry is an alphabetic character
        if(entry_code >= ASCII_A && entry_code <= 90)
        {
            shift_indx = entry_code - ASCII_A; // Get the index of the shifted alphabet to shift to..

            // Shift the entry to the code at the shift index of the shifted alphabet.
            ascii_entry[entry_indx] = alphabet_map[shift_indx];
        }
        // Otherwise (the entry is a special character)
        else
        {
            // Temporarially add 100 to that value to keep track of it.
            ascii_entry[entry_indx] += 100;
        }
    }

    // All alphabetic entries have been shifted.
    // Deallocate the alphabet array.
    delete[] alphabet_map;
    alphabet_map = NULL;

    return;
}



/******************************************************************
 * After the user's name and email have been read in, and all of
 * the alphabetic entries have been shifted, this function converts
 * the encoded entry into a character array to be returned to the
 * user. Also, this function is responsible for properly formatting
 * the array for the decoder. ascii_entry contains the ASCII codes
 * for the user's encoded information. ch_entry is set by this
 * function to contain the character representation of the encoded
 * information.
 ******************************************************************/
void toCharArr(short ascii_entry[], short entry_len, char ch_entry[], short seed)
{
    short current_code = 0, // The value of ascii_entry at the index of the loop's current iteration, whenever the
                            // entry currently being processed is an alphabetic char code.
          ones_place   = 0, // The ones place value of the current code. (literal numeric representation)
          tens_place   = 0, // The tens place value of the current code. (literal numeric representation)
          conv_indx    = 2, // The index of the next char to be entered into the final (encoded) array.
          ch           = 0; // The index counter for the loop.

    char ones_place_ch = ' ', // The ones place value of the current code. (literal char representation)
         tens_place_ch = ' '; // The tens place value of the current code. (literal char representation)

    // Storing (char) tens place value of seed.
    ch_entry[0] = static_cast<char>('0' + ((seed - (seed % 10)) / 10) );
    // Storing (char) ones place value of seed.
    ch_entry[1] = static_cast<char>('0' + (seed % 10));

    // For every value in the ascii representation of entry..
    for(ch = 0; ch < entry_len; ++ch)
    {
        // If the entry is less than 100 (alphabetic char code)
        if(ascii_entry[ch] < 100)
        {
            current_code = ascii_entry[ch]; // save the code as current_code

            // Save the ones place value of the current_code
            ones_place = current_code % 10;
            // Save the tens place value of the current_code
            tens_place = ((current_code - ones_place) / 10);

            // Save the character representation of the ones place value of the current_code
            ones_place_ch = static_cast<char>('0' + ones_place);
            // Save the character representation of the tens place value of the current_code
            tens_place_ch = static_cast<char>('0' + tens_place);

            // Store the character representation of the current_code.
            ch_entry[conv_indx] = tens_place_ch;
            ch_entry[++conv_indx] = ones_place_ch;
        }
        // Otherwise, if 100 less than the entry is a numeric char code (digit)..
        else if( (ascii_entry[ch] - 100) >= '0' && (ascii_entry[ch] - 100) <= '9')
        {
            // This section is used to isolate numeric entries by the user.
            // e.g. bob55@mail.etc -> bob 5 5 @mail.etc

            // If the previous entry was not a space..
            if(ch_entry[conv_indx-1] != ' ')
            {
                ch_entry[conv_indx] = ' '; // Save a space to the current conversion index.
                // Set the next index of the character array to 100 subtracted from the current code.
                ch_entry[++conv_indx] = (ascii_entry[ch] - 100);
            }
            // Otherwise (the previous entry was a space)..
            else
            {
                // Set the current conversion index of the character array to 100 subtracted from the current code.
                ch_entry[conv_indx] = (ascii_entry[ch] - 100);
            }
            // Save a space to the next index.
            ch_entry[++conv_indx] = ' ';
        }
        // Otherwise (the current code is for a non-numeric, non-alphabetic symbol)
        else
            // Save the symbol to the current conversion index of the character array.
            ch_entry[conv_indx] = static_cast<char>(ascii_entry[ch]-100);

        ++conv_indx;; // Increment the conversion index counter.
    }

    ch_entry[conv_indx] = '\0';

    // Conversion is finished. Deallocate memory for the ascii entry array.
    delete[] ascii_entry;
    ascii_entry = NULL; // Point2nul

    return;
}



/******************************************************************
 * Converts lower case characters in str to uppercase characters.
 ******************************************************************/
char* toupper(char str[])
{
    short len = strlen(str), // Save the length of the entry.
          index = 0; // Index counter.

    // For every character in the entry..
    for(; index < len; ++index)
        // Convert that character to an uppercase character.
        str[index] = toupper(str[index]);

    // Return the all-uppercase string.
    return str;
}



/******************************************************************
 * Capitalizes the first letter for each separate word in a user's
 * full name. e.g. nick toth -> Nick Toth.
 ******************************************************************/
void capitalizeName(char name[])
{
    short name_len = strlen(name), // Length of the user's name
          index    = 0; // Index counter for traversing the user's name.

    for(; index < name_len; ++index)
    {
        // Capitalize the first letter.
        if( index == 0 )
            name[index] = toupper(name[index]);

        // If the last character was a space and
        // this character is alphabetic..
        else if( name[index - 1] == ' ' && isalpha(name[index]) )
                name[index] = toupper(name[index]);
    }
}
