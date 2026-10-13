#include <curses.h>

void draw_calculator(double result, double current_num) {
  clear();

  mvprintw(4, 2, "Current Number: %f", current_num);
  mvprintw(6, 2, "Result: %f", result);

  refresh();
}

int main(void) {
  initscr();
  cbreak();
  noecho();

  keypad(stdscr, TRUE);

  double result = 0.0;
  double current_num = 0.0;
  char current_op = 0.0;
  int input = 0;

  while (1) {
    draw_calculator(result, current_num);
    // getnstr(&stuff, sizeof(input) - 1);
    input = getch();

    if (input == 'q' || input == 'Q') {
      break;
    }

    if (input == 'c' || input == 'C') {
      result = 0.0;
      current_num = 0.0;
      current_op = 0;
      // stuff = '\0';
      continue;
    }

    if (input >= '0' && input <= '9') {
      current_num = (current_num * 10) + (input - '0');
    } else if (input == '+' || input == '-' || input == '*' || input == '/') {
      if (current_op == 0) {
        result = current_num;
      } else {
        if (current_op == '+')
          result += current_num;
        if (current_op == '-')
          result -= current_num;
        if (current_op == '*')
          result *= current_num;
        if (current_op == '/') {
          if (current_num != 0)
            result /= current_num;
        }
      }
      current_op = input;
      current_num = 0.0;
    } else if (input == '\n' || input == KEY_ENTER) {
      if (current_op == '+')
        result += current_num;
      else if (current_op == '-')
        result -= current_num;
      else if (current_op == '*')
        result *= current_num;
      else if (current_op == '/') {
        if (current_num != 0)
          result /= current_num;
      }
      current_num = result;
      current_op = 1;
      refresh();
    }
  }

  endwin();
  return 0;
}
