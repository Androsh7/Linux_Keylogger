# Linux_Keylogger
Written by Androsh7 and AndrewBarthUSMC

---

A Linux keylogger using the Xlib library. It does not require escalated privileges, and will record all button presses on the display. `Ctrl + C` or killing the process will terminate the program. When compiling `Keylogger.cpp`, the argument `-lX11` should be added:
```
g++ -o Keylogger Keylogger.cpp -lX11
```

### Future Work

- There has been discussion on attempting to expand this Keylogger so that it works on other OS's.
- Xlib depends on an X server being present, which not all Linux distributions will have. A future implementation could be to ensure that an X server exists on the computer before proceeding.
- Carefully work on edge cases. What about non-standard keyboards? Multiple key presses? Held down key press? Etc.
- When extracting the data, an attacker may want to encrypt or encode the data. It might also be better to look into refraining from writing into the log file every key press, which may give the keylogger away.

### VirusTotal Report (1/17/2025):

![image](https://github.com/user-attachments/assets/c76e2c2d-c7ce-419b-9ead-d206b43d7ece)
