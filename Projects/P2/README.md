# Spell Checker Project
The spell checker works by reading a dictionary file and storing the words in a hash set. It then reads each text file and checks each word in the file. If a word is not in the dictionary or not capitalized correctly, it prints a message indicating that the word is not in the dictionary or not capitalized correctly, along with the filename, line number, and column number where the word was found.

The spell checker handles hyphenated words by splitting the word at each hyphen and checking each part separately. It also ignores any punctuation at the end of the word, and all apostrophes, quotation marks, and brackets/parentheses at the start of the word.

The spell checker is implemented in C and uses the standard C library for file I/O, string manipulation, and character classification. It uses a hash set to store the dictionary, which allows it to check if a word is in the dictionary in constant time on average.

## Design Choices
- Data Structure: I used a hash set to store the dictionary. This allows us to check if a word is in the dictionary in constant time on average. This is because strings are capped on length.

- Case Insensitivity: I converted both the word from the file and the word from the dictionary to lowercase before comparing them. This allows us to treat words as case-insensitive.

- Capitalization: I checked if a word is capitalized correctly by checking if it's all lowercase, all uppercase, or only the first letter is uppercase and the rest are lowercase. If a word is not capitalized correctly, I print a message indicating that it's not capitalized correctly.

- Hyphenated Words: I treated hyphenated words as separate words. If a word contains a hyphen, I split the word at the hyphen and check each part separately.

- Punctuation: I ignored any punctuation at the end of the word, and all apostrophes, quotation marks, and brackets/parentheses at the start of the word.

## Testing Choices
- Basic Words: I tested basic words that are in the dictionary and capitalized correctly.

- Capitalized Words: I tested words that are in the dictionary and capitalized with the first letter uppercase and the rest lowercase.

- Hyphenated Words: I tested words that contain a hyphen and each part of the word is in the dictionary and capitalized correctly.

- Incorrect Capitalization: I tested words that are in the dictionary but not capitalized correctly.

- Multiple Hyphens: I tested words that contain multiple hyphens and each part of the word is in the dictionary and capitalized correctly.

- Punctuation: I tested words that contain punctuation at the start or end of the word.

- Not in Dictionary: I tested words that are not in the dictionary.
