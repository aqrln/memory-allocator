#pragma once

#define TTY_RED     "\x1B[31m"
#define TTY_GREEN   "\x1B[32m"
#define TTY_YELLLOW "\x1B[33m"
#define TTY_BLUE    "\x1B[34m"
#define TTY_MAGENTA "\x1B[35m"
#define TTY_CYAN    "\x1B[36m"
#define TTY_WHITE   "\x1B[37m"
#define TTY_RESET   "\x1B[0m"

#define RED(s)     TTY_RED s TTY_RESET
#define GREEN(s)   TTY_GREEN s TTY_RESET
#define YELLOW(s)  TTY_YELLLOW s TTY_RESET
#define BLUE(s)    TTY_BLUE s TTY_RESET
#define MAGENTA(s) TTY_MAGENTA s TTY_RESET
#define CYAN(s)    TTY_CYAN s TTY_RESET
#define WHITE(s)   TTY_WHITE s TTY_RESET
#define RESET(s)   TTY_RESET s TTY_RESET
